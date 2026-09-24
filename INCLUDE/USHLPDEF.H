#ifndef _USHLPDEF_H
#define _USHLPDEF_H

#ifndef __JJSDK__
#include "sdk.h"
#endif

#pragma pack(1)

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
