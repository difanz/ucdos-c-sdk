; STA.ASM - œ‘ æø®–æ∆¨ºÏ≤‚

.286
locals

NEWGROUP group _NEWTEXT

_NEWTEXT segment byte public 'CODE'
assume cs:_NEWTEXT, ds:_NEWTEXT

public _TESTADP

; ƒ⁄≤øπ§◊˜’ª
stack_buf       db 0FEh dup(?)
stack_top       dw 0
old_ss          dw 0
old_sp          dw 0
new_ss          dw 0

; œ‘ø®¿‡–ÕºÏ≤‚
; ∑µªÿ: AL=¿‡–Õ, AH=VESA÷ß≥÷
_TESTADP proc far
    cli
    mov     [cs:old_ss], ss
    mov     [cs:old_sp], sp
    mov     [cs:new_ss], cs
    mov     ss, [cs:new_ss]
    mov     sp, offset stack_top
    test    sp, 1
    jz      @@sp_aligned
    dec     sp
@@sp_aligned:
    sti
    pusha
    push    ds
    push    es
    push    cs
    pop     ds
    push    cs
    pop     es
    cld
    cli

    ; ≤‚ ‘EGA
    mov     ah, 12h
    mov     bx, 5510h
    int     10h
    cli
    cmp     bh, 55h
    jnz     @@not_ega

    ; ≤‚ ‘µ•…´/≤ …´
    mov     ah, 0Fh
    int     10h
    cli
    cmp     al, 7
    mov     al, 0FEh        ; MDA
    jz      @@is_mda
    mov     al, 0FDh        ; CGA
@@is_mda:
    jmp     @@store_and_check_vesa

@@not_ega:
    ; ≤‚ ‘VGA
    mov     ax, 1A00h
    int     10h
    cli
    cmp     al, 1Ah
    jz      @@is_vga
    mov     al, 0FCh        ; EGA
    jmp     @@store_and_check_vesa

; …®√Ëœ‘ø®BIOSÃÿ’˜¥Æ
scan_all_roms proc near
    mov     di, 0C000h
    call    scan_rom_segment
    jz      @@found
    mov     di, 0C800h
    call    scan_rom_segment
    jz      @@found
    mov     di, 0E000h
    call    scan_rom_segment
    jz      @@found
    mov     di, 0E800h
    call    scan_rom_segment
@@found:
    ret
scan_all_roms endp

; …®√Ëµ•∏ˆROM∂Œ
scan_rom_segment proc near
    mov     es, di
    cmp     word ptr [es:0], 0AA55h
    jnz     @@no_rom
    mov     di, ax
@@scan_loop:
    push    di
    push    si
    push    cx
    repe cmpsb
    pop     cx
    pop     si
    pop     di
    jz      @@matched
    inc     di
    cmp     di, dx
    jna     @@scan_loop
@@no_rom:
@@matched:
    ret
scan_rom_segment endp

@@is_vga:
    mov     ax, 12h
    int     10h
    cli
    mov     dx, 3C4h
    mov     al, 0Bh
    out     dx, al
    inc     dx
    in      al, dx
    and     al, 0Fh
    jnz     @@check_ahead
@@jmp_check_s3:
    jmp     @@check_s3

@@check_ahead:
    cmp     al, 0Ah
    jnc     @@jmp_check_s3

    ; ≤‚ ‘ Ahead 2048
    mov     si, offset str_2048
    mov     cx, 4
    xor     ax, ax
    mov     dx, 100h
    call    scan_all_roms
    jz      @@is_2048

    ; ≤‚ ‘ Trident
    mov     si, offset str_tgui96
    mov     cx, 6
    call    scan_all_roms
    jz      @@is_tgui96

    ; ≤‚ ‘ ARK
    mov     si, offset str_arklogic
    mov     cx, 9
    call    scan_all_roms
    jz      @@is_arklogic

    ; ≤‚ ‘ Cirrus GD7
    mov     si, offset str_clgd7
    mov     cx, 6
    call    scan_all_roms
    jz      @@is_clgd7

    mov     al, 1           ; ±Í◊º VGA
    jmp     short @@store_and_check_vesa

@@is_arklogic:
    mov     al, 0Dh
    jmp     short @@store_and_check_vesa

@@is_tgui96:
    mov     al, 13h
    jmp     short @@store_and_check_vesa

@@is_2048:
    mov     al, 0Ch
    jmp     short @@store_and_check_vesa

@@is_clgd7:
    mov     al, 17h

@@store_and_check_vesa:
    mov     [adapter_id], al
    cli
    push    cs
    pop     es
    mov     ax, 4F00h
    mov     di, offset vesa_buf
    push    di
    int     10h
    cli
    pop     di
    cmp     word ptr [di], 4556h    ; "VE"
    jnz     @@no_vesa
    cmp     word ptr [di+2], 4153h  ; "SA"
    jnz     @@no_vesa
    mov     byte ptr [vesa_flag], 1
@@no_vesa:
    mov     ax, 3
    int     10h
    pop     es
    pop     ds
    popa
    mov     al, [cs:adapter_id]
    mov     ah, [cs:vesa_flag]
    cli
    mov     ss, [cs:old_ss]
    mov     sp, [cs:old_sp]
    sti
    retf

@@check_s3:
    ; ≤‚ ‘ S3 / S100
    mov     si, offset str_s3
    mov     cx, 2
    xor     ax, ax
    mov     dx, 100h
    call    scan_all_roms
    jnz     @@check_acer

    mov     si, offset str_s100
    mov     cx, 3
    call    scan_all_roms
    jnz     @@is_s3_standard
    mov     al, 0Eh
    jmp     short @@store_and_check_vesa

@@is_s3_standard:
    mov     al, 6
    jmp     short @@store_and_check_vesa

@@check_acer:
    ; ≤‚ ‘ Acer
    mov     si, offset str_acer
    mov     cx, 4
    xor     ax, ax
    mov     dx, 100h
    call    scan_all_roms
    jnz     @@check_oak
    mov     al, 7
    jmp     short @@store_and_check_vesa

@@check_oak:
    ; ≤‚ ‘ Oak
    mov     si, offset str_oak
    mov     cx, 3
    xor     ax, ax
    mov     dx, 100h
    call    scan_all_roms
    jnz     @@check_realtek
    mov     al, 8
    jmp     @@store_and_check_vesa

@@check_realtek:
    ; ≤‚ ‘ Realtek
    mov     si, offset str_realtek
    mov     cx, 7
    xor     ax, ax
    mov     dx, 100h
    call    scan_all_roms
    jnz     @@check_igs
    mov     al, 9
    jmp     @@store_and_check_vesa

@@check_igs:
    ; ≤‚ ‘ IGS
    mov     si, offset str_igs
    mov     cx, 3
    xor     ax, ax
    mov     dx, 8000h
    call    scan_all_roms
    jnz     @@check_promotion
    mov     al, 16h
    jmp     @@store_and_check_vesa

@@check_promotion:
    ; ≤‚ ‘ Alliance
    mov     si, offset str_promotion
    mov     cx, 9
    xor     ax, ax
    mov     dx, 8000h
    call    scan_all_roms
    jnz     @@check_clgd6
    mov     al, 15h
    jmp     @@store_and_check_vesa

@@check_clgd6:
    ; ≤‚ ‘ Cirrus GD6
    mov     si, offset str_clgd6
    mov     cx, 6
    xor     ax, ax
    mov     dx, 100h
    call    scan_all_roms
    jnz     @@check_vga_str
    mov     al, 11h
    jmp     @@store_and_check_vesa

@@check_vga_str:
    mov     si, offset str_vga_eq
    mov     cx, 4
    mov     ax, 7Dh
    mov     dx, 81h
    call    scan_all_roms
    jz      @@is_et3000

    mov     ax, 12h
    int     10h
    cli
    mov     dx, 3C4h
    mov     al, 0Bh
    out     dx, al
    inc     dx
    in      al, dx
    and     al, 0Fh
    cmp     al, 0Ah
    jz      @@is_et3000

    mov     dx, 3CEh
    mov     ax, 50Fh
    out     dx, ax
    mov     al, 9
    out     dx, al
    inc     dx
    in      al, dx
    mov     bh, al
    mov     al, 5
    out     dx, al
    xor     al, al
    in      al, dx
    cmp     al, 5
    jnz     @@check_ati

    mov     al, 0Ah
    out     dx, al
    xor     al, al
    in      al, dx
    cmp     al, 0Ah
    jnz     @@check_ati

    mov     al, bh
    out     dx, al

@@is_et3000:
    mov     al, 2
    jmp     @@store_and_check_vesa

@@check_ati:
    ; ≤‚ ‘ ATI / Mach64
    mov     ax, 12h
    int     10h
    cli
    mov     si, offset str_761295520
    mov     cx, 9
    mov     ax, 31h
    mov     dx, 3Ah
    call    scan_all_roms
    jnz     @@check_ports_3cd

    mov     si, offset str_mach64
    mov     cx, 6
    xor     ax, ax
    mov     dx, 1000h
    call    scan_all_roms
    jz      @@is_mach64

    mov     si, offset str_ati6
    mov     cx, 4
    call    scan_all_roms
    jz      @@is_ati6

    mov     al, 3
    jmp     @@store_and_check_vesa

@@is_mach64:
    mov     al, 0Fh
    jmp     @@store_and_check_vesa

@@is_ati6:
    mov     al, 14h
    jmp     @@store_and_check_vesa

@@jmp_check_46e8:
    jmp     @@check_46e8

@@check_ports_3cd:
    mov     dx, 3CDh
    in      al, dx
    mov     ah, al
    and     al, 0C0h
    or      al, 55h
    out     dx, al
    in      al, dx
    cmp     al, 55h
    jnz     @@jmp_check_46e8
    mov     al, 0AAh
    out     dx, al
    in      al, dx
    cmp     al, 0AAh
    jnz     @@jmp_check_46e8

    mov     dx, 3D4h
    mov     al, 33h
    out     dx, al
    inc     dx
    in      al, dx
    mov     cl, al
    dec     dx
    mov     ax, 0A33h
    out     dx, ax
    mov     al, 33h
    out     dx, al
    inc     dx
    in      al, dx
    and     al, 0Fh
    cmp     al, 0Ah
    jnz     @@is_trident_10
    dec     dx
    mov     ax, 533h
    out     dx, ax
    mov     al, 33h
    out     dx, al
    inc     dx
    in      al, dx
    and     al, 0Fh
    cmp     al, 5
    jnz     @@is_trident_10
    dec     dx
    mov     al, 33h
    mov     ah, cl
    out     dx, ax

    ; ≤‚ ‘ Tseng / ET4000
    mov     si, offset str_compaq
    mov     cx, 6
    mov     ax, 5F80h
    mov     dx, 6000h
    call    scan_all_roms
    jz      @@is_tseng_compaq

    mov     si, offset str_cvga
    mov     cx, 4
    xor     ax, ax
    mov     dx, 400h
    call    scan_all_roms
    jz      @@is_tseng_compaq

    mov     si, offset str_tseng
    mov     cx, 5
    call    scan_all_roms
    jz      @@is_tseng_4

@@is_tseng_compaq:
    mov     al, 0Bh
    jmp     @@store_and_check_vesa

@@is_tseng_4:
    mov     al, 4
    jmp     @@store_and_check_vesa

@@is_trident_10:
    mov     al, 0Ah
    jmp     @@store_and_check_vesa

@@check_46e8:
    ; ≤‚ ‘ IBM 8514 / XGA
    mov     ax, 12h
    int     10h
    cli
    mov     dx, 46E8h
    mov     al, 18h
    out     dx, al
    mov     dx, 103h
    mov     al, 80h
    out     dx, al
    inc     dx
    in      al, dx
    mov     ah, al
    dec     dx
    xor     al, al
    out     dx, al
    mov     dx, 46E8h
    mov     al, 8
    out     dx, al
    cmp     ah, 0A5h
    jz      @@is_ibm_8514

    mov     ax, 5F00h
    int     10h
    cli
    cmp     al, 5Fh
    jnz     @@check_video7

@@is_ibm_8514:
    mov     al, 5
    jmp     @@store_and_check_vesa

@@check_video7:
    ; ≤‚ ‘ Video7
    mov     ax, 12h
    int     10h
    cli
    mov     ax, 0BF03h
    xor     bx, bx
    mov     cx, bx
    int     10h
    cli
    or      bl, bl
    jnz     @@v7_check_rev
    or      cl, cl
    jz      @@check_matrox

@@v7_check_rev:
    mov     ax, 10F1h
    int     10h
    cli
    cmp     al, 10h
    jz      @@check_matrox
    mov     al, 10h
    jmp     @@store_and_check_vesa

@@check_matrox:
    ; ≤‚ ‘ Matrox
    mov     si, offset str_matrox
    mov     cx, 6
    xor     ax, ax
    mov     dx, 100h
    call    scan_all_roms
    jnz     @@unknown_adp
    mov     al, 12h
    jmp     @@store_and_check_vesa

@@unknown_adp:
    xor     al, al
    jmp     @@store_and_check_vesa
_TESTADP endp

; VESA ª∫≥Â«¯
vesa_buf        db 100h dup(?)

; –æ∆¨Ãÿ’˜◊÷∑˚¥Æ
adapter_id      db 0
vesa_flag       db 0
str_vga_eq      db "VGA="
str_761295520   db "761295520"
str_s3          db "S3"
str_acer        db "Acer"
str_oak         db "OAK"
str_realtek     db "REALTEK"
str_compaq      db "COMPAQ"
str_tseng       db "TSENG"
str_cvga        db "CVGA"
str_2048        db "2048"
str_arklogic    db "ARK LOGIC"
str_s100        db "S100"
str_mach64      db "MACH64"
str_clgd6       db "CL-GD6"
str_matrox      db "MATROX"
str_tgui96      db "TGUI96"
str_ati6        db "ATI6"
str_promotion   db "ProMotion"
str_igs         db "IGS"
str_clgd7       db "CL-GD7"

_NEWTEXT ends
end
