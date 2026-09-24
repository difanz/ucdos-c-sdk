; ICOAPP.ASM - 内置图标数据

.model large

_TEXT segment byte public 'CODE'
assume cs:_TEXT

public _ICO_APPLICATION

_ICO_APPLICATION:
    db  00h, 00h, 01h, 00h, 01h, 00h, 20h, 20h, 10h, 00h, 01h, 00h, 04h, 00h, 0E8h, 02h
    db  00h, 00h, 16h, 00h, 00h, 00h, 28h, 00h, 00h, 00h, 20h, 00h, 00h, 00h, 40h, 00h
    db  00h, 00h, 01h, 00h, 04h, 00h, 00h, 00h, 00h, 00h, 80h, 02h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 80h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 80h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 80h, 00h, 80h, 80h, 00h, 00h, 80h, 80h, 80h, 00h, 0C0h, 0C0h
    db  0C0h, 00h, 00h, 00h, 0FFh, 00h, 00h, 0FFh, 00h, 00h, 00h, 0FFh, 0FFh, 00h, 0FFh, 00h
    db  00h, 00h, 0FFh, 00h, 0FFh, 00h, 0FFh, 0FFh, 00h, 00h, 0FFh, 0FFh, 0FFh, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 77h
    db  77h, 77h, 77h, 77h, 77h, 77h, 77h, 77h, 77h, 77h, 77h, 77h, 77h, 77h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 07h, 00h, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 07h, 00h, 0FFh
    db  00h, 00h, 00h, 0FFh, 0F0h, 00h, 00h, 0Fh, 0FFh, 00h, 00h, 00h, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 07h, 00h, 0FFh
    db  55h, 0FFh, 55h, 0FFh, 0FFh, 00h, 0FFh, 0FFh, 0FFh, 0F0h, 00h, 0FFh, 0FFh, 07h, 00h, 0FFh
    db  0F5h, 05h, 0Fh, 0FFh, 0FFh, 0Bh, 0Fh, 0FFh, 0FFh, 0Eh, 0EEh, 0Fh, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 5Fh, 5Fh, 0FFh, 0F0h, 0F0h, 0B0h, 0FFh, 0FFh, 0Eh, 0CCh, 0Fh, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 0F5h, 5Fh, 0FFh, 0F0h, 0FFh, 09h, 0Fh, 0FFh, 0Eh, 0CEh, 0Fh, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 0FFh, 0Fh, 0FFh, 0FFh, 0Fh, 0F0h, 0Fh, 0FFh, 0F0h, 00h, 0FFh, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 07h, 00h, 0FFh
    db  00h, 00h, 00h, 0FFh, 0F0h, 00h, 00h, 0Fh, 0FFh, 00h, 00h, 00h, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 07h, 00h, 0FFh
    db  70h, 00h, 00h, 0FFh, 0FFh, 0F0h, 00h, 0FFh, 0FFh, 00h, 00h, 00h, 0FFh, 07h, 00h, 0FFh
    db  08h, 88h, 80h, 0FFh, 0F7h, 00h, 0F0h, 0FFh, 0FFh, 77h, 77h, 70h, 0FFh, 07h, 00h, 0FFh
    db  00h, 00h, 00h, 0FFh, 0F0h, 66h, 00h, 0FFh, 0FFh, 0F7h, 0CCh, 0Fh, 0FFh, 07h, 00h, 0FFh
    db  0F0h, 0FFh, 0Fh, 0FFh, 0FFh, 06h, 60h, 0FFh, 0FFh, 0F7h, 0CCh, 0Fh, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 00h, 0Fh, 0FFh, 0FFh, 0F0h, 00h, 0FFh, 0FFh, 0F7h, 77h, 0Fh, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 07h, 00h, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 07h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 07h, 00h, 8Eh
    db  8Eh, 8Eh, 8Eh, 8Eh, 8Eh, 8Eh, 8Eh, 8Eh, 8Eh, 8Eh, 8Eh, 8Eh, 8Eh, 07h, 00h, 0E8h
    db  0E8h, 0E8h, 0E8h, 0E8h, 0E8h, 0E8h, 0E8h, 0E8h, 0E8h, 0E8h, 0E8h, 0E8h, 0E8h, 07h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0FFh, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0C0h, 00h, 00h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 00h
    db  00h, 01h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh

_TEXT ends
end
