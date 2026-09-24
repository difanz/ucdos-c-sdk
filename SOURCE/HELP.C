/* -----------------------------------------------------------------
   UCDOS 5.0 SDK for C/C++    UC Vision Help System
   HELP.C - UCDOS SDK Help viewer
   Authors: 石磊, 刘晓光
   Copyright: 北京希望高技术集团
   Date: 1996年9月27日
-------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>
#include <dir.h>
#include "sdk.h"
#include <conio.h>
#include "ushlpdef.h"

/* 函数声明 */
void HelpRedraw(WINDOWS *wnd);
void HelpClose(void);
void HelpResize(WINDOWS *wnd);
void HelpHScrollbar(SCROLLBAR *sbar, float prevcure);
void HelpVScrollbar(SCROLLBAR *sbar, float prevcure);
int  HelpKeyPress(int key, char state);
void HelpMousePress(int x, int y, WORD state);
void HelpAbout(void);
void HelpOpen(void);
void HelpContent(void);
void HelpBack(void);
void HelpIndex(void);
void IndexClose(void);
int  IndexKeyPress(int key, char state);
void IndexScrollbar(SCROLLBAR *sbar, float prevcure);
void IndexSelect(void);
void HelpOpenFile(void);
void HelpCloseFile(void);
void HelpReadPage(long file_pos);
int  TranTextXToGraph(int x);
int  TranTextYToGraph(int y);
void HelpShowPage(void);
void HelpShowWin(long target_offset, int x, int y);

/* 全局数据定义 */
char *VarString = "版本: 1.0 (Build 72)";
char *gpcHelpName = "UCDOS SDK for C/C++ Help\0-              ";
WINDOWS *gpwndHelp = NULL;
char *gpcFindString = NULL;
FILE *gpHelpFile = NULL;
int giWinLine = 0;
int giWinWide = 0;
int giPageLine = 0;
int giPageWide = 0;
int giPageFirstLine = 1;
int giPageFirstChar = 1;
int gnRectNumber = 0;
struct HelpRect *gpsttHelpRect = NULL;
int giOldPointer = 0;
char *gpcTextString = NULL;
WINDOWS *gpwndIndex = NULL;
int giIndex = 0;
char **gppcIndexText = NULL;
int giTabSele = 0;

static char *MenuFileSelects[] = {
    "O [O]打开...",
    "-",
    "B [B]后退     ALT+B ",
    "I [I]索引...  ALT+I ",
    "-",
    "X [X]退出       ESC ",
    NULL
};

static void (*MenuFileFunctions[])(void) = {
    HelpOpen,
    NULL,
    HelpBack,
    HelpIndex,
    NULL,
    HelpClose
};

static char *MenuAboutSelects[] = {
    "A [A]关于...  ALT+A ",
    NULL
};

static void (*MenuAboutFunctions[])(void) = {
    HelpAbout
};

static MENUS HelpMenus[] = {
    { "F [F]文件 ", MenuFileSelects, MenuFileFunctions, 0 },
    { "A [A]关于 ", MenuAboutSelects, MenuAboutFunctions, 0 },
    { NULL, NULL, NULL, 0 }
};

/* BSS 变量 */
char gacFileName[80];
struct HelpInfo gsttInfo;
struct HelpContent gsttContent;
struct HelpContent gasttOldContent[10];
char gaTextBuf[220];
USER_MOUSE *gpsttMouse;

/* -----------------------------------------------------------------
   _UC_WinHelp
-------------------------------------------------------------------*/
void UC_WinHelp(char *pcHelpTitle, char *pcFileName, char *pcKeyWord,
                int iLeft, int iTop, int iWidth, int iHigh)
{
    RECT rect;
    int left, top, width, height;

    if (UC_WindowVerify(gpwndHelp)) {
        if (gpwndHelp->minmax == 1) {
            UC_MaxWindow(gpwndHelp);
        } else {
            UC_WindowEnable(gpwndHelp);
        }
        goto open_file;
    }

    left = iLeft;
    top = iTop;
    width = iWidth;
    height = iHigh;

    if (iLeft == 0 && iTop == 0 && iWidth == 0 && iHigh == 0) {
        left = -1;
        top = -1;
        width = -2;
        height = -2;
    } else {
        if (iWidth == 0) {
            width = -2;
        } else if (iWidth > 0) {
            if (UC_RetReal(215) < iWidth) {
                width = UC_RetReal(215);
            }
        }
        if (iHigh == 0) {
            height = -2;
        } else if (iHigh > 0) {
            if (iHigh < 200) {
                height = 200;
            }
        }
    }

    gpwndHelp = UC_DefineWindow(1, left, top, width, height,
                                pcHelpTitle ? pcHelpTitle : gpcHelpName,
                                HelpRedraw, HelpClose, HelpResize);
    UC_DefineWindowMinSize(gpwndHelp, UC_RetReal(215), 200);
    UC_DefineWindowIcon(gpwndHelp, ICO_QUESTION, 1, NULL);
    UC_DefineMenu(gpwndHelp, HelpMenus);
    UC_DefineWindowHScrollbar(gpwndHelp, HelpHScrollbar);
    UC_DefineWindowVScrollbar(gpwndHelp, HelpVScrollbar);
    UC_DefineKeyboardEvent(gpwndHelp, HelpKeyPress);

    UC_GetClientRect(gpwndHelp, &rect);
    gpsttMouse = UC_DefineUserMouseEvent(gpwndHelp, 0, 0, 0, 0, 3, NULL, HelpMousePress);
    UC_WindowEnable(gpwndHelp);

    giWinLine = (rect.bottom - rect.top - UC_RetReal(25) - 5) / (gpwndHelp->syschar_size + UC_RetReal(2));
    giWinWide = (rect.right - rect.left - 5) / (gpwndHelp->syschar_size / 2);

open_file:
    HelpCloseFile();
    gpcFindString = pcKeyWord;
    if (pcFileName == NULL) {
        HelpOpen();
    } else {
        if (strlen(pcFileName) + 1 > 80) {
            UC_DialogWarning(NULL, "打开文件警告", "  您的文件名太长了!\n  打开文件失败!");
            HelpOpen();
        } else {
            memmove(gacFileName, pcFileName, strlen(pcFileName) + 1);
            HelpOpenFile();
        }
    }
}

/* -----------------------------------------------------------------
   _HelpRedraw
-------------------------------------------------------------------*/
void HelpRedraw(WINDOWS *wnd)
{
    int msg;
    RECT rect;
    RECT btn_rect;

    msg = UC_GetMessage();
    UC_SetMessage(msg);
    setcolor(0);

    if (msg == 0x1388 || msg == 0x1389) {
        goto draw_page;
    }

    UC_GetClientRect(gpwndHelp, &rect);
    UC_MouseHide();

    line(rect.left + wnd->vx,
         rect.top + wnd->vy + UC_RetReal(25),
         rect.left + wnd->vx + rect.right - rect.left,
         rect.top + wnd->vy + UC_RetReal(25));

    setcolor(0);
    setfillstyle(SOLID_FILL, 7);

    UC_WindowBar(gpwndHelp, rect.left, rect.top, UC_RetReal(65), UC_RetReal(25));
    UC_WindowBar(gpwndHelp, rect.left + UC_RetReal(65), rect.top, UC_RetReal(65), UC_RetReal(25));
    UC_WindowBar(gpwndHelp, rect.left + 2 * UC_RetReal(65), rect.top, UC_RetReal(65), UC_RetReal(25));

    UC_DrawButton(rect.left + wnd->vx,
                  rect.top + wnd->vy,
                  rect.left + wnd->vx + UC_RetReal(65),
                  rect.top + wnd->vy + UC_RetReal(25),
                  1, 1);

    UC_DrawButton(rect.left + wnd->vx + UC_RetReal(65),
                  rect.top + wnd->vy,
                  rect.left + wnd->vx + 2 * UC_RetReal(65),
                  rect.top + wnd->vy + UC_RetReal(25),
                  1, 1);

    UC_DrawButton(rect.left + wnd->vx + 2 * UC_RetReal(65),
                  rect.top + wnd->vy,
                  rect.left + wnd->vx + 3 * UC_RetReal(65),
                  rect.top + wnd->vy + UC_RetReal(25),
                  1, 1);

    btn_rect = rect;
    btn_rect.left = rect.left + UC_RetReal(65);
    btn_rect.top = rect.top + UC_RetReal(25);
    UC_DrawText(gpwndHelp, &btn_rect, 10, "目录[C]");

    btn_rect.left += UC_RetReal(65);
    btn_rect.right += UC_RetReal(65);
    UC_DrawText(gpwndHelp, &btn_rect, 10, "索引[I]");

    btn_rect.left += UC_RetReal(65);
    btn_rect.right += UC_RetReal(65);
    UC_DrawText(gpwndHelp, &btn_rect, 10, "后退[B]");

    UC_MouseShow();

draw_page:
    if (gpHelpFile != NULL) {
        HelpShowPage();
    }
}

/* -----------------------------------------------------------------
   _HelpClose
-------------------------------------------------------------------*/
void HelpClose(void)
{
    giWinLine = 0;
    giWinWide = 0;
    UC_WindowClose();
    if (gpHelpFile != NULL) {
        HelpCloseFile();
    }
}

/* -----------------------------------------------------------------
   _HelpResize
-------------------------------------------------------------------*/
void HelpResize(WINDOWS *wnd)
{
    RECT rect;

    UC_GetClientRect(gpwndHelp, &rect);
    giWinLine = (rect.bottom - rect.top - UC_RetReal(25) - 5) / (gpwndHelp->syschar_size + UC_RetReal(2));
    giWinWide = (rect.right - rect.left - 5) / (gpwndHelp->syschar_size / 2);

    gpwndHelp->vbar->cure = (float)(giPageFirstLine - 1);
    gpwndHelp->hbar->cure = (float)(giPageFirstChar - 1);

    gpwndHelp->vbar->page = (float)((rect.bottom - rect.top - UC_RetReal(25) - 5) / (gpwndHelp->syschar_size + UC_RetReal(2)) - 2);
    gpwndHelp->hbar->page = (float)(((rect.right - rect.left - 5) / (gpwndHelp->syschar_size / 2)) / 2 - 2);

    gpwndHelp->vbar->max = (float)(giPageLine - (rect.bottom - rect.top - UC_RetReal(25) - 5) / (gpwndHelp->syschar_size + UC_RetReal(2)) + 1);
    if (gpwndHelp->vbar->max < 2.0) {
        gpwndHelp->vbar->max = 1.0;
        gpwndHelp->vbar->cure = 0.0;
        giPageFirstLine = 1;
    }

    gpwndHelp->hbar->max = (float)(giPageWide - (rect.right - rect.left - 5) / (gpwndHelp->syschar_size / 2) + 1);
    if (gpwndHelp->hbar->max < 2.0) {
        gpwndHelp->hbar->max = 1.0;
        gpwndHelp->hbar->cure = 0.0;
        giPageFirstChar = 1;
    }

    UC_DisplayScrollbar(gpwndHelp, gpwndHelp->vbar);
    UC_DisplayScrollbar(gpwndHelp, gpwndHelp->hbar);
}

/* -----------------------------------------------------------------
   _HelpHScrollbar
-------------------------------------------------------------------*/
void HelpHScrollbar(SCROLLBAR *sbar, float prevcure)
{
    giPageFirstChar = (int)(sbar->cure + 1);
    UC_SetMessage(0x1388);
    HelpRedraw(gpwndHelp);
}

/* -----------------------------------------------------------------
   _HelpVScrollbar
-------------------------------------------------------------------*/
void HelpVScrollbar(SCROLLBAR *sbar, float prevcure)
{
    giPageFirstLine = (int)(sbar->cure + 1);
    UC_SetMessage(0x1388);
    HelpRedraw(gpwndHelp);
}

/* -----------------------------------------------------------------
   _HelpKeyPress
-------------------------------------------------------------------*/
int HelpKeyPress(int key, char state)
{
    int old_tab;
    int k = key;

    switch (k) {
    case 0x1e00: /* Alt+A */
        HelpAbout();
        return 1;

    case 0x011b: /* ESC */
        HelpClose();
        return 1;

    case 0x2e00: /* Alt+C */
        HelpContent();
        return 1;

    case 0x3000: /* Alt+B */
    case 0x0e08: /* Backspace */
        HelpBack();
        return 1;

    case 0x1700: /* Alt+I */
    case 0x6800: /* Alt+F1 */
        HelpIndex();
        return 1;

    case 0x0f09: /* Tab */
        if (gpHelpFile == NULL || gnRectNumber == 0) return 1;
        old_tab = giTabSele;
        do {
            if (giTabSele >= gnRectNumber) {
                giTabSele = 1;
            } else {
                giTabSele++;
            }
        } while (giTabSele != old_tab &&
                 (gpsttHelpRect[giTabSele - 1].char_end < giPageFirstChar ||
                  gpsttHelpRect[giTabSele - 1].char_start > giPageFirstChar + giWinWide ||
                  gpsttHelpRect[giTabSele - 1].line_end < giPageFirstLine ||
                  gpsttHelpRect[giTabSele - 1].line_start > giPageFirstLine + giWinLine));

        UC_SetMessage(0x1389);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x0f00: /* Shift+Tab */
        if (gpHelpFile == NULL || gnRectNumber == 0) return 1;
        old_tab = giTabSele;
        do {
            if (giTabSele <= 1) {
                giTabSele = gnRectNumber;
            } else {
                giTabSele--;
            }
        } while (giTabSele != old_tab &&
                 (gpsttHelpRect[giTabSele - 1].char_end < giPageFirstChar ||
                  gpsttHelpRect[giTabSele - 1].char_start > giPageFirstChar + giWinWide ||
                  gpsttHelpRect[giTabSele - 1].line_end < giPageFirstLine ||
                  gpsttHelpRect[giTabSele - 1].line_start > giPageFirstLine + giWinLine));

        UC_SetMessage(0x1389);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x1c0d: /* Enter */
        if (gpHelpFile == NULL || gnRectNumber == 0 || giTabSele == 0) return 1;
        if (gpsttHelpRect[giTabSele - 1].type == 1) {
            HelpReadPage(gpsttHelpRect[giTabSele - 1].target_offset);
            UC_SetMessage(0x1388);
            HelpRedraw(gpwndHelp);
        } else {
            HelpShowWin(gpsttHelpRect[giTabSele - 1].target_offset,
                        TranTextXToGraph(gpsttHelpRect[giTabSele - 1].char_start),
                        TranTextYToGraph(gpsttHelpRect[giTabSele - 1].line_start) + gpwndHelp->syschar_size + UC_RetReal(2));
        }
        return 1;

    case 0x4b00: /* Left */
        if (giPageFirstChar == 1) return 1;
        if (giPageFirstChar > 1) giPageFirstChar--;
        gpwndHelp->hbar->cure = (float)(giPageFirstChar - 1);
        UC_UpdateScrollbar(gpwndHelp, gpwndHelp->hbar);
        UC_SetMessage(0x1388);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x4d00: /* Right */
        if ((float)giPageFirstChar == gpwndHelp->hbar->max) return 1;
        giPageFirstChar++;
        gpwndHelp->hbar->cure = (float)(giPageFirstChar - 1);
        UC_UpdateScrollbar(gpwndHelp, gpwndHelp->hbar);
        UC_SetMessage(0x1388);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x4700: /* Home */
        if (giPageFirstChar == 1) return 1;
        giPageFirstChar = 1;
        gpwndHelp->hbar->cure = (float)(giPageFirstChar - 1);
        UC_UpdateScrollbar(gpwndHelp, gpwndHelp->hbar);
        UC_SetMessage(0x1388);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x4f00: /* End */
        if ((float)giPageFirstChar == gpwndHelp->hbar->max) return 1;
        giPageFirstChar = (int)gpwndHelp->hbar->max;
        gpwndHelp->hbar->cure = (float)(giPageFirstChar - 1);
        UC_UpdateScrollbar(gpwndHelp, gpwndHelp->hbar);
        UC_SetMessage(0x1388);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x4800: /* Up */
        if (giPageFirstLine == 1) return 1;
        if (giPageFirstLine > 1) giPageFirstLine--;
        gpwndHelp->vbar->cure = (float)(giPageFirstLine - 1);
        UC_UpdateScrollbar(gpwndHelp, gpwndHelp->vbar);
        UC_SetMessage(0x1388);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x5000: /* Down */
        if ((float)giPageFirstLine == gpwndHelp->vbar->max) return 1;
        giPageFirstLine++;
        gpwndHelp->vbar->cure = (float)(giPageFirstLine - 1);
        UC_UpdateScrollbar(gpwndHelp, gpwndHelp->vbar);
        UC_SetMessage(0x1388);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x7700: /* Ctrl+PageUp */
        if (giPageFirstLine == 1) return 1;
        giPageFirstLine = 1;
        gpwndHelp->vbar->cure = (float)(giPageFirstLine - 1);
        UC_UpdateScrollbar(gpwndHelp, gpwndHelp->vbar);
        UC_SetMessage(0x1388);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x7500: /* Ctrl+PageDown */
        if ((float)giPageFirstLine == gpwndHelp->vbar->max) return 1;
        giPageFirstLine = (int)gpwndHelp->vbar->max;
        gpwndHelp->vbar->cure = (float)(giPageFirstLine - 1);
        UC_UpdateScrollbar(gpwndHelp, gpwndHelp->vbar);
        UC_SetMessage(0x1388);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x4900: /* PageUp */
        if (giPageFirstLine == 1) return 1;
        giPageFirstLine = (int)((float)giPageFirstLine - gpwndHelp->vbar->page);
        if (giPageFirstLine < 1) giPageFirstLine = 1;
        gpwndHelp->vbar->cure = (float)(giPageFirstLine - 1);
        UC_UpdateScrollbar(gpwndHelp, gpwndHelp->vbar);
        UC_SetMessage(0x1388);
        HelpRedraw(gpwndHelp);
        return 1;

    case 0x5100: /* PageDown */
        if ((float)giPageFirstLine == gpwndHelp->vbar->max) return 1;
        giPageFirstLine = (int)((float)giPageFirstLine + gpwndHelp->vbar->page);
        if ((float)giPageFirstLine > gpwndHelp->vbar->max) {
            giPageFirstLine = (int)gpwndHelp->vbar->max;
        }
        gpwndHelp->vbar->cure = (float)(giPageFirstLine - 1);
        UC_UpdateScrollbar(gpwndHelp, gpwndHelp->vbar);
        UC_SetMessage(0x1388);
        HelpRedraw(gpwndHelp);
        return 1;
    }

    return 0;
}

/* -----------------------------------------------------------------
   _HelpMousePress
-------------------------------------------------------------------*/
void HelpMousePress(int x, int y, WORD state)
{
    RECT rect;
    int char_x, line_y, x1, x2;
    int i;

    UC_GetClientRect(gpwndHelp, &rect);

    if (state != 1) {
        if (x >= gpwndHelp->left + gpwndHelp->vx + rect.left &&
            x <= gpwndHelp->left + gpwndHelp->vx + rect.left + UC_RetReal(65) &&
            y >= gpwndHelp->top + gpwndHelp->vy + rect.top &&
            y <= gpwndHelp->top + gpwndHelp->vy + rect.top + UC_RetReal(25)) {
            if (UC_DoPressButton(NULL, NULL, gpwndHelp->left + gpwndHelp->vx + rect.left,
                                 gpwndHelp->top + gpwndHelp->vy + rect.top,
                                 gpwndHelp->left + gpwndHelp->vx + rect.left + UC_RetReal(65),
                                 gpwndHelp->top + gpwndHelp->vy + rect.top + UC_RetReal(25))) {
                HelpContent();
            }
            return;
        }

        if (x >= gpwndHelp->left + gpwndHelp->vx + rect.left + UC_RetReal(65) &&
            x <= gpwndHelp->left + gpwndHelp->vx + rect.left + 2 * UC_RetReal(65) &&
            y >= gpwndHelp->top + gpwndHelp->vy + rect.top &&
            y <= gpwndHelp->top + gpwndHelp->vy + rect.top + UC_RetReal(25)) {
            if (UC_DoPressButton(NULL, NULL, gpwndHelp->left + gpwndHelp->vx + rect.left + UC_RetReal(65),
                                 gpwndHelp->top + gpwndHelp->vy + rect.top,
                                 gpwndHelp->left + gpwndHelp->vx + rect.left + 2 * UC_RetReal(65),
                                 gpwndHelp->top + gpwndHelp->vy + rect.top + UC_RetReal(25))) {
                HelpIndex();
            }
            return;
        }

        if (x >= gpwndHelp->left + gpwndHelp->vx + rect.left + 2 * UC_RetReal(65) &&
            x <= gpwndHelp->left + gpwndHelp->vx + rect.left + 3 * UC_RetReal(65) &&
            y >= gpwndHelp->top + gpwndHelp->vy + rect.top &&
            y <= gpwndHelp->top + gpwndHelp->vy + rect.top + UC_RetReal(25)) {
            if (UC_DoPressButton(NULL, NULL, gpwndHelp->left + gpwndHelp->vx + rect.left + 2 * UC_RetReal(65),
                                 gpwndHelp->top + gpwndHelp->vy + rect.top,
                                 gpwndHelp->left + gpwndHelp->vx + rect.left + 3 * UC_RetReal(65),
                                 gpwndHelp->top + gpwndHelp->vy + rect.top + UC_RetReal(25))) {
                HelpBack();
            }
            return;
        }
    }

    char_x = (x - gpwndHelp->left - gpwndHelp->vx - 5) / (gpwndHelp->syschar_size / 2);
    line_y = (y - gpwndHelp->top - gpwndHelp->vy - UC_RetReal(25) - 5) / (gpwndHelp->syschar_size + UC_RetReal(2));

    for (i = 0; i < gnRectNumber; i++) {
        x1 = gpsttHelpRect[i].char_start - giPageFirstChar;
        if (x1 < 0) x1 = -1;
        x2 = gpsttHelpRect[i].char_end - giPageFirstChar;
        if (x2 < 0) x2 = -1;

        if (char_x >= x1 && char_x <= x2 && gpsttHelpRect[i].line_start - giPageFirstLine == line_y) {
            break;
        }
    }

    if (state == 1) {
        if (i < gnRectNumber && y <= (line_y + 1) * (gpwndHelp->syschar_size + UC_RetReal(2)) + gpwndHelp->top + gpwndHelp->vy + UC_RetReal(25) + 5 - UC_RetReal(2) - 1) {
            gpsttMouse->usermap = UC_GetIDC(10);
        } else {
            gpsttMouse->usermap = UC_GetIDC(0);
        }
        return;
    }

    if (UC_WaitFreeMouse(gpwndHelp,
                         x1 * (gpwndHelp->syschar_size / 2) + 5,
                         line_y * (gpwndHelp->syschar_size + UC_RetReal(2)) + UC_RetReal(25) + 5,
                         (x2 + 1) * (gpwndHelp->syschar_size / 2) + 4,
                         (line_y + 1) * (gpwndHelp->syschar_size + UC_RetReal(2)) + UC_RetReal(25) + 5 - UC_RetReal(2) - 1,
                         NULL)) {
        if (i < gnRectNumber) {
            if (gpsttHelpRect[i].type == 1) {
                UC_MouseShapeType(0);
                gpsttMouse->usermap = UC_GetIDC(0);
                HelpReadPage(gpsttHelpRect[i].target_offset);
                UC_SetMessage(0x1388);
                HelpRedraw(gpwndHelp);
            } else {
                UC_MouseShapeType(0);
                gpsttMouse->usermap = UC_GetIDC(0);
                HelpShowWin(gpsttHelpRect[i].target_offset,
                            x,
                            ((y - gpwndHelp->top - gpwndHelp->vy - UC_RetReal(25) - 5) / (gpwndHelp->syschar_size + UC_RetReal(2))) * (gpwndHelp->syschar_size + UC_RetReal(2)) + gpwndHelp->top + gpwndHelp->vy + UC_RetReal(25) + 5);
            }
        }
    }
}

/* -----------------------------------------------------------------
   _HelpAbout
-------------------------------------------------------------------*/
void HelpAbout(void)
{
    UC_DialogAbout(NULL, ICO_QUESTION, 0, "关于帮助",
                   "  UC SDK for C/C++ 帮助\n  %s\n  作者: 石磊, 刘晓光\n  版权所有: 北京希望高技术集团\n  出版日期: 1996年9月27日",
                   VarString);
}

/* -----------------------------------------------------------------
   _HelpOpen
-------------------------------------------------------------------*/
void HelpOpen(void)
{
    int argw = 0;
    char *arg[] = { "*.USH", NULL };
    char *path;

    path = UC_DialogFileOpen(1, &argw, arg, "打开帮助文件", NULL, NULL);
    while (strlen(path) + 1 > 80) {
        UC_DialogWarning(NULL, "打开文件警告", "  您的文件名太长了!\n  打开文件失败!");
        path = UC_DialogFileOpen(1, &argw, arg, "打开帮助文件", NULL, NULL);
    }

    if (path == NULL) {
        gacFileName[0] = '\0';
    } else {
        memmove(gacFileName, path, strlen(path) + 1);
    }
    HelpOpenFile();
}

/* -----------------------------------------------------------------
   _HelpContent
-------------------------------------------------------------------*/
void HelpContent(void)
{
    if (gpHelpFile == NULL) {
        UC_DialogWarning(NULL, "帮助警告", "  帮助文件没有打开");
        return;
    }
    HelpReadPage(gsttInfo.contents_offset);
    UC_SetMessage(0x1388);
    HelpRedraw(gpwndHelp);
}

/* -----------------------------------------------------------------
   _HelpBack
-------------------------------------------------------------------*/
void HelpBack(void)
{
    if (gpHelpFile == NULL) {
        UC_DialogWarning(NULL, "帮助警告", "  帮助文件没有打开");
        return;
    }
    HelpReadPage(0L);
    UC_SetMessage(0x1388);
    HelpRedraw(gpwndHelp);
}

/* -----------------------------------------------------------------
   _HelpIndex
-------------------------------------------------------------------*/
void HelpIndex(void)
{
    int i;
    struct HelpContent item;

    if (gpHelpFile == NULL) {
        UC_DialogWarning(NULL, "帮助警告", "  帮助文件没有打开");
        return;
    }

    gpwndIndex = UC_DefineWindow(4, -1, -1, 316, 285, "目录索引", NULL, IndexClose, NULL);
    UC_DefineKeyboardEvent(gpwndIndex, IndexKeyPress);
    UC_DefineListBox(gpwndIndex, 20, 20, 31, 10, (float)gsttInfo.index_count, IndexScrollbar, IndexSelect, 1);
    UC_DefinePressButton(gpwndIndex, IndexSelect, 60, 215, 80, 25, "确认", 0);
    UC_DefinePressButton(gpwndIndex, IndexClose, 160, 215, 80, 25, "放弃", 0);
    UC_DefineActive(gpwndIndex, 3, 1);

    gppcIndexText = (char **)malloc(gsttInfo.index_count * sizeof(char *));
    if (gppcIndexText == NULL) {
        UC_WindowClose();
        UC_DialogWarning(NULL, "索引警告", "  帮助索引字串指针空间不足!");
        return;
    }

    giIndex = 0;
    for (i = 0; i < gsttInfo.index_count; i++) {
        fseek(gpHelpFile, gsttInfo.contents_offset + (long)i * sizeof(struct HelpContent), SEEK_SET);
        fread(&item, sizeof(struct HelpContent), 1, gpHelpFile);
        gppcIndexText[i] = (char *)malloc(item.title_len + 1);
        if (gppcIndexText[i] == NULL) {
            UC_WindowClose();
            UC_DialogWarning(NULL, "索引警告", "  帮助索引字串空间不足!");
            for (i = i - 1; i >= 0; i--) {
                MyFREE(gppcIndexText[i]);
            }
            MyFREE(gppcIndexText);
            gppcIndexText = NULL;
            return;
        }
        gppcIndexText[i][item.title_len] = '\0';
        fseek(gpHelpFile, item.title_offset, SEEK_SET);
        fread(gppcIndexText[i], item.title_len, 1, gpHelpFile);
    }

    UC_WindowEnable(gpwndIndex);
}

/* -----------------------------------------------------------------
   _IndexClose
-------------------------------------------------------------------*/
void IndexClose(void)
{
    int i;

    UC_WindowClose();
    for (i = 0; i < gsttInfo.index_count; i++) {
        MyFREE(gppcIndexText[i]);
    }
    MyFREE(gppcIndexText);
    gppcIndexText = NULL;
}

/* -----------------------------------------------------------------
   _IndexKeyPress
-------------------------------------------------------------------*/
int IndexKeyPress(int key, char state)
{
    if (key == 0x011b) {
        IndexClose();
        return 1;
    }
    return 0;
}

/* -----------------------------------------------------------------
   _IndexScrollbar
-------------------------------------------------------------------*/
void IndexScrollbar(SCROLLBAR *sbar, float prevcure)
{
    giIndex = (int)sbar->cure;
    UC_UpdateListBox(gpwndIndex,
                     gpwndIndex->box_head,
                     gppcIndexText + (int)UC_GetBoxTopCount(gpwndIndex->box_head),
                     (int)((float)gsttInfo.index_count - UC_GetBoxTopCount(gpwndIndex->box_head)));
}

/* -----------------------------------------------------------------
   _IndexSelect
-------------------------------------------------------------------*/
void IndexSelect(void)
{
    IndexClose();
    HelpReadPage(gsttInfo.contents_offset + (long)giIndex * sizeof(struct HelpContent));
    HelpRedraw(gpwndHelp);
}

/* -----------------------------------------------------------------
   _HelpOpenFile
-------------------------------------------------------------------*/
void HelpOpenFile(void)
{
    int i, j;
    FILE *fp;
    char head[14];
    struct HelpContent content;
    long sum;
    char ch;

    if (gacFileName[0] == '\0') {
        return;
    }

    fp = fopen(gacFileName, "rb");
    if (fp == NULL) {
        UC_DialogWarning(NULL, "文件打开错", "  文件打开失败");
        return;
    }

    for (i = 1; strlen(gacFileName) >= i; i++) {
        if (gacFileName[strlen(gacFileName) - i] == '\\') {
            break;
        }
    }

    gpcHelpName[24] = ' ';
    memmove(gpcHelpName + 27, gacFileName + strlen(gacFileName) - i + 1, 12);
    UC_DisplayTitle(gpwndHelp);

    fread(head, 14, 1, fp);
    if (*(WORD *)head != 0x4855) {
        UC_DialogWarning(NULL, "文件打开错", "  不是 UCDOS SDK for C/C++ 的帮助文件!");
        fclose(fp);
        return;
    }

    sum = 0;
    while (!feof(fp)) {
        fread(&ch, 1, 1, fp);
        sum += (unsigned char)ch;
    }

    if (*(long *)&head[2] != sum) {
        UC_DialogWarning(NULL, "文件打开错", "  帮助文件校验错!\n  请重新生成!");
        fclose(fp);
        return;
    }

    if (gpHelpFile != NULL) {
        HelpCloseFile();
    }
    gpHelpFile = fp;
    fseek(gpHelpFile, 0L, SEEK_SET);
    fread(head, 14, 1, gpHelpFile);
    fread(&gsttInfo, sizeof(struct HelpInfo), 1, gpHelpFile);

    gpsttHelpRect = (struct HelpRect *)malloc(gsttInfo.keyword_count * sizeof(struct HelpRect));
    if (gsttInfo.keyword_count != 0 && gpsttHelpRect == NULL) {
        UC_DialogWarning(NULL, "打开文件警告", "  帮助关键字位置结构空间不足!");
        HelpCloseFile();
        return;
    }

    giOldPointer = 0;

    if (gpcFindString == NULL) {
        HelpReadPage(gsttInfo.contents_offset);
    } else {
        gppcIndexText = (char **)malloc(gsttInfo.index_count * sizeof(char *));
        if (gppcIndexText == NULL) {
            UC_DialogWarning(NULL, "打开文件警告", "  帮助索引字串指针空间不足!");
            HelpCloseFile();
            return;
        }
        giIndex = 0;

        for (i = 0; i < strlen(gpcFindString); i++) {
            gpcFindString[i] = toupper(gpcFindString[i]);
        }

        for (i = 0; i < gsttInfo.index_count; i++) {
            fseek(gpHelpFile, gsttInfo.contents_offset + (long)i * sizeof(struct HelpContent), SEEK_SET);
            fread(&content, sizeof(struct HelpContent), 1, gpHelpFile);

            gppcIndexText[i] = (char *)malloc(content.title_len + 1);
            if (gppcIndexText[i] == NULL) {
                UC_DialogWarning(NULL, "打开文件警告", "  帮助索引字串空间不足!");
                HelpCloseFile();
                return;
            }
            gppcIndexText[i][content.title_len] = '\0';
            fseek(gpHelpFile, content.title_offset, SEEK_SET);
            fread(gppcIndexText[i], content.title_len, 1, gpHelpFile);

            for (j = 0; j < content.title_len; j++) {
                gppcIndexText[i][j] = toupper(gppcIndexText[i][j]);
            }

            if (strcmp(gppcIndexText[i], gpcFindString) == 0) {
                MyFREE(gppcIndexText[i]);
                break;
            }
            MyFREE(gppcIndexText[i]);
        }

        MyFREE(gppcIndexText);
        gppcIndexText = NULL;
        gpcFindString = NULL;

        if (i == gsttInfo.index_count) {
            HelpReadPage(gsttInfo.contents_offset);
        } else {
            HelpReadPage(gsttInfo.contents_offset + (long)i * sizeof(struct HelpContent));
        }
    }

    HelpRedraw(gpwndHelp);
}

/* -----------------------------------------------------------------
   _HelpCloseFile
-------------------------------------------------------------------*/
void HelpCloseFile(void)
{
    int i;

    gpcHelpName[24] = '\0';
    for (i = 0; i < 80; i++) {
        gacFileName[i] = '\0';
    }
    gpcFindString = NULL;

    if (gpHelpFile != NULL) {
        fclose(gpHelpFile);
    }
    gpHelpFile = NULL;
    giPageLine = 0;
    giPageWide = 0;
    giPageFirstLine = 1;
    giPageFirstChar = 1;
    gnRectNumber = 0;

    if (gpsttHelpRect != NULL) {
        MyFREE(gpsttHelpRect);
    }
    gpsttHelpRect = NULL;
    giOldPointer = 0;

    memset(&gsttInfo, 0, sizeof(struct HelpInfo));
    memset(&gsttContent, 0, sizeof(struct HelpContent));
    gpcTextString = NULL;
    gpwndIndex = NULL;
    giIndex = 0;

    if (gppcIndexText != NULL) {
        MyFREE(gppcIndexText);
    }
    gppcIndexText = NULL;

    for (i = 0; i < 10; i++) {
        gasttOldContent[i] = gsttContent;
    }
    giTabSele = 0;
    for (i = 0; i < 220; i++) {
        gaTextBuf[i] = '\0';
    }
}

/* -----------------------------------------------------------------
   _HelpReadPage
-------------------------------------------------------------------*/
void HelpReadPage(long file_pos)
{
    int i, j;
    char ch;
    int lines, chars;
    WORD counts[2];
    struct HelpContent tmp_content;
    char tmp_buf[14];

    ch = ' ';
    giPageFirstLine = 1;
    giPageFirstChar = 1;
    giTabSele = 0;

    if (file_pos == 0L) {
        if (giOldPointer != 0) {
            gsttContent = gasttOldContent[giOldPointer - 1];
            giOldPointer--;
        }
    } else {
        if (gsttContent.title_offset != 0L) {
            giOldPointer++;
            if (giOldPointer > 10) {
                giOldPointer--;
                for (i = 0; i < 9; i++) {
                    gasttOldContent[i] = gasttOldContent[i + 1];
                }
            }
            gasttOldContent[giOldPointer - 1] = gsttContent;
        }
        fseek(gpHelpFile, file_pos, SEEK_SET);
        fread(&gsttContent, sizeof(struct HelpContent), 1, gpHelpFile);
    }

    fseek(gpHelpFile, gsttContent.link_offset, SEEK_SET);
    fread(counts, 4, 1, gpHelpFile);
    gnRectNumber = counts[0] + counts[1];

    for (i = 0; i < gnRectNumber; i++) {
        gpsttHelpRect[i].id = i + 1;
        fread(&gpsttHelpRect[i].type, 8, 1, gpHelpFile);
    }

    for (i = 0; i < gnRectNumber; i++) {
        fseek(gpHelpFile, gpsttHelpRect[i].target_offset, SEEK_SET);
        if (gpsttHelpRect[i].type == 1) {
            fread(&tmp_content, sizeof(struct HelpContent), 1, gpHelpFile);
            gpsttHelpRect[i].len = tmp_content.title_len;
        } else {
            fread(tmp_buf, 14, 1, gpHelpFile);
            gpsttHelpRect[i].len = *(WORD *)tmp_buf;
        }
    }

    fseek(gpHelpFile, gsttContent.text_offset, SEEK_SET);
    giPageWide = 0;
    lines = 0;
    chars = 0;
    j = 0;

    for (i = 0; (long)i < gsttContent.text_len; i++) {
        if (gnRectNumber != 0 && j < gnRectNumber && gpsttHelpRect[j].text_pos <= i) {
            gpsttHelpRect[j].line_start = lines + 1;
            gpsttHelpRect[j].line_end = lines + 1;
            gpsttHelpRect[j].char_start = chars + 1;
            gpsttHelpRect[j].char_end = chars + gpsttHelpRect[j].len;
            j++;
        }

        fread(&ch, 1, 1, gpHelpFile);
        if (ch == '\n' || ch == '\0' || ch == '\r' || (unsigned char)ch == 0x8d) {
            lines++;
            if (giPageWide < chars) {
                giPageWide = chars;
            }
            chars = 0;
        } else {
            chars++;
        }
    }

    giPageLine = lines;
    HelpResize(gpwndHelp);
}

/* -----------------------------------------------------------------
   _TranTextXToGraph
-------------------------------------------------------------------*/
int TranTextXToGraph(int x)
{
    int ret;
    ret = (int)((long)(x - giPageFirstChar + 1) * gpwndHelp->syschar_size / 2) + 5;
    if (ret < 5) ret = 5;
    return ret;
}

/* -----------------------------------------------------------------
   _TranTextYToGraph
-------------------------------------------------------------------*/
int TranTextYToGraph(int y)
{
    int ret;
    ret = (y - giPageFirstLine + 1) * (gpwndHelp->syschar_size + UC_RetReal(2)) + UC_RetReal(25) + 5;
    if (UC_RetReal(25) + 5 > ret) {
        ret = UC_RetReal(25) + 5;
    }
    return ret;
}

/* -----------------------------------------------------------------
   _HelpShowPage
-------------------------------------------------------------------*/
void HelpShowPage(void)
{
    int msg;
    RECT rect;
    int line_y, char_x;
    int text_idx;
    int link_idx;
    int chunk_len;
    int seg_len;
    char line_end_ch;
    int k;
    int sub_len;
    int pos;
    int diff;

    msg = UC_GetMessage();
    UC_SetMessage(msg);
    UC_GetClientRect(gpwndHelp, &rect);

    fseek(gpHelpFile, gsttContent.text_offset, SEEK_SET);
    UC_MouseHide();
    setfillstyle(SOLID_FILL, 15);
    setcolorbm(15);

    gpcTextString = (char *)malloc(gsttContent.text_len + 1);
    if (gpcTextString == NULL) {
        UC_DialogWarning(NULL, "帮助警告", "  这段帮助内容太长，内存空间不足!\n  无法显示出来!");
        return;
    }

    gpcTextString[gsttContent.text_len] = '\0';
    fread(gpcTextString, 1, gsttContent.text_len, gpHelpFile);

    for (text_idx = 0; (long)text_idx < gsttContent.text_len; text_idx++) {
        if (gpcTextString[text_idx] == '\n' || gpcTextString[text_idx] == '\0' ||
            gpcTextString[text_idx] == '\r' || (unsigned char)gpcTextString[text_idx] == 0x8d) {
            gpcTextString[text_idx] = '\0';
        }
    }

    line_y = 0;
    char_x = 0;
    settextstyle(0, 0, gpwndHelp->syschar_size);
    text_idx = 0;
    link_idx = 0;

    while ((long)text_idx < gsttContent.text_len) {
        if (link_idx < gnRectNumber && gpsttHelpRect[link_idx].text_pos == text_idx) {
            goto draw_link;
        }

        if (link_idx < gnRectNumber) {
            seg_len = gpsttHelpRect[link_idx].text_pos - text_idx;
        } else {
            seg_len = (int)gsttContent.text_len - text_idx;
        }

        setcolor(0);
        setcolorbm(15);
        chunk_len = 0;
        line_end_ch = ' ';

        while (chunk_len < seg_len) {
            sub_len = strlen(gpcTextString + text_idx + chunk_len);
            if (chunk_len + sub_len >= seg_len) {
                sub_len = seg_len - chunk_len;
                line_end_ch = '\0';
            }

            for (k = 0; k <= giWinWide; k++) gaTextBuf[k] = ' ';
            for (; k < 220; k++) gaTextBuf[k] = '\0';

            if (line_y >= giPageFirstLine - 1 && line_y + 1 - giPageFirstLine <= giWinLine) {
                if (char_x < giPageFirstChar - 1) {
                    if (giPageFirstChar - 1 - char_x < sub_len) {
                        pos = giPageFirstChar - 1 - char_x;
                        if (UC_CheckEditPoint(gpcTextString + text_idx + chunk_len, pos) == 2) {
                            pos++;
                            memmove(gaTextBuf + 1, gpcTextString + text_idx + chunk_len + pos, sub_len - pos);
                            gaTextBuf[sub_len - pos + 1] = '\0';
                        } else {
                            memmove(gaTextBuf, gpcTextString + text_idx + chunk_len + pos, sub_len - pos);
                            gaTextBuf[sub_len - pos] = '\0';
                        }
                    }
                } else {
                    memmove(gaTextBuf, gpcTextString + text_idx + chunk_len, sub_len);
                    gaTextBuf[sub_len] = '\0';
                }

                if (msg != 0x1389) {
                    UC_WinOutTextXY(gpwndHelp,
                                    TranTextXToGraph(char_x),
                                    TranTextYToGraph(line_y),
                                    16, gaTextBuf);

                    if (line_end_ch == ' ' && (int)strlen(gaTextBuf) < giWinWide + 1) {
                        UC_WindowBar(gpwndHelp,
                                     TranTextXToGraph(char_x) + (strlen(gaTextBuf) * gpwndHelp->syschar_size) / 2,
                                     TranTextYToGraph(line_y),
                                     ((giWinWide + 1 - strlen(gaTextBuf)) * gpwndHelp->syschar_size) / 2,
                                     gpwndHelp->syschar_size);
                    }
                }
            }

            chunk_len += sub_len;
            char_x += sub_len;
            if (line_end_ch == ' ') {
                chunk_len++;
                char_x = 0;
                line_y++;
            }
        }

        text_idx += seg_len;
        continue;

draw_link:
        setcolor(2);
        setcolorbm(15);
        if (giTabSele - 1 == link_idx) {
            setcolor(15);
            setcolorbm(0);
        }

        if (line_y + 1 - giPageFirstLine >= 0 && line_y + 1 - giPageFirstLine <= giWinLine) {
            if (char_x < giPageFirstChar - 1) {
                diff = giPageFirstChar - 1 - gpsttHelpRect[link_idx].char_start + 1;
                if (diff < gpsttHelpRect[link_idx].len) {
                    if (UC_CheckEditPoint(gpcTextString + text_idx, diff) == 2) {
                        diff++;
                        memmove(gaTextBuf + 1, gpcTextString + text_idx + diff, gpsttHelpRect[link_idx].len - diff);
                        gaTextBuf[gpsttHelpRect[link_idx].len - diff + 1] = '\0';
                    } else {
                        memmove(gaTextBuf, gpcTextString + text_idx + diff, gpsttHelpRect[link_idx].len - diff);
                        gaTextBuf[gpsttHelpRect[link_idx].len - diff] = '\0';
                    }
                }
            } else {
                memmove(gaTextBuf, gpcTextString + text_idx, gpsttHelpRect[link_idx].len);
                gaTextBuf[gpsttHelpRect[link_idx].len] = '\0';
            }

            if (gpwndHelp->left + gpwndHelp->vx + rect.left <= TranTextXToGraph(gpsttHelpRect[link_idx].char_start) &&
                gpwndHelp->left + gpwndHelp->vx + rect.right >= TranTextXToGraph(gpsttHelpRect[link_idx].char_end) &&
                gpwndHelp->top + gpwndHelp->vy <= TranTextYToGraph(gpsttHelpRect[link_idx].line_start) + gpwndHelp->syschar_size &&
                gpwndHelp->top + gpwndHelp->vy + rect.bottom - rect.top >= TranTextYToGraph(gpsttHelpRect[link_idx].line_start) + gpwndHelp->syschar_size) {

                UC_WinOutTextXY(gpwndHelp,
                                TranTextXToGraph(char_x),
                                TranTextYToGraph(line_y),
                                16, gaTextBuf);

                if (gpsttHelpRect[link_idx].type != 1) {
                    setlinestyle(4, 0xcccc, 1);
                    line(TranTextXToGraph(gpsttHelpRect[link_idx].char_start),
                         TranTextYToGraph(gpsttHelpRect[link_idx].line_start) + gpwndHelp->syschar_size,
                         TranTextXToGraph(gpsttHelpRect[link_idx].char_end),
                         TranTextYToGraph(gpsttHelpRect[link_idx].line_start) + gpwndHelp->syschar_size);
                    setcolor(15);
                    if (giTabSele - 1 == link_idx) setcolor(0);
                    setlinestyle(4, 0x3333, 1);
                }

                line(TranTextXToGraph(gpsttHelpRect[link_idx].char_start),
                     TranTextYToGraph(gpsttHelpRect[link_idx].line_start) + gpwndHelp->syschar_size,
                     TranTextXToGraph(gpsttHelpRect[link_idx].char_end),
                     TranTextYToGraph(gpsttHelpRect[link_idx].line_start) + gpwndHelp->syschar_size);
            }

            setlinestyle(0, 1, 1);
        }

        setcolorbm(15);
        char_x += gpsttHelpRect[link_idx].len;
        text_idx += gpsttHelpRect[link_idx].len;
        link_idx++;
    }

    MyFREE(gpcTextString);
    UC_WindowBar(gpwndHelp,
                 0,
                 (line_y + 1 - giPageFirstLine) * (gpwndHelp->syschar_size + UC_RetReal(2)) + UC_RetReal(25) + 5,
                 rect.right - rect.left + 1,
                 rect.bottom - rect.top);
    UC_MouseShow();
}

/* -----------------------------------------------------------------
   _HelpShowWin
-------------------------------------------------------------------*/
void HelpShowWin(long target_offset, int x, int y)
{
    char *text;
    int y2, x2, y1, x1;
    int cur_len;
    int lines;
    int key;
    void *img_buf;
    long text_offset;
    long text_len;
    char head[14];
    int i;
    int max_len;
    int img_sz;
    int prev_space;
    RECT draw_rc;

    fseek(gpHelpFile, target_offset, SEEK_SET);
    fread(head, 14, 1, gpHelpFile);
    text_len = *(long *)&head[6];
    text_offset = *(long *)&head[10];

    text = (char *)malloc(text_len + 1);
    if (text == NULL) {
        UC_DialogWarning(NULL, "帮助警告", "  存关键词空间不足!");
        return;
    }

    text[text_len] = '\0';
    fseek(gpHelpFile, text_offset, SEEK_SET);
    fread(text, text_len, 1, gpHelpFile);

    cur_len = 1;
    lines = 0;
    max_len = 0;

    for (i = 0; (long)i < text_len; i++) {
        if (text[i] == '\n' || text[i] == '\0' || text[i] == '\r' || (unsigned char)text[i] == 0x8d) {
            text[i] = '\n';
            lines++;
            if (cur_len > max_len) max_len = cur_len;
            cur_len = 0;
        } else {
            cur_len++;
        }
    }

    max_len--;
    if (max_len < 0) max_len = 0;
    i--;
    if (i < 0) i = 0;
    if (text[i] != '\n' && text[i] != '\0' && text[i] != '\r' && (unsigned char)text[i] != 0x8d) {
        lines++;
    }

    x1 = x - (max_len * gpwndHelp->syschar_size) / 4;
    x2 = x + (max_len * gpwndHelp->syschar_size) / 4;
    if (x1 < 15) {
        x2 += 15 - x1;
        x1 = 15;
        if (x2 > getmaxx() - 25) x2 = getmaxx() - 25;
    }
    if (x2 > getmaxx() - 25) {
        x1 -= x2 - (getmaxx() - 25);
        x2 = getmaxx() - 25;
        if (x1 < 15) x1 = 15;
    }

    if (y > getmaxy() / 2) {
        y2 = y - 21;
        y1 = y2 - lines * (gpwndHelp->syschar_size + UC_RetReal(2));
        if (y1 < 15) {
            y2 += 15 - y1;
            y1 = 15;
            if (y2 > getmaxy() - 25) y2 = getmaxy() - 25;
        }
    } else {
        y1 = y + gpwndHelp->syschar_size + UC_RetReal(2) + 10;
        y2 = y1 + lines * (gpwndHelp->syschar_size + UC_RetReal(2));
        if (y2 > getmaxy() - 25) {
            y1 -= y2 - (getmaxy() - 25);
            y2 = getmaxy() - 25;
            if (y1 < 15) y1 = 15;
        }
    }

    UC_MouseHide();
    img_sz = imagesize(x1 - 10, y1 - 10, x2 + 20, y2 + 20);
    if (img_sz == -1) img_sz = 256;
    img_buf = malloc(img_sz);
    if (img_buf == NULL) {
        img_buf = PUBLIC_BUF;
    }

    getimage(x1 - 10, y1 - 10, x2 + 20, y2 + 20, img_buf);

    setcolor(0);
    rectangle(x1 - 9, y1 - 9, x2 + 9, y2 + 9);
    rectangle(x1 - 10, y1 - 10, x2 + 10, y2 + 10);

    for (i = y2 + 20; i > y2 + 10; i--) {
        setlinestyle(4, (i % 2 == 1) ? 0xaaaa : 0x5555, 1);
        line(x2 + 20, i, x1, i);
    }
    for (i = y2 + 10; i > y1; i--) {
        setlinestyle(4, (i % 2 == 1) ? 0xaaaa : 0x5555, 1);
        line(x2 + 20, i, x2 + 10, i);
    }

    setlinestyle(0, 1, 1);
    setfillstyle(SOLID_FILL, 7);
    bar(x1 - 8, y1 - 8, x2 + 8, y2 + 8);

    setcolor(0);
    prev_space = UC_GetLineSpace();
    UC_SetLineSpace(2);
    settextstyle(0, 0, gpwndHelp->syschar_size);

    draw_rc.left = x1;
    draw_rc.top = y1;
    draw_rc.right = x2;
    draw_rc.bottom = y2;
    UC_DrawText(gpwndHelp, &draw_rc, 0, text);
    UC_SetLineSpace(prev_space);

    UC_MouseShow();

    while (!kbhit() && !UC_MouseCheck()) {
        /* idle wait */
    }

    if (UC_MouseCheck()) {
        UC_WaitFreeMouse(NULL, 0, 0, 0, 0, NULL);
    } else {
        key = getch();
        if (key == 0) getch();
    }

    UC_MouseHide();
    putimage(x1 - 10, y1 - 10, img_buf, COPY_PUT);
    killimage(img_buf);
    MyFREE(text);
    UC_MouseShow();
}
