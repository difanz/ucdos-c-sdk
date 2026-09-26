#ifndef _AGI_BMP_INTERNAL_H
#define _AGI_BMP_INTERNAL_H
#include "sdk.h"

/* Driver packet prefix; bits are passed separately in AL. */
#pragma pack(1)
typedef struct {
    char far *SOURCE_PTR;
    char far *DEST_PTR;
    int WIDTH;
    int COLOR;
    char far *SOURCE_PALETTE_PTR;
    char far *DEST_PALETTE_PTR;
    char MODEL;
} DIB_DRIVER_PACKET;
#pragma pack()
#endif
