#ifndef _USHLPDEF_H
#define _USHLPDEF_H

#ifndef __JJSDK__
#include "sdk.h"
#endif

#pragma pack(1)

/* Distinct on-disk records; unknown fields retain their byte extent. */
typedef struct {
    WORD SIGNATURE;
    long CHECKSUM;
    BYTE UNKNOWN_06[8];
} HELP_FILE_HEADER;

typedef struct {
    WORD TITLE_LENGTH;
    BYTE UNKNOWN_02[4];
    long TEXT_LENGTH;
    long TEXT_OFFSET;
} HELP_POPUP_HEADER;

typedef struct {
    WORD TYPE;
    WORD TEXT_POSITION;
    DWORD TARGET_OFFSET;
} HELP_LINK_RECORD;

struct HelpInfo {
    WORD index_count;
    DWORD file_size;
    DWORD contents_offset;
    WORD field_a;
    DWORD keyword_offset;
    WORD keyword_count;
};

struct HelpContent {
    WORD title_len;
    DWORD title_offset;
    DWORD text_len;
    DWORD text_offset;
    DWORD link_offset;
};

struct HelpRect {
    WORD id;
    WORD len;
    WORD type;
    WORD text_pos;
    DWORD target_offset;
    WORD char_start;
    WORD line_start;
    WORD char_end;
    WORD line_end;
};

#pragma pack()

#endif
