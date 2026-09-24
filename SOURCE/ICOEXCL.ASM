; ICOEXCL.ASM - 内置图标数据

.model large

_TEXT segment byte public 'CODE'
assume cs:_TEXT

public _ICO_EXCLAMATION

_ICO_EXCLAMATION:
    db  00h, 00h, 01h, 00h, 01h, 00h, 20h, 20h, 10h, 00h, 00h, 00h, 00h, 00h, 0E8h, 02h
    db  00h, 00h, 16h, 00h, 00h, 00h, 28h, 00h, 00h, 00h, 20h, 00h, 00h, 00h, 40h, 00h
    db  00h, 00h, 01h, 00h, 04h, 00h, 00h, 00h, 00h, 00h, 80h, 02h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 80h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 80h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 80h, 00h, 80h, 80h, 00h, 00h, 80h, 80h, 80h, 00h, 0C0h, 0C0h
    db  0C0h, 00h, 00h, 00h, 0FFh, 00h, 00h, 0FFh, 00h, 00h, 00h, 0FFh, 0FFh, 00h, 0FFh, 00h
    db  00h, 00h, 0FFh, 00h, 0FFh, 00h, 0FFh, 0FFh, 00h, 00h, 0FFh, 0FFh, 0FFh, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 0Bh, 0BBh, 0BBh, 0B0h, 0Bh, 0BBh, 0BBh, 0B0h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 0Bh, 0BBh, 0BBh, 0B0h, 00h, 00h, 0Bh, 0BBh, 0BBh, 0B0h, 00h, 00h, 00h, 00h, 00h
    db  00h, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 00h, 0Bh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 00h
    db  0Bh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 00h, 00h, 00h
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 0Bh
    db  0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 00h, 0Bh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 00h, 0Bh
    db  0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 00h, 0Bh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 00h, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 0Bh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 00h, 0Bh, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 0Bh, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 0Bh, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 0Bh, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 0Bh, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 0Bh, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 0Bh, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 0Bh, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 0BBh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 0Bh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 00h, 0Bh
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 00h, 00h
    db  0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h
    db  0Bh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 00h, 00h, 00h
    db  00h, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 00h
    db  00h, 0Bh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 0BBh, 0BBh, 0B0h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 0Bh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0BBh, 0B0h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 0BBh, 0BBh, 0BBh, 0BBh, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0FFh, 0F0h
    db  0Fh, 0FFh, 0FFh, 80h, 01h, 0FFh, 0FEh, 00h, 00h, 7Fh, 0FCh, 00h, 00h, 3Fh, 0F8h, 00h
    db  00h, 1Fh, 0F0h, 00h, 00h, 0Fh, 0E0h, 00h, 00h, 07h, 0C0h, 00h, 00h, 03h, 0C0h, 00h
    db  00h, 03h, 80h, 00h, 00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h, 00h, 01h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 80h, 00h
    db  00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h, 00h, 01h, 0C0h, 00h, 00h, 03h, 0C0h, 00h
    db  00h, 03h, 0E0h, 00h, 00h, 07h, 0F0h, 00h, 00h, 0Fh, 0F8h, 00h, 00h, 1Fh, 0FCh, 00h
    db  00h, 3Fh, 0FEh, 00h, 00h, 7Fh, 0FFh, 80h, 01h, 0FFh, 0FFh, 0F0h, 0Fh, 0FFh

_TEXT ends
end
