// WINDRAW.C - 窗口绘制渲染

#include <alloc.h>
#include <dos.h>
#include <string.h>
#include "agidrv.h"
#include "struct.h"
#include "agi_bmp.h"
#include "agi_win.h"

/* 内部函数声明 */
void far nomem(char far *text);
void far MyFREE(void far *p);
void far UC_DrawDesktopDefault(void);
void far UC_DrawDesktop(void);
void far UC_WindowAppend(WINDOWS *wnd);
void far UC_WindowDelete(WINDOWS *wnd);
void far UC_WindowHide(void);
BUTTON * far UC_GetButton(WINDOWS *wnd, int count);
CHECK * far UC_GetCheckButton(WINDOWS *wnd, int count);
void far UC_DisplayInputBox(WINDOWS *wnd, INPUTLINE *inpline, int x1, int y1, int x2, int y2);
void far ICO_APPLICATION(void);

void far UC_DrawButton(char BHBH, char BLBL, int x, int y, int right, int bottom);
void far UC_Draw16X16(int left, int top, int right, int bottom, char far *text, int flag);
void far UC_DisplayScrollbar(WINDOWS *wnd, SCROLLBAR *sbar);
void far UC_DisplayListBox(WINDOWS *wnd, LISTBOX *box);
void far UC_UpdateCheckButton(WINDOWS *wnd, CHECK *check);
void far UC_EnableCheckButton(WINDOWS *wnd, CHECK *check, char flag);
void far UC_EnableRadioButton(WINDOWS *wnd, RADIO *radio, char flag);
void far UC_UpdateRadioButton(WINDOWS *wnd, RADIO *radio);
void far UC_DrawTextButton(WINDOWS *wnd, BUTTON *btn);
void far UC_DisplayListBoxText(WINDOWS *wnd, LISTBOX *box, char flag);
void far UC_UpdateScrollbar(WINDOWS *wnd, SCROLLBAR *sbar);
void far UC_RedrawFore(WINDOWS *wnd, int left, int top, int right, int bottom);
void far UC_RedrawBack(int left, int top, int right, int bottom);
void far UC_InitRedraw(WINDOWS *wnd);

WINDOWS * far UC_DefineWindow(WORD mode, int left, int top, int width, int height,
                              char *title, void (*fun_redraw)(WINDOWS *wtmp),
                              void (*fun_close)(), void (*fun_resize)(WINDOWS *wtmp))
{
    WINDOWS *wnd;

    wnd = (WINDOWS *)malloc(sizeof(WINDOWS));
    if (wnd == NULL)
        nomem("定义窗口");

    wnd->inputhot = 0;
    wnd->oldminmax = 0;
    wnd->minmax = 0;
    wnd->ICON = ICO_APPLICATION;
    wnd->ico_count = 0;
    wnd->ico_and = NULL;
    wnd->ico_xor = NULL;
    wnd->ico_handle = 0;
    wnd->ico_y = -1;
    wnd->ico_x = -1;
    wnd->minwidth = SYSCHAR_SIZE << 2;
    wnd->minheight = SYSCHAR_SIZE << 1;
    wnd->mode = mode;
    wnd->hide = 1;
    wnd->syschar_size = SYSCHAR_SIZE;
    wnd->syschar_cstyle = SYSCHAR_CSTYLE;
    wnd->syschar_astyle = SYSCHAR_ASTYLE;
    wnd->boardstyle = 1;
    wnd->menubkc = 15;
    wnd->boardcolor = 15;
    wnd->linecolor = 0;
    wnd->disablecolor = 8;
    wnd->title = title;
    wnd->ico_text = title;
    wnd->title_bkc = 1;
    wnd->title_fgc = 15;
    wnd->title_bkc_dis = 15;
    wnd->title_fgc_dis = 0;
    wnd->cure_type = 0;
    wnd->cure_num = 0;
    wnd->next = NULL;
    wnd->prev = NULL;
    wnd->groupbox = NULL;
    wnd->label = NULL;
    wnd->check = NULL;
    wnd->radio = NULL;
    wnd->sbar = NULL;
    wnd->menu = NULL;
    wnd->curemenu = NULL;
    wnd->button_head = NULL;
    wnd->button_tail = NULL;
    wnd->inpline_head = NULL;
    wnd->inpline_tail = NULL;
    wnd->box_head = NULL;
    wnd->box_tail = NULL;

    if (width == -2)
        width = (getmaxx() << 2) / 5;
    else if (width == -1)
        width = getmaxx();
    else
        UC_GetRealXY(&width);

    if (height == -2)
        height = (getmaxy() << 2) / 5;
    else if (height == -1)
        height = getmaxy();
    else
        UC_GetRealXY(&height);

    if (width < wnd->minwidth)
        width = wnd->minwidth;
    if (height < wnd->minheight)
        height = wnd->minheight;

    if (left == -2) {
        if (wintable_tail == NULL)
            left = 0;
        else
            left = wintable_tail->left + wintable_tail->syschar_size + 7;
    } else if (left == -1) {
        left = (getmaxx() - width) / 2;
        if (left < 0)
            left = 0;
    } else
        UC_GetRealXY(&left);

    if (top == -2) {
        if (wintable_tail != NULL)
            top = wintable_tail->top + wintable_tail->syschar_size + 7;
        else
            top = 0;
    } else if (top == -1) {
        top = (getmaxy() - height) / 2;
        if (top < 0)
            top = 0;
    } else
        UC_GetRealXY(&top);

    if (getmaxx() < left + width)
        left = 0;
    if (getmaxy() < top + height)
        top = 0;

    wnd->old_left = wnd->left = left;
    wnd->old_top = wnd->top = top;
    wnd->old_width = wnd->width = width;
    wnd->old_height = wnd->height = height;

    wnd->vy = (mode & 8) ? 1 : 4;
    wnd->vx = (mode & 8) ? 1 : 4;
    if (title != NULL && strlen(title) != 0)
        wnd->vy += wnd->syschar_size + 3;
    wnd->vr = wnd->vx;
    wnd->vb = wnd->vx;
    wnd->stateline_height = 0;

    if (wnd->mode == 0) {
        wnd->vb = 0;
        wnd->vr = 0;
        wnd->vy = 0;
        wnd->vx = 0;
    }

    wnd->FUNCTION = fun_redraw;
    wnd->CLOSEWIN = fun_close;
    wnd->RESIZEWIN = fun_resize;
    wnd->KB_Entry = NULL;
    wnd->DRAWBOARD = NULL;
    wnd->DRAWICON = NULL;
    wnd->ENABLE = NULL;

    UC_WindowAppend(wnd);
    return wnd;
}

void far UC_DisplayMenu(WINDOWS *wnd)
{
    struct viewporttype view;
    int x, y, top_y, right_x, bottom_y;
    int tw, i;

    UC_MouseHide();
    getviewsettings(&view);
    top_y = wnd->top + wnd->vy - (wnd->syschar_size + 5);
    x = wnd->left + wnd->vx;
    right_x = wnd->left + wnd->width - 1 - wnd->vx;
    bottom_y = top_y + wnd->syschar_size + 4;

    if (UC_SecondViewport(x, top_y, right_x, bottom_y)) {
        i = 0;
        x = wnd->left + wnd->vx;
        y = wnd->top + wnd->vy;
        setfillstyle(1, wnd->menubkc);
        bar(x, y - wnd->syschar_size - 4, right_x, y - 2);
        settextstyle(0, 0, wnd->syschar_size);
        setcolor(wnd->linecolor);
        line(x, y - 1, right_x, y - 1);
        top_y = y - wnd->syschar_size - 3;

        for (i = 0; wnd->menu[i].menu_name != NULL; i++) {
            if (wnd->curemenu == &wnd->menu[i]) {
                setcolor(wnd->title_fgc);
                setfillstyle(1, wnd->title_bkc);
            } else {
                setcolor(wnd->linecolor);
                setfillstyle(1, wnd->menubkc);
            }
            tw = textwidth(wnd->menu[i].menu_name + 1);
            bar(x, top_y - 2, x + tw, top_y + wnd->syschar_size + 1);
            outtextxy(x, top_y, wnd->menu[i].menu_name + 1);
            x += textwidth(wnd->menu[i].menu_name + 1);
        }
        Setviewport(view.left, view.top, view.right, view.bottom);
    }
    UC_MouseShow();
}

void far UC_DrawQuitButton(WINDOWS *wnd, char fgc, char bkc)
{
    int size = wnd->syschar_size;
    int x = wnd->left + wnd->vx;
    int y = wnd->top + wnd->vx;

    settextstyle(0, 0, size);
    setfillstyle(1, 7);
    UC_MouseHide();
    bar(x, y, x + size + 1, y + size + 1);
    setcolor((int)fgc);
    outtextxy(x + 1, y + 1, WIN_QUIT);
    setcolor((int)bkc);
    outtextxy(x, y, WIN_QUIT);
    setcolor(0);
    UC_MouseShow();
}

int far UC_TextPlines(char *text, int width)
{
    RECT rect;
    register int lines;

    rect.left = 0;
    rect.top = 0;
    rect.right = width - 1;
    rect.bottom = 0;

    lines = UC_TextLines(text, &rect, 0);
    return lines * textheight(text) + (lines - 1) * LINESPACE;
}

void far UC_DisplayTitle(WINDOWS *wnd)
{
    struct viewporttype view;
    RECT rect;
    char far *btn_char;
    int tw, x, y, right, top_y, lines_h, w, avail, vx1, vy1, vx2, vy2;
    int size;

    if (!UC_WindowVerify(wnd))
        return;
    if (wnd->mode == 0)
        return;
    if (wnd->title == NULL)
        return;

    size = wnd->syschar_size;
    settextstyle(0, 0, size);

    if (wnd->mode == 1) {
        if (wnd == wintable_tail) {
            setfillstyle(1, wnd->title_bkc);
            setcolor(wnd->title_fgc);
        } else {
            setfillstyle(1, 7);
            setcolor(0);
        }
        getviewsettings(&view);
        if (UC_SecondViewport(0, 0, getmaxx() - 1, getmaxy()) == 0)
            return;
        UC_MouseHide();
        x = wnd->left;
        y = wnd->top + wnd->ico_depth + 3;
        rect.left = x;
        rect.top = y;
        rect.right = x + ICO_TITLEWIDTH - 1;
        rect.bottom = y + ICO_TITLEDEPTH - 1;
        settextstyle(0, 0, IC_SIZE);
        lines_h = UC_TextPlines(wnd->ico_text, ICO_TITLEWIDTH);
        rect.bottom = y + lines_h - 1;
        w = UC_GetTextAppositeWidth(&rect, 0x2a, wnd->ico_text);
        x += (ICO_TITLEWIDTH - w) / 2;
        bar(x - 1, y - 1, x + w, rect.bottom + 1);
        UC_DrawText(NULL, &rect, 0x2a, wnd->ico_text);
    } else {
        btn_char = WIN_MAX;
        if (wnd->mode == 2)
            btn_char = WIN_RES;
        if (wnd->mode & 8)
            btn_char = WIN_MIN;

        tw = textwidth(wnd->title);
        if (tw == 0)
            return;

        x = wnd->left;
        y = wnd->top;
        right = x + wnd->width - 1 - wnd->vx;
        UC_MouseHide();
        setcolor(0);
        x += wnd->vx;
        y += wnd->vx;
        line(x, y + size + 2, right, y + size + 2);
        setfillstyle(1, (wnd == wintable_tail) ? wnd->title_bkc : wnd->title_bkc_dis);
        bar(x, y, right, y + size + 1);
        UC_DrawQuitButton(wnd, 0, 15);
        line(x + size + 2, y, x + size + 2, y + size + 1);

        if ((wnd->mode & 6) == 0 && wnd->mode != 0) {
            if ((wnd->mode & 8) == 0) {
                UC_DrawButton(3, 1, right - (size + 2) * 2, y, (right + 1) - (size + 2), y + size + 2);
                UC_Draw16X16(right - (size + 2) * 2 + 2, y + 2, (right + 1) - (size + 2), y + size + 2, WIN_MIN, 0);
            }
            UC_DrawButton(3, 1, right - (size + 2), y, right + 1, y + size + 2);
            UC_Draw16X16(right - (size + 2) + 2, y + 2, right + 1, y + size + 2, btn_char, 0);
        }

        x += size + wnd->vx;
        if ((wnd->mode & 6) == 0 && wnd->mode != 0) {
            if (size * 3 + 4 + tw < wnd->width) {
                if ((wnd->mode & 8) == 0)
                    avail = wnd->width - size - (size + 2) * 2;
                else
                    avail = wnd->width - size - (size + 2);
                x += (avail - tw) / 2;
            }
        } else if (size + tw < wnd->width) {
            avail = wnd->width - size;
            x += (avail - tw) / 2;
        }

        if (x - wnd->vx > wnd->left + size + wnd->vx + 2)
            x -= wnd->vx;

        setcolor((wnd == wintable_tail) ? wnd->title_fgc : wnd->title_fgc_dis);
        getviewsettings(&view);
        vx1 = wnd->left + wnd->vx;
        vy1 = wnd->top + wnd->vx;
        vx2 = wnd->left + wnd->width - 1 - wnd->vx;
        vy2 = vy1 + wnd->syschar_size + 3;
        if ((wnd->mode & 6) == 0 && wnd->mode != 0)
            vx2 -= ((wnd->syschar_size + 2) * 2 + 1);

        if (UC_SecondViewport(vx1, vy1, vx2, vy2)) {
            outtextxy(x, y + 1, wnd->title);
        }
    }
    Setviewport(view.left, view.top, view.right, view.bottom);
    UC_MouseShow();
}

void far UC_DrawButton(char BHBH, char BLBL, int x, int y, int right, int bottom)
{
    BLBL = 1;
    _AX = 0x90f9;
    _BH = BHBH;
    _BL = BLBL;
    _CX = x;
    _DX = y;
    _SI = right;
    _DI = bottom;
    asm push es;
    asm push bp;
    asm mov bp, 7;
    asm mov es, bp;
    asm mov bp, 0x0f08;
    asm int 0x48;
    asm pop bp;
    asm pop es;
}

void far UC_GetClientRect(WINDOWS *wnd, RECT *rect)
{
    rect->left = 0;
    rect->top = 0;
    rect->right = wnd->width - 1 - wnd->vx - wnd->vr;
    rect->bottom = wnd->height - 1 - wnd->vy - wnd->vb;
}

void far UC_GetScrollbarXY(WINDOWS *wnd, SCROLLBAR *sbar, int *x, int *y, int *leng)
{
    *x = sbar->left;
    *y = sbar->top;
    *leng = sbar->leng;
    if (*x == -1)
        *x = wnd->width - wnd->vx - wnd->vr;
    if (*y == -1)
        *y = wnd->height - wnd->vy - wnd->vb;
    if (*leng == 0) {
        *leng = ((sbar->mode != 0) ? (wnd->height - 1 - wnd->vy - wnd->vb) : (wnd->width - 1 - wnd->vx - wnd->vr)) + 1;
    }
}

void far UC_Draw16X16(int left, int top, int right, int bottom, char far *text, int flag)
{
    WORD old_width = textinfo.FontWidth;
    WORD old_height = textinfo.FontHeight;
    WORD old_chifont = textinfo.ChiFontNo;
    WORD old_ascfont = textinfo.AscFontNo;
    RECT rect;
    WORD format;

    rect.left = left;
    rect.top = top;
    rect.right = right - 1;
    rect.bottom = bottom - 1;

    settextstyle(0, 0, 16);
    format = 0x0a;
    if (flag)
        format |= 0x10;
    UC_DrawText(NULL, &rect, format, text);

    textinfo.FontWidth = old_width;
    textinfo.FontHeight = old_height;
    textinfo.ChiFontNo = old_chifont;
    textinfo.AscFontNo = old_ascfont;
}

void far UC_DisplayScrollbar(WINDOWS *wnd, SCROLLBAR *sbar)
{
    int sx, sy, slen;
    int x1, y1, x2, y2;

    UC_GetScrollbarXY(wnd, sbar, &sx, &sy, &slen);
    x1 = wnd->left + wnd->vx + sx;
    y1 = wnd->top + wnd->vy + sy;

    if (sbar->mode == 0) {
        y2 = wnd->syschar_size;
        x2 = slen;
    } else {
        y2 = slen;
        x2 = wnd->syschar_size;
    }
    x2 += x1;
    y2 += y1;

    setcolor(wnd->linecolor);
    rectangle(x1, y1, x2, y2);
    x2 = x1 + wnd->syschar_size;
    y2 = y1 + wnd->syschar_size;
    UC_DrawButton(3, 1, x1, y1, x2, y2);
    setcolor((sbar->max <= 1.0) ? wnd->disablecolor : wnd->linecolor);
    setfillstyle(1, 7);

    if (sbar->mode == 0) {
        UC_Draw16X16(x1 + 1, y1 + 1, x2, y2, CLICK_LEFT, 0);
        bar(x1 + wnd->syschar_size + 1, y1 + 1, x1 + slen - wnd->syschar_size, y1 + wnd->syschar_size - 1);
        x2 = x1 + slen;
        x1 = x2 - wnd->syschar_size;
        UC_DrawButton(3, 1, x1, y1, x2, y2);
        UC_Draw16X16(x1 + 1, y1 + 1, x2, y2, CLICK_RIGHT, 0);
    } else {
        UC_Draw16X16(x1 + 1, y1 + 1, x2, y2, CLICK_UP, 0);
        bar(x1 + 1, y1 + wnd->syschar_size + 1, x1 + wnd->syschar_size - 1, y1 + slen - wnd->syschar_size);
        y2 = y1 + slen;
        y1 = y2 - wnd->syschar_size;
        UC_DrawButton(3, 1, x1, y1, x2, y2);
        UC_Draw16X16(x1 + 1, y1 + 1, x2, y2, CLICK_DOWN, 0);
    }
    UC_UpdateScrollbar(wnd, sbar);
}

void far UC_DisplayListBox(WINDOWS *wnd, LISTBOX *box)
{
    int left = wnd->left + wnd->vx + box->left;
    int top = wnd->top + wnd->vy + box->top;
    int right = left + ((box->width * wnd->syschar_size) >> 1) + 1;
    int bottom = top + box->height * (wnd->syschar_size + 2);

    setfillstyle(1, wnd->boardcolor);
    bar(left, top, right, bottom);
    setcolor(wnd->linecolor);
    rectangle(left, top, right, bottom);
    UC_DisplayListBoxText(wnd, box, 1);
}

void far UC_UpdateCheckButton(WINDOWS *wnd, CHECK *check)
{
    int x = wnd->left + wnd->vx + check->x;
    int y = wnd->top + wnd->vy + check->y;

    setcolor(check->disable ? wnd->disablecolor : wnd->linecolor);
    setcolorbm(wnd->boardcolor);
    settextstyle(0, 0, wnd->syschar_size);
    UC_MouseHide();
    UC_Draw16X16(x, y, x + 16, y + wnd->syschar_size,
                 check->enable ? CHAR_CHECKYES : CHAR_CHECKNO, 1);
    outtextbm(x + 24, y, check->text);
    UC_MouseShow();
}

void far UC_EnableCheckButton(WINDOWS *wnd, CHECK *check, char flag)
{
    int x = check->x + wnd->left + wnd->vx;
    int y = check->y + wnd->top + wnd->vy;

    if (flag == 0) {
        setcolor(wnd->linecolor);
        setlinestyle(USERBIT_LINE, 0xaaaa, 1);
    } else {
        setcolor(wnd->boardcolor);
        setlinestyle(SOLID_LINE, 0, 1);
    }
    settextstyle(0, 0, wnd->syschar_size);
    UC_MouseHide();
    rectangle(x + 21, y - 2, x + textwidth(check->text) + 25, y + wnd->syschar_size + 1);
    UC_MouseShow();
    setlinestyle(SOLID_LINE, 0, 1);
}

void far UC_EnableRadioButton(WINDOWS *wnd, RADIO *radio, char flag)
{
    int x = radio->x + wnd->left + wnd->vx;
    int y = radio->y + wnd->top + wnd->vy + radio->enable * ((wnd->syschar_size * 3) >> 1);

    if (flag == 0) {
        setcolor(wnd->linecolor);
        setlinestyle(USERBIT_LINE, 0xaaaa, 1);
    } else {
        setcolor(wnd->boardcolor);
        setlinestyle(SOLID_LINE, 0, 1);
    }
    settextstyle(0, 0, wnd->syschar_size);
    UC_MouseHide();
    rectangle(x + 21, y - 2, x + textwidth(radio->list[radio->enable]) + 25, y + wnd->syschar_size + 1);
    UC_MouseShow();
    setlinestyle(SOLID_LINE, 0, 1);
}

void far UC_UpdateRadioButton(WINDOWS *wnd, RADIO *radio)
{
    int i = 0;
    int x, y;

    setcolor(0);
    setcolorbm(wnd->boardcolor);
    settextstyle(0, 0, wnd->syschar_size);
    x = radio->x + wnd->left + wnd->vx;
    y = radio->y + wnd->top + wnd->vy;
    UC_MouseHide();

    while (radio->list[i] != NULL) {
        UC_Draw16X16(x, y, x + 16, y + wnd->syschar_size,
                     (radio->enable == i) ? CHAR_RADIOYES : CHAR_RADIONO, 1);
        outtextbm(x + 24, y, radio->list[i]);
        i++;
        y += (wnd->syschar_size * 3) >> 1;
    }
    UC_MouseShow();
}

void far UC_DefineDrawBackground(WINDOWS *wnd, void (*fun)(WINDOWS *wnd))
{
    wnd->DRAWBOARD = fun;
}

void far UC_DisableObject(WINDOWS *wnd, WORD cure_type, WORD cure_num)
{
    if (cure_type == 1) {
        BUTTON *btn = UC_GetButton(wnd, cure_num);
        if (btn != NULL) {
            btn->disable = 1;
            if (wnd == UC_GetCurrentWindow())
                UC_DrawTextButton(wnd, btn);
        }
    } else if (cure_type == 5) {
        CHECK *chk = UC_GetCheckButton(wnd, cure_num);
        if (chk != NULL) {
            chk->disable = 1;
            if (wnd == UC_GetCurrentWindow())
                UC_UpdateCheckButton(wnd, chk);
        }
    }
}

void far UC_EnableObject(WINDOWS *wnd, WORD cure_type, WORD cure_num)
{
    if (cure_type == 1) {
        BUTTON *btn = UC_GetButton(wnd, cure_num);
        if (btn != NULL) {
            btn->disable = 0;
            if (wnd == UC_GetCurrentWindow())
                UC_DrawTextButton(wnd, btn);
        }
    } else if (cure_type == 5) {
        CHECK *chk = UC_GetCheckButton(wnd, cure_num);
        if (chk != NULL) {
            chk->disable = 0;
            if (wnd == UC_GetCurrentWindow())
                UC_UpdateCheckButton(wnd, chk);
        }
    }
}

void far UC_DrawTextButton(WINDOWS *wnd, BUTTON *btn)
{
    RECT rect;
    int left, top, right, bottom;

    UC_MouseHide();
    left = wnd->left + wnd->vx + btn->left;
    top = wnd->top + wnd->vy + btn->top;
    right = left + btn->width - 1;
    bottom = top + btn->height - 1;
    UC_DrawButton(3, 2, left, top, right, bottom);
    rect.left = left;
    rect.top = top;
    rect.right = right;
    rect.bottom = bottom;
    setcolor(btn->disable ? wnd->disablecolor : wnd->linecolor);
    settextstyle(0, 0, wnd->syschar_size);
    UC_DrawText(NULL, &rect, 0x0a, btn->text);
    UC_MouseShow();
}

void far UC_WindowShow(WINDOWS *wnd)
{
    int left, top, right, bottom;
    int corner, color;
    struct viewporttype view;
    RADIO *rd;
    CHECK *ck;
    BUTTON *btn;
    GROUPBOX *gb;
    LABEL *lb;
    INPUTLINE *inp;
    SCROLLBAR *sb;
    LISTBOX *box;

    if (!UC_WindowVerify(wnd))
        return;
    if (UC_CheckInViewPort(wnd))
        return;

    UC_MouseHide();
    wnd->hide = 0;
    left = wnd->left;
    top = wnd->top;

    if (wnd->mode == 1) {
        left += (ICO_TITLEWIDTH - wnd->ico_width) >> 1;
        if (wnd->DRAWICON == NULL)
            UC_ShowIcon(wnd, wnd->ico_handle, wnd->ico_xor, wnd->ico_and, wnd->ico_width, wnd->ico_depth, left, top);
        else
            UC_Cadi(wnd, wnd->left, top);
        UC_DisplayTitle(wnd);
    } else {
        right = left + wnd->width;
        bottom = top + wnd->height;
        settextstyle(0, 0, wnd->syschar_size);
        if (wnd->boardstyle != 0) {
            setfillstyle(wnd->boardstyle, wnd->boardcolor);
            bar(left, top, right - 1, bottom - 1);
        }
        if (wnd->mode != 2 && wnd->mode != 0) {
            setcolor(wnd->linecolor);
            rectangle(left, top, right - 1, bottom - 1);
            if ((wnd->mode & 8) == 0) {
                if ((wnd->mode & 6) != 0 || wnd->mode == 0)
                    setcolor(wnd->boardcolor);
                rectangle(left + 3, top + 3, right - 4, bottom - 4);
                if ((wnd->mode & 6) == 0 && wnd->mode != 0)
                    color = 7;
                else
                    color = wnd->title_bkc;
                setcolor(color);
                rectangle(left + 1, top + 1, right - 2, bottom - 2);
                rectangle(left + 2, top + 2, right - 3, bottom - 3);
                if ((wnd->mode & 6) == 0 && wnd->mode != 0) {
                    setcolor(wnd->linecolor);
                    corner = wnd->syschar_size + wnd->vx + 2;
                    line(left + 1, top + corner, left + 2, top + corner);
                    line(left + corner, top + 1, left + corner, top + corner);
                    line(left + wnd->width - corner - 1, top + 1, left + wnd->width - corner - 1, top + 2);
                    line(left + wnd->width - 3, top + corner, left + wnd->width - 2, top + corner);
                    line(left + wnd->width - 3, top + wnd->height - corner - 1, left + wnd->width - 2, top + wnd->height - corner - 1);
                    line(left + wnd->width - corner - 1, top + wnd->height - 3, left + wnd->width - corner - 1, top + wnd->height - 2);
                    line(left + corner, top + wnd->height - 3, left + corner, top + wnd->height - 2);
                    line(left + 1, top + wnd->height - corner - 1, left + 2, top + wnd->height - corner - 1);
                }
            }
        }
        UC_DisplayTitle(wnd);
        if (wnd->stateline_height != 0) {
            setcolor(0);
            line(wnd->left + wnd->vx,
                 wnd->top + wnd->height - wnd->vx - wnd->stateline_height,
                 wnd->left + wnd->width - 1 - wnd->vx,
                 wnd->top + wnd->height - wnd->vx - wnd->stateline_height);
        }
        if (wnd->menu != NULL)
            UC_DisplayMenu(wnd);

        getviewsettings(&view);
        setcolor(wnd->linecolor);
        setcolorbm(wnd->boardcolor);
        settextstyle(0, 0, wnd->syschar_size);

        if (UC_SecondViewport(wnd->left + wnd->vx,
                              wnd->top + wnd->vy,
                              wnd->left + wnd->width - 1 - wnd->vx,
                              wnd->top + wnd->height - 1 - wnd->vx)) {
            if (wnd->FUNCTION != NULL)
                (*wnd->FUNCTION)(wnd);

            for (rd = wnd->radio; rd != NULL; rd = rd->next)
                UC_UpdateRadioButton(wnd, rd);

            for (ck = wnd->check; ck != NULL; ck = ck->next)
                UC_UpdateCheckButton(wnd, ck);

            settextstyle(0, 0, wnd->syschar_size);
            for (btn = wnd->button_head; btn != NULL; btn = btn->next)
                UC_DrawTextButton(wnd, btn);

            setcolor(wnd->linecolor);
            setcolorbm(wnd->boardcolor);
            for (gb = wnd->groupbox; gb != NULL; gb = gb->next) {
                int gx = gb->left + wnd->left + wnd->vx;
                int gy = gb->top + wnd->top + wnd->vy;
                int gy_mid = gy + (wnd->syschar_size >> 1);
                rectangle(gx, gy_mid, gx + gb->width, gy + gb->height);
                if (gb->text != NULL)
                    outtextbm(gx + (wnd->syschar_size >> 1), gy_mid - (wnd->syschar_size >> 1), gb->text);
            }

            for (lb = wnd->label; lb != NULL; lb = lb->next) {
                if (lb->text == NULL) {
                    if (lb->ico_addr != NULL) {
                        int lx = lb->left;
                        int ly = lb->top;
                        UC_GetRealXY(&lx);
                        UC_GetRealXY(&ly);
                        UC_ShowIcon(wnd, 0, lb->xor_buf, lb->and_buf, lb->ico_width, lb->ico_depth, lx, ly);
                    }
                } else {
                    UC_WindowPrintf(wnd, lb->left, lb->top, 0, lb->text);
                }
            }

            for (inp = wnd->inpline_head; inp != NULL; inp = inp->next) {
                int ix = wnd->left + wnd->vx + inp->left;
                int iy = wnd->top + wnd->vy + inp->top;
                int iw = ix + ((inp->width * wnd->syschar_size) >> 1) + 4;
                int ih = iy + wnd->syschar_size + 4;
                UC_DisplayInputBox(wnd, inp, ix, iy, iw, ih);
                ix = iw;
                if (inp->box == NULL) {
                    UC_DrawButton(3, 1, iw, iy, iw + wnd->syschar_size, ih);
                    setcolor(wnd->linecolor);
                    settextstyle(0, 0, wnd->syschar_size);
                    UC_Draw16X16(ix + 1, iy + 1, iw + wnd->syschar_size, ih, CLICK_POP, 0);
                }
            }

            for (sb = wnd->sbar; sb != NULL; sb = sb->next) {
                if (sb->hide == 0)
                    UC_DisplayScrollbar(wnd, sb);
            }

            for (box = wnd->box_head; box != NULL; box = box->next) {
                if (box->hide == 0)
                    UC_DisplayListBox(wnd, box);
            }

            UC_UpdateActive(wnd, 0);
            Setviewport(view.left, view.top, view.right, view.bottom);
        }
    }
    UC_MouseShow();
}

void far UC_EDListBox(WINDOWS *wnd, LISTBOX *box, char flag)
{
    int left = wnd->left + wnd->vx + box->left;
    int top = wnd->top + wnd->vy + box->top;
    int right = left + (((box->width + 2) * wnd->syschar_size) >> 1) + 1;
    int bottom = top + box->height * (wnd->syschar_size + 2);

    if (BOXPOP == 0) {
        setcolor(flag ? wnd->boardcolor : wnd->linecolor);
        rectangle(left - 1, top - 1, right + 1, bottom + 1);
    }
}

void far UC_DisplayListBoxText(WINDOWS *wnd, LISTBOX *box, char flag)
{
    UC_EDListBox(wnd, box, flag);
    if (box->sbar != NULL && box->sbar->function != NULL) {
        (*box->sbar->function)(box->sbar, box->sbar->cure);
    }
}

void far UC_UpdateScrollbar(WINDOWS *wnd, SCROLLBAR *sbar)
{
    int sx, sy, slen;
    int old_x, old_y;
    int x, y;
    float f;
    long len;

    UC_GetScrollbarXY(wnd, sbar, &sx, &sy, &slen);
    old_x = sbar->ox;
    old_y = sbar->oy;

    if (sbar->max == 0.0)
        sbar->max = 1.0;
    if (sbar->cure > sbar->max - 1.0)
        sbar->cure = sbar->max - 1.0;

    if (sbar->max <= 1.0) {
        f = 0.0;
    } else {
        len = slen - wnd->syschar_size * 3 - 1;
        f = (sbar->cure / (sbar->max - 1.0)) * (float)len;
    }
    if (f == 0.0)
        f += 1.0;

    if (sbar->mode == 0) {
        x = (int)f;
        y = wnd->top + wnd->vy + sy;
    } else {
        x = wnd->left + wnd->vx + sx;
        y = (int)f;
    }

    UC_MouseHide();
    if (old_x != -1) {
        int x1, y1, x2, y2;
        setfillstyle(1, 7);
        if (sbar->mode == 0) {
            if (x <= old_x) {
                x1 = x + wnd->syschar_size + 1;
                y1 = old_y + 1;
                x2 = old_x + wnd->syschar_size;
                y2 = old_y + wnd->syschar_size - 1;
            } else {
                x1 = old_x + 1;
                y1 = old_y + 1;
                x2 = x - 1;
                y2 = old_y + wnd->syschar_size - 1;
            }
        } else {
            if (old_y < y) {
                x1 = old_x + 1;
                y1 = old_y + 1;
                x2 = old_x + wnd->syschar_size - 1;
                y2 = y - 1;
            } else {
                x1 = old_x + 1;
                y1 = y + wnd->syschar_size + 1;
                x2 = old_x + wnd->syschar_size - 1;
                y2 = old_y + wnd->syschar_size;
            }
        }
        bar(x1, y1, x2, y2);
    }
    UC_DrawButton(3, 1, x, y, x + wnd->syschar_size, y + wnd->syschar_size);
    UC_MouseShow();
    sbar->ox = x;
    sbar->oy = y;
}

int far UC_BeginPaintOnBack(WINDOWS *wnd)
{
    if (UC_WindowVerify(wnd) && !UC_CheckInBack(wnd)) {
        getviewsettings(&backvi);
        if (wnd->mode == 1) {
            Setviewport(wnd->left, wnd->top, wnd->left + wnd->width - 1, wnd->top + wnd->height - 1);
        } else {
            Setviewport(wnd->left + wnd->vx, wnd->top + wnd->vy,
                        wnd->left + wnd->width - 1 - wnd->vx,
                        wnd->top + wnd->height - 1 - wnd->vx);
        }
        return 1;
    }
    return 0;
}

void far UC_EndPaintOnBack(WINDOWS *wnd)
{
    int left = wnd->left;
    int top = wnd->top;
    int right = wnd->left + wnd->width - 1;
    int bottom = wnd->top + wnd->height - 1;

    if (wnd->mode != 1) {
        left += wnd->vx;
        top += wnd->vy;
        right -= wnd->vx;
        bottom -= wnd->vx;
    }
    UC_RedrawFore(wnd, left, top, right, bottom);
    Setviewport(backvi.left, backvi.top, backvi.right, backvi.bottom);
}

void far UC_GetStateLineRect(WINDOWS *wnd, RECT *rect)
{
    rect->left = 0;
    rect->top = wnd->height - wnd->vy - wnd->vr - wnd->stateline_height + 1;
    rect->right = wnd->width - wnd->vx - wnd->vr;
    rect->bottom = wnd->height - wnd->vy - wnd->vr;
}

int far UC_BeginPaintStateLine(WINDOWS *wnd)
{
    if (UC_WindowVerify(wnd) && wnd->stateline_height != 0) {
        getviewsettings(&sl_vi);
        if (UC_SecondViewport(wnd->left + wnd->vx,
                              wnd->top + wnd->height - wnd->vx - wnd->stateline_height,
                              wnd->left + wnd->width - wnd->vx,
                              wnd->top + wnd->height - wnd->vx)) {
            old_vb = wnd->vb;
            old_vr = wnd->vr;
            wnd->vb = wnd->vx;
            wnd->vr = wnd->vx;
            return 1;
        }
    }
    return 0;
}

void far UC_EndPaintStateLine(WINDOWS *wnd)
{
    wnd->vb = old_vb;
    wnd->vr = old_vr;
    Setviewport(sl_vi.left, sl_vi.top, sl_vi.right, sl_vi.bottom);
}

void far UC_RedrawFore(WINDOWS *wnd, int left, int top, int right, int bottom)
{
    struct viewporttype view;

    getviewsettings(&view);
    Setviewport(left, top, right, bottom);

    while (wnd != wintable_tail) {
        wnd = wnd->next;
        if (wnd == wintable_tail && windows_hide != 0)
            break;
        if (!UC_WindowVerify(wnd))
            break;
        if (wnd->left <= right && left <= wnd->left + wnd->width - 1 &&
            wnd->top <= bottom && top <= wnd->top + wnd->height - 1 &&
            wnd->hide == 0) {
            UC_RedrawWindow(wnd);
        }
    }
    Setviewport(view.left, view.top, view.right, view.bottom);
}

void far UC_InvalidataRect(WINDOWS *wnd, RECT *rect)
{
    struct viewporttype view;
    int left, top, right, bottom;

    getviewsettings(&view);
    left = wnd->left + wnd->vx + rect->left;
    if (left <= wnd->left + wnd->width - wnd->vr) {
        top = wnd->top + wnd->vy + rect->top;
        if (top <= wnd->top + wnd->height - wnd->vb) {
            right = wnd->left + wnd->vx + rect->right;
            if (right > wnd->left + wnd->width - wnd->vr)
                right = wnd->left + wnd->width - wnd->vr;
            bottom = wnd->top + wnd->vy + rect->bottom;
            if (bottom > wnd->top + wnd->height - wnd->vb)
                bottom = wnd->top + wnd->height - wnd->vb;
            Setviewport(left, top, right, bottom);
            UC_RedrawWindow(wnd);
            Setviewport(view.left, view.top, view.right, view.bottom);
        }
    }
}

void far UC_RedrawBack(int left, int top, int right, int bottom)
{
    struct viewporttype view;
    WINDOWS *wnd;

    getviewsettings(&view);
    windows_rdbk = 1;
    Setviewport(left, top, right, bottom);

    if (INIT_FIRST == 0)
        UC_DrawDesktop();
    else
        UC_DrawDesktopDefault();

    for (wnd = wintable_head; wnd != wintable_tail; wnd = wnd->next) {
        if (wnd->hide == 0 &&
            wnd->left <= right && left <= wnd->left + wnd->width - 1 &&
            wnd->top <= bottom && top <= wnd->top + wnd->height - 1) {
            UC_RedrawWindow(wnd);
        }
    }
    Setviewport(view.left, view.top, view.right, view.bottom);
    windows_rdbk = 0;
}

void far UC_WindowClose(void)
{
    WINDOWS *wnd = wintable_tail;
    GROUPBOX *gb;
    USER_MOUSE *ms;
    CHECK *ck;
    RADIO *rd;
    LABEL *lb;
    BUTTON *btn;
    SCROLLBAR *sb;
    LISTBOX *box;
    INPUTLINE *inp;

    if (UC_WindowVerify(wnd) && (wnd->ENABLE == NULL || (*wnd->ENABLE)(wnd, 7) == 0)) {
        UC_DestroyCaret(wnd);
        UC_WindowHide();
        if (wnd->ico_handle != 0)
            UC_FreeXMS(wnd->ico_handle);
        if (wnd->ico_xor != NULL)
            MyFREE(wnd->ico_xor);
        if (wnd->ico_and != NULL)
            MyFREE(wnd->ico_and);

        gb = wnd->groupbox;
        while (gb != NULL) {
            GROUPBOX *next = gb->next;
            MyFREE(gb);
            gb = next;
        }

        ms = wnd->mouse;
        while (ms != NULL) {
            USER_MOUSE *next = ms->next;
            MyFREE(ms);
            ms = next;
        }

        ck = wnd->check;
        while (ck != NULL) {
            CHECK *next = ck->next;
            MyFREE(ck);
            ck = next;
        }

        rd = wnd->radio;
        while (rd != NULL) {
            RADIO *next = rd->next;
            MyFREE(rd);
            rd = next;
        }

        lb = wnd->label;
        while (lb != NULL) {
            LABEL *next = lb->next;
            if (lb->text == NULL && lb->ico_addr != NULL) {
                MyFREE(lb->xor_buf);
                MyFREE(lb->and_buf);
            }
            MyFREE(lb);
            lb = next;
        }

        btn = wnd->button_tail;
        while (btn != NULL) {
            BUTTON *prev = btn->prev;
            MyFREE(btn);
            btn = prev;
        }

        sb = wnd->sbar;
        while (sb != NULL) {
            SCROLLBAR *next = sb->next;
            MyFREE(sb);
            sb = next;
        }

        box = wnd->box_tail;
        while (box != NULL) {
            LISTBOX *prev = box->prev;
            MyFREE(box);
            box = prev;
        }

        inp = wnd->inpline_tail;
        while (inp != NULL) {
            INPUTLINE *prev = inp->prev;
            MyFREE(inp);
            inp = prev;
        }

        UC_WindowDelete(wnd);
        MyFREE(wnd);
    }
}

void far UC_InitRedraw(WINDOWS *wnd)
{
    SCROLLBAR *sb;

    if (wnd->mode != 1 && wnd->RESIZEWIN != NULL) {
        (*wnd->RESIZEWIN)(wnd);
    }

    for (sb = wnd->sbar; sb != NULL; sb = sb->next) {
        sb->ox = -1;
        sb->oy = -1;
        if (sb->max < 1.0)
            sb->max = 1.0;
        if (sb->cure < 0.0)
            sb->cure = 0.0;
        if (sb->max <= sb->cure)
            sb->cure = sb->max - 1.0;
    }
    UC_RedrawWindow(wnd);
}

void far UC_WindowResize(int left, int top, int right, int bottom)
{
    WINDOWS *wnd = wintable_tail;
    int old_left, old_top, old_right, old_bottom;
    int r_left, r_right, r_top, r_bottom;

    if ((wnd->ENABLE == NULL || (*wnd->ENABLE)(wnd, 8) == 0) &&
        (wnd->left != left || wnd->top != top ||
         wnd->left + wnd->width - 1 != right ||
         wnd->top + wnd->height - 1 != bottom)) {

        old_left = wnd->left;
        old_top = wnd->top;
        old_right = old_left + wnd->width - 1;
        old_bottom = old_top + wnd->height - 1;

        wnd->left = left;
        wnd->top = top;
        wnd->width = right - left + 1;
        wnd->height = bottom - top + 1;

        if (wnd->width < wnd->minwidth)
            wnd->width = wnd->minwidth;
        if (wnd->height < wnd->minheight)
            wnd->height = wnd->minheight;

        UC_InitRedraw(wnd);

        if (old_top < top) {
            r_left = (left < old_left) ? left : old_left;
            r_right = (right < old_right) ? old_right : right;
            UC_RedrawBack(r_left, old_top, r_right, top - 1);
        }
        if (bottom < old_bottom) {
            r_left = (left < old_left) ? left : old_left;
            r_right = (right < old_right) ? old_right : right;
            UC_RedrawBack(r_left, bottom + 1, r_right, old_bottom);
        }
        if (old_left < left) {
            r_top = (top < old_top) ? old_top : top;
            r_bottom = (old_bottom < bottom) ? old_bottom : bottom;
            UC_RedrawBack(old_left, r_top, left - 1, r_bottom);
        }
        if (right < old_right) {
            if (top < old_top)
                top = old_top;
            if (old_bottom < bottom)
                bottom = old_bottom;
            UC_RedrawBack(right + 1, top, old_right, bottom);
        }
    }
}

void far UC_ScrollBlock(WINDOWS *wnd, int left, int top, int width, int height,
                        int new_left, int new_top)
{
    struct viewporttype view;
    int vl, vt, vr, vb;
    int size;
    void far *buf;

    if (UC_WindowVerify(wnd)) {
        left += wnd->left + wnd->vx;
        top += wnd->top + wnd->vy;
        new_left += wnd->left + wnd->vx;
        new_top += wnd->top + wnd->vy;
    }
    if (getmaxx() < left + width)
        width = getmaxx() - left;
    if (getmaxy() < top + height)
        height = getmaxy() - top;

    if (width > 0 && height > 0) {
        getviewsettings(&view);
        if (wnd != NULL) {
            vl = wnd->left + wnd->vx;
            vt = wnd->top + wnd->vy;
            vr = wnd->left + wnd->width - 1 - wnd->vr;
            vb = wnd->top + wnd->height - 1 - wnd->vb;
        } else {
            vl = 0;
            vt = 0;
            vr = getmaxx() - 1;
            vb = getmaxy();
        }
        if (UC_SecondViewport(vl, vt, vr, vb)) {
            UC_MouseHide();
            size = imagesize(left, top, left + width - 1, top + height - 1);
            if (size == -1)
                size = 0x100;
            buf = malloc(size);
            if (buf == NULL)
                buf = PUBLIC_BUF;
            getimage(left, top, left + width - 1, top + height - 1, buf);
            putimage(new_left, new_top, buf, 0);
            killimage(buf);
            Setviewport(view.left, view.top, view.right, view.bottom);
            UC_MouseShow();
        }
    }
}

void far UC_WindowMove(int left, int top)
{
    WINDOWS *wnd = wintable_tail;
    int old_left, old_top;
    int r_left, r_right;
    SCROLLBAR *sb;

    if (wnd->ENABLE != NULL && (*wnd->ENABLE)(wnd, 6) != 0)
        return;

    if (wnd->mode == 1) {
        wnd->left = left;
        wnd->top = top;
        UC_RedrawWindow(wnd);
        return;
    }

    if (!UC_WindowVerify(wnd))
        return;

    old_left = wnd->left;
    old_top = wnd->top;

    for (sb = wnd->sbar; sb != NULL; sb = sb->next) {
        if (sb->ox != -1) {
            sb->ox += left - old_left;
            sb->oy += top - old_top;
        }
    }

    if (old_left == left && old_top == top)
        return;

    wnd->left = left;
    wnd->top = top;
    Setviewport(0, 0, getmaxx() - 1, getmaxy() - 1);

    if (getmaxx() < old_left + wnd->width || getmaxy() < old_top + wnd->height ||
        old_left < 0 || old_top < 0) {
        UC_RedrawWindow(wnd);
    } else {
        UC_ScrollBlock(NULL, old_left, old_top, wnd->width, wnd->height, left, top);
    }

    if (old_left + wnd->width - 1 < left || left + wnd->width - 1 < old_left ||
        old_top + wnd->height - 1 < top || top + wnd->height - 1 < old_top) {
        UC_RedrawBack(old_left, old_top, old_left + wnd->width - 1, old_top + wnd->height - 1);
        return;
    }

    if (left > old_left) {
        UC_RedrawBack(old_left, old_top, left - 1, old_top + wnd->height - 1);
        r_left = left;
        r_right = old_left + wnd->width - 1;
        if (top == old_top)
            return;
        if (top > old_top) {
            UC_RedrawBack(left, old_top, r_right, top - 1);
            return;
        }
    } else {
        r_left = left + wnd->width;
        if (left != old_left)
            UC_RedrawBack(r_left, old_top, old_left + wnd->width - 1, old_top + wnd->height - 1);
        r_right = r_left - 1;
        r_left = old_left;
        if (top == old_top)
            return;
        if (top > old_top) {
            UC_RedrawBack(r_left, old_top, r_right, top - 1);
            return;
        }
    }
    UC_RedrawBack(r_left, top + wnd->height, r_right, old_top + wnd->height - 1);
}
