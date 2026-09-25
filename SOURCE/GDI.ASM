; GDI.ASM - 统一图形驱动分发接口

.286
locals

; Flood-fill records. Equates describe existing storage only.
FILL_SPAN6_LEFT equ 0
FILL_SPAN6_RIGHT equ 2
FILL_SPAN6_Y equ 4
FILL_SPAN6_SIZE equ 6
FILL_FRAME14_LEFT equ 0
FILL_FRAME14_RIGHT equ 2
FILL_FRAME14_Y equ 4
FILL_FRAME14_Y_STEP equ 6
FILL_FRAME14_SAVE_BX equ 8
FILL_FRAME14_SAVE_CX equ 0Ah
FILL_FRAME14_SAVE_DX equ 0Ch
FILL_FRAME14_SIZE equ 0Eh

NEWGROUP group _NEWTEXT

_NEWTEXT segment byte public 'CODE'
assume cs:_NEWTEXT, ds:_NEWTEXT

public _GDI
public _SETOLDVICTOR
public _SETMYVICTOR

; 驱动主入口与栈
_GDI:
L_0000:
    jmp     near ptr L_3FC5
    db      254 dup(0)
    dw      0h
L_0103:
    db      32 dup(0)
    db      20h

; 功能向量表
L_0124:
    dw      0h
L_0126:
    dw      0h
L_0128:
    dw      0h
    dw      0h
L_012C:
    dw      0h
    dw      0h
L_0130:
    dw      0h
    dw      0h
L_0134:
    dw      0h
    dw      0h
L_0138:
    dw      0h
    dw      0h
L_013C:
    dw      0h
L_013E:
    dw      0h
L_0140:
    db      0h
L_0141:
    dw      0h
L_0143:
    dw      0h
L_0145:
    mov     bx,word ptr ds:[1Ch]
    mov     [bx],ax
    inc     bx
    inc     bx
    cmp     bx,word ptr ds:[82h]
    jc      short L_0157
    mov     bx,word ptr ds:[80h]
L_0157:
    mov     word ptr ds:[1Ch],bx
    ret     

; 键盘中断
L_015C:
    push    ax
    push    bx
    push    ds
    mov     ax,40h
    mov     ds,ax
    mov     ah,byte ptr ds:[17h]
    and     ah,0Fh
    in      al,60h
    cmp     byte ptr [cs:L_0140],0h
    jz      short L_0187
    mov     byte ptr [cs:L_0140],0h
    cmp     al,0B8h
    jnz     short L_0193
    mov     ax,3800h
    call    L_0145
    jmp     short L_0198
L_0187:
    cmp     al,38h
    jnz     short L_0193
    mov     byte ptr [cs:L_0140],1h
    jmp     short L_0198
L_0193:
    cmp     ah,4h
    jz      short L_01A0
L_0198:
    pop     ds
    pop     bx
    pop     ax
    jmp     dword ptr [cs:L_0141]
L_01A0:
    push    ax
    and     al,7Fh
    cmp     al,3Eh
    jz      short L_01B6
    cmp     al,3Fh
    jz      short L_01B6
    cmp     al,41h
    jz      short L_01B6
    cmp     al,44h
    jz      short L_01B6
    pop     ax
    jmp     short L_0198
L_01B6:
    mov     ah,0F0h
    pop     bx
    test    bl,80h
    jnz     short L_0198
    call    L_0145
    mov     al,20h
    out     20h,al
    pop     ds
    pop     bx
    pop     ax
    iret    
L_01C9:
    dw      1E0h
L_01CB:
    dw      280h
L_01CD:
    db      1h
L_01CE:
    db      0h
L_01CF:
    db      0h
L_01D0:
    db      0h

; 屏幕BIOS分发
L_01D1:
    sti     
    cld     
    cmp     ah,0FFh
    jz      short L_01DD
    cmp     ah,95h
    jnz     short L_0202
L_01DD:
    push    bx
    push    cx
    push    dx
    push    si
    push    di
    push    bp
    push    es
    push    ds
    push    ax
    jmp     near ptr L_0632
L_01E9:
    pop     ax
L_01EA:
    pop     ds
    pop     es
    pop     bp
    pop     di
    pop     si
    pop     dx
    pop     cx
    pop     bx
    iret    
L_01F3:
    pop     ax
    pop     ds
    pop     es
    pop     bp
    pop     di
    pop     si
    pop     dx
    pop     cx
    pop     bx
L_01FC:
    cli     
    jmp     dword ptr [cs:L_0124]
L_0202:
    or      ah,ah
    jnz     short L_01FC
    cmp     byte ptr [cs:L_01CE],0h
    ja      short L_01FC
    push    bx
    push    cx
    push    dx
    push    si
    push    di
    push    bp
    push    es
    push    ds
    push    ax
    cmp     byte ptr [cs:L_01CF],0h
    jz      short L_0222
    call    L_05FC
L_0222:
    mov     byte ptr [cs:L_01D0],0h
    cmp     al,80h
    jc      short L_0234
    and     al,7Fh
    mov     byte ptr [cs:L_01D0],80h
L_0234:
    cmp     al,7h
    jz      short L_0247
    cmp     al,3h
    jna     short L_0247
    xor     ah,ah
    call    dword ptr [cs:L_012C]
    or      ah,ah
    jz      short L_025A
L_0247:
    mov     ax,0FFF2h
    int     33h
    mov     byte ptr [cs:L_01CF],0h
    mov     byte ptr [cs:L_01CD],0h
    jmp     short L_01F3
L_025A:
    mov     byte ptr [cs:L_01CF],al
    and     bl,0Fh
    mov     byte ptr [cs:L_01CD],bl
    mov     word ptr [cs:L_01C9],dx
    mov     word ptr [cs:L_01CB],cx
    call    L_05EE
    or      al,byte ptr [cs:L_01D0]
    mov     byte ptr [cs:L_01CE],1h
    xor     ah,ah
    pushf   
    call    dword ptr [cs:L_0124]
    mov     byte ptr [cs:L_01CE],0h
    cmp     byte ptr [cs:L_01CD],2h
    jnz     short L_02A6
    mov     ax,1010h
    mov     bx,0FFh
    mov     cx,3F3Fh
    mov     dh,ch
    pushf   
    cli     
    call    dword ptr [cs:L_0124]
L_02A6:
    mov     ax,0FFF1h
    mov     cl,1h
    mov     dl,cl
    int     33h
    mov     ax,7h
    xor     cx,cx
    mov     dx,word ptr [cs:L_01CB]
    dec     dx
    int     33h
    mov     ax,8h
    xor     cx,cx
    mov     dx,word ptr [cs:L_01C9]
    dec     dx
    int     33h
    jmp     near ptr L_01E9
L_02CC:
    dw      0h
L_02CE:
    dw      0h
L_02D0:
    db      32 dup(0)
L_02F0:
    dw      13h
    dw      0h
    dw      0h
    dw      0A000h
    dw      0h
    dw      5019h
    dw      0C8h
    dw      140h
    dw      140h
    dw      0h
    dw      108h
    dw      0h
    dw      offset L_030C
    dw      offset L_0340
L_030C:
    dw      5019h
    dw      0h
L_0310:
    dw      offset L_039B
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_03DB
    dw      offset L_04B6
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_0398
    dw      offset L_0398
L_0332:
    dw      0h
L_0334:
    dw      0h
L_0336:
    dw      0h
L_0338:
    dw      0h
L_033A:
    dw      0h
L_033C:
    dw      0h
L_033E:
    dw      0h

; 底层绘图
L_0340:
    cmp     bp,byte ptr -7h
    jc      short L_0349
    mov     ax,0C000h
    retf    
L_0349:
    mov     word ptr [cs:L_0332],bx
    mov     word ptr [cs:L_0334],cx
    mov     word ptr [cs:L_0336],dx
    mov     word ptr [cs:L_0338],si
    mov     word ptr [cs:L_033A],di
    mov     word ptr [cs:L_033C],ds
    mov     word ptr [cs:L_033E],es
    shl     bp,1
    push    cs
    call    cs:word ptr [bp+L_0310]
    mov     es,word ptr [cs:L_033E]
    mov     ds,word ptr [cs:L_033C]
    mov     di,word ptr [cs:L_033A]
    mov     si,word ptr [cs:L_0338]
    mov     dx,word ptr [cs:L_0336]
    mov     cx,word ptr [cs:L_0334]
    mov     bx,word ptr [cs:L_0332]
    retf    
L_0398:
    xor     ax,ax
    retf    
L_039B:
    mov     al,byte ptr [cs:L_01CF]
    mov     ah,1h
    call    dword ptr [cs:L_0134]
    retf    
    db      0h
    db      0h
    db      0h
    db      0h
    db      0h
    db      2Ah
    db      0h
    db      2Ah
    db      0h
    db      0h
    db      2Ah
    db      2Ah
    db      2Ah
    db      0h
    db      0h
    db      2Ah
    db      0h
    db      2Ah
    db      2Ah
    db      15h
    db      0h
    db      2Ah
    db      2Ah
    db      2Ah
    db      15h
    db      15h
    db      15h
    db      15h
    db      15h
    db      3Fh
    db      15h
    db      3Fh
    db      15h
    db      15h
    db      3Fh
    db      3Fh
    db      3Fh
    db      15h
    db      15h
    db      3Fh
    db      15h
    db      3Fh
    db      3Fh
    db      3Fh
    db      15h
    db      3Fh
    db      3Fh
    db      3Fh
L_03D7:
    dw      0h
    db      0h
L_03DA:
    db      0h
L_03DB:
    call    L_052C
    push    cs
    pop     es
    mov     al,bh
    shr     al,byte ptr 4h
    push    ax
    push    ax
    mov     bl,3h
    mul     bl
    mov     bx,ax
    mov     ax,[cs:bx]
    mov     word ptr [cs:L_03D7],ax
    mov     al,[cs:bx+3h]
    mov     byte ptr [cs:L_03DA],al
    mov     bp, offset L_03D7
    mov     si,word ptr [cs:L_01CB]
    mov     di,14h
    mov     dx,word ptr [cs:L_01C9]
    xor     cx,cx
    sub     dx,di
    pop     bx
    mov     al,bl
    xor     bh,bh
    shl     bx,byte ptr 2h
    mov     bp, offset L_04EC
    add     bp,bx
    mov     bx,401h
    mov     ah,4h
    call    dword ptr [cs:L_013C]
    pop     ax
    or      al,al
    jz      short L_0455
    xor     cx,cx
    mov     dx,word ptr [cs:L_01C9]
    sub     dx,14h
    mov     si,word ptr [cs:L_01CB]
    mov     di,dx
    mov     bp, offset L_04EC
    mov     ax,200h
    call    dword ptr [cs:L_013C]
    inc     dx
    inc     di
    mov     bp, offset L_0528
    mov     ax,20Fh
    call    dword ptr [cs:L_013C]
L_0455:
    call    L_057D
    retf    
L_0459:
    push    ax
    push    cx
    push    dx
    push    ds
    push    es
    mov     dx,ax
    int     7Fh
    mov     ds,dx
    xor     si,si
    mov     di, offset L_02D0
    mov     cx,10h
    push    cs
    pop     es
    rep     movsw
    pop     es
    pop     ds
    pop     dx
    pop     cx
    pop     ax
    ret     
L_0476:
    mov     dh,bh
    les     bx,dword ptr [cs:L_0128]
    mov     [es:bx],ax
    mov     [es:bx+4h],cs
    mov     [es:bx+2h],si
    mov     word ptr [es:bx+6h],8h
    or      ah,ah
    jz      short L_0496
    shl     word ptr [es:bx+6h],1
L_0496:
    mov     word ptr [es:bx+8h],10h
    mov     [es:bx+0Ah],bp
    mov     [es:bx+0Ch],di
    mov     al,dh
    shr     al,byte ptr 4h
    mov     [es:bx+0Eh],al
    mov     al,dh
    and     al,0Fh
    mov     [es:bx+0Fh],al
    ret     
L_04B6:
    call    L_052C
    call    L_0459
    push    ax
    mov     ax,8h
    mul     dl
    mov     di,ax
    mov     bp,word ptr [cs:L_01C9]
    sub     bp,11h
    pop     ax
    mov     si, offset L_02D0
    call    L_0476
    mov     ax,1h
    cli     
    call    dword ptr [cs:L_0130]
    sti     
    call    L_057D
    retf    
L_04E1:
    db      0h
L_04E2:
    dw      0h
L_04E4:
    dw      0h
L_04E6:
    dw      0h
L_04E8:
    dw      0h
L_04EA:
    dw      0h
L_04EC:
    dw      0h
    dw      0h
    dw      0h
    db      80h
    db      0h
    db      0h
    db      80h
    db      0h
    db      0h
    db      0h
    db      80h
    db      80h
    db      0h
    db      80h
    db      0h
    db      0h
    db      0h
    db      80h
    db      0h
    db      80h
    db      0h
    db      80h
    db      80h
    db      0h
    db      0h
    db      0B0h
    db      0B0h
    db      0B0h
    db      0h
    db      80h
    db      80h
    db      80h
    db      0h
    db      0h
    db      0h
    db      0FFh
    db      0h
    db      0h
    db      0FFh
    db      0h
    db      0h
    db      0h
    db      0FFh
    db      0FFh
    db      0h
    db      0FFh
    db      0h
    db      0h
    db      0h
    db      0FFh
    db      0h
    db      0FFh
    db      0h
    db      0FFh
    db      0FFh
    db      0h
    db      0h
L_0528:
    db      0EBh
    db      0FFh
    db      0FFh
    db      0h
L_052C:
    pusha   
    push    ds
    push    cs
    pop     ds
    mov     ax,702h
    call    dword ptr [L_013C]
    mov     byte ptr [L_04E1],al
    mov     word ptr [L_04E2],cx
    mov     ax,901h
    call    dword ptr [L_013C]
    mov     word ptr [L_04E4],cx
    mov     word ptr [L_04E6],dx
    mov     word ptr [L_04E8],si
    mov     word ptr [L_04EA],di
    mov     ax,500h
    call    dword ptr [L_013C]
    mov     ah,6h
    mov     cx,0FFFFh
    call    dword ptr [L_013C]
    xor     cx,cx
    xor     dx,dx
    mov     si,word ptr [L_01CB]
    dec     si
    mov     di,word ptr [L_01C9]
    dec     di
    mov     ax,900h
    call    dword ptr [L_013C]
    pop     ds
    popa    
    ret     
L_057D:
    push    ds
    pusha   
    push    cs
    pop     ds
    mov     al,byte ptr [L_04E1]
    mov     ah,5h
    call    dword ptr [L_013C]
    mov     ah,6h
    mov     cx,word ptr [L_04E2]
    call    dword ptr [L_013C]
    mov     cx,word ptr [L_04E4]
    mov     dx,word ptr [L_04E6]
    mov     si,word ptr [L_04E8]
    mov     di,word ptr [L_04EA]
    mov     ax,900h
    call    dword ptr [L_013C]
    popa    
    pop     ds
    ret     
L_05AE:
    mov     ax,0FF00h
    mov     bl,3h
    mov     bh,byte ptr [cs:L_01CF]
    pushf   
    call    dword ptr [cs:L_0124]
    mov     word ptr [cs:L_02CC],bp
    mov     word ptr [cs:L_02CE],es
    push    cs
    pop     es
    mov     ax,0FF00h
    mov     bl,5h
    mov     bh,byte ptr [cs:L_01CF]
    pushf   
    call    dword ptr [cs:L_0124]
    mov     ax,0FF00h
    mov     bl,4h
    mov     bh,byte ptr [cs:L_01CF]
    mov     bp, offset L_02F0
    pushf   
    call    dword ptr [cs:L_0124]
    ret     
L_05EE:
    pusha   
    mov     al,byte ptr [cs:L_01CF]
    mov     byte ptr [cs:L_02F0],al
    call    L_05AE
    popa    
    ret     
L_05FC:
    pusha   
    push    ds
    push    es
    mov     bh,byte ptr [cs:L_01CF]
    mov     bp,word ptr [cs:L_02CC]
    mov     es,word ptr [cs:L_02CE]
    mov     bl,4h
    mov     ax,0FF00h
    or      bp,bp
    jnz     short L_0621
    push    bp
    mov     bp,es
    or      bp,bp
    pop     bp
    jnz     short L_0621
    mov     bl,5h
L_0621:
    pushf   
    call    dword ptr [cs:L_0124]
    mov     byte ptr [cs:L_01CF],0h
    pop     es
    pop     ds
    popa    
    ret     
L_0631:
    db      0h
L_0632:
    cmp     ah,0FFh
    jnz     short L_0645
    cmp     al,96h
    jnz     short L_0642
    pop     ax
    mov     ax,96FFh
    jmp     near ptr L_01EA
L_0642:
    jmp     near ptr L_01F3
L_0645:
    cmp     al,9h
    jz      short L_064B
    jmp     short L_0697
L_064B:
    cmp     bh,5h
    jz      short L_0653
    jmp     near ptr L_01E9
L_0653:
    mov     byte ptr [cs:L_0631],bl
    les     bx,dword ptr [cs:L_0128]
    mov     word ptr [es:bx],0h
    lodsw   
    mov     [es:bx+2h],ax
    lodsw   
    mov     [es:bx+4h],ax
    lodsw   
    mov     [es:bx+6h],ax
    lodsw   
    mov     [es:bx+8h],ax
    lodsw   
    mov     [es:bx+0Ch],ax
    lodsw   
    mov     [es:bx+0Ah],ax
    lodsw   
    mov     [es:bx+0Eh],ax
    xor     ah,ah
    mov     al,byte ptr [cs:L_0631]
    inc     al
    cli     
    call    dword ptr [cs:L_0130]
    sti     
    jmp     near ptr L_01E9
L_0697:
    cmp     al,0Fh
    jnz     short L_06A6
    pop     ax
    mov     ah,2h
    call    dword ptr [cs:L_012C]
    jmp     near ptr L_01EA
L_06A6:
    cmp     al,14h
    jz      short L_06AC
    jmp     short L_06DA
L_06AC:
    cmp     bh,5h
    jnz     short L_06BC
    pop     ax
    mov     ax,cx
    call    dword ptr [cs:L_0138]
    jmp     near ptr L_01EA
L_06BC:
    cmp     bh,8h
    jnz     short L_06D7
    cmp     byte ptr [cs:L_01CF],0h
    ja      short L_06D7
    mov     ax,3h
    call    dword ptr [cs:L_012C]
    mov     al,bh
    xor     ah,ah
    int     10h
L_06D7:
    jmp     near ptr L_01E9
L_06DA:
    cmp     al,35h
    jz      short L_06E0
    jmp     short L_0719
L_06E0:
    or      bh,bh
    jnz     short L_06F6
    pop     ax
    pop     ds
    pop     es
    pop     bp
    pop     di
    pop     si
    pop     dx
    pop     cx
    pop     bx
    push    cs
    pop     es
    mov     bx, offset L_01D1
    mov     dx,0h
    iret    
L_06F6:
    cmp     bh,4h
    jnz     short L_0716
    or      bl,bl
    jz      short L_0702
    jmp     near ptr L_01E9
L_0702:
    pop     ax
    pop     ds
    pop     es
    pop     bp
    pop     di
    pop     si
    pop     dx
    pop     cx
    pop     bx
    mov     es,word ptr [cs:L_013E]
    mov     dx,word ptr [cs:L_013C]
    iret    
L_0716:
    jmp     near ptr L_01E9
L_0719:
    cmp     al,0FEh
    jnz     short L_073B
    pop     ax
    pop     ds
    pop     es
    pop     bp
    pop     di
    pop     si
    pop     dx
    pop     cx
    pop     bx
    mov     ax,word ptr [cs:L_01CB]
    mov     bl,19h
    mov     bh,0FFh
    mov     ch,10h
    mov     dl,byte ptr [cs:L_01CD]
    mov     di,word ptr [cs:L_01C9]
    iret    
L_073B:
    cmp     al,0FDh
    jz      short L_0742
    jmp     near ptr L_01F3
L_0742:
    pop     ax
    pop     ds
    pop     es
    pop     bp
    pop     di
    pop     si
    pop     dx
    pop     cx
    pop     bx
    xor     bx,bx
    mov     ax,500h
    iret    
L_0751:
    db      2048 dup(0)
L_0F51:
    db      2048 dup(0)
L_1751:
    db      0h
L_1752:
    db      0h
L_1753:
    dw      0h
L_1755:
    dw      0h
L_1757:
    dw      0h
L_1759:
    dw      0h
L_175B:
    dw      1h
L_175D:
    db      4 dup(0)

; 字符输出与字库缓存
L_1761:
    cmp     byte ptr [L_175B],0Ch
    jc      short L_1799
    cld     
    push    cs
    pop     es
    push    si
    mov     si, offset L_2555
    mov     al,byte ptr [L_1751]
    mov     bl,3h
    mul     bl
    mov     bx,ax
    mov     di, offset L_175D
    mov     al,[bx+si]
    shl     al,byte ptr 2h
    stosb   
    mov     al,[bx+si+1h]
    shl     al,byte ptr 2h
    stosb   
    mov     al,[bx+si+2h]
    shl     al,byte ptr 2h
    stosb   
    xor     al,al
    stosb   
    mov     bp, offset L_175D
    pop     si
    cmp     bp,0h
L_1799:
    ret     
L_179A:
    push    es
    call    L_1761
    jnc     short L_17A3
    mov     al,byte ptr [L_1751]
L_17A3:
    mov     ah,0h
    mov     cx,word ptr [L_1753]
    mov     dx,word ptr [L_1755]
    call    dword ptr [cs:L_013C]
    pop     es
    ret     
L_17B4:
    mov     cx,word ptr [L_1753]
    mov     dx,word ptr [L_1755]
    mov     ah,1h
    push    es
    push    bp
    push    cs
    pop     es
    mov     bp, offset L_175D
    call    dword ptr [cs:L_013C]
    cmp     byte ptr [L_175B],0Ch
    jc      short L_17D4
    call    L_24D6
L_17D4:
    mov     byte ptr [L_1752],al
    pop     bp
    pop     es
    ret     
L_17DA:
    push    es
    call    L_1761
    jnc     short L_17E3
    mov     al,byte ptr [L_1751]
L_17E3:
    mov     ah,2h
    mov     cx,word ptr [L_1753]
    mov     dx,word ptr [L_1755]
    mov     si,word ptr [L_1757]
    mov     di,word ptr [L_1759]
    call    dword ptr [cs:L_013C]
    pop     es
    ret     
L_17FC:
    db      0h
L_17FD:
    dw      0h
L_17FF:
    dw      0h
L_1801:
    dw      0h
L_1803:
    dw      0h
L_1805:
    dw      0FFFFh
L_1807:
    dw      0FFFFh
L_1809:
    db      0h
L_180A:
    db      0h
L_180B:
    dw      0h
L_180D:
    dw      0h
L_180F:
    dw      0h
L_1811:
    dw      0h
    db      0h
L_1814:
    db      0h
L_1815:
    db      0h
L_1816:
    db      0h
L_1817:
    db      0h

; 绘圆与圆弧
L_1818:
    push    ax
    call    L_179A
    pop     ax
    ret     
L_181E:
    mov     ax,word ptr [L_1753]
    mov     cx,word ptr [L_1757]
    cmp     cx,ax
    jnl     short L_1840
    mov     bx,word ptr [L_1755]
    mov     dx,word ptr [L_1759]
    mov     word ptr [L_1753],cx
    mov     word ptr [L_1755],dx
    mov     word ptr [L_1757],ax
    mov     word ptr [L_1759],bx
L_1840:
    jmp     short L_17DA
L_1842:
    push    es
    mov     byte ptr [L_1751],0h
    call    L_18F5
    inc     word ptr [L_1753]
    inc     word ptr [L_1755]
    dec     word ptr [L_1757]
    dec     word ptr [L_1759]
    mov     ax,word ptr [L_1753]
    mov     word ptr [L_180B],ax
    mov     ax,word ptr [L_1755]
    mov     word ptr [L_180D],ax
    mov     ax,word ptr [L_1757]
    mov     word ptr [L_180F],ax
    mov     ax,word ptr [L_1759]
    mov     word ptr [L_1811],ax
L_1873:
    mov     al,byte ptr [L_1814]
    mov     byte ptr [L_1751],al
    push    word ptr [L_1759]
    mov     ax,word ptr [L_1755]
    mov     word ptr [L_1759],ax
    call    L_181E
    pop     word ptr [L_1759]
    push    word ptr [L_1757]
    mov     ax,word ptr [L_1753]
    mov     word ptr [L_1757],ax
    call    L_181E
    pop     word ptr [L_1757]
    mov     al,byte ptr [L_1815]
    mov     byte ptr [L_1751],al
    push    word ptr [L_1755]
    mov     ax,word ptr [L_1759]
    mov     word ptr [L_1755],ax
    call    L_181E
    pop     word ptr [L_1755]
    push    word ptr [L_1753]
    mov     ax,word ptr [L_1757]
    mov     word ptr [L_1753],ax
    call    L_181E
    pop     word ptr [L_1753]
    inc     word ptr [L_1753]
    inc     word ptr [L_1755]
    dec     word ptr [L_1757]
    dec     word ptr [L_1759]
    dec     byte ptr [L_1809]
    jnz     short L_1873
    pop     es
    cmp     byte ptr [L_180A],2h
    jc      short L_18ED
    mov     byte ptr [L_1809],1h
    mov     ax,es
    mov     byte ptr [L_1751],al
    jmp     short L_18EE
L_18ED:
    ret     
L_18EE:
    cmp     byte ptr [L_1809],0h
    ja      short L_193A
L_18F5:
    push    word ptr [L_1759]
    mov     ax,word ptr [L_1755]
    mov     word ptr [L_1759],ax
    call    L_181E
    pop     word ptr [L_1759]
    push    word ptr [L_1753]
    mov     ax,word ptr [L_1757]
    mov     word ptr [L_1753],ax
    call    L_181E
    pop     word ptr [L_1753]
    push    word ptr [L_1755]
    mov     ax,word ptr [L_1759]
    mov     word ptr [L_1755],ax
    call    L_181E
    pop     word ptr [L_1755]
    push    word ptr [L_1757]
    mov     ax,word ptr [L_1753]
    mov     word ptr [L_1757],ax
    call    L_181E
    pop     word ptr [L_1757]
    ret     
L_193A:
    mov     ax,word ptr [L_1753]
    cmp     ax,word ptr [L_1757]
    jng     short L_194E
    push    word ptr [L_1757]
    mov     word ptr [L_1757],ax
    pop     word ptr [L_1753]
L_194E:
    mov     ax,word ptr [L_1755]
    cmp     ax,word ptr [L_1759]
    jng     short L_1962
    push    word ptr [L_1759]
    mov     word ptr [L_1759],ax
    pop     word ptr [L_1755]
L_1962:
    cmp     byte ptr [L_1809],1h
    jnz     short L_1988
L_1969:
    push    word ptr [L_1759]
    mov     ax,word ptr [L_1755]
    mov     word ptr [L_1759],ax
    call    L_181E
    pop     word ptr [L_1759]
    inc     word ptr [L_1755]
    mov     ax,word ptr [L_1755]
    cmp     ax,word ptr [L_1759]
    jng     short L_1969
    ret     
L_1988:
    test    byte ptr [L_1809],80h
    jnz     short L_1992
    call    L_18F5
L_1992:
    cmp     byte ptr [L_1809],82h
    jz      short L_19A0
    cmp     byte ptr [L_1809],2h
    jnz     short L_19DA
L_19A0:
    mov     cl,byte ptr [L_1816]
    xor     ch,ch
L_19A6:
    push    word ptr [L_1759]
    mov     ax,word ptr [L_1755]
    mov     word ptr [L_1759],ax
    push    cx
    call    L_181E
    pop     cx
    pop     word ptr [L_1759]
    inc     word ptr [L_1755]
    mov     ax,word ptr [L_1755]
    cmp     ax,word ptr [L_1759]
    jg      short L_19D9
    loop    short L_19A6
    mov     cl,byte ptr [L_1817]
    add     word ptr [L_1755],cx
    mov     ax,word ptr [L_1755]
    cmp     ax,word ptr [L_1759]
    jng     short L_19A0
L_19D9:
    ret     
L_19DA:
    cmp     byte ptr [L_1809],83h
    jz      short L_19E8
    cmp     byte ptr [L_1809],3h
    jnz     short L_1A22
L_19E8:
    mov     cl,byte ptr [L_1816]
    xor     ch,ch
L_19EE:
    push    word ptr [L_1757]
    mov     ax,word ptr [L_1753]
    mov     word ptr [L_1757],ax
    push    cx
    call    L_181E
    pop     cx
    pop     word ptr [L_1757]
    inc     word ptr [L_1753]
    mov     ax,word ptr [L_1753]
    cmp     ax,word ptr [L_1757]
    jg      short L_1A21
    loop    short L_19EE
    mov     cl,byte ptr [L_1817]
    add     word ptr [L_1753],cx
    mov     ax,word ptr [L_1753]
    cmp     ax,word ptr [L_1757]
    jng     short L_19E8
L_1A21:
    ret     
L_1A22:
    mov     al,byte ptr [L_1816]
    mov     byte ptr [L_1751],al
    test    byte ptr [L_1809],80h
    jnz     short L_1A3F
    inc     word ptr [L_1755]
    inc     word ptr [L_1753]
    dec     word ptr [L_1759]
    dec     word ptr [L_1757]
L_1A3F:
    push    cx
    call    L_1761
    mov     al,byte ptr [L_1751]
    mov     ah,4h
    mov     bx,401h
    mov     cx,word ptr [L_1753]
    mov     dx,word ptr [L_1755]
    mov     si,word ptr [L_1757]
    mov     di,word ptr [L_1759]
    sub     si,cx
    inc     si
    sub     di,dx
    inc     di
    call    dword ptr [cs:L_013C]
    pop     cx
    ret     
L_1A68:
    push    cx
    push    dx
    mov     ax,1h
    sub     ax,word ptr [L_180D]
    shl     ax,1
    mov     word ptr [L_1811],ax
L_1A76:
    mov     ax,word ptr [L_180D]
    cmp     ax,word ptr [L_180B]
    jnc     short L_1A92
    pop     di
    pop     si
    test    byte ptr [L_1809],7Fh
    jz      short L_1A91
    mov     ch,byte ptr [L_1751]
    mov     cl,ch
    jmp     near ptr L_2159
L_1A91:
    ret     
L_1A92:
    mov     ax,word ptr [L_1753]
    mov     bx,word ptr [L_1755]
    push    ax
    push    bx
    add     ax,word ptr [L_180B]
    add     bx,word ptr [L_180D]
    call    L_1B5F
    pop     bx
    pop     ax
    push    ax
    push    bx
    sub     ax,word ptr [L_180B]
    add     bx,word ptr [L_180D]
    call    L_1B5F
    pop     bx
    pop     ax
    push    ax
    push    bx
    add     ax,word ptr [L_180B]
    sub     bx,word ptr [L_180D]
    call    L_1B5F
    pop     bx
    pop     ax
    push    ax
    push    bx
    sub     ax,word ptr [L_180B]
    sub     bx,word ptr [L_180D]
    call    L_1B5F
    pop     bx
    pop     ax
    push    ax
    push    bx
    add     ax,word ptr [L_180D]
    add     bx,word ptr [L_180B]
    call    L_1B5F
    pop     bx
    pop     ax
    push    ax
    push    bx
    sub     ax,word ptr [L_180D]
    add     bx,word ptr [L_180B]
    call    L_1B5F
    pop     bx
    pop     ax
    push    ax
    push    bx
    add     ax,word ptr [L_180D]
    sub     bx,word ptr [L_180B]
    call    L_1B5F
    pop     bx
    pop     ax
    push    ax
    push    bx
    sub     ax,word ptr [L_180D]
    sub     bx,word ptr [L_180B]
    call    L_1B5F
    pop     bx
    pop     ax
    mov     word ptr [L_1753],ax
    mov     word ptr [L_1755],bx
    mov     ax,word ptr [L_1811]
    add     ax,word ptr [L_180D]
    test    ax,8000h
    jnz     short L_1B38
    dec     word ptr [L_180D]
    mov     ax,word ptr [L_180D]
    shl     ax,1
    inc     ax
    mov     bx,word ptr [L_1811]
    sub     bx,ax
    mov     word ptr [L_1811],bx
L_1B38:
    test    word ptr [L_1811],8000h
    jnz     short L_1B4C
    mov     ax,word ptr [L_180B]
    cmp     ax,word ptr [L_1811]
    ja      short L_1B4C
L_1B49:
    jmp     near ptr L_1A76
L_1B4C:
    inc     word ptr [L_180B]
    mov     ax,word ptr [L_180B]
    shl     ax,1
    inc     ax
    add     ax,word ptr [L_1811]
    mov     word ptr [L_1811],ax
    jmp     short L_1B49
L_1B5F:
    mov     word ptr [L_1753],ax
    mov     word ptr [L_1755],bx
    jmp     near ptr L_1818
L_1B69:
    db      0h
    db      2h
    db      4h
    db      5h
    db      7h
    db      9h
    db      0Bh
    db      0Ch
    db      0Eh
    db      10h
    db      12h
    db      14h
    db      15h
    db      17h
    db      19h
    db      1Bh
    db      1Ch
    db      1Eh
    db      20h
    db      22h
    db      24h
    db      25h
    db      27h
    db      29h
    db      2Bh
    db      2Ch
    db      2Eh
    db      30h
    db      32h
    db      33h
    db      35h
    db      37h
    db      39h
    db      3Ah
    db      3Ch
    db      3Eh
    db      40h
    db      41h
    db      43h
    db      45h
    db      47h
    db      48h
    db      4Ah
    db      4Ch
    db      4Dh
    db      4Fh
    db      51h
    db      52h
    db      54h
    db      56h
    db      57h
    db      59h
    db      5Bh
    db      5Dh
    db      5Eh
    db      60h
    db      61h
    db      63h
    db      65h
    db      66h
    db      68h
    db      6Ah
    db      6Bh
    db      6Dh
    db      6Fh
    db      70h
    db      72h
    db      73h
    db      75h
    db      77h
    db      78h
    db      7Ah
    db      7Bh
    db      7Dh
    db      7Eh
    db      80h
    db      81h
    db      83h
    db      85h
    db      86h
    db      88h
    db      89h
    db      8Bh
    db      8Ch
    db      8Eh
    db      8Fh
    db      91h
    db      92h
    db      94h
    db      95h
    db      96h
    db      98h
    db      99h
    db      9Bh
    db      9Ch
    db      9Eh
    db      9Fh
    db      0A0h
    db      0A2h
    db      0A3h
    db      0A4h
    db      0A6h
    db      0A7h
    db      0A9h
    db      0AAh
    db      0ABh
    db      0ADh
    db      0AEh
    db      0AFh
    db      0B0h
    db      0B2h
    db      0B3h
    db      0B4h
    db      0B6h
    db      0B7h
    db      0B8h
    db      0B9h
    db      0BBh
    db      0BCh
    db      0BDh
    db      0BEh
    db      0BFh
    db      0C1h
    db      0C2h
    db      0C3h
    db      0C4h
    db      0C5h
    db      0C6h
    db      0C7h
    db      0C9h
    db      0CAh
    db      0CBh
    db      0CCh
    db      0CDh
    db      0CEh
    db      0CFh
    db      0D0h
    db      0D1h
    db      0D2h
    db      0D3h
    db      0D4h
    db      0D5h
    db      0D6h
    db      0D7h
    db      0D8h
    db      0D9h
    db      0DAh
    db      0DBh
    db      0DCh
    db      0DDh
    db      0DEh
    db      0DFh
    db      0DFh
    db      0E0h
    db      0E1h
    db      0E2h
    db      0E3h
    db      0E4h
    db      0E4h
    db      0E5h
    db      0E6h
    db      0E7h
    db      0E8h
    db      0E8h
    db      0E9h
    db      0EAh
    db      0EBh
    db      0EBh
    db      0ECh
    db      0EDh
    db      0EDh
    db      0EEh
    db      0EFh
    db      0EFh
    db      0F0h
    db      0F0h
    db      0F1h
    db      0F2h
    db      0F2h
    db      0F3h
    db      0F3h
    db      0F4h
    db      0F4h
    db      0F5h
    db      0F6h
    db      0F6h
    db      0F7h
    db      0F7h
    db      0F7h
    db      0F8h
    db      0F8h
    db      0F9h
    db      0F9h
    db      0FAh
    db      0FAh
    db      0FAh
    db      0FBh
    db      0FBh
    db      0FBh
    db      0FCh
    db      0FCh
    db      0FCh
    db      0FDh
    db      0FDh
    db      0FDh
    db      0FDh
    db      0FEh
    db      0FEh
    db      0FEh
    db      0FEh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
    db      0FFh
L_1C4B:
    db      0h
L_1C4C:
    dw      0h
L_1C4E:
    dw      0h
L_1C50:
    dw      0h
L_1C52:
    dw      0h
L_1C54:
    dw      0h
L_1C56:
    dw      0h
L_1C58:
    dw      0h

; 多边形光栅化与区域填充
L_1C5A:
    mov     word ptr [L_1F8D],0FFFFh
    mov     word ptr [L_1F85],0FFFFh
    mov     word ptr [L_1F87],0FFFFh
    mov     word ptr [L_1F8F],0h
    mov     word ptr [L_1F89],0h
    mov     word ptr [L_1F8B],0h
    mov     al,[es:si]
    mov     byte ptr [L_1C4B],al
    mov     al,[es:si+1h]
    mov     byte ptr [L_1751],al
    mov     ax,[es:si+2h]
    mov     word ptr [L_1753],ax
    mov     word ptr [L_180B],ax
    mov     ax,[es:si+4h]
    mov     word ptr [L_1755],ax
    mov     word ptr [L_180D],ax
    mov     ax,[es:si+6h]
    mov     word ptr [L_1C4C],ax
    mov     ax,[es:si+8h]
    mov     word ptr [L_1C4E],ax
    mov     byte ptr [L_1F83],0h
    mov     ax,[es:si+0Ch]
    sub     ax,[es:si+0Ah]
    cmp     ax,168h
    jnz     short L_1CC4
    mov     byte ptr [L_1F83],1h
L_1CC4:
    mov     ax,[es:si+0Ah]
    mov     cx,0FBh
    mul     cx
    mov     cx,64h
    div     cx
    cmp     dx,32h
    jc      short L_1CD8
    inc     ax
L_1CD8:
    mov     word ptr [L_1C50],ax
    mov     ax,[es:si+0Ch]
    mov     cx,0FBh
    mul     cx
    mov     cx,64h
    div     cx
    cmp     dx,32h
    jc      short L_1CEF
    inc     ax
L_1CEF:
    mov     word ptr [L_1C52],ax
    cmp     ax,word ptr [L_1C50]
    jnz     short L_1D09
    mov     byte ptr [L_1F83],1h
    mov     word ptr [L_1C50],0h
    mov     word ptr [L_1C52],388h
L_1D09:
    mov     ax,[es:si+0Eh]
    mov     word ptr [L_1C54],ax
    push    cs
    pop     es
    mov     cx,word ptr [L_1C50]
    mov     dx,word ptr [L_1C52]
    cmp     dx,cx
    jnc     short L_1D23
    sub     cx,dx
    mov     dx,388h
L_1D23:
    sub     dx,cx
    jnz     short L_1D28
    inc     dx
L_1D28:
    mov     word ptr [L_1C58],dx
    call    L_1DC4
    cmp     byte ptr [L_1C4B],0h
    ja      short L_1D37
    ret     
L_1D37:
    cmp     byte ptr [L_1F83],0h
    ja      short L_1D51
    mov     cx,word ptr [L_180B]
    mov     dx,word ptr [L_180D]
    mov     word ptr [L_1757],cx
    mov     word ptr [L_1759],dx
    call    L_181E
L_1D51:
    cmp     byte ptr [L_1C4B],2h
    jnz     short L_1DC3
    mov     ax,word ptr [L_1F8F]
    sub     ax,word ptr [L_1F8D]
    cmp     ax,2h
    jc      short L_1DC3
    mov     ax,word ptr [L_1F89]
    cmp     ax,word ptr [L_1F85]
    jnc     short L_1D71
    xchg    ax,word ptr [L_1F85]
L_1D71:
    sub     ax,word ptr [L_1F85]
    shr     ax,1
    add     ax,word ptr [L_1F85]
    cmp     ax,word ptr [L_180B]
    jz      short L_1D87
    jc      short L_1D86
    dec     ax
    jmp     short L_1D87
L_1D86:
    inc     ax
L_1D87:
    mov     bx,ax
    mov     ax,word ptr [L_1F8B]
    sub     ax,word ptr [L_1F87]
    cmp     ax,2h
    jc      short L_1DC3
    shr     ax,1
    add     ax,word ptr [L_1F87]
    cmp     ax,word ptr [L_180D]
    jz      short L_1DA7
    jc      short L_1DA6
    dec     ax
    jmp     short L_1DA7
L_1DA6:
    inc     ax
L_1DA7:
    cmp     bx,word ptr [L_180B]
    jnz     short L_1DB4
    cmp     ax,word ptr [L_180D]
    jnz     short L_1DB4
    dec     bx
L_1DB4:
    mov     si,bx
    mov     di,ax
    mov     cl,byte ptr [L_1751]
    mov     ch,byte ptr [L_1C54]
    jmp     near ptr L_2159
L_1DC3:
    ret     
L_1DC4:
    mov     byte ptr [L_1F84],0h
L_1DC9:
    cmp     word ptr [L_1C50],0E2h
    ja      short L_1E37
    mov     cx,0E2h
    xor     si,si
    mov     di,0E1h
    mov     word ptr [L_1C56],0h
L_1DDF:
    mov     ax,word ptr [L_1C50]
    cmp     ax,word ptr [L_1C56]
    ja      short L_1E2F
    push    cx
    mov     ax,word ptr [L_1C4C]
    mov     cl,byte ptr [di+L_1B69]
    xor     ch,ch
    mul     cx
    mov     cx,100h
    div     cx
    mov     cx,ax
    add     cx,word ptr [L_180B]
    mov     ax,word ptr [L_1C4E]
    mov     bl,byte ptr [si+L_1B69]
    xor     bh,bh
    mul     bx
    mov     bx,100h
    div     bx
    mov     dx,word ptr [L_180D]
    sub     dx,ax
    jnc     short L_1E1E
    mov     byte ptr [L_1F84],0h
    jmp     short L_1E21
L_1E1E:
    call    L_1F91
L_1E21:
    pop     cx
    dec     word ptr [L_1C58]
    mov     ax,word ptr [L_1C58]
    cmp     ax,0FFFFh
    jnz     short L_1E2F
    ret     
L_1E2F:
    inc     si
    dec     di
    inc     word ptr [L_1C56]
    loop    short L_1DDF
L_1E37:
    cmp     word ptr [L_1C50],1C4h
    ja      short L_1EA7
    xor     di,di
    mov     si,0E1h
    mov     cx,0E2h
    mov     word ptr [L_1C56],0E1h
L_1E4D:
    mov     ax,word ptr [L_1C50]
    cmp     ax,word ptr [L_1C56]
    ja      short L_1E9F
    push    cx
    mov     ax,word ptr [L_1C4C]
    mov     cl,byte ptr [di+L_1B69]
    xor     ch,ch
    mul     cx
    mov     cx,100h
    div     cx
    mov     cx,word ptr [L_180B]
    sub     cx,ax
    jc      short L_1E87
    mov     ax,word ptr [L_1C4E]
    mov     bl,byte ptr [si+L_1B69]
    xor     bh,bh
    mul     bx
    mov     bx,100h
    div     bx
    mov     dx,word ptr [L_180D]
    sub     dx,ax
    jnc     short L_1E8E
L_1E87:
    mov     byte ptr [L_1F84],0h
    jmp     short L_1E91
L_1E8E:
    call    L_1F91
L_1E91:
    pop     cx
    dec     word ptr [L_1C58]
    mov     ax,word ptr [L_1C58]
    cmp     ax,0FFFFh
    jnz     short L_1E9F
    ret     
L_1E9F:
    dec     si
    inc     di
    inc     word ptr [L_1C56]
    loop    short L_1E4D
L_1EA7:
    cmp     word ptr [L_1C50],2A6h
    ja      short L_1F15
    xor     si,si
    mov     di,0E1h
    mov     cx,0E2h
    mov     word ptr [L_1C56],1C3h
L_1EBD:
    mov     ax,word ptr [L_1C50]
    cmp     ax,word ptr [L_1C56]
    ja      short L_1F0D
    push    cx
    mov     ax,word ptr [L_1C4C]
    mov     cl,byte ptr [di+L_1B69]
    xor     ch,ch
    mul     cx
    mov     cx,100h
    div     cx
    mov     cx,word ptr [L_180B]
    sub     cx,ax
    jnc     short L_1EE6
    mov     byte ptr [L_1F84],0h
    jmp     short L_1EFF
L_1EE6:
    mov     ax,word ptr [L_1C4E]
    mov     bl,byte ptr [si+L_1B69]
    xor     bh,bh
    mul     bx
    mov     bx,100h
    div     bx
    mov     dx,word ptr [L_180D]
    add     dx,ax
    call    L_1F91
L_1EFF:
    pop     cx
    dec     word ptr [L_1C58]
    mov     ax,word ptr [L_1C58]
    cmp     ax,0FFFFh
    jnz     short L_1F0D
    ret     
L_1F0D:
    inc     si
    dec     di
    inc     word ptr [L_1C56]
    loop    short L_1EBD
L_1F15:
    cmp     word ptr [L_1C50],388h
    ja      short L_1F7A
    xor     di,di
    mov     si,0E1h
    mov     cx,0E2h
    mov     word ptr [L_1C56],2A5h
L_1F2B:
    mov     ax,word ptr [L_1C50]
    cmp     ax,word ptr [L_1C56]
    ja      short L_1F72
    push    cx
    mov     ax,word ptr [L_1C4C]
    mov     cl,byte ptr [di+L_1B69]
    xor     ch,ch
    mul     cx
    mov     cx,100h
    div     cx
    mov     cx,ax
    add     cx,word ptr [L_180B]
    mov     ax,word ptr [L_1C4E]
    mov     bl,byte ptr [si+L_1B69]
    xor     bh,bh
    mul     bx
    mov     bx,100h
    div     bx
    mov     dx,word ptr [L_180D]
    add     dx,ax
    call    L_1F91
    pop     cx
    dec     word ptr [L_1C58]
    mov     ax,word ptr [L_1C58]
    cmp     ax,0FFFFh
    jnz     short L_1F72
    ret     
L_1F72:
    dec     si
    inc     di
    inc     word ptr [L_1C56]
    loop    short L_1F2B
L_1F7A:
    mov     word ptr [L_1C50],0h
    jmp     near ptr L_1DC9
L_1F83:
    db      0h
L_1F84:
    db      0h
L_1F85:
    dw      0FFFFh
L_1F87:
    dw      0FFFFh
L_1F89:
    dw      0h
L_1F8B:
    dw      0h
L_1F8D:
    dw      0FFFFh
L_1F8F:
    dw      0h
L_1F91:
    cmp     cx,word ptr [L_1F8D]
    jnc     short L_1F9B
    mov     word ptr [L_1F8D],cx
L_1F9B:
    cmp     cx,word ptr [L_1F8F]
    jna     short L_1FA5
    mov     word ptr [L_1F8F],cx
L_1FA5:
    cmp     dx,word ptr [L_1F87]
    jnc     short L_1FB3
    mov     word ptr [L_1F85],cx
    mov     word ptr [L_1F87],dx
L_1FB3:
    cmp     dx,word ptr [L_1F8B]
    jna     short L_1FC1
    mov     word ptr [L_1F89],cx
    mov     word ptr [L_1F8B],dx
L_1FC1:
    cmp     byte ptr [L_1F83],0h
    ja      short L_1FCF
    cmp     byte ptr [L_1C4B],0h
    ja      short L_1FE4
L_1FCF:
    cmp     byte ptr [L_1F84],0h
    ja      short L_1FE4
    mov     word ptr [L_1753],cx
    mov     word ptr [L_1755],dx
    mov     byte ptr [L_1F84],1h
    ret     
L_1FE4:
    mov     word ptr [L_1757],cx
    mov     word ptr [L_1759],dx
    pusha   
    call    L_181E
    popa    
    mov     word ptr [L_1753],cx
    mov     word ptr [L_1755],dx
    ret     
L_1FFA:
    dw      0h
L_1FFC:
    dw      0h
L_1FFE:
    dw      0h
L_2000:
    dw      0h
L_2002:
    dw      0h
L_2004:
    dw      0h
L_2006:
    dw      0h
L_2008:
    dw      0h
L_200A:
    dw      4080h
    dw      1020h
    dw      408h
    dw      102h
    dw      78F0h
    dw      1E3Ch
    dw      870Fh
    dw      0E1C3h
    dw      201h
    dw      804h
    dw      2010h
    dw      8040h
    dw      1E0Fh
    dw      783Ch
    dw      0E1F0h
    dw      87C3h
    dw      101h
    dw      101h
    dw      101h
    dw      0FF01h
    dw      4180h
    dw      1422h
    dw      1408h
    dw      4122h
    dw      80h
    dw      0h
    dw      8h
    dw      0h
    dw      88h
    dw      22h
    dw      88h
    dw      22h
    dw      1FFh
    dw      101h
    dw      10FFh
    dw      1010h
    dw      1408h
    dw      552Ah
    dw      142Ah
    dw      8008h
L_205A:
    db      255 dup(8)

; 图形视口管理
L_2159:
    mov     word ptr [L_1FFA],sp
    mov     word ptr [L_1753],si
    mov     word ptr [L_1755],di
    cmp     byte ptr [L_175B],0h
    jnz     short L_2178
    or      ch,ch
    jz      short L_2172
    mov     ch,0Fh
L_2172:
    or      cl,cl
    jz      short L_2178
    mov     cl,0Fh
L_2178:
    mov     byte ptr [L_1751],ch
    mov     byte ptr [L_1752],cl
    mov     ax,95FEh
    int     10h
    dec     ax
    dec     di
    mov     word ptr [L_1FFE],di
    mov     word ptr [L_1FFC],ax
    mov     ax,700h
    call    dword ptr [cs:L_013C]
    mov     ax,500h
    call    dword ptr [cs:L_013C]
    cmp     word ptr [L_2008],byte ptr +0h
    jnz     short L_21AF
    mov     ah,6h
    mov     cx,0FFFFh
    call    dword ptr [cs:L_013C]
L_21AF:
    mov     cx,word ptr [L_1753]
    mov     dx,word ptr [L_1755]
    call    L_2497
    jc      short L_2215
    cmp     al,byte ptr [L_1752]
    jz      short L_2215
    mov     word ptr [L_2006],0h
    mov     di, offset L_0F51
    mov     cx,word ptr [L_1753]
    mov     dx,word ptr [L_1755]
    mov     [di+FILL_FRAME14_Y],dx
    mov     word ptr [L_2004],dx
    call    L_221E
    mov     [di+FILL_FRAME14_LEFT],cx
    mov     word ptr [L_2000],cx
    mov     cx,word ptr [L_1753]
    call    L_2246
    mov     [di+FILL_FRAME14_RIGHT],cx
    mov     word ptr [L_2002],cx
    call    L_2441
    mov     word ptr [di+FILL_FRAME14_Y_STEP],1h
    call    L_22DB
    mov     ax,word ptr [L_2000]
    mov     [di+FILL_FRAME14_LEFT],ax
    mov     ax,word ptr [L_2002]
    mov     [di+FILL_FRAME14_RIGHT],ax
    mov     ax,word ptr [L_2004]
    mov     [di+FILL_FRAME14_Y],ax
    mov     word ptr [di+FILL_FRAME14_Y_STEP],0FFFFh
    call    L_22E1
L_2215:
    mov     ax,701h
    call    dword ptr [cs:L_013C]
    ret     
L_221E:
    call    L_2497
    jc      short L_2245
    cmp     al,byte ptr [L_1752]
    jnz     short L_2238
L_2229:
    inc     cx
    call    L_2497
    jc      short L_2235
    cmp     al,byte ptr [L_1752]
    jz      short L_2229
L_2235:
    dec     cx
    jmp     short L_2245
L_2238:
    dec     cx
    call    L_2497
    jc      short L_2244
    cmp     al,byte ptr [L_1752]
    jnz     short L_2238
L_2244:
    inc     cx
L_2245:
    ret     
L_2246:
    call    L_2497
    jc      short L_226D
    cmp     al,byte ptr [L_1752]
    jnz     short L_2260
L_2251:
    dec     cx
    call    L_2497
    jc      short L_225D
    cmp     al,byte ptr [L_1752]
    jz      short L_2251
L_225D:
    inc     cx
    jmp     short L_226D
L_2260:
    inc     cx
    call    L_2497
    jc      short L_226C
    cmp     al,byte ptr [L_1752]
    jnz     short L_2260
L_226C:
    dec     cx
L_226D:
    ret     
L_226E:
    push    ax
    push    bx
    push    cx
    mov     bx, offset L_0751
    mov     cx,word ptr [L_2006]
L_2278:
    jcxz    short L_2296
    mov     ax,[bx+FILL_SPAN6_LEFT]
    cmp     ax,[di+FILL_FRAME14_LEFT]
    jnz     short L_2290
    mov     ax,[bx+FILL_SPAN6_RIGHT]
    cmp     ax,[di+FILL_FRAME14_RIGHT]
    jnz     short L_2290
    mov     ax,[bx+FILL_SPAN6_Y]
    cmp     ax,[di+FILL_FRAME14_Y]
    jz      short L_2298
L_2290:
    add     bx,FILL_SPAN6_SIZE
    dec     cx
    jmp     short L_2278
L_2296:
    or      bx,bx
L_2298:
    pop     cx
    pop     bx
    pop     ax
    ret     
L_229C:
    call    L_226E
    jnz     short L_22A2
    ret     
L_22A2:
    cmp     word ptr [L_2006],0C8h
    jc      short L_22B1
    mov     sp,word ptr [L_1FFA]
    jmp     near ptr L_2215
L_22B1:
    push    ax
    push    bx
    push    dx
    mov     ax,word ptr [L_2006]
    mov     bx,ax
    shl     ax,1
    add     bx,ax
    shl     bx,1
    add     bx,offset L_0751
    mov     ax,[di+FILL_FRAME14_LEFT]
    mov     [bx+FILL_SPAN6_LEFT],ax
    mov     ax,[di+FILL_FRAME14_RIGHT]
    mov     [bx+FILL_SPAN6_RIGHT],ax
    mov     ax,[di+FILL_FRAME14_Y]
    mov     [bx+FILL_SPAN6_Y],ax
    inc     word ptr [L_2006]
    pop     dx
    pop     bx
    pop     ax
    ret     
L_22DB:
    call    L_226E
    jnz     short L_22E1
L_22E0:
    ret     
L_22E1:
    call    L_229C
L_22E4:
    mov     dx,[di+FILL_FRAME14_Y]
    cmp     word ptr [di+FILL_FRAME14_Y_STEP],1h
    jnz     short L_22FB
    cmp     dx,word ptr [L_1807]
    jnc     short L_22E0
    cmp     dx,word ptr [L_1FFE]
    jnc     short L_22E0
    jmp     short L_2305
L_22FB:
    cmp     dx,word ptr [L_1803]
    jna     short L_22E0
    or      dx,dx
    jz      short L_22E0
L_2305:
    add     dx,[di+FILL_FRAME14_Y_STEP]
    mov     cx,[di+FILL_FRAME14_LEFT]
L_230A:
    call    L_221E
    call    L_2497
    cmp     al,byte ptr [L_1752]
    jnz     short L_231F
    mov     ax,cx
    inc     ax
    cmp     ax,[di+FILL_FRAME14_RIGHT]
    jnc     short L_22E0
    inc     cx
L_231F:
    mov     ax,cx
    inc     ax
    cmp     ax,[di+FILL_FRAME14_LEFT]
    jnc     short L_236B
    push    cx
    push    dx
    mov     cx,[di+FILL_FRAME14_LEFT]
    mov     dx,[di+FILL_FRAME14_Y]
    dec     cx
    call    L_2246
    mov     ax,cx
    pop     dx
    pop     cx
    cmp     cx,ax
    jnc     short L_236B
    neg     word ptr [di+FILL_FRAME14_Y_STEP]
    call    L_229C
    neg     word ptr [di+FILL_FRAME14_Y_STEP]
    mov     [di+FILL_FRAME14_SAVE_CX],cx
    mov     [di+FILL_FRAME14_SAVE_DX],dx
    add     di,FILL_FRAME14_SIZE
    mov     [di+FILL_FRAME14_LEFT],cx
    mov     ax,[di-FILL_FRAME14_SIZE+FILL_FRAME14_LEFT]
    dec     ax
    mov     [di+FILL_FRAME14_RIGHT],ax
    mov     [di+FILL_FRAME14_Y],dx
    mov     ax,[di-FILL_FRAME14_SIZE+FILL_FRAME14_Y_STEP]
    neg     ax
    mov     [di+FILL_FRAME14_Y_STEP],ax
    call    L_22DB
    sub     di,FILL_FRAME14_SIZE
    mov     cx,[di+FILL_FRAME14_SAVE_CX]
    mov     dx,[di+FILL_FRAME14_SAVE_DX]
L_236B:
    mov     bx,cx
    call    L_2246
    mov     ax,cx
    dec     ax
    cmp     ax,[di+FILL_FRAME14_RIGHT]
    jng     short L_23C4
    push    cx
    push    dx
    mov     cx,[di+FILL_FRAME14_RIGHT]
    mov     dx,[di+FILL_FRAME14_Y]
    inc     cx
    call    L_221E
    mov     ax,cx
    pop     dx
    pop     cx
    cmp     ax,cx
    jnc     short L_23C4
    neg     word ptr [di+FILL_FRAME14_Y_STEP]
    call    L_229C
    neg     word ptr [di+FILL_FRAME14_Y_STEP]
    mov     [di+FILL_FRAME14_SAVE_BX],bx
    mov     [di+FILL_FRAME14_SAVE_CX],cx
    mov     [di+FILL_FRAME14_SAVE_DX],dx
    add     di,FILL_FRAME14_SIZE
    mov     ax,[di-FILL_FRAME14_SIZE+FILL_FRAME14_RIGHT]
    inc     ax
    mov     [di+FILL_FRAME14_LEFT],ax
    mov     [di+FILL_FRAME14_RIGHT],cx
    mov     [di+FILL_FRAME14_Y],dx
    mov     ax,[di-FILL_FRAME14_SIZE+FILL_FRAME14_Y_STEP]
    neg     ax
    mov     [di+FILL_FRAME14_Y_STEP],ax
    call    L_22DB
    sub     di,FILL_FRAME14_SIZE
    mov     bx,[di+FILL_FRAME14_SAVE_BX]
    mov     cx,[di+FILL_FRAME14_SAVE_CX]
    mov     dx,[di+FILL_FRAME14_SAVE_DX]
L_23C4:
    push    cx
    mov     cx,[di+FILL_FRAME14_RIGHT]
    call    L_2246
    call    L_2497
    cmp     al,byte ptr [L_1752]
    jnz     short L_23D5
    dec     cx
L_23D5:
    mov     ax,cx
    pop     cx
    cmp     ax,cx
    jz      short L_241F
    push    bx
    push    cx
    push    dx
    mov     si,cx
    mov     cx,bx
    call    L_2453
    pop     dx
    pop     cx
    pop     bx
    neg     word ptr [di+FILL_FRAME14_Y_STEP]
    call    L_229C
    neg     word ptr [di+FILL_FRAME14_Y_STEP]
    mov     [di+FILL_FRAME14_SAVE_BX],bx
    mov     [di+FILL_FRAME14_SAVE_CX],cx
    mov     [di+FILL_FRAME14_SAVE_DX],dx
    add     di,FILL_FRAME14_SIZE
    mov     [di+FILL_FRAME14_LEFT],bx
    mov     [di+FILL_FRAME14_RIGHT],cx
    mov     [di+FILL_FRAME14_Y],dx
    mov     ax,[di-FILL_FRAME14_SIZE+FILL_FRAME14_Y_STEP]
    mov     [di+FILL_FRAME14_Y_STEP],ax
    call    L_22DB
    sub     di,FILL_FRAME14_SIZE
    mov     bx,[di+FILL_FRAME14_SAVE_BX]
    mov     cx,[di+FILL_FRAME14_SAVE_CX]
    inc     cx
    mov     dx,[di+FILL_FRAME14_SAVE_DX]
    jmp     near ptr L_230A
L_241F:
    xchg    bx,[di+FILL_FRAME14_LEFT]
    xchg    cx,[di+FILL_FRAME14_RIGHT]
    xchg    dx,[di+FILL_FRAME14_Y]
    call    L_226E
    jz      short L_2432
    call    L_2441
    jmp     near ptr L_22E4
L_2432:
    mov     [di+FILL_FRAME14_LEFT],bx
    mov     [di+FILL_FRAME14_RIGHT],cx
    mov     [di+FILL_FRAME14_Y],dx
    neg     word ptr [di+FILL_FRAME14_Y_STEP]
    call    L_229C
    ret     
L_2441:
    push    bx
    push    cx
    push    dx
    mov     cx,[di+FILL_FRAME14_LEFT]
    mov     si,[di+FILL_FRAME14_RIGHT]
    mov     dx,[di+FILL_FRAME14_Y]
    call    L_2453
    pop     dx
    pop     cx
    pop     bx
    ret     
L_2453:
    push    di
    cmp     word ptr [L_2008],byte ptr +0h
    jz      short L_2475
    push    si
    push    cx
    mov     si,word ptr [L_2008]
    mov     cx,dx
    and     cx,7h
    add     si,cx
    mov     cl,[si]
    mov     ch,cl
    mov     ah,6h
    call    dword ptr [cs:L_013C]
    pop     cx
    pop     si
L_2475:
    mov     di,dx
    cmp     byte ptr [L_175B],0Ch
    jc      short L_248B
    push    bx
    push    cx
    push    dx
    push    si
    push    di
    call    L_1761
    pop     di
    pop     si
    pop     dx
    pop     cx
    pop     bx
L_248B:
    mov     al,byte ptr [L_1751]
    mov     ah,2h
    call    dword ptr [cs:L_013C]
    pop     di
    ret     
L_2497:
    cmp     cx,word ptr [L_1801]
    jc      short L_24D1
    cmp     dx,word ptr [L_1803]
    jc      short L_24D1
    cmp     cx,word ptr [L_1805]
    ja      short L_24D1
    cmp     dx,word ptr [L_1807]
    ja      short L_24D1
    cmp     cx,word ptr [L_1FFC]
    ja      short L_24D1
    cmp     dx,word ptr [L_1FFE]
    ja      short L_24D1
    mov     bp, offset L_175D
    mov     ah,1h
    call    dword ptr [cs:L_013C]
    cmp     byte ptr [L_175B],0Ch
    jc      short L_24CF
    call    L_24D6
L_24CF:
    clc     
    ret     
L_24D1:
    stc     
    ret     
L_24D3:
    db      0h
L_24D4:
    db      0h
L_24D5:
    db      0h
L_24D6:
    push    si
    push    bx
    push    cx
    push    dx
    mov     byte ptr [L_24D4],0FFh
    mov     bx, offset L_175D
    mov     dh,[bx]
    shr     dh,byte ptr 2h
    mov     ch,[bx+1h]
    shr     ch,byte ptr 2h
    mov     cl,[bx+2h]
    shr     cl,byte ptr 2h
    xor     bx,bx
    xor     al,al
    mov     si, offset L_2555
L_24FA:
    call    L_2518
    add     bx,3h
    inc     al
    jz      short L_250C
    cmp     al,byte ptr [L_17FC]
    ja      short L_250C
    jmp     short L_24FA
L_250C:
    pop     dx
    pop     cx
    pop     bx
    pop     si
    mov     al,byte ptr [L_24D5]
    and     al,byte ptr [L_17FC]
    ret     
L_2518:
    mov     byte ptr [L_24D3],0h
    mov     ah,dh
    sub     ah,[bx+si]
    jnc     short L_2525
    neg     ah
L_2525:
    add     byte ptr [L_24D3],ah
    mov     ah,ch
    sub     ah,[bx+si+1h]
    jnc     short L_2532
    neg     ah
L_2532:
    add     byte ptr [L_24D3],ah
    mov     ah,cl
    sub     ah,[bx+si+2h]
    jnc     short L_253F
    neg     ah
L_253F:
    add     byte ptr [L_24D3],ah
    mov     ah,byte ptr [L_24D3]
    cmp     ah,byte ptr [L_24D4]
    jnc     short L_2554
    mov     byte ptr [L_24D4],ah
    mov     byte ptr [L_24D5],al
L_2554:
    ret     
L_2555:
    db      0h
    db      0h
    db      0h
    db      0h
    db      0h
    db      2Ah
    db      0h
    db      2Ah
    db      0h
    db      0h
    db      2Ah
    db      2Ah
    db      2Ah
    db      0h
    db      0h
    db      2Ah
    db      0h
    db      2Ah
    db      2Ah
    db      15h
    db      0h
    db      2Ah
    db      2Ah
    db      2Ah
    db      15h
    db      15h
    db      15h
    db      15h
    db      15h
    db      3Fh
    db      15h
    db      3Fh
    db      15h
    db      15h
    db      3Fh
    db      3Fh
    db      3Fh
    db      15h
    db      15h
    db      3Fh
    db      15h
    db      3Fh
    db      3Fh
    db      3Fh
    db      15h
    db      3Fh
    db      3Fh
    db      3Fh
    db      0h
    db      0h
    db      0h
    db      1h
    db      1h
    db      1h
    db      2h
    db      2h
    db      2h
    db      3h
    db      3h
    db      3h
    db      4h
    db      4h
    db      4h
    db      5h
    db      5h
    db      5h
    db      6h
    db      6h
    db      6h
    db      7h
    db      7h
    db      7h
    db      8h
    db      8h
    db      8h
    db      9h
    db      9h
    db      9h
    db      0Ah
    db      0Ah
    db      0Ah
    db      0Bh
    db      0Bh
    db      0Bh
    db      0Ch
    db      0Ch
    db      0Ch
    db      0Dh
    db      0Dh
    db      0Dh
    db      0Eh
    db      0Eh
    db      0Eh
    db      0Fh
    db      0Fh
    db      0Fh
    db      10h
    db      10h
    db      10h
    db      11h
    db      11h
    db      11h
    db      12h
    db      12h
    db      12h
    db      13h
    db      13h
    db      13h
    db      14h
    db      14h
    db      14h
    db      15h
    db      15h
    db      15h
    db      16h
    db      16h
    db      16h
    db      17h
    db      17h
    db      17h
    db      18h
    db      18h
    db      18h
    db      19h
    db      19h
    db      19h
    db      1Ah
    db      1Ah
    db      1Ah
    db      1Bh
    db      1Bh
    db      1Bh
    db      1Ch
    db      1Ch
    db      1Ch
    db      1Dh
    db      1Dh
    db      1Dh
    db      1Eh
    db      1Eh
    db      1Eh
    db      1Fh
    db      1Fh
    db      1Fh
    db      20h
    db      20h
    db      20h
    db      21h
    db      21h
    db      21h
    db      22h
    db      22h
    db      22h
    db      23h
    db      23h
    db      23h
    db      24h
    db      24h
    db      24h
    db      25h
    db      25h
    db      25h
    db      26h
    db      26h
    db      26h
    db      27h
    db      27h
    db      27h
    db      28h
    db      28h
    db      28h
    db      29h
    db      29h
    db      29h
    db      2Ah
    db      2Ah
    db      2Ah
    db      2Bh
    db      2Bh
    db      2Bh
    db      2Ch
    db      2Ch
    db      2Ch
    db      2Dh
    db      2Dh
    db      2Dh
    db      2Eh
    db      2Eh
    db      2Eh
    db      2Fh
    db      2Fh
    db      2Fh
    db      30h
    db      30h
    db      30h
    db      31h
    db      31h
    db      31h
    db      32h
    db      32h
    db      32h
    db      33h
    db      33h
    db      33h
    db      34h
    db      34h
    db      34h
    db      35h
    db      35h
    db      35h
    db      36h
    db      36h
    db      36h
    db      37h
    db      37h
    db      37h
    db      38h
    db      38h
    db      38h
    db      39h
    db      39h
    db      39h
    db      3Ah
    db      3Ah
    db      3Ah
    db      3Bh
    db      3Bh
    db      3Bh
    db      3Ch
    db      3Ch
    db      3Ch
    db      3Dh
    db      3Dh
    db      3Dh
    db      3Eh
    db      3Eh
    db      3Eh
    db      3Fh
    db      3Fh
    db      3Fh
    db      3Fh
    db      3Eh
    db      3Eh
    db      3Fh
    db      3Dh
    db      3Dh
    db      3Fh
    db      3Ch
    db      3Ch
    db      3Fh
    db      3Bh
    db      3Bh
    db      3Fh
    db      3Ah
    db      3Ah
    db      3Fh
    db      39h
    db      39h
    db      3Fh
    db      38h
    db      38h
    db      3Fh
    db      37h
    db      37h
    db      3Fh
    db      36h
    db      36h
    db      3Fh
    db      35h
    db      35h
    db      3Fh
    db      34h
    db      34h
    db      3Fh
    db      33h
    db      33h
    db      3Fh
    db      32h
    db      32h
    db      3Fh
    db      31h
    db      31h
    db      3Fh
    db      30h
    db      30h
    db      3Fh
    db      2Fh
    db      2Fh
    db      3Fh
    db      2Eh
    db      2Eh
    db      3Fh
    db      2Dh
    db      2Dh
    db      3Fh
    db      2Ch
    db      2Ch
    db      3Fh
    db      2Bh
    db      2Bh
    db      3Fh
    db      2Ah
    db      2Ah
    db      3Fh
    db      29h
    db      29h
    db      3Fh
    db      28h
    db      28h
    db      3Fh
    db      27h
    db      27h
    db      3Fh
    db      26h
    db      26h
    db      3Fh
    db      25h
    db      25h
    db      3Fh
    db      24h
    db      24h
    db      3Fh
    db      23h
    db      23h
    db      3Fh
    db      22h
    db      22h
    db      3Fh
    db      21h
    db      21h
    db      3Fh
    db      20h
    db      20h
    db      3Fh
    db      1Fh
    db      1Fh
    db      3Fh
    db      1Eh
    db      1Eh
    db      3Fh
    db      1Dh
    db      1Dh
    db      3Fh
    db      1Ch
    db      1Ch
    db      3Fh
    db      1Bh
    db      1Bh
    db      3Fh
    db      1Ah
    db      1Ah
    db      3Fh
    db      19h
    db      19h
    db      3Fh
    db      18h
    db      18h
    db      3Fh
    db      17h
    db      17h
    db      3Fh
    db      16h
    db      16h
    db      3Fh
    db      15h
    db      15h
    db      3Fh
    db      14h
    db      14h
    db      3Fh
    db      13h
    db      13h
    db      3Fh
    db      12h
    db      12h
    db      3Fh
    db      11h
    db      11h
    db      3Fh
    db      10h
    db      10h
    db      3Fh
    db      0Fh
    db      0Fh
    db      3Fh
    db      0Eh
    db      0Eh
    db      3Fh
    db      0Dh
    db      0Dh
    db      3Fh
    db      0Ch
    db      0Ch
    db      3Fh
    db      0Bh
    db      0Bh
    db      3Fh
    db      0Ah
    db      0Ah
    db      3Fh
    db      9h
    db      9h
    db      3Fh
    db      8h
    db      8h
    db      3Fh
    db      7h
    db      7h
    db      3Fh
    db      6h
    db      6h
    db      3Fh
    db      5h
    db      5h
    db      3Fh
    db      4h
    db      4h
    db      3Fh
    db      3h
    db      3h
    db      3Fh
    db      2h
    db      2h
    db      3Fh
    db      1h
    db      1h
    db      3Fh
    db      0h
    db      0h
    db      3Eh
    db      1h
    db      0h
    db      3Dh
    db      2h
    db      0h
    db      3Ch
    db      3h
    db      0h
    db      3Bh
    db      4h
    db      0h
    db      3Ah
    db      5h
    db      0h
    db      39h
    db      6h
    db      0h
    db      38h
    db      7h
    db      0h
    db      37h
    db      8h
    db      0h
    db      36h
    db      9h
    db      0h
    db      35h
    db      0Ah
    db      0h
    db      34h
    db      0Bh
    db      0h
    db      33h
    db      0Ch
    db      0h
    db      32h
    db      0Dh
    db      0h
    db      31h
    db      0Eh
    db      0h
    db      30h
    db      0Fh
    db      0h
    db      2Fh
    db      10h
    db      0h
    db      2Eh
    db      11h
    db      0h
    db      2Dh
    db      12h
    db      0h
    db      2Ch
    db      13h
    db      0h
    db      2Bh
    db      14h
    db      0h
    db      2Ah
    db      15h
    db      0h
    db      29h
    db      16h
    db      0h
    db      28h
    db      17h
    db      0h
    db      27h
    db      18h
    db      0h
    db      26h
    db      19h
    db      0h
    db      25h
    db      1Ah
    db      0h
    db      24h
    db      1Bh
    db      0h
    db      23h
    db      1Ch
    db      0h
    db      22h
    db      1Dh
    db      0h
    db      21h
    db      1Eh
    db      0h
    db      20h
    db      1Fh
    db      0h
    db      1Fh
    db      20h
    db      0h
    db      1Eh
    db      21h
    db      0h
    db      1Dh
    db      22h
    db      0h
    db      1Ch
    db      23h
    db      0h
    db      1Bh
    db      24h
    db      0h
    db      1Ah
    db      25h
    db      0h
    db      19h
    db      26h
    db      0h
    db      18h
    db      27h
    db      0h
    db      17h
    db      28h
    db      0h
    db      16h
    db      29h
    db      0h
    db      15h
    db      2Ah
    db      0h
    db      14h
    db      2Bh
    db      0h
    db      13h
    db      2Ch
    db      0h
    db      12h
    db      2Dh
    db      0h
    db      11h
    db      2Eh
    db      0h
    db      10h
    db      2Fh
    db      0h
    db      0Fh
    db      30h
    db      0h
    db      0Eh
    db      31h
    db      0h
    db      0Dh
    db      32h
    db      0h
    db      0Ch
    db      33h
    db      0h
    db      0Bh
    db      34h
    db      0h
    db      0Ah
    db      35h
    db      0h
    db      9h
    db      36h
    db      0h
    db      8h
    db      37h
    db      0h
    db      7h
    db      38h
    db      0h
    db      6h
    db      39h
    db      0h
    db      5h
    db      3Ah
    db      0h
    db      4h
    db      3Bh
    db      0h
    db      3h
    db      3Ch
    db      0h
    db      2h
    db      3Dh
    db      0h
    db      1h
    db      3Eh
    db      0h
    db      0h
    db      3Fh
    db      0h
    db      0h
    db      3Eh
    db      1h
    db      0h
    db      3Dh
    db      2h
    db      0h
    db      3Ch
    db      3h
    db      0h
    db      3Bh
    db      4h
    db      0h
    db      3Ah
    db      5h
    db      0h
    db      39h
    db      6h
    db      0h
    db      38h
    db      7h
    db      0h
    db      37h
    db      8h
    db      0h
    db      36h
    db      9h
    db      0h
    db      35h
    db      0Ah
    db      0h
    db      34h
    db      0Bh
    db      0h
    db      33h
    db      0Ch
    db      0h
    db      32h
    db      0Dh
    db      0h
    db      31h
    db      0Eh
    db      0h
    db      30h
    db      0Fh
    db      0h
    db      2Fh
    db      10h
    db      0h
    db      2Eh
    db      11h
    db      0h
    db      2Dh
    db      12h
    db      0h
    db      2Ch
    db      13h
    db      0h
    db      2Bh
    db      14h
    db      0h
    db      2Ah
    db      15h
    db      0h
    db      29h
    db      16h
    db      0h
    db      28h
    db      17h
    db      0h
    db      27h
    db      18h
    db      0h
    db      26h
    db      19h
    db      0h
    db      25h
    db      1Ah
    db      0h
    db      24h
    db      1Bh
    db      0h
    db      23h
    db      1Ch
    db      0h
    db      22h
    db      1Dh
    db      0h
    db      21h
    db      1Eh
    db      0h
    db      20h
    db      1Fh
    db      0h
    db      1Fh
    db      20h
    db      0h
    db      1Eh
    db      21h
    db      0h
    db      1Dh
    db      22h
    db      0h
    db      1Ch
    db      23h
    db      0h
    db      1Bh
    db      24h
    db      0h
    db      1Ah
    db      25h
    db      0h
    db      19h
    db      26h
    db      0h
    db      18h
    db      27h
    db      0h
    db      17h
    db      28h
    db      0h
    db      16h
    db      29h
    db      0h
    db      15h
    db      2Ah
    db      0h
    db      14h
    db      2Bh
    db      0h
    db      13h
    db      2Ch
    db      0h
    db      12h
    db      2Dh
    db      0h
    db      11h
    db      2Eh
    db      0h
    db      10h
    db      2Fh
    db      0h
    db      0Fh
    db      30h
    db      0h
    db      0Eh
    db      31h
    db      0h
    db      0Dh
    db      32h
L_2855:
    push    bp
    push    es
    push    ds
    push    di
    push    ax
    mov     ax,95FEh
    int     10h
    or      dl,dl
    jz      short L_286B
    mov     bx,di
    mov     cx,ax
    xor     dh,dh
    jmp     short L_2871
L_286B:
    xor     cx,cx
    xor     bx,bx
    xor     dx,dx
L_2871:
    pop     ax
    pop     di
    pop     ds
    pop     es
    pop     bp
    ret     

; 中断 48h 服务程序
L_2877:
    sti     
    cmp     ah,90h
    jz      short L_287E
    iret    
L_287E:
    push    es
    push    ds
    pusha   
    push    cs
    pop     ds
    cmp     al,81h
    jnz     short L_289F
    push    cs
    pop     es
    mov     ah,49h
    int     21h
    xor     ax,ax
    mov     es,ax
    mov     di,120h
    mov     si, offset L_17FD
    cld     
    cli     
    movsw   
    movsw   
    sti     
    jmp     near ptr L_2B6C
L_289F:
    pusha   
    mov     ax,95FEh
    int     10h
    cmp     dl,3h
    jnz     short L_28AC
    xor     dl,dl
L_28AC:
    mov     byte ptr [L_175B],dl
    mov     byte ptr [L_17FC],0FFh
    cmp     dl,2h
    jz      short L_28BF
    mov     byte ptr [L_17FC],0Fh
L_28BF:
    popa    
    cld     
    cmp     al,0F0h
    jc      short L_28C8
    jmp     near ptr L_2968
L_28C8:
    cmp     al,0E6h
    jnz     short L_28D8
    popa    
    mov     ax,901h
    call    dword ptr [cs:L_013C]
    jmp     near ptr L_2B6D
L_28D8:
    cmp     al,0EFh
    jz      short L_28DE
    jmp     short L_293C
L_28DE:
    popa    
    call    L_2855
    jmp     near ptr L_2B6D
L_28E5:
    mov     byte ptr [L_1751],bl
    mov     ax,95FEh
    int     10h
    mov     cx,ax
    mov     bx,di
    or      dl,dl
    jnz     short L_28FC
    popa    
    xor     ax,ax
    jmp     near ptr L_2B6D
L_28FC:
    mov     word ptr [L_1753],0h
    mov     word ptr [L_1755],0h
    dec     cx
    dec     bx
    mov     word ptr [L_1757],cx
    mov     word ptr [L_1759],bx
    mov     ax,500h
    call    dword ptr [cs:L_013C]
    mov     ax,600h
    mov     cx,0FFFFh
    call    dword ptr [cs:L_013C]
    mov     byte ptr [cs:L_1809],1h
    call    L_18EE
    mov     word ptr [L_1753],0h
    mov     word ptr [L_1755],0h
    jmp     short L_2962
L_293C:
    cmp     al,0EEh
    jnz     short L_2965
    or      bh,bh
    jnz     short L_2946
    jmp     short L_28E5
L_2946:
    push    cs
    pop     es
    dec     bh
    dec     bh
    dec     bh
    jnz     short L_2955
    call    L_2159
    jmp     short L_2962
L_2955:
    popa    
    pop     ds
    push    ds
    pusha   
    mov     di, offset L_205A
    mov     cx,8h
    cld     
    rep     movsb
L_2962:
    jmp     near ptr L_2B6C
L_2965:
    jmp     near ptr L_2B6C
L_2968:
    cmp     al,0F0h
    jnz     short L_2997
    mov     word ptr [L_1753],cx
    mov     word ptr [L_1755],dx
    mov     byte ptr [L_1751],bl
    or      bh,bh
    jnz     short L_2982
    call    L_1818
    jmp     near ptr L_2B6C
L_2982:
    cmp     bh,1h
    jnz     short L_2994
    call    L_17B4
    popa    
    mov     bl,byte ptr [L_1752]
    pop     ds
    pop     es
    jmp     near ptr L_2B6F
L_2994:
    jmp     near ptr L_2B6C
L_2997:
    cmp     al,0F1h
    jnz     short L_29CC
    or      bh,bh
    jnz     short L_29C5
    mov     word ptr [L_1753],cx
    mov     word ptr [L_1755],dx
L_29A7:
    mov     word ptr [L_1757],si
    mov     word ptr [L_1759],di
    mov     byte ptr [L_1751],bl
    push    si
    push    di
    call    L_181E
    pop     di
    pop     si
    mov     word ptr [L_1753],si
    mov     word ptr [L_1755],di
    jmp     near ptr L_2B6C
L_29C5:
    dec     bh
    jz      short L_29A7
    jmp     near ptr L_2B6C
L_29CC:
    cmp     al,0F4h
    jnz     short L_2A17
    call    L_29D6
    jmp     near ptr L_2B6C
L_29D6:
    mov     word ptr [L_1753],cx
    mov     word ptr [L_1755],dx
    mov     word ptr [L_1757],si
    mov     word ptr [L_1759],di
    mov     byte ptr [L_1751],bl
    mov     byte ptr [L_1809],bh
    mov     word ptr [L_1816],bp
    cmp     bh,4h
    jz      short L_2A08
    cmp     bh,84h
    jz      short L_2A08
    cmp     byte ptr [L_1816],0h
    jnz     short L_2A08
    mov     byte ptr [L_1816],1h
L_2A08:
    cmp     byte ptr [L_1817],0h
    jnz     short L_2A14
    mov     byte ptr [L_1817],1h
L_2A14:
    jmp     near ptr L_18EE
L_2A17:
    cmp     al,0F5h
    jz      short L_2A1E
    jmp     near ptr L_2AC0
L_2A1E:
    cmp     bl,3h
    ja      short L_2A2E
    mov     al,bl
    mov     ah,5h
    call    dword ptr [cs:L_013C]
    jmp     short L_2A3A
L_2A2E:
    sub     bl,4h
    jnz     short L_2A3D
    mov     ah,6h
    call    dword ptr [cs:L_013C]
L_2A3A:
    jmp     near ptr L_2B6C
L_2A3D:
    dec     bl
    jnz     short L_2A4E
    popa    
    mov     ah,7h
    mov     al,bh
    call    dword ptr [cs:L_013C]
    jmp     near ptr L_2B6D
L_2A4E:
    xor     si,si
    or      bh,bh
    jz      short L_2A67
    dec     bh
    cmp     bh,0Ah
    ja      short L_2A67
    mov     si, offset L_200A
    mov     al,bh
    xor     ah,ah
    shl     ax,byte ptr 3h
    add     si,ax
L_2A67:
    mov     word ptr [L_2008],si
    mov     di, offset L_2AA0
    push    cs
    pop     es
    or      si,si
    jnz     short L_2A7E
    mov     ax,0FFFFh
    mov     cx,10h
    rep     stosw
    jmp     short L_2A90
L_2A7E:
    mov     ch,2h
L_2A80:
    push    si
    mov     cl,8h
L_2A83:
    lodsb   
    mov     ah,al
    stosw   
    dec     cl
    jnz     short L_2A83
    pop     si
    dec     ch
    jnz     short L_2A80
L_2A90:
    mov     bp, offset L_2AA0
    mov     ah,4h
    mov     bx,400h
    call    dword ptr [cs:L_013C]
    jmp     near ptr L_2B6C
L_2AA0:
    db      32 dup(0FFh)
L_2AC0:
    cmp     al,0F6h
    jnz     short L_2AFE
    test    cx,8000h
    jz      short L_2ACC
    xor     cx,cx
L_2ACC:
    test    dx,8000h
    jz      short L_2AD4
    xor     dx,dx
L_2AD4:
    test    si,8000h
    jz      short L_2ADC
    xor     si,si
L_2ADC:
    test    di,8000h
    jz      short L_2AE4
    xor     di,di
L_2AE4:
    mov     word ptr [L_1801],cx
    mov     word ptr [L_1803],dx
    mov     word ptr [L_1805],si
    mov     word ptr [L_1807],di
    mov     ax,900h
    call    dword ptr [cs:L_013C]
    jmp     short L_2B6C
L_2AFE:
    cmp     al,0F9h
    jnz     short L_2B36
    mov     word ptr [L_1753],cx
    mov     word ptr [L_1755],dx
    mov     word ptr [L_1757],si
    mov     word ptr [L_1759],di
    or      bl,bl
    jnz     short L_2B18
    inc     bl
L_2B18:
    mov     byte ptr [L_1809],bl
    mov     byte ptr [L_180A],bh
    mov     cx,bp
    test    bh,1h
    jnz     short L_2B29
    xchg    cl,ch
L_2B29:
    mov     byte ptr [L_1814],ch
    mov     byte ptr [L_1815],cl
    call    L_1842
    jmp     short L_2B6C
L_2B36:
    cmp     al,0FAh
    jnz     short L_2B6C
    cmp     bh,2h
    jc      short L_2B48
    popa    
    pop     es
    push    es
    pusha   
    call    L_1C5A
    jmp     short L_2B6C
L_2B48:
    mov     word ptr [L_1753],cx
    mov     word ptr [L_1755],dx
    or      si,si
    jnz     short L_2B55
    inc     si
L_2B55:
    mov     word ptr [L_180D],si
    mov     word ptr [L_180B],0h
    mov     byte ptr [L_1751],bl
    mov     byte ptr [L_1809],bh
    call    L_1A68
    jmp     short L_2B6C
L_2B6C:
    popa    
L_2B6D:
    pop     ds
    pop     es
L_2B6F:
    iret    
L_2B70:
    dw      0h
L_2B72:
    dw      0h
L_2B74:
    db      0h
L_2B75:
    db      0FFh
L_2B76:
    dw      0h
L_2B78:
    dw      0h
L_2B7A:
    dw      138h
L_2B7C:
    dw      0DCh
L_2B7E:
    dw      0FFh
    db      0h
L_2B81:
    dw      0h
L_2B83:
    dw      0h
L_2B85:
    dw      13F8h
L_2B87:
    dw      0EF8h
    db      1h
L_2B8A:
    db      0FFh
L_2B8B:
    db      0FFh
L_2B8C:
    dw      0FFFFh
L_2B8E:
    dw      0h
L_2B90:
    dw      0h
L_2B92:
    dw      0h
L_2B94:
    dw      0h
L_2B96:
    db      1h
L_2B97:
    dw      7FFFh
    dw      3FFFh
    dw      1FFFh
    dw      0FFFh
    dw      7FFh
    dw      3FFh
    dw      1FFh
    dw      0FFh
    dw      7Fh
    dw      3Fh
    dw      1FFh
    dw      1FFh
    dw      30FFh
    dw      70FFh
    dw      0F87Fh
    dw      0F87Fh
    dw      0h
    dw      0h
    dw      4000h
    dw      6000h
    dw      7000h
    dw      7800h
    dw      7C00h
    dw      7E00h
    dw      7F00h
    dw      7800h
    dw      6C00h
    dw      4C00h
    dw      600h
    dw      600h
    dw      300h
    dw      0h
    db      92 dup(0)
L_2C33:
    db      932 dup(0)

; 鼠标中断与驱动
L_2FD7:
    inc     byte ptr [cs:L_2B75]
    cmp     byte ptr [cs:L_2B75],0h
    jz      short L_2FE6
    jmp     short L_3055
L_2FE6:
    push    bp
    push    ax
    push    bx
    push    cx
    push    dx
    push    ds
    push    es
    push    si
    push    di
    call    L_3066
    mov     word ptr [cs:L_2B7A],cx
    mov     word ptr [cs:L_2B7C],dx
    mov     byte ptr [cs:L_2B74],0h
    cmp     byte ptr [cs:L_2B7E],0h
    jl      short L_3033
    cmp     cx,word ptr [cs:L_2B8C]
    jnz     short L_301A
    cmp     dx,word ptr [cs:L_2B8E]
    jnz     short L_301A
    jmp     short L_3033
L_301A:
    cmp     byte ptr [cs:L_2B7E],0h
    jl      short L_3033
    push    cx
    push    dx
    call    L_3325
    pop     dx
    pop     cx
    mov     word ptr [cs:L_2B8C],cx
    mov     word ptr [cs:L_2B8E],dx
L_3033:
    pop     di
    pop     si
    pop     es
    pop     ds
    pop     dx
    pop     cx
    pop     bx
    pop     ax
    pop     bp
    and     word ptr [cs:L_2B94],byte ptr +1Fh
    test    word ptr [cs:L_2B94],ax
    jz      short L_3055
    push    cx
    push    dx
    call    L_3066
    call    dword ptr [cs:L_2B90]
    pop     dx
    pop     cx
L_3055:
    dec     byte ptr [cs:L_2B75]
    retf    
L_305B:
    dw      0h
L_305D:
    dw      0h
L_305F:
    shl     cx,byte ptr 3h
    shl     dx,byte ptr 3h
    ret     
L_3066:
    shr     cx,byte ptr 3h
    shr     dx,byte ptr 3h
    ret     
L_306D:
    cmp     ax,0FFF1h
    jz      short L_3084
    cmp     ax,0FFF2h
    jz      short L_3084
    cmp     byte ptr [cs:L_2B96],0h
    jz      short L_3084
    jmp     dword ptr [cs:L_2B70]
L_3084:
    inc     byte ptr [cs:L_2B75]
    sti     
    push    si
    push    di
    push    ds
    push    es
    cmp     ax,0h
    jz      short L_3096
    jmp     near ptr L_3147
L_3096:
    push    bp
    push    ax
    push    bx
    push    cx
    push    dx
    push    si
    push    di
    mov     ax,95FEh
    int     10h
    shl     ax,byte ptr 3h
    shl     di,byte ptr 3h
    dec     ax
    mov     word ptr [cs:L_305B],ax
    dec     di
    mov     word ptr [cs:L_305D],di
    pop     di
    pop     si
    pop     dx
    pop     cx
    pop     bx
    pop     ax
    pop     bp
    mov     word ptr [cs:L_2B94],0h
    cmp     byte ptr [cs:L_2B7E],0h
    jl      short L_30D4
    push    ax
    push    bx
    push    cx
    push    dx
    call    L_337F
    pop     dx
    pop     cx
    pop     bx
    pop     ax
L_30D4:
    pushf   
    call    dword ptr [cs:L_2B70]
    mov     byte ptr [cs:L_2B7E],0FFh
    mov     word ptr [cs:L_2B8C],0FFFFh
    push    ax
    push    cx
    push    dx
    mov     ax,7h
    xor     cx,cx
    mov     dx,word ptr [cs:L_305B]
    pushf   
    call    dword ptr [cs:L_2B70]
    mov     ax,8h
    xor     cx,cx
    mov     dx,word ptr [cs:L_305D]
    pushf   
    call    dword ptr [cs:L_2B70]
    mov     ax,0Ch
    mov     cx,1Fh
    mov     dx, offset L_2FD7
    push    cs
    pop     es
    pushf   
    call    dword ptr [cs:L_2B70]
    mov     ax,0Fh
    mov     cx,1h
    mov     dx,2h
    pushf   
    call    dword ptr [cs:L_2B70]
    mov     ax,4h
    mov     cx,word ptr [cs:L_305B]
    mov     dx,word ptr [cs:L_305D]
    shr     cx,1
    shr     dx,1
    pushf   
    call    dword ptr [cs:L_2B70]
    pop     dx
    pop     cx
    pop     ax
    jmp     near ptr L_331B
L_3147:
    cmp     ax,1h
    jnz     short L_3176
    push    ax
    push    bx
    push    cx
    push    dx
    cmp     byte ptr [cs:L_2B7E],0h
    jz      short L_316F
    cmp     byte ptr [cs:L_2B7E],0FFh
    jnz     short L_316A
    mov     word ptr [cs:L_2B8C],0FFFFh
    call    L_3325
L_316A:
    inc     byte ptr [cs:L_2B7E]
L_316F:
    pop     dx
    pop     cx
    pop     bx
    pop     ax
    jmp     near ptr L_331B
L_3176:
    cmp     ax,2h
    jnz     short L_31A5
    cmp     byte ptr [cs:L_2B7E],0h
    jl      short L_3196
    cmp     word ptr [cs:L_2B8C],byte ptr -1h
    jz      short L_3196
    push    ax
    push    bx
    push    cx
    push    dx
    call    L_337F
    pop     dx
    pop     cx
    pop     bx
    pop     ax
L_3196:
    mov     word ptr [cs:L_2B8C],0FFFFh
    dec     byte ptr [cs:L_2B7E]
    jmp     near ptr L_331B
L_31A5:
    cmp     ax,3h
    jnz     short L_31B6
    pushf   
    call    dword ptr [cs:L_2B70]
    call    L_3066
    jmp     near ptr L_331B
L_31B6:
    cmp     ax,4h
    jnz     short L_31D1
    mov     word ptr [cs:L_2B7A],cx
    mov     word ptr [cs:L_2B7C],dx
    call    L_305F
    pushf   
    call    dword ptr [cs:L_2B70]
    jmp     near ptr L_331B
L_31D1:
    cmp     ax,5h
    jnz     short L_31E2
    pushf   
    call    dword ptr [cs:L_2B70]
    call    L_3066
    jmp     near ptr L_331B
L_31E2:
    cmp     ax,6h
    jnz     short L_31F3
    pushf   
    call    dword ptr [cs:L_2B70]
    call    L_3066
    jmp     near ptr L_331B
L_31F3:
    cmp     ax,7h
    jnz     short L_320E
    call    L_305F
    mov     word ptr [cs:L_2B81],cx
    mov     word ptr [cs:L_2B85],dx
    pushf   
    call    dword ptr [cs:L_2B70]
    jmp     near ptr L_331B
L_320E:
    cmp     ax,8h
    jnz     short L_3229
    call    L_305F
    mov     word ptr [cs:L_2B83],cx
    mov     word ptr [cs:L_2B87],dx
    pushf   
    call    dword ptr [cs:L_2B70]
    jmp     near ptr L_331B
L_3229:
    cmp     ax,9h
    jnz     short L_324C
    mov     si,dx
    mov     di, offset L_2B97
    push    es
    pop     ds
    push    cs
    pop     es
    push    cx
    mov     cx,20h
    cld     
    rep     movsw
    pop     cx
    mov     word ptr [cs:L_2B76],bx
    mov     word ptr [cs:L_2B78],cx
    jmp     near ptr L_331B
L_324C:
    cmp     ax,0Ah
    jnz     short L_3254
    jmp     near ptr L_331B
L_3254:
    cmp     ax,0Bh
    jnz     short L_3262
    pushf   
    call    dword ptr [cs:L_2B70]
    jmp     near ptr L_331B
L_3262:
    cmp     ax,0Ch
    jnz     short L_3279
    mov     word ptr [cs:L_2B90],dx
    mov     word ptr [cs:L_2B92],es
    mov     word ptr [cs:L_2B94],cx
    jmp     near ptr L_331B
L_3279:
    cmp     ax,14h
    jnz     short L_32A5
    pop     es
    push    ax
    mov     ax,word ptr [cs:L_2B94]
    mov     word ptr [cs:L_2B94],cx
    mov     cx,ax
    mov     ax,word ptr [cs:L_2B92]
    mov     word ptr [cs:L_2B92],es
    mov     es,ax
    mov     ax,word ptr [cs:L_2B90]
    mov     word ptr [cs:L_2B90],dx
    mov     dx,ax
    pop     ax
    push    es
    jmp     short L_331B
L_32A5:
    cmp     ax,15h
    jnz     short L_32AF
    mov     bx,190h
    jmp     short L_331B
L_32AF:
    cmp     ax,16h
    jnz     short L_32D0
    push    cx
    push    ax
    mov     si, offset L_2B7A
    mov     di,dx
    mov     cx,183h
L_32BE:
    mov     al,[cs:si]
    mov     [es:di],al
    inc     si
    inc     di
    loop    short L_32BE
    pop     ax
    pop     cx
    push    dx
    mov     dx,di
    pop     dx
    jmp     short L_331B
L_32D0:
    cmp     ax,17h
    jnz     short L_32F1
    push    cx
    push    ax
    mov     di, offset L_2B7A
    mov     si,dx
    mov     cx,183h
L_32DF:
    mov     al,[es:si]
    mov     [cs:di],al
    inc     si
    inc     di
    loop    short L_32DF
    pop     ax
    pop     cx
    push    dx
    mov     dx,di
    pop     dx
    jmp     short L_331B
L_32F1:
    cmp     ax,21h
    jnz     short L_32F9
    jmp     near ptr L_3096
L_32F9:
    cmp     ax,0FFF1h
    jnz     short L_3306
    mov     byte ptr [cs:L_2B96],0h
    jmp     short L_331B
L_3306:
    cmp     ax,0FFF2h
    jnz     short L_3313
    mov     byte ptr [cs:L_2B96],1h
    jmp     short L_331B
L_3313:
    pop     es
    pushf   
    call    dword ptr [cs:L_2B70]
    push    es
L_331B:
    pop     es
    pop     ds
    pop     di
    pop     si
    dec     byte ptr [cs:L_2B75]
    iret    
L_3325:
    inc     byte ptr [cs:L_2B8A]
    jz      short L_332E
    jmp     short L_3379
L_332E:
    cmp     word ptr [cs:L_2B8C],byte ptr -1h
    jz      short L_3339
    call    L_337F
L_3339:
    mov     cx,word ptr [cs:L_2B7A]
    mov     dx,word ptr [cs:L_2B7C]
    mov     word ptr [cs:L_2B8C],cx
    mov     word ptr [cs:L_2B8E],dx
    sub     cx,word ptr [cs:L_2B76]
    sub     dx,word ptr [cs:L_2B78]
    test    cx,8000h
    jz      short L_335F
    xor     cx,cx
L_335F:
    test    dx,8000h
    jz      short L_3367
    xor     dx,dx
L_3367:
    mov     ah,8h
    mov     bh,0h
    push    es
    push    bp
    push    cs
    pop     es
    mov     bp, offset L_2B97
    call    dword ptr [cs:L_013C]
    pop     bp
    pop     es
L_3379:
    dec     byte ptr [cs:L_2B8A]
    ret     
L_337F:
    inc     byte ptr [cs:L_2B8B]
    jz      short L_3388
    jmp     short L_33BE
L_3388:
    mov     cx,word ptr [cs:L_2B8C]
    mov     dx,word ptr [cs:L_2B8E]
    sub     cx,word ptr [cs:L_2B76]
    sub     dx,word ptr [cs:L_2B78]
    test    cx,8000h
    jz      short L_33A4
    xor     cx,cx
L_33A4:
    test    dx,8000h
    jz      short L_33AC
    xor     dx,dx
L_33AC:
    mov     ah,8h
    mov     bh,1h
    push    es
    push    bp
    push    cs
    pop     es
    mov     bp, offset L_2B97
    call    dword ptr [cs:L_013C]
    pop     bp
    pop     es
L_33BE:
    dec     byte ptr [cs:L_2B8B]
    ret     

; 15区制表符图形点阵
L_33C4:
    dw      1010h
    dw      1010h
    dw      1010h
    dw      0FEFEh
    dw      1010h
    dw      1010h
    dw      0FEFEh
    dw      0h
    dw      404h
    dw      0FEFEh
    dw      8484h
    dw      8484h
    dw      8484h
    dw      8484h
    dw      0FCFCh
    dw      404h
    dw      8020h
    dw      4020h
    dw      0FE3Fh
    dw      244h
    dw      1441h
    dw      487Ah
    dw      0A4A4h
    dw      1021h
    dw      0EFEh
    dw      424h
    dw      0F823h
    dw      826h
    dw      82Ah
    dw      832h
    dw      0F823h
    dw      802h
    dw      2008h
    dw      0FC7Fh
    dw      2009h
    dw      0A00Ch
    dw      600Ah
    dw      2009h
    dw      0FEFFh
    dw      2008h
    dw      1010h
    dw      0FE3Fh
    dw      4C2h
    dw      4h
    dw      0F00Fh
    dw      1018h
    dw      1028h
    dw      0F04Fh
    dw      400h
    dw      0FEFDh
    dw      411h
    dw      0FC11h
    dw      411h
    dw      0FC11h
    dw      20F8h
    dw      0FF13h
    dw      10h
    dw      0FC11h
    dw      411h
    dw      0FC1Dh
    dw      0A8F0h
    dw      2441h
    dw      2202h
    dw      6000h
    dw      83Eh
    dw      1020h
    dw      643Eh
    dw      820h
    dw      723Eh
    dw      420h
    dw      88FFh
    dw      3004h
    dw      204h
    dw      0FFFFh
    dw      8h
    dw      0F817h
    dw      1022h
    dw      20C1h
    dw      0E001h
    dw      1E0Eh
    dw      4000h
    dw      2444h
    dw      0FE2Fh
    dw      10h
    dw      28h
    dw      0FC49h
    dw      489h
    dw      409h
    dw      0FC19h
    dw      2028h
    dw      0A048h
    dw      2889h
    dw      260Ah
    dw      2208h
    dw      0A050h
    dw      4020h
    dw      812h
    dw      0DC13h
    dw      6824h
    dw      0A86Ah
    dw      3EA3h
    dw      822h
    dw      482Ch
    dw      4020h
    dw      0FE2Fh
    dw      4020h
    dw      5021h
    dw      5021h
    dw      4822h
    dw      4822h
    dw      4424h
    dw      4228h
    dw      8000h
    dw      4040h
    dw      0FC27h
    dw      404h
    dw      404h
    dw      0FC87h
    dw      44h
    dw      5004h
    dw      4804h
    dw      0FE27h
    dw      6024h
    dw      6044h
    dw      5044h
    dw      8888h
    dw      499h
    dw      202h
    dw      4020h
    dw      4010h
    dw      4010h
    dw      42FCh
    dw      0FE0Fh
    dw      8010h
    dw      8018h
    dw      435h
    dw      0FE55h
    dw      8492h
    dw      8414h
    dw      8410h
    dw      8410h
    dw      8410h
    dw      0FC10h
    dw      8410h
    dw      4000h
    dw      4004h
    dw      40FCh
    dw      4410h
    dw      0FE17h
    dw      4020h
    dw      0E020h
    dw      507Dh
    dw      48A5h
    dw      4E26h
    dw      4424h
    dw      0F025h
    dw      4024h
    dw      403Ch
    dw      4024h
    dw      4000h
    dw      220h
    dw      0E221h
    dw      227Dh
    dw      2289h
    dw      0EA11h
    dw      2A7Dh
    dw      2A55h
    dw      0EA55h
    dw      2A7Dh
    dw      2A55h
    dw      0EA55h
    dw      0AA7Dh
    dw      8200h
    dw      4255h
    dw      2AAAh
    dw      1684h
    dw      828h
    dw      83Fh
    dw      2848h
    dw      283Fh
    dw      2808h
    dw      283Fh
    dw      2829h
    dw      82Bh
    dw      5808h
    dw      4000h
    dw      0FE7Fh
    dw      100Dh
    dw      0A014h
    dw      4025h
    dw      2006h
    dw      1C0Ch
    dw      4010h
    dw      7C10h
    dw      44FEh
    dw      4410h
    dw      4810h
    dw      5092h
    dw      4892h
    dw      4492h
    dw      4492h
    dw      4492h
    dw      44FEh
    dw      5892h
    dw      4010h
    dw      4010h
    dw      4011h
    dw      400Fh
    dw      0h
    dw      0F01Fh
    dw      1010h
    dw      0F01Fh
    dw      1010h
    dw      0F01Fh
    dw      1010h
    dw      4h
    dw      2008h
    dw      2034h
    dw      2004h
    dw      0FE7Fh
    dw      2004h
    dw      2004h
    dw      2008h
    dw      2010h
    dw      2801h
    dw      2801h
    dw      0F8FBh
    dw      2821h
    dw      0EA21h
    dw      3F21h
    dw      0EA41h
    dw      2AB9h
    dw      2A29h
    dw      0FA2Fh
    dw      0AA2Ah
    dw      1A2Bh
    dw      0A3Ah
    dw      0FA2Bh
    dw      1200h
    dw      2600h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0FC7Fh
    dw      280h
    dw      0E29Ch
    dw      92A2h
    dw      8AA0h
    dw      8AA0h
    dw      8AA0h
    dw      92A2h
    dw      0E29Ch
    dw      280h
    dw      0FC7Fh
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      18h
    dw      3Ch
    dw      7Eh
    dw      18h
    dw      18h
    dw      18h
    dw      18h
    dw      18h
    dw      18h
    dw      18h
    dw      0D81Eh
    dw      0D80Eh
    dw      0h
    dw      0h
    dw      0FFFFh
    dw      3C0h
    dw      5A0h
    dw      990h
    dw      1188h
    dw      2184h
    dw      4182h
    dw      8181h
    dw      8181h
    dw      4182h
    dw      2184h
    dw      1188h
    dw      990h
    dw      5A0h
    dw      3C0h
    dw      0FFFFh
    dw      0FFFFh
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      0FFFFh
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      8003h
    dw      8003h
    dw      8003h
    dw      8003h
    dw      8003h
    dw      0E00Fh
    dw      0C007h
    dw      8003h
    dw      1h
    dw      0E00Fh
    dw      0E00Fh
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      8001h
    dw      0C003h
    dw      0E007h
    dw      0F00Fh
    dw      0h
    dw      0h
    dw      0F00Fh
    dw      0E007h
    dw      0C003h
    dw      8001h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0F00Fh
    dw      0E007h
    dw      0C003h
    dw      8001h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      8001h
    dw      0C003h
    dw      0E007h
    dw      0F00Fh
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      4000h
    dw      6000h
    dw      0F00Fh
    dw      0F80Fh
    dw      0F00Fh
    dw      6000h
    dw      4000h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      2h
    dw      6h
    dw      0F00Fh
    dw      0F01Fh
    dw      0F00Fh
    dw      6h
    dw      2h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      8003h
    dw      8003h
    dw      8003h
    dw      8003h
    dw      8003h
    dw      0E00Fh
    dw      0C007h
    dw      8003h
    dw      1h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      1h
    dw      8003h
    dw      0C007h
    dw      0E00Fh
    dw      8003h
    dw      8003h
    dw      8003h
    dw      8003h
    dw      8003h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0C007h
    dw      3018h
    dw      820h
    dw      3440h
    dw      3440h
    dw      3284h
    dw      328Ch
    dw      328Ch
    dw      0F29Fh
    dw      0F2BFh
    dw      45Ch
    dw      44Ch
    dw      824h
    dw      3018h
    dw      0C007h
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      6A4h
    dw      6ACh
    dw      0F6BFh
    dw      6ACh
    dw      6A4h
    dw      9680h
    dw      0D680h
    dw      0F6BFh
    dw      0D680h
    dw      9680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      26B2h
    dw      56A5h
    dw      46A4h
    dw      46B2h
    dw      46A1h
    dw      46A1h
    dw      56A5h
    dw      26B2h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      46BCh
    dw      0C6A0h
    dw      46A0h
    dw      46BCh
    dw      46A0h
    dw      46A0h
    dw      46A0h
    dw      46A0h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      0E6BCh
    dw      16A1h
    dw      16A0h
    dw      26BCh
    dw      46A0h
    dw      86A0h
    dw      6A1h
    dw      0F6A1h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      0E6BCh
    dw      16A1h
    dw      16A0h
    dw      66BCh
    dw      16A0h
    dw      16A0h
    dw      16A1h
    dw      0E6A0h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      26BCh
    dw      66A0h
    dw      0A6A0h
    dw      26BDh
    dw      26A1h
    dw      0F6A1h
    dw      26A0h
    dw      26A0h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      0F6BDh
    dw      6A1h
    dw      6A1h
    dw      0E6BDh
    dw      16A0h
    dw      16A0h
    dw      16A1h
    dw      0E6A0h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      0E6BCh
    dw      16A1h
    dw      6A1h
    dw      0E6BDh
    dw      16A1h
    dw      16A1h
    dw      16A1h
    dw      0E6A0h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      0F6BDh
    dw      16A0h
    dw      16A0h
    dw      26BCh
    dw      46A0h
    dw      86A0h
    dw      86A0h
    dw      86A0h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      0E6BCh
    dw      16A1h
    dw      16A1h
    dw      0E6BCh
    dw      16A1h
    dw      16A1h
    dw      16A1h
    dw      0E6A0h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      0E6BCh
    dw      16A1h
    dw      16A1h
    dw      16BDh
    dw      0F6A0h
    dw      16A0h
    dw      16A1h
    dw      0E6A0h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      26B9h
    dw      56A3h
    dw      56A1h
    dw      56B9h
    dw      56A1h
    dw      56A1h
    dw      56A1h
    dw      26A1h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      26B9h
    dw      66A3h
    dw      26A1h
    dw      26B9h
    dw      26A1h
    dw      26A1h
    dw      26A1h
    dw      26A1h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0FCFFh
    dw      680h
    dw      680h
    dw      66BAh
    dw      96A2h
    dw      16A2h
    dw      16BAh
    dw      26A2h
    dw      46A2h
    dw      86A2h
    dw      0F6A2h
    dw      680h
    dw      680h
    dw      0FEFFh
    dw      0FE7Fh
    dw      0h
    dw      0h
    dw      0FC07h
    dw      604h
    dw      604h
    dw      604h
    dw      2604h
    dw      267Ch
    dw      2641h
    dw      2643h
    dw      0E647h
    dw      643h
    dw      641h
    dw      640h
    dw      0FE7Fh
    dw      0FE3Fh
    dw      0h
    dw      0ABAAh
    dw      55D5h
    dw      0ABAAh
    dw      55D5h
    dw      0ABAAh
    dw      55D5h
    dw      0ABAAh
    dw      55D5h
    dw      0ABAAh
    dw      55D5h
    dw      0ABAAh
    dw      55D5h
    dw      0ABAAh
    dw      55D5h
    dw      0ABAAh
    dw      55D5h
    dw      0FFFFh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      0FFFFh
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0AAAAh
    dw      5555h
    dw      0h
    dw      0h
    dw      0FC7Fh
    dw      280h
    dw      280h
    dw      0E28Fh
    dw      2288h
    dw      0FABFh
    dw      2288h
    dw      0E28Fh
    dw      280h
    dw      280h
    dw      0FC7Fh
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0FC7Fh
    dw      280h
    dw      280h
    dw      0F280h
    dw      280h
    dw      280h
    dw      280h
    dw      280h
    dw      280h
    dw      280h
    dw      0FC7Fh
    dw      0h
    dw      0h
    dw      0h
    dw      0C003h
    dw      300Ch
    dw      810h
    dw      420h
    dw      0C243h
    dw      0E247h
    dw      0F18Fh
    dw      0F18Fh
    dw      0F18Fh
    dw      0F18Fh
    dw      0E247h
    dw      0C263h
    dw      420h
    dw      810h
    dw      300Ch
    dw      0C003h
    dw      0C003h
    dw      300Ch
    dw      810h
    dw      420h
    dw      240h
    dw      240h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      240h
    dw      260h
    dw      420h
    dw      810h
    dw      300Ch
    dw      0C003h
    dw      0h
    dw      0E07Fh
    dw      3040h
    dw      285Bh
    dw      2440h
    dw      0A45Dh
    dw      3C40h
    dw      845Dh
    dw      440h
    dw      0E45Eh
    dw      440h
    dw      645Bh
    dw      440h
    dw      440h
    dw      0FC7Fh
    dw      0h
    dw      0h
    dw      0FC7Fh
    dw      440h
    dw      440h
    dw      0FC7Fh
    dw      440h
    dw      0F45Fh
    dw      1451h
    dw      1451h
    dw      0F45Fh
    dw      1451h
    dw      1451h
    dw      0F45Fh
    dw      440h
    dw      0FC7Fh
    dw      0h
    dw      0h
    dw      0FC7Fh
    dw      440h
    dw      440h
    dw      0FC7Fh
    dw      440h
    dw      440h
    dw      440h
    dw      440h
    dw      440h
    dw      440h
    dw      440h
    dw      440h
    dw      440h
    dw      0FC7Fh
    dw      0h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      0FFFFh
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      0FFFFh
    dw      0FFFFh
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      0FFFFh
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0FFFFh
    dw      0FFFFh
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      100h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      80h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      0FFFFh
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0h
    dw      0FFFFh
    dw      0FFFFh
    dw      180h
    dw      180h
    dw      4180h
    dw      6180h
    dw      7180h
    dw      0F99Fh
    dw      0FD9Fh
    dw      0F99Fh
    dw      7180h
    dw      6180h
    dw      4180h
    dw      180h
    dw      180h
    dw      180h
    dw      0FFFFh
    dw      0FFFFh
    dw      180h
    dw      180h
    dw      182h
    dw      186h
    dw      18Eh
    dw      0F99Fh
    dw      0F9BFh
    dw      0F99Fh
    dw      18Eh
    dw      186h
    dw      182h
    dw      180h
    dw      180h
    dw      180h
    dw      0FFFFh
    dw      0FFFFh
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      8181h
    dw      0C183h
    dw      0E187h
    dw      0F18Fh
    dw      0F99Fh
    dw      0FDBFh
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      0FFFFh
    dw      0FFFFh
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      0FDBFh
    dw      0F99Fh
    dw      0F18Fh
    dw      0E187h
    dw      0C183h
    dw      8181h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      0FFFFh
    dw      0FFFFh
    dw      180h
    dw      180h
    dw      8183h
    dw      8183h
    dw      8183h
    dw      8183h
    dw      8183h
    dw      8183h
    dw      0F19Fh
    dw      0E18Fh
    dw      0C187h
    dw      8183h
    dw      181h
    dw      180h
    dw      0FFFFh
    dw      0FFFFh
    dw      180h
    dw      181h
    dw      8183h
    dw      0C187h
    dw      0E18Fh
    dw      0F19Fh
    dw      8183h
    dw      8183h
    dw      8183h
    dw      8183h
    dw      8183h
    dw      8183h
    dw      180h
    dw      180h
    dw      0FFFFh
    dw      0FFFFh
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      0E187h
    dw      0E187h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      180h
    dw      0FFFFh
L_3F84:
    dw      0h
L_3F86:
    dw      0h

; 汉字字库缓存服务 (INT 7Fh)
L_3F88:
    jmp     dword ptr [cs:L_3F84]
L_3F8D:
    cld     
    cmp     dh,0AFh
    jnz     short L_3F88
    push    es
    push    ds
    pusha   
    sub     dx,0AFA1h
    shl     dx,byte ptr 5h
    mov     si,dx
    add     si,offset L_33C4
    push    cs
    pop     ds
    push    cs
    pop     es
    mov     di, offset L_0103
    shr     di,byte ptr 4h
    shl     di,byte ptr 4h
    mov     cx,10h
    rep     movsw
    popa    
    pop     ds
    pop     es
    push    ax
    mov     dx, offset L_0103
    shr     dx,byte ptr 4h
    mov     ax,cs
    add     dx,ax
    pop     ax
    iret    

; 驱动初始化与芯片校验
L_3FC5:
    cld     
    pusha   
    push    ds
    push    es
    push    cs
    pop     ds
    push    cs
    pop     es
    mov     ax,3518h
    int     21h
    mov     di,bx
    mov     si, offset L_408C
    mov     cx,1Eh
    sub     di,cx
    repe    cmpsb
    jz      short L_3FE9
    pop     es
    pop     ds
    popa    
    mov     ax,0FFFEh
    retf    
L_3FE7:
    add     [bx+si],al
L_3FE9:
    push    es
    pop     ds
    push    bx
    pop     si
    push    cs
    pop     es
    mov     di, offset L_0128
    mov     cx,[si]
    shl     cx,byte ptr 2h
    add     si,2h
    cld     
    rep     movsb
    push    cs
    pop     ds
    mov     ax,21h
    int     33h
    cmp     ax,0FFFFh
    jz      short L_4018
    xor     ax,ax
    int     33h
    cmp     ax,0FFFFh
    jz      short L_4018
    mov     word ptr [L_3FE7],0FFFFh
L_4018:
    mov     ax,0FF0Fh
    mov     bl,87h
    int     10h
    mov     byte ptr [L_408A],al
    mov     ax,0FF01h
    mov     bl,11h
    int     10h
    mov     byte ptr [L_408B],al
    mov     ax,3509h
    int     21h
    mov     word ptr [L_0141],bx
    mov     word ptr [L_0143],es
    mov     ax,3510h
    int     21h
    mov     word ptr [L_0124],bx
    mov     word ptr [L_0126],es
    mov     ax,3533h
    int     21h
    mov     word ptr [L_2B70],bx
    mov     word ptr [L_2B72],es
    mov     ax,3548h
    int     21h
    mov     word ptr [L_17FF],es
    mov     word ptr [L_17FD],bx
    mov     ax,357Fh
    int     21h
    mov     word ptr [L_3F86],es
    mov     word ptr [L_3F84],bx
    pop     es
    pop     ds
    popa    
    mov     ax,word ptr [cs:L_3FE7]
    retf    
L_4075:
    cli     
    xor     ah,ah
    shl     ax,byte ptr 2h
    mov     si,ax
    xor     bx,bx
    mov     ds,bx
    mov     [si],dx
    mov     [si+2h],cs
    push    cs
    pop     ds
    sti     
    ret     
L_408A:
    db      0h
L_408B:
    db      0h
L_408C:
    db      "UCDOS 5.0 SDK/UCVision Driver", 0

; 恢复原始中断向量
_SETOLDVICTOR:
L_40AA:
    pusha   
    push    es
    push    ds
    mov     ax,21h
    int     33h
    cmp     ax,0FFFFh
    jz      short L_40BB
    xor     ax,ax
    int     33h
L_40BB:
    mov     dx,word ptr [cs:L_0141]
    mov     ds,word ptr [cs:L_0143]
    mov     ax,2509h
    int     21h
    mov     dx,word ptr [cs:L_0124]
    mov     ds,word ptr [cs:L_0126]
    mov     ax,2510h
    int     21h
    mov     dx,word ptr [cs:L_2B70]
    mov     ds,word ptr [cs:L_2B72]
    mov     ax,2533h
    int     21h
    mov     dx,word ptr [cs:L_17FD]
    mov     ds,word ptr [cs:L_17FF]
    mov     ax,2548h
    int     21h
    mov     dx,word ptr [cs:L_3F84]
    mov     ds,word ptr [cs:L_3F86]
    mov     ax,257Fh
    int     21h
    xor     dx,dx
    mov     ds,dx
    mov     ax,2518h
    int     21h
    mov     ax,21h
    int     33h
    cmp     ax,0FFFFh
    jz      short L_411D
    xor     ax,ax
    int     33h
L_411D:
    mov     ax,0Fh
    mov     cx,8h
    mov     dx,10h
    int     33h
    mov     ax,0FF0Fh
    mov     bl,7h
    mov     bh,byte ptr [cs:L_408A]
    int     10h
    mov     ax,0FF01h
    mov     bl,1h
    mov     bh,byte ptr [cs:L_408B]
    int     10h
    mov     ax,0FF0Ah
    mov     bl,1h
    int     16h
    pop     ds
    pop     es
    popa    
    retf    

; 设置系统中断向量
_SETMYVICTOR:
L_414B:
    pusha   
    push    ds
    push    es
    mov     dx, offset L_015C
    mov     al,9h
    call    L_4075
    mov     dx, offset L_01D1
    mov     al,10h
    call    L_4075
    mov     dx, offset L_306D
    mov     al,33h
    call    L_4075
    mov     dx, offset L_2877
    mov     al,48h
    call    L_4075
    mov     dx, offset L_3F8D
    mov     al,7Fh
    call    L_4075
    call    L_417D
    pop     es
    pop     ds
    popa    
    retf    
L_417D:
    mov     ax,0FF00h
    mov     bx,0h
    int     10h
    or      al,al
    jz      short L_4191
    mov     ax,0FF00h
    mov     bx,2h
    int     10h
L_4191:
    mov     ax,0FF0Fh
    mov     bx,7h
    int     10h
    mov     ax,0FF01h
    mov     bx,1h
    int     10h
    mov     ax,0FF0Ah
    mov     bl,0h
    int     16h
    mov     ax,0FF01h
    mov     bl,6h
    int     16h
    ret     

_NEWTEXT ends
end
