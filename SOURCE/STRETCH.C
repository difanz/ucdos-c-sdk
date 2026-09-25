#include <stdio.h>
#include <dos.h>

WORD ZDDB_X = 0;
WORD ZDDB_Y = 0;

void StretchLine(char far *src, char far *dst, int src_w, int dst_w, int depth);

void UC_SetZoomDDB(WORD dstwidth, WORD dstheight)
{
    ZDDB_X = dstwidth;
    ZDDB_Y = dstheight;
}

void UC_GetZoomDDB(WORD *dstwidth, WORD *dstheight)
{
    *dstwidth = ZDDB_X;
    *dstheight = ZDDB_Y;
}

int UC_ZoomIcon(char *ddbbuf, WORD width, WORD depth,
                char *DstFile, WORD DstWidth, WORD DstHeight,
                char *buf, WORD BufLen)
{
    char path[80];
    int p;
    char far *dbuf;
    char far *sbuf;
    int dy;
    int sy;
    int planes = 1;
    int dst_rowbytes;
    FILE *fp;
    char bpp;
    int src_rowbytes;
    int i;

    strcpy(path, DstFile);
    for (i = 0; i < strlen(path); i++) {
        if (path[i] == '.')
            break;
    }
    path[i] = 0;
    strcat(path, ".DDB");

    bpp = UC_GetColourDepth();
    switch (bpp) {
    case 1:
        planes = 4;
        src_rowbytes = (width + 7) >> 3;
        dst_rowbytes = (DstWidth + 7) >> 3;
        break;
    case 2:
        src_rowbytes = width;
        dst_rowbytes = DstWidth;
        break;
    case 3:
        src_rowbytes = (width + 7) >> 3;
        dst_rowbytes = (DstWidth + 7) >> 3;
        break;
    case 12:
    case 13:
        src_rowbytes = width << 1;
        dst_rowbytes = DstWidth << 1;
        break;
    case 14:
        src_rowbytes = width * 3;
        dst_rowbytes = DstWidth * 3;
        break;
    case 15:
        src_rowbytes = width << 2;
        dst_rowbytes = DstWidth << 2;
        break;
    default:
        return -5;
    }

    if (src_rowbytes * planes + dst_rowbytes > BufLen)
        return -4;

    fp = fopen(path, "wb");
    if (fp == NULL)
        return -2;

    *(WORD far *)((DDBHEAD far *)buf)->id = 0x4244;
    ((DDBHEAD far *)buf)->acmode = bpp;
    ((DDBHEAD far *)buf)->width = DstWidth;
    ((DDBHEAD far *)buf)->depth = DstHeight;
    ((DDBHEAD far *)buf)->bytes = dst_rowbytes * planes;
    fwrite(buf, 9, 1, fp);

    sbuf = buf;
    dbuf = buf + src_rowbytes * planes;

    dy = 0;
    sy = 0;
    while (sy < depth) {
        while (dy < depth) {
            memmove(sbuf, ddbbuf, src_rowbytes * planes);
            ddbbuf += src_rowbytes * planes;
            dy += DstHeight;
            sy++;
        }
        while (dy >= depth) {
            for (p = 0; p < planes; p++) {
                StretchLine(sbuf + p * src_rowbytes, dbuf, width, DstWidth, bpp);
                fwrite(dbuf, 1, dst_rowbytes, fp);
            }
            dy -= depth;
        }
    }

    fclose(fp);
    return 0;
}

int UC_ZoomDDB(char *SrcFile, char *DstFile, WORD DstWidth,
               WORD DstHeight, char *Buf, WORD BufLen)
{
    char src[80];
    char dst[80];
    int p;
    char far *dbuf;
    char far *sbuf;
    int dy;
    int sy;
    int planes = 1;
    int dst_rowbytes;
    int src_rowbytes;
    DDBHEAD hdr;
    FILE *dfp;
    FILE *sfp;
    int i;

    strcpy(src, SrcFile);
    strcpy(dst, DstFile);
    for (i = 0; i < strlen(src); i++) {
        if (src[i] == '.')
            break;
    }
    src[i] = 0;
    strcat(src, ".DDB");

    for (i = 0; i < strlen(dst); i++) {
        if (dst[i] == '.')
            break;
    }
    dst[i] = 0;
    strcat(dst, ".DDB");

    sfp = fopen(src, "rb");
    if (sfp == NULL)
        return -1;

    if (fread(&hdr, 9, 1, sfp) != 1 || *(WORD far *)hdr.id != 0x4244) {
        fclose(sfp);
        return -3;
    }

    switch ((char)hdr.acmode) {
    case 1:
        planes = 4;
        src_rowbytes = (hdr.width + 7) >> 3;
        dst_rowbytes = (DstWidth + 7) >> 3;
        break;
    case 2:
        src_rowbytes = hdr.width;
        dst_rowbytes = DstWidth;
        break;
    case 3:
        src_rowbytes = (hdr.width + 7) >> 3;
        dst_rowbytes = (DstWidth + 7) >> 3;
        break;
    case 12:
    case 13:
        src_rowbytes = hdr.width << 1;
        dst_rowbytes = DstWidth << 1;
        break;
    case 14:
        src_rowbytes = hdr.width * 3;
        dst_rowbytes = DstWidth * 3;
        break;
    case 15:
        src_rowbytes = hdr.width << 2;
        dst_rowbytes = DstWidth << 2;
        break;
    default:
        fclose(sfp);
        return -5;
    }

    if (src_rowbytes * planes + dst_rowbytes > BufLen) {
        fclose(sfp);
        return -4;
    }

    dfp = fopen(dst, "wb");
    if (dfp == NULL) {
        fclose(sfp);
        return -2;
    }

    *(WORD far *)((DDBHEAD far *)Buf)->id = 0x4244;
    ((DDBHEAD far *)Buf)->acmode = (char)hdr.acmode;
    ((DDBHEAD far *)Buf)->width = DstWidth;
    ((DDBHEAD far *)Buf)->depth = DstHeight;
    ((DDBHEAD far *)Buf)->bytes = dst_rowbytes * planes;
    fwrite(Buf, 9, 1, dfp);

    sbuf = Buf;
    dbuf = Buf + src_rowbytes * planes;

    dy = 0;
    sy = 0;
    while (sy < hdr.depth) {
        while (dy < hdr.depth) {
            fread(sbuf, src_rowbytes * planes, 1, sfp);
            dy += DstHeight;
            sy++;
        }
        while (dy >= hdr.depth) {
            for (p = 0; p < planes; p++) {
                StretchLine(sbuf + p * src_rowbytes, dbuf, hdr.width, DstWidth, (char)hdr.acmode);
                fwrite(dbuf, 1, dst_rowbytes, dfp);
            }
            dy -= hdr.depth;
        }
    }

    fclose(sfp);
    fclose(dfp);
    return 0;
}
