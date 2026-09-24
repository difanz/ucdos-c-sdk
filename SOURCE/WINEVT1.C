// WINEVT1.C - 窗口事件处理 (一)

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
char far UC_GetKeyboardState(void);
void far UC_KeyBack(const int keycode);
int far UC_CheckKeyboard(void);
void far UC_InputCursorMove(WINDOWS far *wnd);
void far UC_DrawButtonBox(WINDOWS far *wnd, BUTTON far *btn, char flag);
int far UC_GetLastNumber(WINDOWS far *wnd, int cure_type);
USER_MOUSE far * far UC_GetUserMouse(WINDOWS far *wnd, int count);
GROUPBOX far * far UC_GetGroupBox(WINDOWS far *wnd, int count);
SCROLLBAR far * far UC_GetScrollbar(WINDOWS far *wnd, int count);
LABEL far * far UC_GetLabel(WINDOWS far *wnd, int count);
BUTTON far * far UC_GetButton(WINDOWS far *wnd, int count);
RADIO far * far UC_GetRadioButton(WINDOWS far *wnd, int count);
CHECK far * far UC_GetCheckButton(WINDOWS far *wnd, int count);
LISTBOX far * far UC_GetListBox(WINDOWS far *wnd, int count);
INPUTLINE far * far UC_GetInputBox(WINDOWS far *wnd, int count);
void far UC_UpdateActive(WINDOWS far *wnd, char color);
void far set_sbarcure(WINDOWS far *wnd, SCROLLBAR far *sbar, int x, int y, int l1, int l2, int leng);
int far UC_CheckScrollbarClick(WINDOWS far *wnd, SCROLLBAR far *sbar);
int far UC_CheckScrollbarMidButton(WINDOWS far *wnd, SCROLLBAR far *sbar, int cur_x, int cur_y);
int far UC_CheckScrollbarButton(WINDOWS far *wnd, SCROLLBAR far *sbar, int step_dir, int x, int y);
int far check_iconwin(WINDOWS far *wnd);
int far UC_CheckListBox(WINDOWS far *wnd);
SCROLLBAR far * far UC_CheckScrollbar(WINDOWS far *wnd);
RADIO far * far UC_CheckRadio(WINDOWS far *wnd);
CHECK far * far UC_CheckCheck(WINDOWS far *wnd);
BUTTON far * far UC_CheckButtonKey(WINDOWS far *wnd, int key);
BUTTON far * far UC_CheckButton(WINDOWS far *wnd);
void far UC_CheckPopBox(WINDOWS far *wnd, INPUTLINE far *inp);
INPUTLINE far * far UC_CheckInputLine(WINDOWS far *wnd);
void far UC_RunCheck(WINDOWS far *wnd, CHECK far *chk);
void far UC_DoCheckButton(WINDOWS far *wnd, int key);
void far UC_DoRadioButton(WINDOWS far *wnd, int key);
void far UC_DoListBox(WINDOWS far *wnd, int key);

/* 外部函数声明 */
extern void far UC_MoveCaret(WINDOWS far *wnd, int x, int y);
extern void far UC_CreateCaret(WINDOWS far *wnd, int width, int height);
extern void far UC_DestroyCaret(WINDOWS far *wnd);
extern void far UC_DisplayInputText(WINDOWS far *wnd, INPUTLINE far *inp, int focus);
extern void far UC_DoScrollbarButton(WINDOWS far *wnd, SCROLLBAR far *sbar, int step_dir, int left, int top, int right, int bottom);
extern void far UC_DisplayListBoxText(WINDOWS far *wnd, LISTBOX far *box, char focus);
extern int far UC_DoPressButton(WINDOWS far *wnd, BUTTON far *btn, int left, int top, int right, int bottom);
extern int far UC_SecondViewport(int left, int top, int right, int bottom);
extern void far killimage(char far *bitmap);

#pragma warn -rvl
char far UC_GetKeyboardState(void)
{
    asm {
        mov ah, 2
        int 0x16
        mov KEYB_STATE, al
        mov al, KEYB_STATE
    }
}
#pragma warn .rvl

void far UC_KeyBack(const int keycode)
{
    asm {
        mov ax, keycode
        push ds
        mov bx, 0x40
        mov ds, bx
        db 0x8b, 0x1e, 0x1a, 0x00  
    }
__loop1:
    asm {
        db 0x3b, 0x1e, 0x1c, 0x00  
        jz __put_key
        add bx, 2
        cmp bx, 0x3e
        jc __loop1
        mov bx, 0x1e
        jmp __loop1
    }
__put_key:
    asm {
        mov [bx], ax
        add bx, 2
        cmp bx, 0x3e
        jc __set_tail
        mov bx, 0x1e
    }
__set_tail:
    asm {
        db 0x89, 0x1e, 0x1c, 0x00  
        pop ds
    }
}

int far UC_CheckKeyboard(void)
{
    int key;

    asm {
        mov ah, 2
        int 0x16
        mov KEYB_STATE, al
        mov ah, 0x11
        int 0x16
        mov word ptr key, 0
        jz __no_key
        mov ah, 0x10
        int 0x16
        mov key, ax
    }
__no_key:
    switch (key) {
    case 0xe00d: key = 0x1c0d; break;
    case 0xf03e: key = 0x6100; break;
    case 0xf03f: key = 0x6200; break;
    case 0xf041: key = 0x6400; break;
    case 0xf044: key = 0x6700; break;
    }

    if ((key & 0xff00) != 0) {
        if ((key & 0x00ff) == 0xe0 || (key & 0x00ff) == 0xf0)
            key &= 0xff00;
    }

    if (!BOXPOP) {
        if ((wintable_tail->mode & 0x8000) || wintable_tail->minmax != 1) {
            if (USER_MAY && wintable_tail->KB_Entry != NULL) {
                USER_CHANGE = 0;
                if (key != 0) {
                    if (wintable_tail->KB_Entry(key, KEYB_STATE)) {
                        USER_CHANGE = 1;
                        return 0;
                    }
                }
            }
        }
    }
    return key;
}

void far UC_InputCursorMove(WINDOWS far *wnd)
{
    int y;
    INPUTLINE far *inp;

    if (wnd->cure_type == 2 && wnd->inpline_head != NULL) {
        inp = UC_GetInputBox(wnd, wnd->cure_num);
        if (inp != NULL) {
            y = inp->top + 2;
            UC_MoveCaret(wnd,
                inp->left + (((inp->cursx + 1) * wnd->syschar_size) >> 1) + 2,
                y);
        }
    }
}

void far UC_DrawButtonBox(WINDOWS far *wnd, BUTTON far *btn, char flag)
{
    int x2, y2;

    if (wnd->button_head == NULL)
        return;

    {
        int x1 = wnd->left + wnd->vx + btn->left;
        int y1 = wnd->top + wnd->vy + btn->top;
        x2 = x1 + btn->width;
        y2 = y1 + btn->height;

        UC_MouseHide();
        if (!flag) {
            UC_DrawButton(1, 2, x1 + 1, y1 + 1, x2 - 2, y2 - 2);
            UC_MouseShow();
        } else {
            UC_DrawButton(1, 2, x1, y1, x2 - 1, y2 - 1);
            setcolor(7);
            rectangle(x1 + 2, y1 + 2, x2 - 3, y2 - 3);
            UC_MouseShow();
        }
    }
}

int far UC_GetLastNumber(WINDOWS far *wnd, int cure_type)
{
    int count = 0;
    switch (cure_type) {
    case 1: {
        BUTTON far *p = wnd->button_head;
        while (p != NULL) {
            count++;
            p = p->next;
        }
        break;
    }
    case 2: {
        INPUTLINE far *p = wnd->inpline_head;
        while (p != NULL) {
            count++;
            p = p->next;
        }
        break;
    }
    case 3: {
        LISTBOX far *p = wnd->box_head;
        while (p != NULL) {
            count++;
            p = p->next;
        }
        break;
    }
    case 4: {
        RADIO far *p = wnd->radio;
        while (p != NULL) {
            count++;
            p = p->next;
        }
        break;
    }
    case 5: {
        CHECK far *p = wnd->check;
        while (p != NULL) {
            count++;
            p = p->next;
        }
        break;
    }
    }
    return count;
}

USER_MOUSE far * far UC_GetUserMouse(WINDOWS far *wnd, int count)
{
    USER_MOUSE far *p;
    if (count == 0) return NULL;
    p = wnd->mouse;
    if (p == NULL) return NULL;
    while (count > 1) {
        p = p->next;
        if (p == NULL) return NULL;
        count--;
    }
    return p;
}

GROUPBOX far * far UC_GetGroupBox(WINDOWS far *wnd, int count)
{
    GROUPBOX far *p;
    if (count == 0) return NULL;
    p = wnd->groupbox;
    if (p == NULL) return NULL;
    while (count > 1) {
        p = p->next;
        if (p == NULL) return NULL;
        count--;
    }
    return p;
}

SCROLLBAR far * far UC_GetScrollbar(WINDOWS far *wnd, int count)
{
    SCROLLBAR far *p;
    if (count == 0) return NULL;
    p = wnd->sbar;
    if (p == NULL) return NULL;
    while (count > 1) {
        p = p->next;
        if (p == NULL) return NULL;
        count--;
    }
    return p;
}

LABEL far * far UC_GetLabel(WINDOWS far *wnd, int count)
{
    LABEL far *p;
    if (count == 0) return NULL;
    p = wnd->label;
    if (p == NULL) return NULL;
    while (count > 1) {
        p = p->next;
        if (p == NULL) return NULL;
        count--;
    }
    return p;
}

BUTTON far * far UC_GetButton(WINDOWS far *wnd, int count)
{
    BUTTON far *p;
    if (count == 0) return NULL;
    p = wnd->button_head;
    if (p == NULL) return NULL;
    while (count > 1) {
        p = p->next;
        if (p == NULL) return NULL;
        count--;
    }
    return p;
}

RADIO far * far UC_GetRadioButton(WINDOWS far *wnd, int count)
{
    RADIO far *p;
    if (count == 0) return NULL;
    p = wnd->radio;
    if (p == NULL) return NULL;
    while (count > 1) {
        p = p->next;
        if (p == NULL) return NULL;
        count--;
    }
    return p;
}

CHECK far * far UC_GetCheckButton(WINDOWS far *wnd, int count)
{
    CHECK far *p;
    if (count == 0) return NULL;
    p = wnd->check;
    if (p == NULL) return NULL;
    while (count > 1) {
        p = p->next;
        if (p == NULL) return NULL;
        count--;
    }
    return p;
}

LISTBOX far * far UC_GetListBox(WINDOWS far *wnd, int count)
{
    LISTBOX far *p;
    if (count == 0) return NULL;
    p = wnd->box_head;
    if (p == NULL) return NULL;
    while (count > 1) {
        p = p->next;
        if (p == NULL) return NULL;
        count--;
    }
    return p;
}

INPUTLINE far * far UC_GetInputBox(WINDOWS far *wnd, int count)
{
    INPUTLINE far *p;
    if (count == 0) return NULL;
    p = wnd->inpline_head;
    if (p == NULL) return NULL;
    while (count > 1) {
        p = p->next;
        if (p == NULL) return NULL;
        count--;
    }
    return p;
}

void far UC_UpdateActive(WINDOWS far *wnd, char color)
{
    struct viewporttype old_vp;

    getviewsettings(&old_vp);
    inp_first = 0;

    if (UC_SecondViewport(wnd->left + wnd->vx,
                          wnd->top + wnd->vy,
                          wnd->left + wnd->width - 1 - wnd->vx,
                          wnd->top + wnd->height - 1 - wnd->vx)) {
        UC_MouseHide();
        switch (wnd->cure_type) {
        case 1: {
            BUTTON far *btn = UC_GetButton(wnd, wnd->cure_num);
            UC_DrawButtonBox(wnd, btn, color);
            break;
        }
        case 2: {
            INPUTLINE far *inp = UC_GetInputBox(wnd, wnd->cure_num);
            inp->cursx = 0;
            inp->curepos = 0;
            UC_DisplayInputText(wnd, inp, color);
            inp_first = 1;
            break;
        }
        case 3: {
            LISTBOX far *box = UC_GetListBox(wnd, wnd->cure_num);
            UC_EDListBox(wnd, box, color);
            box = UC_GetListBox(wnd, wnd->cure_num);
            UC_UpdateScrollbar(wnd, box->sbar);
            break;
        }
        case 4: {
            RADIO far *rad = UC_GetRadioButton(wnd, wnd->cure_num);
            UC_EnableRadioButton(wnd, rad, color);
            break;
        }
        case 5: {
            CHECK far *chk = UC_GetCheckButton(wnd, wnd->cure_num);
            UC_EnableCheckButton(wnd, chk, color);
            break;
        }
        }
        UC_MouseShow();
        Setviewport(old_vp.left, old_vp.top, old_vp.right, old_vp.bottom);
        if (UC_GetCurrentWindow() == wnd) {
            if (color == 0) {
                INPUTLINE far *inp = UC_GetInputBox(wnd, wnd->cure_num);
                if (inp->length != 0 && wnd->cure_type == 2) {
                    UC_CreateCaret(wnd, 2, wnd->syschar_size);
                    UC_InputCursorMove(wnd);
                    FLASH_CURS = 1;
                }
            } else if (FLASH_CURS) {
                UC_DestroyCaret(wnd);
                FLASH_CURS = 0;
            }
        }
    }
}

void far set_sbarcure(WINDOWS far *wnd, SCROLLBAR far *sbar, int x, int y, int l1, int l2, int leng)
{
    float old_cure = sbar->cure;
    int delta;
    int round_int;

    if (sbar->mode == 0) {
        delta = x - (wnd->left + wnd->vx + l1 + wnd->syschar_size - 1);
    } else {
        delta = y - (wnd->top + wnd->vy + l2 + wnd->syschar_size - 1);
    }

    if (sbar->max <= 1.0) {
        sbar->cure = 0.0;
    } else {
        sbar->cure = (sbar->max - 1.0) * (float)delta / (float)(leng - wnd->syschar_size * 3);
    }

    round_int = (int)sbar->cure;
    sbar->cure -= (float)round_int;
    if (sbar->cure > 0.5)
        round_int++;
    sbar->cure = (float)round_int;

    if (sbar->cure >= sbar->max)
        sbar->cure = sbar->max - 1.0;

    UC_MouseHide();
    UC_UpdateScrollbar(wnd, sbar);
    UC_MouseShow();

    if (sbar->max >= 1.0 && sbar->function != NULL) {
        sbar->function(sbar, old_cure);
    }
}

int far UC_CheckScrollbarClick(WINDOWS far *wnd, SCROLLBAR far *sbar)
{
    int left, top, leng;
    float old_cure = sbar->cure;
    int mx, my;
    int w, h;
    char page_dir;
    char at_limit;
    char first_wait;
    struct time t1, t2;

    UC_GetScrollbarXY(wnd, sbar, &left, &top, &leng);
    mx = MOUSE_AGI.x;
    my = MOUSE_AGI.y;
    left += wnd->left + wnd->vx;
    top += wnd->top + wnd->vy;

    if (sbar->mode == 0) {
        w = leng;
        h = wnd->syschar_size;
    } else {
        w = wnd->syschar_size;
        h = leng;
    }

    if (mx < left || mx > left + w || my < top || my > top + h)
        return 0;

    if (sbar->mode == 0) {
        page_dir = (mx > sbar->ox) ? 1 : 0;
    } else {
        page_dir = (my > sbar->oy) ? 1 : 0;
    }

    at_limit = 0;
    first_wait = 1;

    while (MOUSE_AGI.s & HAND_LR) {
        char cur_dir;
        if (sbar->mode == 0) {
            cur_dir = (mx > sbar->ox) ? 1 : 0;
        } else {
            cur_dir = (my > sbar->oy) ? 1 : 0;
        }

        if (at_limit || cur_dir != page_dir) {
            UC_WaitFreeMouse(NULL, 0, 0, 0, 0, NULL);
            return 1;
        }

        if (cur_dir == 0) {
            sbar->cure -= sbar->page;
            if (sbar->cure < 0.0) {
                sbar->cure = 0.0;
                at_limit = 1;
            }
        } else {
            sbar->cure += sbar->page;
            if (sbar->cure > sbar->max - 1.0) {
                if (sbar->max <= 1.0)
                    sbar->cure = 0.0;
                else
                    sbar->cure = sbar->max - 1.0;
                at_limit = 1;
            }
        }

        UC_UpdateScrollbar(wnd, sbar);
        if (sbar->function != NULL) {
            sbar->function(sbar, old_cure);
        }

        gettime(&t1);
        while (MOUSE_AGI.s & HAND_LR) {
            UC_MouseCheck();
            gettime(&t2);
            if (first_wait) {
                if (abs(t2.ti_hund - t1.ti_hund) > 40) {
                    first_wait = 0;
                    break;
                }
            } else {
                if (abs(t2.ti_hund - t1.ti_hund) > 3)
                    break;
            }
        }
        mx = MOUSE_AGI.x;
        my = MOUSE_AGI.y;
    }
    return 1;
}

int far UC_CheckScrollbarMidButton(WINDOWS far *wnd, SCROLLBAR far *sbar, int cur_x, int cur_y)
{
    int left, top, leng;
    int mx, my;
    int max_val;
    int base_x, base_y;

    UC_GetScrollbarXY(wnd, sbar, &left, &top, &leng);
    mx = MOUSE_AGI.x;
    my = MOUSE_AGI.y;

    if (mx < cur_x || mx > cur_x + wnd->syschar_size ||
        my < cur_y || my > cur_y + wnd->syschar_size)
        return 0;

    setwritemode(1);
    setlinestyle(4, 0xaaaa, 1);
    setcolor(15);

    base_x = wnd->left + wnd->vx + left + wnd->syschar_size;
    base_y = wnd->top + wnd->vy + top + wnd->syschar_size;
    max_val = base_x + leng - wnd->syschar_size * 3;

    while (MOUSE_AGI.s & HAND_LR) {
        UC_MouseHide();
        rectangle(cur_x, cur_y, cur_x + wnd->syschar_size, cur_y + wnd->syschar_size);
        UC_MouseShow();
        mx = MOUSE_AGI.x;
        my = MOUSE_AGI.y;
        while (!UC_MouseCheck())
            ;
        UC_MouseHide();
        rectangle(cur_x, cur_y, cur_x + wnd->syschar_size, cur_y + wnd->syschar_size);
        UC_MouseShow();

        if (sbar->mode == 0) {
            cur_x += (MOUSE_AGI.x - mx);
            if (cur_x < base_x) cur_x = base_x;
            if (cur_x > max_val) cur_x = max_val;
        } else {
            cur_y += (MOUSE_AGI.y - my);
            if (cur_y < base_y) cur_y = base_y;
            if (cur_y > max_val) cur_y = max_val;
        }
    }

    setwritemode(0);
    setlinestyle(0, 0, 1);
    set_sbarcure(wnd, sbar, cur_x, cur_y, left, top, leng);
    return 1;
}

int far UC_CheckScrollbarButton(WINDOWS far *wnd, SCROLLBAR far *sbar, int step_dir, int x, int y)
{
    int x2 = x + wnd->syschar_size;
    int y2 = y + wnd->syschar_size;

    if (MOUSE_AGI.x < x || MOUSE_AGI.x > x2 ||
        MOUSE_AGI.y < y || MOUSE_AGI.y > y2)
        return 0;

    UC_DoScrollbarButton(wnd, sbar, step_dir, x, y, x2, y2);
    return 1;
}

int far check_iconwin(WINDOWS far *wnd)
{
    if (MOUSE_AGI.x < wnd->left || MOUSE_AGI.x > wnd->left + wnd->width ||
        MOUSE_AGI.y < wnd->top  || MOUSE_AGI.y > wnd->top + wnd->height)
        return 0;
    return 1;
}

int far UC_CheckListBox(WINDOWS far *wnd)
{
    LISTBOX far *box = wnd->box_head;
    int index = 1;

    while (box != NULL) {
        if ((!BOXPOP || (INPOPINP != 0 && ((INPUTLINE far *)INPOPINP)->box == box)) && box->hide) {
            int x1 = wnd->left + wnd->vx + box->left;
            int y1 = wnd->top + wnd->vy + box->top;
            int x2 = x1 + ((box->width * wnd->syschar_size) >> 1) - 1;
            int y2 = y1 + box->height * (wnd->syschar_size + 2) - 1;

            if (MOUSE_AGI.x >= x1 && MOUSE_AGI.x <= x2 &&
                MOUSE_AGI.y >= y1 && MOUSE_AGI.y <= y2) {
                if (!(MOUSE_AGI.s & 0x8000)) {
                    int line = (MOUSE_AGI.y - y1) / (wnd->syschar_size + 2);
                    if ((float)line + box->top_count < box->sbar->max) {
                        box->sbar->cure = (float)line + box->top_count;
                    }
                    if (wnd->cure_type != 3 || wnd->cure_num != index) {
                        UC_UpdateActive(wnd, 1);
                        wnd->cure_type = 3;
                        wnd->cure_num = index;
                        UC_UpdateActive(wnd, 0);
                    }
                    UC_DisplayListBoxText(wnd, box, 0);
                    UC_WaitFreeMouse(NULL, 0, 0, 0, 0, NULL);
                    if (!BOXPOP)
                        return 1;
                } else {
                    UC_WaitFreeMouse(NULL, 0, 0, 0, 0, NULL);
                }
                return 2;
            }
        }
        box = box->next;
        index++;
    }
    return 0;
}

SCROLLBAR far * far UC_CheckScrollbar(WINDOWS far *wnd)
{
    SCROLLBAR far *sbar = wnd->sbar;
    int left, top, leng;
    int x, y;

    while (sbar != NULL) {
        if ((!BOXPOP || (INPOPINP != 0 && ((INPUTLINE far *)INPOPINP)->box != NULL && ((INPUTLINE far *)INPOPINP)->box->sbar == sbar)) &&
            sbar->hide && sbar->max > 1.0) {

            UC_GetScrollbarXY(wnd, sbar, &left, &top, &leng);
            x = wnd->left + wnd->vx + left;
            y = wnd->top + wnd->vy + top;

            if (UC_CheckScrollbarButton(wnd, sbar, 0, x, y))
                return sbar;

            if (sbar->mode == 0) {
                x += (leng - wnd->syschar_size);
            } else {
                y += (leng - wnd->syschar_size);
            }

            if (UC_CheckScrollbarButton(wnd, sbar, 1, x, y))
                return sbar;

            if (UC_CheckScrollbarMidButton(wnd, sbar, sbar->ox, sbar->oy))
                return sbar;

            if (UC_CheckScrollbarClick(wnd, sbar))
                return sbar;
        }
        sbar = sbar->next;
    }
    return NULL;
}

RADIO far * far UC_CheckRadio(WINDOWS far *wnd)
{
    RADIO far *rad = wnd->radio;
    int index = 1;
    int my = MOUSE_AGI.y;
    int x1, y1;

    while (rad != NULL) {
        x1 = wnd->left + wnd->vx + rad->x;
        y1 = wnd->top + wnd->vy + rad->y;
        if (MOUSE_AGI.x >= x1 && MOUSE_AGI.x <= x1 + rad->width &&
            my >= y1 && my <= y1 + rad->height) {
            break;
        }
        rad = rad->next;
        index++;
    }

    if (rad == NULL)
        return NULL;

    UC_UpdateActive(wnd, 1);
    rad->enable = (my - y1) / ((wnd->syschar_size * 3) >> 1);
    UC_UpdateRadioButton(wnd, rad);
    wnd->cure_type = 4;
    wnd->cure_num = index;
    UC_UpdateActive(wnd, 0);
    UC_WaitFreeMouse(NULL, 0, 0, 0, 0, NULL);
    return rad;
}

CHECK far * far UC_CheckCheck(WINDOWS far *wnd)
{
    CHECK far *chk = wnd->check;
    int index = 1;
    int mx = MOUSE_AGI.x;
    int my = MOUSE_AGI.y;

    while (chk != NULL) {
        int x1 = wnd->left + wnd->vx + chk->x;
        int y1 = wnd->top + wnd->vy + chk->y;
        int text_w = textwidth(chk->text);
        int x2 = x1 + text_w + ((wnd->syschar_size * 3) >> 1);
        int y2 = y1 + wnd->syschar_size;

        if (!chk->disable && mx >= x1 && mx <= x2 && my >= y1 && my <= y2) {
            UC_UpdateActive(wnd, 1);
            wnd->cure_type = 5;
            wnd->cure_num = index;
            UC_UpdateActive(wnd, 0);

            if (UC_WaitFreeMouse(NULL, x1, y1, x2, y2, NULL)) {
                chk->enable = 1 - chk->enable;
                UC_UpdateCheckButton(wnd, chk);
                UC_EnableCheckButton(wnd, chk, 0);
                return chk;
            }
        }
        chk = chk->next;
        index++;
    }
    return NULL;
}

BUTTON far * far UC_CheckButtonKey(WINDOWS far *wnd, int key)
{
    BUTTON far *btn = wnd->button_head;
    int index = 1;

    while (btn != NULL) {
        if (!btn->disable && btn->key == key)
            break;
        btn = btn->next;
        index++;
    }

    if (btn == NULL)
        return NULL;

    {
        int x1 = wnd->left + wnd->vx + btn->left;
        int y1 = wnd->top + wnd->vy + btn->top;
        int w = btn->width;
        int h = btn->height;

        UC_UpdateActive(wnd, 1);
        wnd->cure_type = 1;
        wnd->cure_num = index;
        UC_UpdateActive(wnd, 0);

        AUTOPRESS = 1;
        UC_DoPressButton(wnd, btn, x1, y1, x1 + w - 1, y1 + h - 1);
        return btn;
    }
}

BUTTON far * far UC_CheckButton(WINDOWS far *wnd)
{
    BUTTON far *btn = wnd->button_head;
    int index = 1;
    int mx = MOUSE_AGI.x;
    int my = MOUSE_AGI.y;

    while (btn != NULL) {
        int x1 = wnd->left + wnd->vx + btn->left;
        int y1 = wnd->top + wnd->vy + btn->top;
        int x2 = x1 + btn->width - 1;
        int y2 = y1 + btn->height - 1;

        if (!btn->disable && mx >= x1 && mx <= x2 && my >= y1 && my <= y2) {
            UC_UpdateActive(wnd, 1);
            wnd->cure_type = 1;
            wnd->cure_num = index;
            UC_UpdateActive(wnd, 0);

            if (UC_DoPressButton(wnd, btn, x1, y1, x2, y2))
                return btn;
        }
        btn = btn->next;
        index++;
    }
    return NULL;
}

void far UC_CheckPopBox(WINDOWS far *wnd, INPUTLINE far *inp)
{
    int x1, y1, x2, y2;
    int size;

    BOXPOP = 1 - (BOXPOP != 0);
    INPOPINP = (long)inp;

    x1 = inp->left;
    y1 = inp->top;
    x2 = x1 + ((inp->box->width * wnd->syschar_size) >> 1) + wnd->syschar_size + 2;
    y2 = y1 + inp->box->height * (wnd->syschar_size + 2);

    if (!BOXPOP) {
        x1 = wnd->left + wnd->vx + inp->box->left;
        y1 = wnd->top + wnd->vy + inp->box->top;
        UC_MouseHide();
        putimage(x1, y1, MENU_BUF, 0);
        killimage(MENU_BUF);
        UC_MouseShow();
        inp->box->hide = 0;
        inp->box->sbar->hide = 0;
        wnd->cure_type = TMPTYPE;
        wnd->cure_num = TMPNUM;
        UC_UpdateActive(wnd, 0);
        if (TMPSELE != 0) {
            if (inp->box->fun_sele != NULL)
                inp->box->fun_sele();
        } else {
            inp->box->sbar->cure = TMPBARCURE;
        }
    } else {
        size = imagesize(x1, y1, x2, y2);
        if (size == -1) size = 0x100;
        MENU_BUF = malloc(size);
        if (MENU_BUF == NULL) {
            MENU_BUF = PUB_text;
        }
        x1 = wnd->left + wnd->vx + inp->box->left;
        y1 = wnd->top + wnd->vy + inp->box->top;
        x2 = x1 + ((inp->box->width * wnd->syschar_size) >> 1) + wnd->syschar_size + 2;
        y2 = y1 + inp->box->height * (wnd->syschar_size + 2);
        UC_MouseHide();
        getimage(x1, y1, x2, y2, MENU_BUF);
        UC_DisplayListBox(wnd, inp->box);
        UC_DisplayScrollbar(wnd, inp->box->sbar);
        UC_MouseShow();
        inp->box->hide = 1;
        inp->box->sbar->hide = 1;
        TMPTYPE = wnd->cure_type;
        TMPNUM = wnd->cure_num;
        TMPBARCURE = inp->box->sbar->cure;
        TMPSELE = 0;
        wnd->cure_type = 3;
        wnd->cure_num = 0;
        while (UC_GetListBox(wnd, wnd->cure_num) != inp->box) {
            wnd->cure_num++;
        }
        UC_UpdateActive(wnd, 0);
    }
}

int far UC_WaitFreeMouse(WINDOWS far *wnd, int left, int top, int right, int bottom, void (*fun)(int x, int y, WORD s))
{
    int mx, my;

    if (!UC_WindowVerify(wnd)) {
        if (right == 0) right = getmaxx() - 1;
        if (bottom == 0) bottom = getmaxy() - 1;
    } else {
        left += wnd->left + wnd->vx;
        top  += wnd->top  + wnd->vy;
        if (right == 0) {
            right = wnd->left + wnd->width - 1 - wnd->vr;
        } else {
            right += wnd->left + wnd->vx;
        }
        if (bottom == 0) {
            bottom = wnd->top + wnd->height - 1 - wnd->vb;
        } else {
            bottom += wnd->top + wnd->vy;
        }
    }

    while (1) {
        mx = MOUSE_AGI.x;
        my = MOUSE_AGI.y;
        while (!UC_MouseCheck())
            ;
        if (!(MOUSE_AGI.s & HAND_LR))
            break;
        if (fun != NULL && (MOUSE_AGI.x != mx || MOUSE_AGI.y != my)) {
            fun(MOUSE_AGI.x, MOUSE_AGI.y, MOUSE_AGI.s);
        }
    }

    if (MOUSE_AGI.x < left || MOUSE_AGI.x > right ||
        MOUSE_AGI.y < top  || MOUSE_AGI.y > bottom)
        return 0;

    return 1;
}

INPUTLINE far * far UC_CheckInputLine(WINDOWS far *wnd)
{
    INPUTLINE far *inp = wnd->inpline_head;
    int index = 1;
    int mx = MOUSE_AGI.x;
    int my = MOUSE_AGI.y;

    while (inp != NULL) {
        if (!BOXPOP || INPOPINP == (long)inp) {
            int x1 = wnd->left + wnd->vx + inp->left;
            int y1 = wnd->top + wnd->vy + inp->top;
            int x2 = x1 + ((inp->width * wnd->syschar_size) >> 1) + 4;
            int y2 = y1 + wnd->syschar_size + 4;

            if (mx >= x1 && mx <= x2 && my >= y1 && my <= y2) {
                UC_UpdateActive(wnd, 1);
                wnd->cure_type = 2;
                wnd->cure_num = index;
                UC_UpdateActive(wnd, 0);

                if (inp->length == 0) {
                    UC_CheckPopBox(wnd, inp);
                }
                UC_WaitFreeMouse(NULL, 0, 0, 0, 0, NULL);
                return inp;
            }

            if (inp->length == 0) {
                int bx1 = x2;
                int bx2 = x2 + wnd->syschar_size;
                if (mx >= bx1 && mx <= bx2 && my >= y1 && my <= y2) {
                    if (UC_DoPressButton(NULL, NULL, bx1, y1, bx2, y2)) {
                        UC_UpdateActive(wnd, 1);
                        wnd->cure_type = 2;
                        wnd->cure_num = index;
                        UC_UpdateActive(wnd, 0);
                        UC_CheckPopBox(wnd, inp);
                    }
                }
            }
        }
        inp = inp->next;
        index++;
    }
    return NULL;
}

void far UC_RunCheck(WINDOWS far *wnd, CHECK far *chk)
{
    chk->enable = 1 - (chk->enable != 0);
    UC_UpdateCheckButton(wnd, chk);
    UC_EnableCheckButton(wnd, chk, 0);
    if (chk->fun != NULL) {
        chk->fun(chk->enable);
    }
}

void far UC_DoCheckButton(WINDOWS far *wnd, int key)
{
    CHECK far *chk = UC_GetCheckButton(wnd, wnd->cure_num);
    if (key == 0x3920) {
        UC_RunCheck(wnd, chk);
    }
}

void far UC_DoRadioButton(WINDOWS far *wnd, int key)
{
    RADIO far *rad = UC_GetRadioButton(wnd, wnd->cure_num);
    int cure = rad->enable;

    switch (key & 0xff00) {
    case 0x4800:
    case 0x4b00:
        if (cure != 0)
            cure--;
        break;
    case 0x4d00:
    case 0x5000:
        if (rad->list[cure + 1] != NULL)
            cure++;
        break;
    default:
        return;
    }

    UC_EnableRadioButton(wnd, rad, 1);
    rad->enable = cure;
    UC_UpdateRadioButton(wnd, rad);
    UC_EnableRadioButton(wnd, rad, 0);
    if (rad->fun != NULL) {
        rad->fun(rad->enable);
    }
}

void far UC_DoListBox(WINDOWS far *wnd, int key)
{
    LISTBOX far *box = UC_GetListBox(wnd, wnd->cure_num);
    char ch = (char)(key & 0xff);

    if (ch == 0) {
        switch (key) {
        case 0x4800:
            if (box->sbar->cure > 0.0) {
                box->sbar->cure -= 1.0;
                goto __update_sbar;
            }
            break;
        case 0x5000:
            if (box->sbar->cure < box->sbar->max - 1.0) {
                box->sbar->cure += 1.0;
                goto __update_sbar;
            }
            break;
        case 0x4900:
            if (box->sbar->cure > 0.0) {
                if (box->sbar->cure > box->sbar->page)
                    box->sbar->cure -= box->sbar->page;
                else
                    box->sbar->cure = 0.0;
                goto __update_sbar;
            }
            break;
        case 0x5100:
            if (box->sbar->cure < box->sbar->max - 1.0) {
                if (box->sbar->cure + box->sbar->page < box->sbar->max)
                    box->sbar->cure += box->sbar->page;
                else
                    box->sbar->cure = box->sbar->max - 1.0;
                goto __update_sbar;
            }
            break;
        case 0x4700:
            if (box->sbar->cure > 0.0) {
                box->sbar->cure = 0.0;
                goto __update_sbar;
            }
            break;
        case 0x4f00:
            if (box->sbar->cure < box->sbar->max - 1.0) {
                box->sbar->cure = box->sbar->max - 1.0;
                goto __update_sbar;
            }
            break;
        }
        return;

__update_sbar:
        UC_MouseHide();
        UC_DisplayListBoxText(wnd, box, 0);
        UC_UpdateScrollbar(wnd, box->sbar);
        UC_MouseShow();
    } else {
        if (ch == 13) {
            if (!BOXPOP) {
                if (box->fun_sele != NULL)
                    box->fun_sele();
            } else {
                TMPSELE = 1;
                UC_CheckPopBox(wnd, (INPUTLINE far *)INPOPINP);
            }
        }
        if (ch == 27) {
            if (BOXPOP) {
                UC_CheckPopBox(wnd, (INPUTLINE far *)INPOPINP);
            }
        }
    }
}
