; STRELINE.ASM - 扫描线缩放加速

.286
.model large, c

_TEXT segment byte public 'CODE'
assume cs:_TEXT

public StretchLine

src_w   dw 0
dst_w   dw 0
bpp_val db 0
cur_bit db 0

stretch_1bit:
    xor     dx, dx
    mov     cx, cs:src_w
    mov     bx, 0801h
@@loop_src:
    mov     cs:cur_bit, 0
    dec     bl
    jnz     @@has_bit
    mov     bl, 8
    lodsb

@@has_bit:
    add     dx, cs:dst_w
    shl     al, 1
    jnc     @@bit_0
    mov     cs:cur_bit, 1

@@bit_0:
@@check_dst:
    cmp     dx, cs:src_w
    jc      @@next_src
    clc
    cmp     cs:cur_bit, 0
    jz      @@store_bit
    stc

@@store_bit:
    rcl     ah, 1
    dec     bh
    jnz     @@byte_not_full
    mov     es:[di], ah
    inc     di
    mov     bh, 8

@@byte_not_full:
    sub     dx, cs:src_w
    jmp     short @@check_dst

@@next_src:
    loop    @@loop_src
    cmp     bh, 8
    jz      @@done_1bit
    mov     cl, bh
    shl     ah, cl
    mov     al, ah
    stosb

@@done_1bit:
    retf


stretch_8bit:
    mov     cx, cs:src_w
    xor     dx, dx

@@loop_8:
    lodsb
    add     dx, cs:dst_w

@@check_8:
    cmp     dx, cs:src_w
    jc      @@next_8
    stosb
    sub     dx, cs:src_w
    jmp     short @@check_8

@@next_8:
    loop    @@loop_8
    ret


stretch_16bit:
    mov     cx, cs:src_w
    shl     cx, 1
    xor     dx, dx

@@loop_16:
    lodsw
    add     dx, cs:dst_w

@@check_16:
    cmp     dx, cs:src_w
    jc      @@next_16
    stosw
    sub     dx, cs:src_w
    jmp     short @@check_16

@@next_16:
    loop    @@loop_16
    ret


stretch_24bit:
    mov     ax, cs:src_w
    mov     cx, ax
    shl     ax, 1
    add     cx, ax
    xor     dx, dx

@@loop_24:
    lodsw
    mov     bx, ax
    lodsb
    add     dx, cs:dst_w

@@check_24:
    cmp     dx, cs:src_w
    jc      @@next_24
    mov     es:[di], bx
    inc     di
    inc     di
    stosb
    sub     dx, cs:src_w
    jmp     short @@check_24

@@next_24:
    loop    @@loop_24
    ret


stretch_32bit:
    mov     cx, cs:src_w
    shl     cx, 1
    shl     cx, 1
    xor     dx, dx

@@loop_32:
    lodsw
    mov     bx, ax
    lodsw
    add     dx, cs:dst_w

@@check_32:
    cmp     dx, cs:src_w
    jc      @@next_32
    mov     es:[di], bx
    inc     di
    inc     di
    stosw
    sub     dx, cs:src_w
    jmp     short @@check_32

@@next_32:
    loop    @@loop_32
    ret


StretchLine proc far
    push    bp
    mov     bp, sp
    push    bx
    push    si
    push    di
    push    ds
    push    es

    lds     si, [bp+6]
    les     di, [bp+10]
    mov     ax, [bp+14]
    mov     cs:src_w, ax
    mov     ax, [bp+16]
    mov     cs:dst_w, ax
    mov     al, [bp+18]
    mov     cs:bpp_val, al

    cmp     al, 1
    jz      @@call_1bit
    cmp     al, 3
    jnz     @@check_8bit

@@call_1bit:
    push    cs
    call    near ptr stretch_1bit
    jmp     short @@exit

@@check_8bit:
    cmp     al, 2
    jnz     @@check_16bit
    call    stretch_8bit
    jmp     short @@exit

@@check_16bit:
    cmp     al, 12
    jz      @@call_16bit
    cmp     al, 13
    jnz     @@check_24bit

@@call_16bit:
    call    stretch_16bit
    jmp     short @@exit

@@check_24bit:
    cmp     al, 14
    jnz     @@call_32bit
    call    stretch_24bit
    jmp     short @@exit

@@call_32bit:
    call    stretch_32bit

@@exit:
    pop     es
    pop     ds
    pop     di
    pop     si
    pop     bx
    pop     bp
    ret
StretchLine endp

_TEXT ends
end
