; ICOEXIT.ASM - 内置图标数据

.model large

_TEXT segment byte public 'CODE'
assume cs:_TEXT

public _ICO_EXIT

_ICO_EXIT:
    db  00h, 00h, 01h, 00h, 01h, 00h, 20h, 20h, 10h, 00h, 00h, 00h, 00h, 00h, 0E8h, 02h
    db  00h, 00h, 16h, 00h, 00h, 00h, 28h, 00h, 00h, 00h, 20h, 00h, 00h, 00h, 40h, 00h
    db  00h, 00h, 01h, 00h, 04h, 00h, 00h, 00h, 00h, 00h, 80h, 02h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 10h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 80h, 00h, 00h, 80h, 00h, 00h, 00h, 80h, 80h, 00h, 80h, 00h
    db  00h, 00h, 80h, 00h, 80h, 00h, 80h, 80h, 00h, 00h, 0C0h, 0C0h, 0C0h, 00h, 80h, 80h
    db  80h, 00h, 00h, 00h, 0FFh, 00h, 00h, 0FFh, 00h, 00h, 00h, 0FFh, 0FFh, 00h, 0FFh, 00h
    db  00h, 00h, 0FFh, 00h, 0FFh, 00h, 0FFh, 0FFh, 00h, 00h, 0FFh, 0FFh, 0FFh, 00h, 00h, 00h
    db  88h, 0FFh, 0F8h, 8Fh, 0FFh, 88h, 0FFh, 0F0h, 00h, 0FFh, 88h, 0FFh, 00h, 00h, 00h, 8Fh
    db  0FFh, 88h, 0FFh, 0F8h, 8Fh, 0FFh, 88h, 0F0h, 0EEh, 0Fh, 0FFh, 88h, 0FFh, 00h, 0Fh, 0F8h
    db  8Fh, 0FFh, 88h, 0FFh, 0F8h, 8Fh, 0FFh, 80h, 0EEh, 60h, 8Fh, 0FFh, 88h, 0F0h, 88h, 0FFh
    db  0F8h, 8Fh, 0FFh, 88h, 0FFh, 0F8h, 8Fh, 0F0h, 0EEh, 66h, 08h, 8Fh, 0FFh, 88h, 0FFh, 88h
    db  0FFh, 0F8h, 8Fh, 0FFh, 88h, 0FFh, 0F8h, 80h, 0EEh, 66h, 60h, 0F8h, 8Fh, 0FFh, 8Fh, 0FFh
    db  88h, 0FFh, 0F8h, 8Fh, 0FFh, 88h, 0FFh, 0F0h, 0EEh, 66h, 66h, 0Fh, 0F8h, 8Fh, 00h, 00h
    db  00h, 00h, 00h, 00h, 0Fh, 0FFh, 88h, 0F0h, 0EEh, 66h, 66h, 60h, 00h, 00h, 0FFh, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 08h, 8Fh, 0FFh, 80h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0Fh, 0F8h, 8Fh, 0F0h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0FFh, 8Fh, 00h, 00h, 00h, 00h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0F0h, 88h, 07h, 77h, 77h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0F0h, 08h, 07h, 77h, 77h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0F0h, 60h, 07h, 77h, 77h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0F0h, 66h, 07h, 77h, 77h, 70h, 0EEh, 60h, 06h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  88h, 88h, 80h, 0E6h, 60h, 77h, 77h, 70h, 0EEh, 07h, 06h, 60h, 78h, 0FFh, 0FFh, 0F8h
    db  88h, 88h, 80h, 0EEh, 66h, 07h, 77h, 70h, 0EEh, 0Fh, 06h, 60h, 78h, 0FFh, 0FFh, 00h
    db  00h, 00h, 00h, 0EEh, 0E6h, 60h, 77h, 70h, 0EEh, 60h, 06h, 60h, 78h, 0FFh, 0F0h, 66h
    db  66h, 66h, 66h, 0EEh, 0EEh, 66h, 07h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0F0h, 0EEh
    db  0EEh, 0EEh, 0EEh, 0EEh, 0EEh, 0E6h, 60h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0F0h, 0EEh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0EEh, 60h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0F0h, 0EEh
    db  0EEh, 0EEh, 0EEh, 0EEh, 0FEh, 0E6h, 07h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 00h
    db  00h, 00h, 00h, 0EFh, 0EEh, 60h, 77h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0F0h, 0EEh, 0E6h, 07h, 77h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0F0h, 0EEh, 60h, 77h, 77h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0F0h, 0E6h, 07h, 77h, 77h, 70h, 0EEh, 66h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0F0h, 60h, 07h, 77h, 77h, 77h, 0Eh, 0E6h, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0F0h, 0Fh, 07h, 77h, 77h, 77h, 70h, 0EEh, 66h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 07h, 77h, 77h, 77h, 77h, 0Eh, 0E6h, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 07h, 77h, 77h, 77h, 77h, 70h, 0EEh, 60h, 78h, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 07h, 77h, 77h, 77h, 77h, 77h, 0Eh, 0E0h, 7Fh, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 0FFh, 0FFh, 0FFh, 0FFh
    db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0F0h, 00h
    db  00h, 0Fh, 0C0h, 00h, 00h, 03h, 80h, 00h, 00h, 01h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h
    db  00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h, 00h

_TEXT ends
end
