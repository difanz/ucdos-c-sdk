; VGAFADE.ASM - 调色板平滑淡入淡出

.286
.model large, c

_NEWTEXT segment byte public 'CODE'
assume cs:_NEWTEXT

public VGAFADEIN
public VGAFADEOUT

; 调色板淡入
VGAFADEIN proc far
    cld
    push    bp
    mov     bp, sp
    pusha
    push    ds

    mov     si, [bp+0Ah]        ; palette segment
    mov     ds, si
    mov     cx, 40h             ; 64 DAC steps

@@fadein_outer:
    push    cx
    mov     si, [bp+08h]        ; palette offset
    mov     cx, 100h            ; 256 colors
    xor     bl, bl

@@fadein_inner:
    call    set_fadein_color
    inc     bl
    push    cx
    mov     cx, [bp+06h]        ; delay count
@@fadein_delay:
    loop    @@fadein_delay
    pop     cx
    loop    @@fadein_inner

    pop     cx
    loop    @@fadein_outer

    pop     ds
    popa
    pop     bp
    ret
VGAFADEIN endp


; 调色板淡出
VGAFADEOUT proc far
    cld
    push    bp
    mov     bp, sp
    pusha
    push    ds

    mov     si, [bp+0Ah]        ; palette segment
    mov     ds, si
    mov     cx, 40h             ; 64 DAC steps

@@fadeout_outer:
    push    cx
    mov     si, [bp+08h]        ; palette offset
    mov     cx, 100h            ; 256 colors
    xor     bl, bl

@@fadeout_inner:
    call    set_fadeout_color
    inc     bl
    push    cx
    mov     cx, [bp+06h]        ; delay count
@@fadeout_delay:
    loop    @@fadeout_delay
    pop     cx
    loop    @@fadeout_inner

    pop     cx
    loop    @@fadeout_outer

    pop     ds
    popa
    pop     bp
    ret
VGAFADEOUT endp


set_fadein_color proc near
    cli
    push    cx
    mov     dx, 3C7h
    mov     al, bl
    out     dx, al
    add     dx, 2
    in      al, dx
    jmp     short $+2
    mov     ah, al
    in      al, dx
    jmp     short $+2
    mov     ch, al
    in      al, dx
    jmp     short $+2
    mov     cl, al

    lodsb
    shr     al, 2
    cmp     ah, al
    jae     @@no_inc_r
    inc     ah
@@no_inc_r:
    lodsb
    shr     al, 2
    cmp     ch, al
    jae     @@no_inc_g
    inc     ch
@@no_inc_g:
    lodsb
    shr     al, 2
    cmp     cl, al
    jae     @@no_inc_b
    inc     cl
@@no_inc_b:
    mov     dx, 3C8h
    mov     al, bl
    out     dx, al
    inc     dx
    mov     al, ah
    out     dx, al
    jmp     short $+2
    mov     al, ch
    out     dx, al
    jmp     short $+2
    mov     al, cl
    out     dx, al
    jmp     short $+2
    pop     cx
    ret
set_fadein_color endp


set_fadeout_color proc near
    cli
    push    cx
    mov     dx, 3C7h
    mov     al, bl
    out     dx, al
    add     dx, 2
    in      al, dx
    jmp     short $+2
    mov     ah, al
    in      al, dx
    jmp     short $+2
    mov     ch, al
    in      al, dx
    jmp     short $+2
    mov     cl, al

    or      ah, ah
    jz      @@no_dec_r
    dec     ah
@@no_dec_r:
    or      ch, ch
    jz      @@no_dec_g
    dec     ch
@@no_dec_g:
    or      cl, cl
    jz      @@no_dec_b
    dec     cl
@@no_dec_b:
    mov     dx, 3C8h
    mov     al, bl
    out     dx, al
    inc     dx
    mov     al, ah
    out     dx, al
    jmp     short $+2
    mov     al, ch
    out     dx, al
    jmp     short $+2
    mov     al, cl
    out     dx, al
    jmp     short $+2
    pop     cx
    ret
set_fadeout_color endp

_NEWTEXT ends
end
