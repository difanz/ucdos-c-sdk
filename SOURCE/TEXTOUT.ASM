; TEXTOUT.ASM - 英文字符图形输出

.286
locals

_TEXT segment byte public 'CODE'
assume cs:_TEXT, ds:_TEXT

public _EngTextOut
public _EngTextExtent

lines_per_band      dw 0
glyph_buf_off       dw 0
str_len             dw 0
text_width          dw 0
bytes_per_line      dw 0
cur_x_bit           dw 0
glyph_width         dw 0
glyph_bytes         dw 0
max_width           dw 0

; 文本参数块
; Packed TEXTINFO: 31 bytes, matching SDK.H in the large model.
TEXTINFO31_SIZE equ 1Fh
TEXTINFO31 label byte
TEXTINFO31_X              dw 0
TEXTINFO31_Y              dw 0
TEXTINFO31_STRING            dd 0
TEXTINFO31_ASC_FONT_NO             dw 0
TEXTINFO31_CHI_FONT_NO            dw 0
TEXTINFO31_FONT_WIDTH          dw 0
TEXTINFO31_FONT_HEIGHT         dw 0
TEXTINFO31_FR_COLOR            dw 0
TEXTINFO31_BG_COLOR            dw 0
TEXTINFO31_CHAR_SPACE          dw 0
TEXTINFO31_EQUAL_WIDTH          dw 0
TEXTINFO31_BUFFER             dd 0
TEXTINFO31_BUFFER_LENGTH            dw 0
TEXTINFO31_ICON          db 0

; 字符状态
cur_char            dw 0
cur_dir             dw 0
cur_char_width      dw 0
tot_height          dw 0
band_y_start        dw 0
band_y_end          dw 0
flag_3d             db 1
pad_3e              db 0
saved_3f            dw 0FFFFh

; 点阵光栅化
draw_glyph_scanlines proc near
    mov     si, [glyph_buf_off]
    les     di, [TEXTINFO31_BUFFER]
    mov     ax, [cur_x_bit]
    push    ax
    shr     ax, 1
    shr     ax, 1
    shr     ax, 1
    add     di, ax
    pop     bx
    and     bl, 7
    sub     bl, 8
    neg     bl
    xor     bh, bh
    mov     ax, [cur_char_width]
    mov     cx, [TEXTINFO31_CHAR_SPACE]
    cmp     byte ptr [cur_char+1], 0
    jz      @@no_double_extra
    shl     cx, 1
@@no_double_extra:
    add     cx, ax
    add     [cur_x_bit], cx
    mov     [glyph_width], ax
    add     ax, 7
    shr     ax, 1
    shr     ax, 1
    shr     ax, 1
    mov     [glyph_bytes], ax
    mov     cx, [band_y_end]
    sub     cx, [band_y_start]
    inc     cx
    mov     ds, [word ptr TEXTINFO31_BUFFER+2]

@@line_loop:
    push    cx
    push    di
    cmp     bl, 8
    jz      @@byte_aligned
    push    bx
    mov     ah, [di]
    mov     cl, bl
    shr     ah, cl
    mov     cx, [cs:glyph_width]

@@bit_loop:
    or      bh, bh
    jnz     @@have_bits
    lodsb
    mov     bh, 8
@@have_bits:
    shl     al, 1
    rcl     ah, 1
    dec     bh
    dec     bl
    jnz     @@dest_not_full
    mov     [es:di], ah
    inc     di
    mov     bl, 8
@@dest_not_full:
    loop    @@bit_loop
    cmp     bl, 8
    jz      @@line_done
    mov     cl, bl
    shl     ah, cl
    mov     [es:di], ah

@@line_done:
    pop     bx

@@pop_and_next:
    pop     di
    pop     cx
    add     di, [cs:bytes_per_line]
    loop    @@line_loop
    push    cs
    pop     ds
    ret

@@byte_aligned:
    mov     cx, [cs:glyph_bytes]
    rep     movsb
    jmp     short @@pop_and_next
draw_glyph_scanlines endp


; 取字符点阵
load_char_glyph proc near
    mov     bx, [TEXTINFO31_FONT_WIDTH]
    or      ah, ah
    jz      @@single_byte
    shl     bx, 1
@@single_byte:
    mov     [cur_char_width], bx
    mov     [cur_char], ax
    mov     es, [word ptr TEXTINFO31_BUFFER+2]
    mov     di, [glyph_buf_off]
    cmp     [TEXTINFO31_FONT_HEIGHT], 16
    jnz     @@use_int7e
    cmp     [TEXTINFO31_FONT_WIDTH], 8
    jnz     @@use_int7e
    cmp     [cur_dir], 0
    jnz     @@use_int7e
    mov     dx, ax
    int     7Fh
    mov     ds, dx
    xor     si, si
    mov     cx, 16
    rep     movsw
    push    cs
    pop     ds
    jmp     short @@do_draw

@@use_int7e:
    mov     si, offset cur_char
    int     7Eh

@@do_draw:
    call    draw_glyph_scanlines
    ret
load_char_glyph endp


; 显示接口参数块
blit_buf_ptr        dd 0
blit_width          dw 0
blit_height         dw 0
blit_x              dw 0
blit_y              dw 0
blit_bk_color       db 0
blit_color          db 0


; 输出到显存
blit_to_screen proc near
    les     ax, [TEXTINFO31_BUFFER]
    mov     [word ptr blit_buf_ptr+2], es
    mov     [word ptr blit_buf_ptr], ax
    mov     ax, [TEXTINFO31_X]
    mov     [blit_x], ax
    mov     ax, [text_width]
    mov     [blit_width], ax
    mov     ax, [TEXTINFO31_Y]
    add     ax, [band_y_start]
    mov     [blit_y], ax
    mov     ax, [band_y_end]
    sub     ax, [band_y_start]
    inc     ax
    mov     [blit_height], ax
    mov     ax, [TEXTINFO31_FR_COLOR]
    mov     [blit_color], al
    mov     ax, [TEXTINFO31_BG_COLOR]
    mov     [blit_bk_color], al
    mov     ax, 9509h
    mov     bh, 5
    mov     bl, [TEXTINFO31_ICON]
    mov     si, offset blit_buf_ptr
    int     10h
    ret
blit_to_screen endp


; 取下一个字符
get_next_char proc near
    xor     ah, ah
    mov     bx, [cs:TEXTINFO31_ASC_FONT_NO]
    lodsb
    dec     cx
    jz      @@done
    cmp     al, 0A1h
    jc      @@done
    cmp     byte ptr [si], 0A1h
    jc      @@done
    mov     ah, al
    lodsb
    dec     cx
    mov     bx, [cs:TEXTINFO31_CHI_FONT_NO]
@@done:
    mov     [cs:cur_dir], bx
    ret
get_next_char endp


; 计算文本尺寸
calc_text_extent proc near
    lds     si, [TEXTINFO31_STRING]
    mov     bx, 0FFFFh
@@count_len:
    lodsb
    inc     bx
    or      al, al
    jnz     @@count_len
    push    cs
    pop     ds
    mov     [str_len], bx
    cmp     [TEXTINFO31_ASC_FONT_NO], 0
    jz      @@fixed_width
    cmp     [TEXTINFO31_EQUAL_WIDTH], 1
    jz      @@fixed_width
    mov     [text_width], 0
    lds     si, [TEXTINFO31_STRING]
    mov     cx, bx

@@char_loop:
    call    get_next_char
    push    si
    push    ds
    push    cs
    pop     ds
    mov     bx, [TEXTINFO31_FONT_WIDTH]
    shl     bx, 1
    add     bx, [TEXTINFO31_CHAR_SPACE]
    or      ah, ah
    jnz     @@add_extra
    mov     [cur_char], ax
    mov     ax, [TEXTINFO31_FONT_WIDTH]
    mov     [cur_char_width], ax
    push    [saved_3f]
    mov     [saved_3f], 0
    mov     si, offset cur_char
    les     di, [TEXTINFO31_BUFFER]
    int     7Eh
    pop     [saved_3f]
    mov     bx, [cur_char_width]
    cmp     bx, [max_width]
    jna     @@add_extra
    mov     [max_width], bx
@@add_extra:
    add     bx, [TEXTINFO31_CHAR_SPACE]
    add     [text_width], bx
    pop     ds
    pop     si
    inc     cx
    loop    @@char_loop

    push    cs
    pop     ds
@@check_max_extent:
    mov     ax, [TEXTINFO31_FONT_WIDTH]
    shl     ax, 1
    cmp     ax, [max_width]
    jna     @@done_extent
    mov     [max_width], ax
@@done_extent:
    ret

@@fixed_width:
    mov     ax, [TEXTINFO31_FONT_WIDTH]
    add     ax, [TEXTINFO31_CHAR_SPACE]
    mul     bx
    mov     [text_width], ax
    jmp     short @@check_max_extent
calc_text_extent endp


; 西文快速输出接口
_EngTextOut proc far
    push    bp
    mov     bp, sp
    push    bx
    push    si
    push    di
    push    ds
    push    es
    lds     si, [bp+6]
    mov     di, offset TEXTINFO31
    push    cs
    pop     es
    mov     cx, TEXTINFO31_SIZE
    rep     movsb
    push    cs
    pop     ds
    cmp     [TEXTINFO31_BUFFER_LENGTH], 1000h
    ja      @@buf_size_ok

@@fail_exit:
    mov     ax, 0
    jmp     @@ret_exit

@@buf_size_ok:
    mov     ax, [TEXTINFO31_FONT_WIDTH]
    mov     [max_width], ax
    mov     ax, [TEXTINFO31_FONT_HEIGHT]
    mov     [tot_height], ax
    dec     ax
    mov     [band_y_end], ax
    mov     [band_y_start], 0
    mov     al, 1
    cmp     [TEXTINFO31_EQUAL_WIDTH], 1
    jnz     @@no_flag40
    or      al, 40h
@@no_flag40:
    mov     [flag_3d], al
    call    calc_text_extent
    cmp     [text_width], 0
    jz      @@fail_exit
    mov     ax, [text_width]
    add     ax, 7
    shr     ax, 1
    shr     ax, 1
    shr     ax, 1
    mov     [bytes_per_line], ax
    mov     bx, ax
    mov     ax, [max_width]
    add     ax, 7
    shr     ax, 1
    shr     ax, 1
    shr     ax, 1
    shl     ax, 1
    add     bx, ax
    mov     ax, [TEXTINFO31_BUFFER_LENGTH]
    sub     ax, 1000h
    xor     dx, dx
    div     bx
    mov     [lines_per_band], ax
    or      ax, ax
    jz      @@fail_exit
    mul     [bytes_per_line]
    push    ax
    sub     ax, [TEXTINFO31_BUFFER_LENGTH]
    neg     ax
    mov     [saved_3f], ax
    pop     ax
    add     ax, [word ptr TEXTINFO31_BUFFER]
    mov     [glyph_buf_off], ax

@@band_loop:
    mov     [cur_x_bit], 0
    mov     ax, [band_y_start]
    add     ax, [lines_per_band]
    cmp     ax, [tot_height]
    jna     @@band_not_exceed
    mov     ax, [tot_height]
@@band_not_exceed:
    dec     ax
    mov     [band_y_end], ax
    mov     cx, [str_len]
    lds     si, [TEXTINFO31_STRING]

@@band_char_loop:
    call    get_next_char
    push    cx
    push    si
    push    ds
    push    cs
    pop     ds
    call    load_char_glyph
    pop     ds
    pop     si
    pop     cx
    inc     cx
    loop    @@band_char_loop

    push    cs
    pop     ds
    call    blit_to_screen
    mov     ax, [band_y_end]
    inc     ax
    mov     [band_y_start], ax
    cmp     ax, [tot_height]
    jc      @@band_loop
    mov     ax, 1

@@ret_exit:
    pop     es
    pop     ds
    pop     di
    pop     si
    pop     bx
    pop     bp
    retf
_EngTextOut endp


; 取西文尺寸接口
_EngTextExtent proc far
    push    bp
    mov     bp, sp
    push    bx
    push    si
    push    di
    push    ds
    push    es
    lds     si, [bp+6]
    mov     di, offset TEXTINFO31
    push    cs
    pop     es
    mov     cx, TEXTINFO31_SIZE
    rep     movsb
    push    cs
    pop     ds
    mov     ax, [TEXTINFO31_FONT_HEIGHT]
    mov     [tot_height], ax
    dec     ax
    mov     [band_y_end], ax
    mov     [band_y_start], 0
    mov     al, 1
    cmp     [TEXTINFO31_EQUAL_WIDTH], 1
    jnz     @@no_flag40
    or      al, 40h
@@no_flag40:
    mov     [flag_3d], al
    call    calc_text_extent
    mov     ax, [text_width]
    pop     es
    pop     ds
    pop     di
    pop     si
    pop     bx
    pop     bp
    retf
_EngTextExtent endp

_TEXT ends
end
