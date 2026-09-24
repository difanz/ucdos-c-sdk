; ICOSTOP.ASM - 内置图标数据

.model large

_TEXT segment byte public 'CODE'
assume cs:_TEXT

public _ICO_HAND

_ICO_HAND:
    db  00h, 00h, 01h, 00h, 01h, 00h, 20h, 20h, 10h, 00h, 00h, 00h, 00h, 00h, 0E8h, 02h
    db  00h, 00h, 16h, 00h, 00h, 00h, 28h, 00h, 00h, 00h, 20h, 00h, 00h, 00h, 40h, 00h
    db  00h, 00h, 01h, 00h, 04h, 00h, 00h, 00h, 00h, 00h, 80h, 02h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 80h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 80h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 80h, 00h, 80h, 80h, 00h, 00h, 80h, 80h, 80h, 00h, 0C0h, 0C0h
    db  0C0h, 00h, 00h, 00h, 0FFh, 00h, 00h, 0FFh, 00h, 00h, 00h, 0FFh, 0FFh, 00h, 0FFh, 00h
    db  00h, 00h, 0FFh, 00h, 0FFh, 00h, 0FFh, 0FFh, 00h, 00h, 0FFh, 0FFh, 0FFh, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 01h, 11h, 11h, 11h, 11h, 11h, 11h, 10h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 01h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 10h, 00h, 00h, 00h, 00h, 00h
    db  00h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 00h, 00h, 00h, 00h, 00h
    db  01h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 10h, 00h, 00h, 00h, 00h
    db  11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 00h, 00h, 00h, 01h
    db  11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 10h, 00h, 00h, 11h
    db  11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 00h, 01h, 11h
    db  11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 10h, 01h, 11h
    db  11h, 1Fh, 0F1h, 11h, 1Fh, 11h, 11h, 0FFh, 11h, 1Fh, 11h, 11h, 11h, 10h, 01h, 11h
    db  11h, 0F1h, 1Fh, 11h, 1Fh, 11h, 1Fh, 11h, 0F1h, 1Fh, 11h, 11h, 11h, 10h, 01h, 11h
    db  11h, 0F1h, 1Fh, 11h, 1Fh, 11h, 1Fh, 11h, 0F1h, 1Fh, 11h, 11h, 11h, 10h, 01h, 11h
    db  11h, 0F1h, 1Fh, 11h, 1Fh, 11h, 1Fh, 11h, 0F1h, 1Fh, 11h, 11h, 11h, 10h, 01h, 11h
    db  11h, 11h, 1Fh, 11h, 1Fh, 11h, 1Fh, 11h, 0F1h, 1Fh, 11h, 11h, 11h, 10h, 01h, 11h
    db  11h, 11h, 1Fh, 11h, 1Fh, 11h, 1Fh, 11h, 0F1h, 1Fh, 11h, 11h, 11h, 10h, 01h, 11h
    db  11h, 1Fh, 0F1h, 11h, 1Fh, 11h, 1Fh, 11h, 0F1h, 1Fh, 0FFh, 11h, 11h, 10h, 01h, 11h
    db  11h, 0F1h, 11h, 11h, 1Fh, 11h, 1Fh, 11h, 0F1h, 1Fh, 11h, 0F1h, 11h, 10h, 01h, 11h
    db  11h, 0F1h, 11h, 11h, 1Fh, 11h, 1Fh, 11h, 0F1h, 1Fh, 11h, 0F1h, 11h, 10h, 01h, 11h
    db  11h, 0F1h, 1Fh, 11h, 1Fh, 11h, 1Fh, 11h, 0F1h, 1Fh, 11h, 0F1h, 11h, 10h, 01h, 11h
    db  11h, 0F1h, 1Fh, 11h, 1Fh, 11h, 1Fh, 11h, 0F1h, 1Fh, 11h, 0F1h, 11h, 10h, 01h, 11h
    db  11h, 1Fh, 0F1h, 1Fh, 0FFh, 0FFh, 11h, 0FFh, 11h, 1Fh, 0FFh, 11h, 11h, 10h, 01h, 11h
    db  11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 10h, 00h, 11h
    db  11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 00h, 00h, 01h
    db  11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 10h, 00h, 00h, 00h
    db  11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 00h, 00h, 00h, 00h
    db  01h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 10h, 00h, 00h, 00h, 00h
    db  00h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 00h, 00h, 00h, 00h, 00h
    db  00h, 01h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 10h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 11h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 01h, 11h, 11h, 11h, 11h, 11h, 11h, 10h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0FFh, 0FFh
    db  0FFh, 0FFh, 0FFh, 80h, 01h, 0FFh, 0FFh, 00h, 00h, 0FFh, 0FEh, 00h, 00h, 7Fh, 0FCh, 00h
    db  00h, 3Fh, 0F8h, 00h, 00h, 1Fh, 0F0h, 00h, 00h, 0Fh, 0E0h, 00h, 00h, 07h, 0C0h, 00h
    db  00h, 03h, 80h, 00h, 00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h
    db  00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h
    db  00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h
    db  00h, 01h, 80h, 00h, 00h, 01h, 80h, 00h, 00h, 01h, 0C0h, 00h, 00h, 03h, 0E0h, 00h
    db  00h, 07h, 0F0h, 00h, 00h, 0Fh, 0F8h, 00h, 00h, 1Fh, 0FCh, 00h, 00h, 3Fh, 0FEh, 00h
    db  00h, 7Fh, 0FFh, 00h, 00h, 0FFh, 0FFh, 80h, 01h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh

_TEXT ends
end
