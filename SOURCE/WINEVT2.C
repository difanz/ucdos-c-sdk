// WINEVT2.C - 窗口事件处理 (二)

#ifndef _WIN_DATA2_H
#include <alloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mem.h>
#include <dos.h>
#include <dir.h>
#include <math.h>
#include "agidrv.h"
#include <conio.h>
#include "struct.h"
#include "agi_bmp.h"
#include "agi_win.h"
#include "WINDATA1.H"
#include "WINDATA2.H"
#endif

/* 函数声明 */
int far UC_CheckEditPoint(char far *str, int pos);
int far UC_CheckInputHZ(INPUTLINE *inp);
void far UC_DoInputLine(WINDOWS *wnd, int key);
void far UC_CalculateMenuXY(WINDOWS *wnd, int *x, int *y);
void far UC_ClosePopMenu(WINDOWS *wnd);
void far UC_CalculateMenuMaxSize(char **selects, int *pcount, int *pmaxw);
void far UC_OpenPopMenu(WINDOWS *wnd);
void far UC_DisplayPopMenuLine(WINDOWS *wnd, int line);
MENUS * far UC_GetPopMenuSelect(WINDOWS *wnd);
int far UC_CheckMenuSelect(WINDOWS *wnd);
MENUS * far UC_CheckMenu(WINDOWS *wnd);
MENUS * far UC_GetMenuSelect(WINDOWS *wnd);
void far UC_DoMenuSelect(WINDOWS *wnd);
int far UC_CheckMenuSelectHotKey(WINDOWS *wnd, int key);
MENUS * far UC_CheckMenuHotKey(WINDOWS *wnd, int key);
int far UC_CheckInputBoxPop(WINDOWS *wnd, INPUTLINE *inp);
void far UC_MaxWindow(WINDOWS *wnd);
void far UC_MinWindow(WINDOWS *wnd);
void far UC_ArrangeIcons(void);
void far UC_GetWindowIconXY(WINDOWS *wnd);
void far Drawjj(WINDOWS *wnd);
void far UC_TimeJJ(void);
void far UC_CloseJJ(void);
void far UC_SecretWindow(void);
void far UC_MainLoop(void);
int far UC_InMouseEvenRect(USER_MOUSE *ms, int x, int y);
int far UC_AllMainOnBack(void);

/* 外部函数声明 */
extern char PUBLIC_BUF[];
extern INPUTLINE * far UC_GetInputBox(WINDOWS *wnd, int num);
extern void far UC_InputCursorMove(WINDOWS far *wnd);
extern void far UC_CheckPopBox(WINDOWS far *wnd, INPUTLINE far *inp);
extern void far UC_DisplayMenu(WINDOWS *wnd);
extern int far UC_CheckKeyboard(void);
extern void far UC_WindowsCentral(void);
extern void far UC_WindowHide(void);
extern void far UC_InitRedraw(WINDOWS *wnd);
extern void far UC_RedrawBack(int x1, int y1, int x2, int y2);
extern void far UC_DisplayInputText(WINDOWS *wnd, INPUTLINE *inp, int focus);
extern int far UC_WindowPrintf(WINDOWS *wnd, int x, int y, char mode, char *fmt, ...);
extern void far UC_TextOut(WINDOWS *wnd, int x, int y, char mode, char *text);
extern void far UC_FlashCaret(void);
extern void far nomem(char far *text);
extern void far MyFREE(void far *p);
extern void far Setviewport(int left, int top, int right, int bottom);
extern void far setascstyle(int style);
extern void far setcolorbm(int color);
extern void far ICO_APPLICATION(void);

int far UC_CheckEditPoint(char far *str, int pos)
{
    int i;
    int flag;

    if ((int)strlen(str) <= pos)
        return 0;

    if ((unsigned char)str[pos] < 0xa1)
        return 3;

    flag = 0;
    for (i = 0; i <= pos; i++) {
        if ((unsigned char)str[i] > 0xa0)
            flag = 1 - flag;
        else
            flag = 0;
    }

    if (flag) {
        if ((unsigned char)str[pos + 1] > 0xa0)
            return 1;
        return 3;
    }
    return 2;
}

int far UC_CheckInputHZ(INPUTLINE *inp)
{
    int pos = inp->curepos + inp->cursx;
    return UC_CheckEditPoint(inp->text, pos);
}

void far UC_DoInputLine(WINDOWS *wnd, int key)
{
    INPUTLINE *inp;
    int len;
    char ch;
    int pos;

    inp = UC_GetInputBox(wnd, wnd->cure_num);

    if (key == 0x1c0d) {
        if (inp->length != 0 || BOXPOP) {
            if (inp->fun_sele)
                inp->fun_sele();
            return;
        }
        key = 0x5100;
    }

    if (key == 0x11b)
        return;

    len = (int)strlen(inp->text);
    ch = (char)(key & 0xff);

    if (ch != 0) {
        /* 可见字符与退格 */
        if (inp->length == 0)
            return;

        if (ch == 0x08) {
            /* 退格处理 */
            if (inp->cursx == 0 && inp->curepos == 0)
                goto DO_REDRAW;

            pos = inp->curepos + inp->cursx;
            if (UC_CheckEditPoint(inp->text, pos - 1) == 2) {
                movmem(inp->text + pos, inp->text + pos - 1, len - pos + 1);
                if (inp->curepos != 0)
                    inp->curepos--;
                else
                    inp->cursx--;
                pos = inp->curepos + inp->cursx;
            }
            movmem(inp->text + pos, inp->text + pos - 1, len - pos + 1);
            if (inp->curepos != 0) {
                inp->curepos--;
                goto DO_REDRAW;
            }
            goto DO_REDRAW_DEC;
        }

        /* 插入字符 */
        if (inp_first) {
            inp->text[0] = '\0';
            inp->curepos = 0;
            inp->cursx = 0;
            len = 0;
        }

        if (inp->length <= (WORD)len)
            goto DO_REDRAW;

        pos = inp->curepos + inp->cursx;
        movmem(inp->text + pos, inp->text + pos + 1, len - pos + 1);
        inp->text[pos] = ch;
        inp->cursx++;
        if (inp->cursx > inp->width - 1) {
            inp->curepos++;
            inp->cursx--;
        }
        goto DO_REDRAW;
    }

    /* 控制键处理 */
    switch (key) {
    case 0x4b00: /* 左移 */
LEFT_ARROW:
        if (inp->cursx != 0)
            inp->cursx--;
        else if (inp->curepos != 0)
            inp->curepos--;
        if (UC_CheckInputHZ(inp) == 2)
            goto LEFT_ARROW;
        goto DO_REDRAW;

    case 0x4d00: /* 右移 */
RIGHT_ARROW:
        if (inp->cursx + inp->curepos < (WORD)len)
            inp->cursx++;
        if (inp->cursx > inp->width - 1) {
            inp->cursx--;
            inp->curepos++;
        }
        if (UC_CheckInputHZ(inp) == 2)
            goto RIGHT_ARROW;
        goto DO_REDRAW;

    case 0x5300: /* 删除 */
        if (inp->length == 0)
            return;
        if (inp_first) {
            inp->text[0] = '\0';
            inp->curepos = 0;
            inp->cursx = 0;
            len = 0;
        }
        if (inp->cursx + inp->curepos >= (WORD)len)
            goto DO_REDRAW;

        pos = inp->curepos + inp->cursx;
        if (UC_CheckInputHZ(inp) == 1) {
            movmem(inp->text + pos + 2, inp->text + pos, len - pos - 1);
        } else {
            movmem(inp->text + pos + 1, inp->text + pos, len - pos);
        }
        goto DO_REDRAW;

    case 0x4700: 
        inp->cursx = 0;
        inp->curepos = 0;
        goto DO_REDRAW;

    case 0x4f00: 
        if (inp->cursx + inp->curepos >= (WORD)len)
            goto DO_REDRAW;
        if (len > inp->width - 2)
            inp->cursx = inp->width - 2;
        else
            inp->cursx = len;
        inp->curepos = len - inp->cursx;
        goto DO_REDRAW;

    case 0x1c0d: /* Enter */
    case 0x5000: /* 下移 */
    case 0x5100: /* 下页 */
        if (inp->length != 0)
            return;
        if (BOXPOP != 0)
            return;
        UC_CheckPopBox(wnd, inp);
        return;

    default:
        return;
    }

DO_REDRAW_DEC:
    inp->cursx--;
DO_REDRAW:
    UC_DisplayInputText(wnd, inp, 1);
    UC_InputCursorMove(wnd);
    inp_first = 0;
}

void far UC_CalculateMenuXY(WINDOWS *wnd, int *x, int *y)
{
    MENUS *p;

    *x = wnd->left + wnd->vx;
    *y = wnd->top + wnd->vy - 1;

    settextstyle(0, 0, wnd->syschar_size);
    for (p = wnd->menu; p != wnd->curemenu; p++) {
        *x += textwidth(p->menu_name + 1);
    }
}

void far UC_ClosePopMenu(WINDOWS *wnd)
{
    int x, y;

    if (!wnd->curemenu->menu_selects || !wnd->curemenu->menu_selects[0] ||
        wnd->curemenu->menu_selects[0][0] == '\0')
        return;

    settextstyle(0, 0, wnd->syschar_size);
    UC_CalculateMenuXY(wnd, &x, &y);
    UC_MouseHide();
    putimage(x, y, MENU_BUF, 0);
    killimage(MENU_BUF);
    UC_MouseShow();
}

void far UC_CalculateMenuMaxSize(char **selects, int *pcount, int *pmaxw)
{
    int w;

    *pcount = 0;
    *pmaxw = 0;

    settextstyle(0, 0, wintable_tail->syschar_size);
    while (selects[*pcount] && selects[*pcount][0] != '\0') {
        w = textwidth(selects[*pcount] + 1);
        if (w > *pmaxw)
            *pmaxw = w;
        (*pcount)++;
    }
}

void far UC_OpenPopMenu(WINDOWS *wnd)
{
    int x1, y1, x2, y2;
    int count, maxw;
    int i;
    unsigned int size;
    void far *buf;

    if (!wnd->curemenu->menu_selects || !wnd->curemenu->menu_selects[0] ||
        wnd->curemenu->menu_selects[0][0] == '\0')
        return;

    settextstyle(0, 0, wnd->syschar_size);
    UC_CalculateMenuXY(wnd, &x1, &y1);
    UC_CalculateMenuMaxSize(wnd->curemenu->menu_selects, &count, &maxw);

    if (wnd->curemenu->cure_sele + 1 > (WORD)count)
        wnd->curemenu->cure_sele = 0;

    x2 = x1 + maxw + 1;
    y2 = y1 + count * (wnd->syschar_size + 2) + 4 - 1;

    size = imagesize(x1, y1, x2, y2);
    if (size == (unsigned int)-1)
        size = 256;

    buf = (void far *)malloc(size);
    if (!buf)
        buf = (void far *)PUBLIC_BUF;

    MENU_BUF = buf;

    UC_MouseHide();
    getimage(x1, y1, x2, y2, buf);
    setfillstyle(1, wnd->menubkc);
    bar(x1, y1, x2, y2);
    setcolor(wnd->linecolor);
    rectangle(x1, y1, x2, y2);

    for (i = 0; wnd->curemenu->menu_selects[i] && wnd->curemenu->menu_selects[i][0] != '\0'; i++) {
        if (wnd->curemenu->menu_selects[i][0] == '-') {
            int line_y = y1 + i * (wnd->syschar_size + 2) + wnd->syschar_size / 2 + 3;
            setcolor(wnd->linecolor);
            line(x1, line_y, x2, line_y);
        } else {
            UC_DisplayPopMenuLine(wnd, i);
        }
    }
    UC_MouseShow();
}

void far UC_DisplayPopMenuLine(WINDOWS *wnd, int line)
{
    int x, y;
    int count, maxw;
    int x2;
    int line_y;

    settextstyle(0, 0, wnd->syschar_size);
    UC_CalculateMenuXY(wnd, &x, &y);
    UC_CalculateMenuMaxSize(wnd->curemenu->menu_selects, &count, &maxw);

    x2 = x + maxw - 1;
    if (wnd->curemenu->cure_sele == (WORD)line) {
        setcolor(wnd->title_fgc);
        setfillstyle(1, wnd->title_bkc);
    } else {
        setcolor(wnd->linecolor);
        setfillstyle(1, wnd->menubkc);
    }

    line_y = y + line * (wnd->syschar_size + 2) + 3;
    UC_MouseHide();
    bar(x, line_y - 1, x2, line_y + wnd->syschar_size);
    outtextxy(x, line_y, wnd->curemenu->menu_selects[line] + 1);
    UC_MouseShow();
}

MENUS * far UC_GetPopMenuSelect(WINDOWS *wnd)
{
    int di = -1;
    MENUS *cure = wnd->curemenu;
    MENUS *new_menu;
    int key;
    int rmb, lmb;

    for (;;) {
        di = -1;
        if (UC_MouseCheck()) {
            new_menu = UC_CheckMenu(wnd);
            if (new_menu && new_menu != wnd->curemenu) {
                UC_ClosePopMenu(wnd);
                wnd->curemenu = new_menu;
                UC_DisplayMenu(wnd);
                UC_OpenPopMenu(wnd);
                cure = new_menu;
            }
            di = UC_CheckMenuSelect(wnd);
            if (di != -1 && (WORD)di != cure->cure_sele) {
                int old_sele = cure->cure_sele;
                cure->cure_sele = di;
                UC_DisplayPopMenuLine(wnd, old_sele);
                UC_DisplayPopMenuLine(wnd, cure->cure_sele);
            }
        }

        /* 鼠标按键处理 */
        rmb = 3 - HAND_LR;
        if (MOUSE_AGI.s & rmb)
            return NULL;

        lmb = HAND_LR;
        if (MOUSE_AGI.s & lmb) {
            /* 鼠标左键按下 */
            while (MOUSE_AGI.s & lmb) {
                new_menu = UC_CheckMenu(wnd);
                if (new_menu && new_menu != cure) {
                    UC_ClosePopMenu(wnd);
                    wnd->curemenu = new_menu;
                    UC_DisplayMenu(wnd);
                    UC_OpenPopMenu(wnd);
                    cure = new_menu;
                }
                di = UC_CheckMenuSelect(wnd);
                if (cure->cure_sele != (WORD)di && di != -1) {
                    int old_sele = cure->cure_sele;
                    cure->cure_sele = di;
                    UC_DisplayPopMenuLine(wnd, old_sele);
                    UC_DisplayPopMenuLine(wnd, cure->cure_sele);
                }
                UC_MouseCheck();
            }
            if (di != -1)
                return cure;
            if (new_menu)
                continue;
            return NULL;
        }

        key = UC_CheckKeyboard();
        if (key == 0)
            continue;

        /* 顶层菜单热键 */
        new_menu = UC_CheckMenuHotKey(wnd, key);
        if (new_menu) {
            UC_ClosePopMenu(wnd);
            wnd->curemenu = new_menu;
            UC_DisplayMenu(wnd);
            UC_OpenPopMenu(wnd);
            cure = new_menu;
            continue;
        }

        /* 弹出菜单热键 */
        di = UC_CheckMenuSelectHotKey(wnd, key);
        if (di != -1) {
            cure->cure_sele = di;
            return cure;
        }

        switch (key) {
        case 0x011b: /* Esc */
        case 0x3800: /* Alt */
        case 0x4400: /* F10 */
            return NULL;

        case 0x1c0d: /* Enter */
            return cure;

        case 0x4b00: /* 左移 */
            UC_ClosePopMenu(wnd);
            if (cure == wnd->menu) {
                while ((cure + 1)->menu_name)
                    cure++;
            } else {
                cure--;
            }
            wnd->curemenu = cure;
            UC_DisplayMenu(wnd);
            UC_OpenPopMenu(wnd);
            break;

        case 0x4d00: /* 右移 */
            UC_ClosePopMenu(wnd);
            if (!(cure + 1)->menu_name)
                cure = wnd->menu;
            else
                cure++;
            wnd->curemenu = cure;
            UC_DisplayMenu(wnd);
            UC_OpenPopMenu(wnd);
            break;

        case 0x5000: /* 下移 */
            di = cure->cure_sele;
            do {
                if (cure->menu_selects[cure->cure_sele + 1] &&
                    cure->menu_selects[cure->cure_sele + 1][0] != '\0')
                    cure->cure_sele++;
                else
                    cure->cure_sele = 0;
            } while (cure->menu_selects[cure->cure_sele][0] == '-');
            UC_DisplayPopMenuLine(wnd, di);
            UC_DisplayPopMenuLine(wnd, cure->cure_sele);
            break;

        case 0x4800: /* 上移 */
            di = cure->cure_sele;
            do {
                if (cure->cure_sele > 0) {
                    cure->cure_sele--;
                } else {
                    cure->cure_sele = 0;
                    while (cure->menu_selects[cure->cure_sele + 1] &&
                           cure->menu_selects[cure->cure_sele + 1][0] != '\0')
                        cure->cure_sele++;
                }
            } while (cure->menu_selects[cure->cure_sele][0] == '-');
            UC_DisplayPopMenuLine(wnd, di);
            UC_DisplayPopMenuLine(wnd, cure->cure_sele);
            break;
        }
    }
}

int far UC_CheckMenuSelect(WINDOWS *wnd)
{
    int mx = MOUSE_AGI.x;
    int my = MOUSE_AGI.y;
    int x1, y1;
    int count, maxw;
    int x2;
    int i;
    int item_y2;

    UC_CalculateMenuXY(wnd, &x1, &y1);
    UC_CalculateMenuMaxSize(wnd->curemenu->menu_selects, &count, &maxw);

    x1++;
    maxw++;
    x2 = x1 + maxw;

    for (i = 0; wnd->curemenu->menu_selects[i] && wnd->curemenu->menu_selects[i][0] != '\0'; i++) {
        item_y2 = y1 + wnd->syschar_size + 2;
        if (wnd->curemenu->menu_selects[i][0] != '-') {
            if (mx >= x1 && mx <= x2 && my >= y1 && my <= item_y2)
                return i;
        }
        y1 += wnd->syschar_size + 2;
    }
    return -1;
}

MENUS * far UC_CheckMenu(WINDOWS *wnd)
{
    int mx = MOUSE_AGI.x;
    int my = MOUSE_AGI.y;
    MENUS *save_cure = wnd->curemenu;
    MENUS *p;
    int x, y;
    int y2, x2;

    settextstyle(0, 0, wnd->syschar_size);

    for (p = wnd->menu; p->menu_name; p++) {
        wnd->curemenu = p;
        UC_CalculateMenuXY(wnd, &x, &y);
        y2 = y - 2;
        y = y - wnd->syschar_size - 4;
        x2 = x + textwidth(p->menu_name + 1) - 1;

        if (mx >= x && mx <= x2 && my >= y && my <= y2) {
            wnd->curemenu = save_cure;
            return p;
        }
    }
    wnd->curemenu = save_cure;
    return NULL;
}

MENUS * far UC_GetMenuSelect(WINDOWS *wnd)
{
    MENUS *cure = wnd->curemenu;
    MENUS *new_menu;
    int key;
    int rmb, lmb;

    for (;;) {
        rmb = 3 - HAND_LR;
        if (MOUSE_AGI.s & rmb)
            return NULL;

        lmb = HAND_LR;
        if (MOUSE_AGI.s & lmb) {
            new_menu = UC_CheckMenu(wnd);
            if (!new_menu)
                return NULL;
            wnd->curemenu = new_menu;
            UC_DisplayMenu(wnd);
            return new_menu;
        }

        UC_MouseCheck();
        key = UC_CheckKeyboard();
        if (key == 0)
            continue;

        new_menu = UC_CheckMenuHotKey(wnd, key);
        if (new_menu) {
            UC_DisplayMenu(wnd);
            return new_menu;
        }

        switch (key) {
        case 0x011b: /* Esc */
        case 0x3800: /* Alt */
        case 0x4400: /* F10 */
            return NULL;

        case 0x1c0d: /* Enter */
        case 0x5000: /* 下移 */
            return cure;

        case 0x4b00: /* 左移 */
            if (cure == wnd->menu) {
                while ((cure + 1)->menu_name)
                    cure++;
            } else {
                cure--;
            }
            wnd->curemenu = cure;
            UC_DisplayMenu(wnd);
            break;

        case 0x4d00: /* 右移 */
            if (!(cure + 1)->menu_name)
                cure = wnd->menu;
            else
                cure++;
            wnd->curemenu = cure;
            UC_DisplayMenu(wnd);
            break;
        }
    }
}

void far UC_DoMenuSelect(WINDOWS *wnd)
{
    MENUS *mnu;

    wnd->curemenu = UC_GetMenuSelect(wnd);
    if (!wnd->curemenu) {
        wnd->curemenu = NULL;
        UC_DisplayMenu(wnd);
        return;
    }

    UC_OpenPopMenu(wnd);
    mnu = UC_GetPopMenuSelect(wnd);
    UC_ClosePopMenu(wnd);
    wnd->curemenu = NULL;
    UC_DisplayMenu(wnd);

    if (mnu) {
        if (mnu->menu_function && mnu->menu_function[mnu->cure_sele])
            mnu->menu_function[mnu->cure_sele]();
    }
}

int far UC_CheckMenuSelectHotKey(WINDOWS *wnd, int key)
{
    char ch = (char)(key & 0xff);
    int i;

    if (ch >= 'a' && ch <= 'z')
        ch &= 0xdf;

    for (i = 0; wnd->curemenu->menu_selects[i]; i++) {
        if (wnd->curemenu->menu_selects[i][0] == '\0')
            break;
        if (wnd->curemenu->menu_selects[i][0] == '-')
            continue;
        if (wnd->curemenu->menu_selects[i][0] == ch)
            return i;
    }
    return -1;
}

MENUS * far UC_CheckMenuHotKey(WINDOWS *wnd, int key)
{
    unsigned char scan = (unsigned char)((key >> 8) & 0xff);
    MENUS *p;

    wnd->curemenu = NULL;
    UC_GetKeyboardState();
    if (!(KEYB_STATE & 8))
        return NULL;

    for (p = wnd->menu; p->menu_name; p++) {
        char ch = p->menu_name[0];
        if (ch >= 'a' && ch <= 'z')
            ch &= 0xdf;
        if (ch >= 'A' && ch <= 'Z') {
            if (key_scancodes[ch - 'A'] == scan) {
                wnd->curemenu = p;
                return p;
            }
        }
    }
    return NULL;
}

int far UC_CheckInputBoxPop(WINDOWS *wnd, INPUTLINE *inp)
{
    int mx = MOUSE_AGI.x;
    int my = MOUSE_AGI.y;
    int x1, y1, x2, y2;

    x1 = wnd->left + wnd->vx + inp->left;
    y1 = wnd->top + wnd->vy + inp->top;

    x2 = x1 + (inp->box->width * wnd->syschar_size) / 2 + wnd->syschar_size + 2;
    y2 = y1 + inp->box->height * (wnd->syschar_size + 2) + wnd->syschar_size + wnd->vx;

    if (mx >= x1 && mx <= x2 && my >= y1 && my <= y2)
        return 1;
    return 0;
}

void far UC_MaxWindow(WINDOWS *wnd)
{
    if ((wnd->mode & 6) != 0 || wnd->mode == 0)
        return;

    if (UC_GetCurrentWindow() != wnd)
        UC_WindowEnable(wnd);

    if (wnd->minmax == 1) {
        if (wnd->ENABLE) {
            if (wnd->ENABLE(wnd, 4) != 0)
                return;
        }
        wnd->minmax = wnd->oldminmax;
        if (wnd->oldminmax == 2) {
            UC_WindowResize(0, 0, getmaxx() - 1, getmaxy());
        } else {
            windows_hide = 1;
            UC_RedrawBack(wnd->left, wnd->top, wnd->left + wnd->width - 1, wnd->top + wnd->height - 1);
            windows_hide = 0;
            wnd->left = wnd->old_left;
            wnd->top = wnd->old_top;
            wnd->width = wnd->old_width;
            wnd->height = wnd->old_height;
            UC_InitRedraw(wnd);
        }
        if (wnd->ico_xor) {
            MyFREE(wnd->ico_xor);
            wnd->ico_xor = NULL;
        }
        if (wnd->ico_and) {
            MyFREE(wnd->ico_and);
            wnd->ico_and = NULL;
        }
        if (wnd->ico_handle) {
            UC_FreeXMS(wnd->ico_handle);
            wnd->ico_handle = 0;
        }
        if (wnd->inputhot)
            UC_SetTimer(UC_FlashCaret, CARET_SPEED);
        return;
    }

    if (wnd->minmax == 2) {
        if (wnd->ENABLE) {
            if (wnd->ENABLE(wnd, 4) != 0)
                return;
        }
        wnd->minmax = 0;
        wnd->vx += 4;
        wnd->vy += 4;
        wnd->vr += 4;
        wnd->vb += 4;
        UC_WindowResize(wnd->old_left, wnd->old_top,
                        wnd->old_left + wnd->old_width - 1,
                        wnd->old_top + wnd->old_height - 1);
        return;
    }

    if (wnd->ENABLE) {
        if (wnd->ENABLE(wnd, 3) != 0)
            return;
    }
    if (wnd->mode & 8) {
        UC_WindowEnable(wnd);
        return;
    }
    wnd->minmax = 2;
    wnd->old_left = wnd->left;
    wnd->old_top = wnd->top;
    wnd->old_width = wnd->width;
    wnd->old_height = wnd->height;
    wnd->vx -= 4;
    wnd->vy -= 4;
    wnd->vr -= 4;
    wnd->vb -= 4;
    UC_WindowResize(0, 0, getmaxx() - 1, getmaxy());
}

void far UC_MinWindow(WINDOWS *wnd)
{
    if (wnd->ico_handle != 0)
        goto DO_ENABLE;
    if (wnd->ico_xor != NULL && wnd->ico_and != NULL)
        goto DO_ENABLE;

    if ((wnd->mode & 6) != 0 || wnd->mode == 0)
        return;

    if (wnd->ENABLE) {
        if (wnd->ENABLE(wnd, 2) != 0)
            return;
    }

    if (wnd->hide == 0) {
        if (UC_GetCurrentWindow() != wnd)
            UC_WindowEnable(wnd);
        UC_WindowHide();
    }

    if (!UC_CreateIcon((char far *)wnd->ICON, &wnd->ico_xor, &wnd->ico_and,
                       (void far *)&wnd->ico_width, (void far *)&wnd->ico_depth, wnd->ico_count)) {
        nomem("创建最小化图标");
    }

    if (!wnd->DRAWICON) {
        wnd->ico_handle = UC_XMSLoadIcon(wnd->ico_xor, wnd->ico_and, wnd->ico_width, wnd->ico_depth);
        if (wnd->ico_handle != 0) {
            MyFREE(wnd->ico_xor);
            MyFREE(wnd->ico_and);
            wnd->ico_xor = NULL;
            wnd->ico_and = NULL;
        }
    } else {
        MyFREE(wnd->ico_xor);
        MyFREE(wnd->ico_and);
        wnd->ico_xor = NULL;
        wnd->ico_and = NULL;
        wnd->ico_handle = 0;
    }

    wnd->oldminmax = wnd->minmax;
    if (wnd->minmax == 0) {
        wnd->old_left = wnd->left;
        wnd->old_top = wnd->top;
        wnd->old_width = wnd->width;
        wnd->old_height = wnd->height;
    }

    UC_GetWindowIconXY(wnd);
    wnd->width = ICO_TITLEWIDTH + 1;
    settextstyle(0, 0, 16);
    wnd->height = wnd->ico_depth + UC_TextPlines(wnd->ico_text, ICO_TITLEWIDTH) + 4;
    wnd->minmax = 1;

DO_ENABLE:
    UC_WindowEnable(wnd);
}

void far UC_ArrangeIcons(void)
{
    WINDOWS *wnd;
    WINDOWS *head = wintable_head;

    if (!head)
        return;

    winicox = 0xbb8;
    winicoy = 0;

    UC_MouseHide();
    for (wnd = head; ; ) {
        UC_WindowEnable(wnd);
        wnd->ico_x = -1;
        wnd->ico_y = -1;
        if (wnd->minmax == 1) {
            UC_WindowHide();
            UC_GetWindowIconXY(wnd);
            UC_WindowEnable(wnd);
        }
        wnd = wintable_head;
        if (wnd == head)
            break;
    }
    UC_MouseShow();
}

void far UC_GetWindowIconXY(WINDOWS *wnd)
{
    int di = ICO_TITLEWIDTH + 10;
    int si = wnd->ico_depth + ICO_TITLEDEPTH + 13;

    if (wnd->ico_x == -1 || wnd->ico_y == -1) {
        winicox += di;
        if (getmaxx() - di - 10 < winicox) {
            winicox = 0;
            if (winicoy > si) {
                winicoy -= si;
            } else {
                winicox = 0;
                winicoy = getmaxy() - si;
            }
        }
        wnd->ico_x = winicox;
        if (wnd->ico_depth - 32 < (WORD)winicoy)
            wnd->ico_y = winicoy - (wnd->ico_depth - 32);
        else
            wnd->ico_y = 0;
    }
    wnd->left = wnd->ico_x;
    wnd->top = wnd->ico_y;
}

void far Drawjj(WINDOWS *wnd)
{
    int left, top;
    int rx1, rx2, rx3, rx4;

    UC_MouseHide();
    setcolor(4);
    UC_WindowPrintf(wnd, 80, 20, 0, "UCDOS 5.0 SDK for C/C++\nUCVision 1.0 简晶作品 (Build 3230)");
    setcolor(1);
    UC_WindowPrintf(wnd, 80, 70, 0, "北京希望高技术集团, 1996年10月 出品");

    left = wnd->left + wnd->vx;
    top = wnd->top + wnd->vy;

    rx1 = UC_RetReal(80);
    rx2 = UC_RetReal(100);
    rx3 = UC_RetReal(270);
    rx4 = UC_RetReal(65);

    setfillstyle(1, 7);
    bar(left + rx1, top + rx2, left + rx1 + rx3, top + rx2 + rx2);

    setcolor(0);
    line(left + rx1, top + rx4, left + rx1 + rx3, top + rx4);
    line(left + rx1, top + rx2, left + rx1 + rx3, top + rx2);
    line(left + rx1, top + rx2 + rx2, left + rx1, top + rx2);

    setcolor(15);
    line(left + rx1, top + rx2 + rx2, left + rx1 + rx3, top + rx2 + rx2);
    line(left + rx1 + rx3, top + rx2, left + rx1 + rx3, top + rx2 + rx2);
    UC_MouseShow();
}

void far UC_TimeJJ(void)
{
    char far *credits[29];
    WINDOWS *wnd;
    struct viewporttype vp;
    int y;

    _fmemcpy(credits, jj_credits, sizeof(jj_credits));
    wnd = UC_GetCurrentWindow();

    JJ_infocure += 2;
    if (!credits[JJ_infocure])
        JJ_infocure = 0;

    settextstyle(3, 0, 28);
    setascstyle(0);
    setcolor(0);
    setcolorbm(7);
    getviewsettings(&vp);

    Setviewport(wnd->left + wnd->vx + UC_RetReal(81),
                wnd->top + wnd->vy + UC_RetReal(101),
                wnd->left + wnd->vx + UC_RetReal(349),
                wnd->top + wnd->vy + UC_RetReal(199));

    settextstyle(0, 0, UC_RetReal(16));
    UC_MouseHide();
    for (y = 400; y > 90; y -= 5) {
        UC_TextOut(wnd, y, 115, 0x10, credits[JJ_infocure]);
    }

    settextstyle(3, 0, UC_RetReal(48));
    for (y = 400; y > 80; y -= 5) {
        UC_TextOut(wnd, y, 135, 0x10, credits[JJ_infocure + 1]);
    }
    UC_MouseShow();
    Setviewport(vp.left, vp.top, vp.right, vp.bottom);
}

void far UC_CloseJJ(void)
{
    char pal[768];

    in_JJ = 0;
    UC_KillTimer(UC_TimeJJ);
    UC_ReadBiosPalette(pal);
    UC_VGAFadeOut(256);
    UC_WindowClose();
    UC_VGAFadeIn(256, pal);
}

void far UC_SecretWindow(void)
{
    WINDOWS *wnd;

    wnd = UC_DefineWindow(4, -1, -1, 400, 300, "希望 SDK/UCVision 制作群",
                          Drawjj, UC_CloseJJ, NULL);
    UC_DefineLabel(wnd, 20, 20, 0, NULL, NULL, ICO_APPLICATION, 1);
    UC_DefinePressButton(wnd, UC_CloseJJ, 240, 220, 100, 0, "退 出", 0x1c0d);
    UC_DefineActive(wnd, 1, 1);
    wnd->boardcolor = 7;
    wnd->title_fgc = 3;
    JJ_infocure = (WORD)-2;
    UC_SetTimer(UC_TimeJJ, 2000);
    in_JJ = 1;
    UC_WindowEnable(wnd);
}

void far UC_MainLoop(void)
{
    for (;;) {
        UC_WindowsCentral();
    }
}

int far UC_InMouseEvenRect(USER_MOUSE *ms, int x, int y)
{
    int x1, y1, x2, y2;
    WINDOWS *wnd = wintable_tail;

    x1 = ms->left + wnd->left + wnd->vx;
    y1 = ms->top + wnd->top + wnd->vy;

    if (ms->right != 0)
        x2 = ms->right + wnd->left + wnd->vx;
    else
        x2 = wnd->left + wnd->width - 1 - wnd->vr;

    if (ms->bottom != 0)
        y2 = ms->bottom + wnd->top + wnd->vy;
    else
        y2 = wnd->top + wnd->height - 1 - wnd->vb;

    if (x >= x1 && x <= x2 && y >= y1 && y <= y2)
        return 1;
    return 0;
}

int far UC_AllMainOnBack(void)
{
    WINDOWS *wnd;

    for (wnd = wintable_tail->prev; wnd; wnd = wnd->prev) {
        if (wnd->mode != 0 && !(wnd->mode & 1))
            return 0;
    }
    return 1;
}
