; ICOASKM.ASM - 内置图标数据

.model large

_TEXT segment byte public 'CODE'
assume cs:_TEXT

public _ICO_QUESTION

_ICO_QUESTION:
    db  00h, 00h, 01h, 00h, 01h, 00h, 20h, 20h, 10h, 00h, 00h, 00h, 00h, 00h, 0E8h, 02h
    db  00h, 00h, 16h, 00h, 00h, 00h, 28h, 00h, 00h, 00h, 20h, 00h, 00h, 00h, 40h, 00h
    db  00h, 00h, 01h, 00h, 04h, 00h, 00h, 00h, 00h, 00h, 80h, 02h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 80h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 80h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 80h, 00h, 80h, 80h, 00h, 00h, 80h, 80h, 80h, 00h, 0C0h, 0C0h
    db  0C0h, 00h, 00h, 00h, 0FFh, 00h, 00h, 0FFh, 00h, 00h, 00h, 0FFh, 0FFh, 00h, 0FFh, 00h
    db  00h, 00h, 0FFh, 00h, 0FFh, 00h, 0FFh, 0FFh, 00h, 00h, 0FFh, 0FFh, 0FFh, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 22h, 22h, 22h, 22h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 02h, 22h, 22h, 2Fh, 0F2h, 22h, 22h, 20h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 02h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 20h, 00h, 00h, 00h, 00h, 00h
    db  00h, 22h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 00h, 00h, 00h, 00h, 00h
    db  02h, 22h, 22h, 22h, 0FFh, 0FFh, 0FFh, 0FFh, 22h, 22h, 22h, 20h, 00h, 00h, 00h, 00h
    db  22h, 22h, 22h, 22h, 0FFh, 0FFh, 0FFh, 0FFh, 22h, 22h, 22h, 22h, 00h, 00h, 00h, 02h
    db  22h, 22h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 22h, 20h, 00h, 00h, 02h
    db  22h, 22h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 22h, 20h, 00h, 00h, 22h
    db  22h, 22h, 22h, 22h, 22h, 2Fh, 0F2h, 22h, 22h, 22h, 22h, 22h, 22h, 00h, 00h, 22h
    db  22h, 22h, 22h, 22h, 22h, 22h, 22h, 22h, 22h, 22h, 22h, 22h, 22h, 00h, 00h, 22h
    db  22h, 22h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 22h, 22h, 00h, 02h, 22h
    db  22h, 22h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 22h, 22h, 20h, 02h, 22h
    db  22h, 22h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 22h, 22h, 20h, 02h, 22h
    db  22h, 22h, 22h, 22h, 22h, 0FFh, 0FFh, 0FFh, 22h, 22h, 22h, 22h, 22h, 20h, 02h, 22h
    db  22h, 22h, 22h, 22h, 22h, 0FFh, 0FFh, 0FFh, 22h, 22h, 22h, 22h, 22h, 20h, 02h, 22h
    db  22h, 22h, 22h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 22h, 20h, 02h, 22h
    db  22h, 22h, 22h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 22h, 20h, 02h, 22h
    db  22h, 22h, 22h, 22h, 22h, 22h, 0FFh, 0FFh, 0FFh, 22h, 22h, 22h, 22h, 20h, 02h, 22h
    db  22h, 22h, 22h, 22h, 22h, 22h, 0FFh, 0FFh, 0FFh, 22h, 22h, 22h, 22h, 20h, 00h, 22h
    db  22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 00h, 00h, 22h
    db  22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 00h, 00h, 22h
    db  22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 22h, 00h, 00h, 02h
    db  22h, 22h, 2Fh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0F2h, 22h, 22h, 20h, 00h, 00h, 02h
    db  22h, 22h, 2Fh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0F2h, 22h, 22h, 20h, 00h, 00h, 00h
    db  22h, 22h, 22h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 22h, 22h, 22h, 00h, 00h, 00h, 00h
    db  02h, 22h, 22h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 22h, 22h, 20h, 00h, 00h, 00h, 00h
    db  00h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0FFh, 0FFh, 0F2h, 22h, 22h, 00h, 00h, 00h, 00h, 00h
    db  00h, 02h, 22h, 22h, 2Fh, 0FFh, 0FFh, 0F2h, 22h, 22h, 20h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 02h, 22h, 22h, 22h, 22h, 22h, 22h, 20h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 22h, 22h, 22h, 22h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
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
