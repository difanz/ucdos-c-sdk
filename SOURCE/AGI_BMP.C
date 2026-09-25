#include <mem.h>
#include <dos.h>
#include <dir.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <alloc.h>
#include <fcntl.h>
#include <io.h>
#include "agidrv.h"
#include <conio.h>
#include "struct.h"
#include "agi_win.h"
#include "agi_bmp.h"
#include "stretch.c"

WORD DAC_BEGIN = 0;
WORD DAC_COUNT = 16;

extern void (far *grp_entry)();
extern void (far *xms_entry)();

void UC_ReadBiosPalette(char *pal)
{
    int depth;
    char far *p;

    depth = UC_GetColourDepth();
    if (depth == 1 || depth == 2) {
        p = pal;
        asm {
            push es
            mov ax, word ptr p+2
            mov bx, word ptr p
            mov es, ax
            xor cl, cl
        }
    read_pal:
        asm {
            mov dx, 0x3c7
            mov al, cl
            out dx, al
            add dx, 2
            in al, dx
            shl al, 2
            mov es:[bx], al
            in al, dx
            shl al, 2
            mov es:[bx+1], al
            in al, dx
            shl al, 2
            mov es:[bx+2], al
            add bx, 3
            inc cl
            or cl, cl
            jnz read_pal
            pop es
        }
    }
}

void UC_SetBMPPalette(char *pal, int count)
{
    char far *p = pal;
    asm {
        push ds
        mov cx, count
        mov si, word ptr p
        mov di, word ptr p+2
        mov ds, di
        mov di, si
    }
set_pal:
    asm {
        mov bl, [si+1]
        mov [di+1], bl
        mov bh, [si]
        mov bl, [si+2]
        mov [di], bl
        mov [di+2], bh
        add di, 3
        add si, 4
        loop set_pal
        pop ds
    }
}

void UC_DIB2DDB(char *sbuf, char *dbuf, char *spal, char *dpal,
                int color, char model, int width, int depth, int bits)
{
    DIB_DRIVER_PACKET dib;
    char far *p;

    dib.SOURCE_PTR = sbuf;
    dib.DEST_PTR = dbuf;
    dib.SOURCE_PALETTE_PTR = spal;
    dib.DEST_PALETTE_PTR = dpal;
    dib.WIDTH = width;
    dib.MODEL = model;
    dib.COLOR = color;
    p = (char far *)&dib;

    asm {
        mov ah, 0x0a
        mov al, byte ptr bits
        mov cx, depth
        mov bx, word ptr p+2
        mov dx, word ptr p
        push es
        push bp
        mov es, bx
        mov bp, dx
        call far ptr [grp_entry]
        pop bp
        pop es
    }
}

int UC_GetIconInfo(char *ico_addr, WORD icn, WORD *width, WORD *depth,
                   WORD *bits)
{
    ICOHEAD icohdr;
    ICODIR icodir;
    ICODIB icodib;

    if (ico_addr == NULL)
        return 0;

    memmove(&icohdr, ico_addr, 6);
    if (icohdr.icoresv != 0 || icohdr.icotype < 1 || icohdr.icotype > 2 || icn > icohdr.icocount)
        return 0;

    if (icn == 0)
        icn = 1;

    memmove(&icodir, ico_addr + (icn - 1) * 16 + 6, 16);
    memmove(&icodib, ico_addr + icodir.icodiboffset, 40);

    if (icodib.infosize != 40 || icodib.bicompression != 0)
        return 0;

    *width = (WORD)icodib.width;
    *depth = (WORD)(icodib.depth >> 1);
    *bits = icodib.bits;
    return 1;
}

int UC_CreateMiniBMP(char *bmp_addr, char **ddb_buf, WORD *width,
                     WORD *depth, WORD *handle)
{
    BMPHEAD bmphdr;
    char id[3];
    int pal_bytes;
    int dib_pal_bytes;
    int rowbytes;
    int ddb_rowbytes;
    char far *dpal;
    char far *spal;
    char far *sbuf;
    char far *dbuf;
    int color;
    long i;

    if (bmp_addr == NULL)
        return 0;

    dpal = (char far *)PUBLIC_BUF;
    spal = (char far *)(PUBLIC_BUF + 0x300);

    memmove(&bmphdr, bmp_addr, 54);
    bmp_addr += 54;

    id[0] = bmphdr.id[0];
    id[1] = bmphdr.id[1];
    id[2] = 0;

    if (strcmp(id, "BM") != 0 || bmphdr.infosize != 40 || bmphdr.bicompression != 0)
        return 0;

    if (bmphdr.bits == 4) {
        if (bmphdr.biclrused == 0) {
            pal_bytes = 0x40;
            dib_pal_bytes = 0x30;
        } else {
            pal_bytes = (int)(bmphdr.biclrused << 2);
            dib_pal_bytes = (int)(bmphdr.biclrused * 3);
        }
    }
    if (bmphdr.bits == 8) {
        if (bmphdr.biclrused == 0) {
            pal_bytes = 0x400;
            dib_pal_bytes = 0x300;
        } else {
            pal_bytes = (int)(bmphdr.biclrused << 2);
            dib_pal_bytes = (int)(bmphdr.biclrused * 3);
        }
    }

    if (bmphdr.bits == 4 || bmphdr.bits == 8) {
        memmove(spal, bmp_addr, pal_bytes);
        bmp_addr += pal_bytes;
        UC_SetBMPPalette(spal, pal_bytes >> 2);
    }

    UC_ReadBiosPalette(dpal);

    if (bmphdr.bits == 4 || bmphdr.bits == 8) {
        setsavepalette(spal, dpal, 0, 256, dib_pal_bytes);
    }

    switch (bmphdr.bits) {
    case 1:
        rowbytes = (int)((bmphdr.width + 7) >> 3);
        break;
    case 4:
        rowbytes = (int)((bmphdr.width + 1) >> 1);
        break;
    case 8:
        rowbytes = (int)bmphdr.width;
        break;
    case 24:
        rowbytes = (int)(bmphdr.width * 3);
        break;
    }

    if (rowbytes & 3)
        rowbytes = (rowbytes | 3) + 1;

    sbuf = GRAPH_BUFFER;
    ddb_rowbytes = (int)bmphdr.width;
    asm {
        mov ah, 4
        mov bh, 1
        mov cx, ddb_rowbytes
        call far ptr [grp_entry]
        mov ddb_rowbytes, ax
    }
    dbuf = sbuf + rowbytes;

    *ddb_buf = malloc((long)ddb_rowbytes * bmphdr.depth);
    if (*ddb_buf == NULL)
        return 0;

    color = getmaxcolor();
    if (color != 15 && color != 255)
        color = 15;

    for (i = 0; i < (long)bmphdr.depth; i++) {
        memmove(sbuf, bmp_addr, rowbytes);
        bmp_addr += rowbytes;
        UC_DIB2DDB(sbuf, dbuf, spal, dpal, color, 0, (int)bmphdr.width, (int)i, bmphdr.bits);
        memmove(*ddb_buf + (long)(bmphdr.depth - i - 1) * ddb_rowbytes, dbuf, ddb_rowbytes);
    }

    *width = (WORD)bmphdr.width;
    *depth = (WORD)bmphdr.depth;
    *handle = UC_XMSLoadIcon(*ddb_buf, NULL, (int)bmphdr.width, (int)bmphdr.depth);
    if (*handle != 0) {
        MyFREE(*ddb_buf);
        *ddb_buf = NULL;
    }
    return 1;
}

void UC_ShowMiniBMP(WINDOWS *wnd, WORD handle, char *ddb_buf,
                    WORD width, WORD depth, int x, int y)
{
    int depth_val;
    char far *buf;
    int planes;
    int x1, y1, x2, y2;
    struct viewporttype vp;

    if (!UC_VerifyXMSHandle(handle)) {
        depth_val = UC_GetColourDepth();
        buf = ddb_buf;
        if (depth_val == 1 || depth_val == 3) {
            planes = (depth_val == 1) ? 4 : 1;
            buf = GRAPH_BUFFER;
            memmove(buf, ddb_buf, ((width + 7) >> 3) * planes * depth);
        }
    }

    if (x < 0) x = 0;
    if (y < 0) y = 0;

    if (wnd != NULL) {
        x += wnd->left + wnd->vx;
        y += wnd->top + wnd->vy;
        x1 = wnd->left + wnd->vx;
        y1 = wnd->top + wnd->vy;
        x2 = wnd->left + wnd->width - 1 - wnd->vr;
        y2 = wnd->top + wnd->height - 1 - wnd->vb;
    } else {
        x1 = 0;
        y1 = 0;
        x2 = getmaxx();
        y2 = getmaxy();
    }

    getviewsettings(&vp);
    if (UC_SecondViewport(x1, y1, x2, y2)) {
        UC_MouseHide();
        if (!UC_VerifyXMSHandle(handle)) {
            UC_PutScreenBlock(buf, width, depth, x, y);
        } else {
            UC_XMSputscreen(handle, x, y);
        }
        UC_MouseShow();
        Setviewport(vp.left, vp.top, vp.right, vp.bottom);
    }
}

int UC_CreateIcon(char *ico_addr, char **xor_buf, char **and_buf,
                  WORD *ico_width, WORD *ico_depth, WORD icn)
{
    ICOHEAD icohdr;
    ICODIR icodir;
    ICODIB icodib;
    int pal_bytes;
    int dib_pal_bytes;
    int xor_rowbytes;
    int and_rowbytes;
    int ddb_rowbytes;
    char far *dpal;
    char far *spal;
    char far *sbuf;
    char far *dbuf;
    int color;
    int depth;
    int i;

    if (ico_addr == NULL) {
        *xor_buf = NULL;
        *and_buf = NULL;
        return 0;
    }

    dpal = (char far *)PUBLIC_BUF;
    spal = (char far *)(PUBLIC_BUF + 0x300);

    memmove(&icohdr, ico_addr, 6);
    if (icohdr.icoresv != 0 || icohdr.icotype < 1 || icohdr.icotype > 2 || icn > icohdr.icocount)
        return 0;

    if (icn == 0)
        icn = 1;

    memmove(&icodir, ico_addr + (icn - 1) * 16 + 6, 16);
    ico_addr += icodir.icodiboffset;
    memmove(&icodib, ico_addr, 40);
    ico_addr += 40;

    if (icodib.infosize != 40 || icodib.bicompression != 0)
        return 0;

    if (icodib.bits == 4) {
        if (icodib.biclrused == 0) {
            pal_bytes = 0x40;
            dib_pal_bytes = 0x30;
        } else {
            pal_bytes = (int)(icodib.biclrused << 2);
            dib_pal_bytes = (int)(icodib.biclrused * 3);
        }
    }
    if (icodib.bits == 8) {
        if (icodib.biclrused == 0) {
            pal_bytes = 0x400;
            dib_pal_bytes = 0x300;
        } else {
            pal_bytes = (int)(icodib.biclrused << 2);
            dib_pal_bytes = (int)(icodib.biclrused * 3);
        }
    }
    if (icodib.bits == 1) {
        ico_addr += 8;
    }

    if (icodib.bits == 4 || icodib.bits == 8) {
        memmove(spal, ico_addr, pal_bytes);
        ico_addr += pal_bytes;
        UC_SetBMPPalette(spal, pal_bytes >> 2);
    }

    UC_ReadBiosPalette(dpal);

    if (icodib.bits == 4 || icodib.bits == 8) {
        setsavepalette(spal, dpal, 0, 256, dib_pal_bytes);
    }

    switch (icodib.bits) {
    case 1:
        xor_rowbytes = (int)((icodib.width + 7) >> 3);
        break;
    case 4:
        xor_rowbytes = (int)((icodib.width + 1) >> 1);
        break;
    case 8:
        xor_rowbytes = (int)icodib.width;
        break;
    }

    if (xor_rowbytes & 3)
        xor_rowbytes = (xor_rowbytes | 3) + 1;

    sbuf = GRAPH_BUFFER;
    ddb_rowbytes = (int)icodib.width;
    asm {
        mov ah, 4
        mov bh, 1
        mov cx, ddb_rowbytes
        call far ptr [grp_entry]
        mov ddb_rowbytes, ax
    }
    depth = (int)(icodib.depth >> 1);

    *xor_buf = malloc((long)ddb_rowbytes * depth);
    if (*xor_buf == NULL)
        nomem("建立ICO, xor_buf");

    *and_buf = malloc((long)ddb_rowbytes * depth);
    if (*and_buf == NULL)
        nomem("建立ICO, and_buf");

    color = getmaxcolor();
    if (color != 15 && color != 255)
        color = 15;

    dbuf = sbuf + xor_rowbytes;
    for (i = 0; i < depth; i++) {
        memmove(sbuf, ico_addr, xor_rowbytes);
        ico_addr += xor_rowbytes;
        UC_DIB2DDB(sbuf, dbuf, spal, dpal, color, 0, (int)icodib.width, i, icodib.bits);
        memmove(*xor_buf + (depth - i - 1) * ddb_rowbytes, dbuf, ddb_rowbytes);
    }

    and_rowbytes = (int)((icodib.width + 7) >> 3);
    if (and_rowbytes & 3)
        and_rowbytes = (and_rowbytes | 3) + 1;

    for (i = 0; i < depth; i++) {
        memmove(sbuf, ico_addr, and_rowbytes);
        ico_addr += and_rowbytes;
        UC_DIB2DDB(sbuf, dbuf, spal, dpal, 15, 0, (int)icodib.width, i, 1);
        memmove(*and_buf + (depth - i - 1) * ddb_rowbytes, dbuf, ddb_rowbytes);
    }

    *ico_width = (WORD)icodib.width;
    *ico_depth = (WORD)depth;
    return 1;
}

int UC_CreateScreenDDB(char *path, int x, int y, int width, int depth)
{
    char buf[80];
    int rowbytes;
    char depth_val;
    int block_lines;
    FILE *fp;
    int di;
    int si;
    int i;

    if (path == NULL || strlen(path) == 0)
        return 0;

    strcpy(buf, path);
    for (i = 0; i < strlen(buf); i++) {
        if (buf[i] == '.')
            break;
    }
    buf[i] = 0;
    strcat(buf, ".DDB");
    unlink(buf);

    rowbytes = width;
    asm {
        mov ah, 4
        mov bh, 1
        mov cx, rowbytes
        call far ptr [grp_entry]
        mov rowbytes, ax
    }
    depth_val = (char)UC_GetColourDepth();
    block_lines = BUFFER_LEN / rowbytes;

    fp = fopen(buf, "wb");
    if (fp == NULL)
        return 0;

    fwrite("DB", 1, 2, fp);
    fwrite(&depth_val, 1, 1, fp);
    fwrite(&width, 1, 2, fp);
    fwrite(&depth, 1, 2, fp);
    fwrite(&rowbytes, 1, 2, fp);

    for (di = y; di < y + depth; di += si) {
        si = block_lines;
        if (di + si > y + depth)
            si = y + depth - di;
        UC_GetScreenBlock(GRAPH_BUFFER, width, si, x, di);
        fwrite(GRAPH_BUFFER, rowbytes, si, fp);
    }
    fclose(fp);
    return 1;
}

void UC_GetBMPInfo(char *path, BMPHEAD *bmp)
{
    FILE *fp;
    if (path != NULL && strlen(path) != 0) {
        fp = fopen(path, "rb");
        if (fp != NULL) {
            if (fread(bmp, 1, 54, fp) != 54) {
                fclose(fp);
                return;
            }
            fclose(fp);
        }
    }
}

int UC_CreateBitmap(char *path)
{
    char name[80];
    char palname[80];
    float i;
    char bpp;
    int x, y;
    char far *dpal;
    char far *spal;
    FILE *fp;
    FILE *dfp;
    FILE *pfp;
    BMPHEAD bmphdr;
    char id[3];
    int pal_bytes;
    int dib_pal_bytes;
    int rowbytes;
    int ddb_rowbytes;
    char far *sbuf;
    char far *dbuf;
    int color;

    if (path == NULL || strlen(path) == 0)
        return 0;

    strcpy(name, path);
    strcpy(palname, path);

    i = 0.0f;
    while (i < strlen(name)) {
        if (name[(int)i] == '.')
            break;
        i += 1.0f;
    }
    name[(int)i] = 0;
    palname[(int)i] = 0;
    strcat(name, ".DDB");
    strcat(palname, ".PAL");
    name[(int)(i + 4.0f)] = 0;
    palname[(int)(i + 4.0f)] = 0;

    if (UC_CheckDDB(name))
        return 1;

    bpp = (char)UC_GetColourDepth();
    x = 10;
    y = 30;
    UC_GetRealXY(&x);
    UC_GetRealXY(&y);

    dpal = (char far *)PUBLIC_BUF;
    spal = (char far *)(PUBLIC_BUF + 0x300);

    fp = fopen(path, "rb");
    if (fp == NULL)
        return 0;

    if (fread(&bmphdr, 1, 54, fp) != 54) {
        fclose(fp);
        return 0;
    }

    id[0] = bmphdr.id[0];
    id[1] = bmphdr.id[1];
    id[2] = 0;

    if (strcmp(id, "BM") != 0 || bmphdr.infosize != 40 || bmphdr.bicompression != 0) {
        fclose(fp);
        return 0;
    }

    if (bmphdr.bits == 4) {
        if (bmphdr.biclrused == 0) {
            pal_bytes = 0x40;
            dib_pal_bytes = 0x30;
        } else {
            pal_bytes = (int)(bmphdr.biclrused << 2);
            dib_pal_bytes = (int)(bmphdr.biclrused * 3);
        }
    }
    if (bmphdr.bits == 8) {
        if (bmphdr.biclrused == 0) {
            pal_bytes = 0x400;
            dib_pal_bytes = 0x300;
        } else {
            pal_bytes = (int)(bmphdr.biclrused << 2);
            dib_pal_bytes = (int)(bmphdr.biclrused * 3);
        }
    }

    if (bmphdr.bits == 4 || bmphdr.bits == 8) {
        fread(spal, 1, pal_bytes, fp);
        UC_SetBMPPalette(spal, pal_bytes >> 2);
    }

    UC_ReadBiosPalette(dpal);

    if (bmphdr.bits == 4 || bmphdr.bits == 8) {
        setsavepalette(spal, dpal, DAC_BEGIN, DAC_COUNT, dib_pal_bytes);
    }

    switch (bmphdr.bits) {
    case 1:
        rowbytes = (int)((bmphdr.width + 7) >> 3);
        break;
    case 4:
        rowbytes = (int)((bmphdr.width + 1) >> 1);
        break;
    case 8:
        rowbytes = (int)bmphdr.width;
        break;
    case 24:
        rowbytes = (int)(bmphdr.width * 3);
        break;
    }

    if (rowbytes & 3)
        rowbytes = (rowbytes | 3) + 1;

    sbuf = GRAPH_BUFFER;
    ddb_rowbytes = (int)bmphdr.width;
    asm {
        mov ah, 4
        mov bh, 1
        mov cx, ddb_rowbytes
        call far ptr [grp_entry]
        mov ddb_rowbytes, ax
    }
    dbuf = sbuf + rowbytes;

    unlink(name);
    dfp = fopen(name, "wb");
    if (dfp == NULL) {
        fclose(fp);
        return 0;
    }

    fwrite("DB", 1, 2, dfp);
    fwrite(&bpp, 1, 1, dfp);
    fwrite(&bmphdr.width, 1, 2, dfp);
    fwrite(&bmphdr.depth, 1, 2, dfp);
    fwrite(&ddb_rowbytes, 1, 2, dfp);

    fseek(fp, bmphdr.headersize, SEEK_SET);

    color = getmaxcolor();
    if (color != 15 && color != 255)
        color = 15;

    for (i = 0.0f; i < (float)bmphdr.depth; i += 1.0f) {
        if (fread(sbuf, 1, rowbytes, fp) != rowbytes)
            break;
        UC_DIB2DDB(sbuf, dbuf, spal, dpal, color, 0, (int)bmphdr.width, (int)i, bmphdr.bits);
        fseek(dfp, (long)((bmphdr.depth - i - 1) * ddb_rowbytes + 9.0f), SEEK_SET);
        if (fwrite(dbuf, 1, ddb_rowbytes, dfp) != ddb_rowbytes)
            break;
    }

    fclose(fp);
    fclose(dfp);

    if (bpp == 2) {
        pfp = fopen(palname, "wb");
        if (pfp != NULL) {
            fwrite(spal, 1, 0x300, pfp);
            fclose(pfp);
            setvgapal(PUBLIC_BUF);
        }
    }
    return 1;
}

WORD UC_XMSLoadIcon(char *xor_buf, char *and_buf, int width, int depth)
{
    WORD handle;
    int rowbytes;
    int total_kb;
    int offset;

    if (xms_entry != NULL) {
        asm {
            mov ah, 4
            mov bh, 1
            mov cx, width
            call far ptr [grp_entry]
            mov rowbytes, ax
        }
        total_kb = (int)(((long)rowbytes * depth * 2 + 1027) / 1024);
        if (UC_AllocXMS(total_kb, &handle)) {
            UC_InitToXMS(handle, &width, 0L);
            UC_MoveToXMS(2);
            UC_InitToXMS(handle, &depth, 2L);
            UC_MoveToXMS(2);
            if (and_buf == NULL) {
                offset = 4;
            } else {
                UC_InitToXMS(handle, and_buf, 4L);
                UC_MoveToXMS(depth * rowbytes);
                offset = depth * rowbytes + 4;
            }
            UC_InitToXMS(handle, xor_buf, (DWORD)offset);
            UC_MoveToXMS(depth * rowbytes);
            return handle;
        }
    }
    return 0;
}

WORD UC_XMSLoadDDB(char *path)
{
    char buf[80];
    char magic[3];
    int i;
    FILE *fp;
    DDBHEAD hdr;
    int target_w, target_h;
    WORD handle;
    int rowbytes;
    int block_lines;
    int n;

    if (xms_entry == NULL || path == NULL || strlen(path) == 0)
        return 0;

    strcpy(buf, path);
    for (i = 0; i < strlen(buf); i++) {
        if (buf[i] == '.')
            break;
    }
    buf[i] = 0;
    strcat(buf, ".DDB");

    fp = fopen(buf, "rb");
    if (fp == NULL)
        return 0;

    fread(&hdr, 1, 9, fp);
    magic[0] = hdr.id[0];
    magic[1] = hdr.id[1];
    magic[2] = 0;
    if (strcmp(magic, "DB") != 0 || hdr.acmode != (BYTE)UC_GetColourDepth()) {
        fclose(fp);
        return 0;
    }
    {
        target_w = hdr.width;
        if (ZDDB_X != 0)
            target_w = ZDDB_X;
        target_h = hdr.depth;
        if (ZDDB_Y != 0)
            target_h = ZDDB_Y;

        if (target_w == ZDDB_X || target_h == ZDDB_Y) {
            fclose(fp);
            UC_ZoomDDB(buf, "tmpzoom.ddb", target_w, target_h, GRAPH_BUFFER, BUFFER_LEN);
            fp = fopen("tmpzoom.ddb", "rb");
            fread(&hdr, 1, 9, fp);
        }

        rowbytes = hdr.bytes;
        block_lines = BUFFER_LEN / rowbytes;
        if (UC_AllocXMS((WORD)(((long)rowbytes * hdr.depth + 1027) / 1024), &handle)) {
            UC_InitToXMS(handle, GRAPH_BUFFER, 0L);
            memmove(GRAPH_BUFFER, &hdr.width, 2);
            memmove(GRAPH_BUFFER + 2, &hdr.depth, 2);
            do {
                UC_MoveToXMS(block_lines * rowbytes);
                n = fread(GRAPH_BUFFER, rowbytes, block_lines, fp);
            } while (n != 0);

            fclose(fp);
            unlink("tmpzoom.ddb");
            return handle;
        }
    }
    fclose(fp);
    return 0;
}

WORD UC_XMSgetscreen(int x, int y, int width, int depth)
{
    WORD handle;
    int rowbytes;
    int block_lines;
    int di, si;

    if (xms_entry != NULL) {
        rowbytes = width;
        asm {
            mov ah, 4
            mov bh, 1
            mov cx, rowbytes
            call far ptr [grp_entry]
            mov rowbytes, ax
        }
        if (UC_AllocXMS((WORD)(((long)rowbytes * depth + 1027) / 1024), &handle)) {
            block_lines = BUFFER_LEN / rowbytes;
            UC_InitToXMS(handle, GRAPH_BUFFER, 0L);
            memmove(GRAPH_BUFFER, &width, 2);
            memmove(GRAPH_BUFFER + 2, &depth, 2);
            UC_MoveToXMS(4);
            for (di = y; di < y + depth; di += si) {
                si = block_lines;
                if (di + si > y + depth)
                    si = y + depth - di;
                UC_GetScreenBlock(GRAPH_BUFFER, width, si, x, di);
                UC_MoveToXMS(si * rowbytes);
            }
            return handle;
        }
    }
    return 0;
}

void UC_XMSputicon(WORD handle, int x, int y)
{
    WORD width, depth;
    int rowbytes;
    int block_lines;
    int di, si;

    if (UC_VerifyXMSHandle(handle)) {
        UC_InitFromXMS(handle, GRAPH_BUFFER, 0L);
        UC_MoveFromXMS(4);
        memmove(&width, GRAPH_BUFFER, 2);
        memmove(&depth, GRAPH_BUFFER + 2, 2);

        rowbytes = width;
        asm {
            mov ah, 4
            mov bh, 1
            mov cx, rowbytes
            call far ptr [grp_entry]
            mov rowbytes, ax
        }
        block_lines = BUFFER_LEN / rowbytes;

        setwritemode(1);
        for (di = y; di < y + depth; di += si) {
            si = block_lines;
            if (di + si > y + depth)
                si = y + depth - di;
            UC_MoveFromXMS(si * rowbytes);
            UC_PutScreenBlock(GRAPH_BUFFER, width, si, x, di);
        }

        setwritemode(3);
        for (di = y; di < y + depth; di += si) {
            si = block_lines;
            if (di + si > y + depth)
                si = y + depth - di;
            UC_MoveFromXMS(si * rowbytes);
            UC_PutScreenBlock(GRAPH_BUFFER, width, si, x, di);
        }
        setwritemode(0);
    }
}

void UC_XMSputscreen(WORD handle, int x, int y)
{
    WORD width, depth;
    int rowbytes;
    int block_lines;
    int di, si;

    if (UC_VerifyXMSHandle(handle)) {
        UC_InitFromXMS(handle, GRAPH_BUFFER, 0L);
        UC_MoveFromXMS(4);
        memmove(&width, GRAPH_BUFFER, 2);
        memmove(&depth, GRAPH_BUFFER + 2, 2);

        rowbytes = width;
        asm {
            mov ah, 4
            mov bh, 1
            mov cx, rowbytes
            call far ptr [grp_entry]
            mov rowbytes, ax
        }
        block_lines = BUFFER_LEN / rowbytes;

        for (di = y; di < y + depth; di += si) {
            si = block_lines;
            if (di + si > y + depth)
                si = y + depth - di;
            UC_MoveFromXMS(si * rowbytes);
            UC_PutScreenBlock(GRAPH_BUFFER, width, si, x, di);
        }
    }
}

void UC_GetScreenBlock(char *buf, int width, int depth, int x, int y)
{
    char far *p = buf;
    asm {
        push es
        push bp
        mov cx, x
        mov dx, y
        mov si, width
        mov di, depth
        mov ax, word ptr p
        mov bx, word ptr p+2
        mov es, bx
        mov bp, ax
        mov ah, 4
        mov bh, 2
        call far ptr [grp_entry]
        pop bp
        pop es
    }
}

void UC_PutScreenBlock(char *buf, int width, int depth, int x, int y)
{
    char far *p = buf;
    asm {
        push es
        push bp
        mov cx, x
        mov dx, y
        mov si, width
        mov di, depth
        mov ax, word ptr p
        mov bx, word ptr p+2
        mov es, bx
        mov bp, ax
        mov ah, 4
        mov bh, 3
        call far ptr [grp_entry]
        pop bp
        pop es
    }
}

int UC_CheckDDB(char *path)
{
    FILE *fp;
    char ddb_hdr[9];
    char magic[3];
    char depth;

    if (path == NULL || strlen(path) == 0)
        return 0;

    fp = fopen(path, "rb");
    if (fp == NULL)
        return 0;

    fread(ddb_hdr, 1, 9, fp);
    fclose(fp);

    magic[0] = ddb_hdr[0];
    magic[1] = ddb_hdr[1];
    magic[2] = 0;
    if (strcmp(magic, "DB") != 0)
        return 0;

    depth = (char)UC_GetColourDepth();
    if (depth != ddb_hdr[2])
        return 0;

    return 1;
}

int UC_DrawDDB(WINDOWS *wnd, WORD handle, char *path, int x, int y,
               int width, int height, int smode, int wmode)
{
    char palpath[80];
    char magic[3];
    char ddbpath[80];
    int i;
    char bpp;
    FILE *fp;
    int maxw, maxh;
    struct viewporttype vp;
    DDBHEAD hdr;
    int target_w, target_h;
    int rowbytes;
    int block_lines;
    int left, top, right, bottom;
    int cur_x, cur_y;
    int chunk_lines;
    int n;

    if (path == NULL || strlen(path) == 0)
        return 0;

    fp = NULL;
    strcpy(palpath, path);
    strcpy(ddbpath, path);
    for (i = 0; i < strlen(palpath); i++) {
        if (palpath[i] == '.')
            break;
    }
    palpath[i] = 0;
    ddbpath[i] = 0;
    strcat(palpath, ".PAL");
    strcat(ddbpath, ".DDB");
    palpath[i + 4] = 0;
    ddbpath[i + 4] = 0;

    bpp = (char)UC_GetColourDepth();
    if (bpp == 2 && (windows_exec || !windows_rdbk)) {
        fp = fopen(palpath, "rb");
        if (fp != NULL) {
            fread(PUBLIC_BUF, 1, 0x300, fp);
            fclose(fp);
            setvgapal(PUBLIC_BUF);
        }
    }

    if (!UC_WindowVerify(wnd)) {
        maxw = getmaxx() - x;
        maxh = getmaxy() - y;
    } else {
        maxw = wnd->width - wnd->vx - wnd->vr - x;
        maxh = wnd->height - wnd->vy - wnd->vb - y;
    }

    if (maxw < width)
        width = maxw;
    if (maxh < height)
        height = maxh;

    getviewsettings(&vp);

    if (UC_VerifyXMSHandle(handle)) {
        UC_InitFromXMS(handle, GRAPH_BUFFER, 0L);
        UC_MoveFromXMS(4);
        memmove(&target_w, GRAPH_BUFFER, 2);
        memmove(&target_h, GRAPH_BUFFER + 2, 2);

        rowbytes = target_w;
        asm {
            mov ah, 4
            mov bh, 1
            mov cx, rowbytes
            call far ptr [grp_entry]
            mov rowbytes, ax
        }
        goto draw_it;
    }

    fp = fopen(ddbpath, "rb");
    if (fp != NULL) {
        char magic[3];
        fread(&hdr, 1, 9, fp);
        magic[0] = hdr.id[0];
        magic[1] = hdr.id[1];
        magic[2] = 0;
        if (strcmp(magic, "DB") != 0 || hdr.acmode != bpp) {
            fclose(fp);
            return 0;
        }
        {
            target_w = hdr.width;
            if (ZDDB_X != 0)
                target_w = ZDDB_X;
            target_h = hdr.depth;
            if (ZDDB_Y != 0)
                target_h = ZDDB_Y;

            if (target_w == ZDDB_X || target_h == ZDDB_Y) {
                fclose(fp);
                UC_ZoomDDB(ddbpath, "tmpzoom.ddb", target_w, target_h, GRAPH_BUFFER, BUFFER_LEN);
                fp = fopen("tmpzoom.ddb", "rb");
                fread(&hdr, 1, 9, fp);
            }
            rowbytes = hdr.bytes;
            goto draw_it;
        }
        fclose(fp);
    }
    return 0;

draw_it:
    block_lines = BUFFER_LEN / rowbytes;
    if (wmode == 1) {
        if (target_w < maxw)
            x += (maxw - target_w) >> 1;
        if (target_h < maxh)
            y += (maxh - target_h) >> 1;
    }
    if (wmode == 2) {
        x = 0;
        y = 0;
    }

    if (UC_WindowVerify(wnd)) {
        x += wnd->left + wnd->vx;
        y += wnd->top + wnd->vy;
    }

    left = x;
    top = y;
    if (x < 0) left = 0;
    if (y < 0) top = 0;

    if (UC_WindowVerify(wnd)) {
        if (left < wnd->left + wnd->vx)
            left = wnd->left + wnd->vx;
        if (top < wnd->top + wnd->vy)
            top = wnd->top + wnd->vy;
    }

    right = left + width - 1;
    bottom = top + height - 1;

    if (UC_SecondViewport(left, top, right, bottom)) {
        setwritemode(smode);
        UC_MouseHide();

        if (wmode == 2) {
            for (cur_y = y; cur_y < bottom; cur_y += target_h) {
                for (cur_x = x; cur_x < right; cur_x += target_w) {
                    if (!UC_VerifyXMSHandle(handle)) {
                        fseek(fp, 9L, SEEK_SET);
                        if (cur_x <= right && left <= cur_x + target_w &&
                            cur_y <= bottom && top <= cur_y + target_h) {
                            chunk_lines = cur_y;
                            while (chunk_lines < cur_y + target_h) {
                                n = fread(GRAPH_BUFFER, rowbytes, block_lines, fp);
                                if (n <= 0) break;
                                if (top <= chunk_lines + n)
                                    UC_PutScreenBlock(GRAPH_BUFFER, target_w, n, cur_x, chunk_lines);
                                chunk_lines += n;
                                if (chunk_lines > bottom) break;
                            }
                        }
                    } else {
                        UC_XMSputscreen(handle, cur_x, cur_y);
                    }
                }
                if (!UC_WindowVerify(wnd))
                    x = 0;
                else
                    x = wnd->left + wnd->vx;
            }
        } else {
            if (x <= right && left <= x + target_w &&
                y <= bottom && top <= y + target_h) {
                if (!UC_VerifyXMSHandle(handle)) {
                    chunk_lines = y;
                    while (chunk_lines < y + target_h) {
                        n = fread(GRAPH_BUFFER, rowbytes, block_lines, fp);
                        if (n <= 0) break;
                        if (top <= chunk_lines + n)
                            UC_PutScreenBlock(GRAPH_BUFFER, target_w, n, x, chunk_lines);
                        chunk_lines += n;
                        if (chunk_lines > bottom) break;
                    }
                } else {
                    UC_XMSputscreen(handle, x, y);
                }
            }
        }

        Setviewport(vp.left, vp.top, vp.right, vp.bottom);
        setwritemode(0);
        UC_MouseShow();
    }

    if (!UC_VerifyXMSHandle(handle)) {
        fclose(fp);
        unlink("tmpzoom.ddb");
    }
    return 1;
}

void UC_ShowIcon(WINDOWS *wnd, WORD handle, char *xor_buf, char *and_buf,
                 int width, int depth, int x, int y)
{
    int depth_val;
    char far *xor_p;
    char far *and_p;
    int planes;
    int x1, y1, x2, y2;
    struct viewporttype vp;

    if (!UC_VerifyXMSHandle(handle)) {
        depth_val = UC_GetColourDepth();
        xor_p = xor_buf;
        and_p = and_buf;
        if (depth_val == 1 || depth_val == 3) {
            planes = (depth_val == 1) ? 4 : 1;
            xor_p = GRAPH_BUFFER;
            and_p = GRAPH_BUFFER + ((width + 7) >> 3) * planes * depth;
            memmove(xor_p, xor_buf, ((width + 7) >> 3) * planes * depth);
            memmove(and_p, and_buf, ((width + 7) >> 3) * planes * depth);
        }
    }

    if (wnd != NULL) {
        x += wnd->left + wnd->vx;
        y += wnd->top + wnd->vy;
        x1 = wnd->left + wnd->vx;
        y1 = wnd->top + wnd->vy;
        x2 = wnd->left + wnd->width - 1 - wnd->vr;
        y2 = wnd->top + wnd->height - 1 - wnd->vb;
    } else {
        x1 = 0;
        y1 = 0;
        x2 = getmaxx();
        y2 = getmaxy();
    }

    getviewsettings(&vp);
    if (UC_SecondViewport(x1, y1, x2, y2)) {
        UC_MouseHide();
        if (!UC_VerifyXMSHandle(handle)) {
            setwritemode(1);
            UC_PutScreenBlock(and_p, width, depth, x, y);
            setwritemode(3);
            UC_PutScreenBlock(xor_p, width, depth, x, y);
            setwritemode(0);
        } else {
            UC_XMSputicon(handle, x, y);
        }
        UC_MouseShow();
        Setviewport(vp.left, vp.top, vp.right, vp.bottom);
    }
}

void UC_Safe256palette(int dac_begin, int dac_count)
{
    asm {
        mov dx, dac_begin
        mov ax, dac_count
        db 0x0b, 0xd2
        jl l1
        cmp dx, 255
        jng l2
    }
l1:
    asm db 0x33, 0xc0
l2:
    asm {
        cmp ax, 256
        jng l3
        mov dx, 256
    }
l3:
    asm {
        mov word ptr DAC_BEGIN, dx
        mov word ptr DAC_COUNT, dx
    }
}
