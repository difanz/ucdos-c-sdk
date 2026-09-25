// WINTEXT.C - 窗口文本输出与光标管理

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
void far UC_WindowsCentral(void);
int far UC_SecondViewport(int left, int top, int right, int bottom);
int far UC_WindowPrintf(WINDOWS *wnd, int x, int y, char mode, char *fmt, ...);
int far UC_TextLines(char *text, RECT *rect, WORD format);
void far UC_GetLineText(char *text, int line, char *dtext, RECT *rect, WORD format);
int far UC_GetLineSpace(void);
void far UC_SetLineSpace(int linespace);
int far UC_GetTextAppositeWidth(RECT *rect, WORD format, char *fmt, ...);
int far UC_DrawText(WINDOWS *wnd, RECT *rect, WORD format, char *fmt, ...);
void far UC_TextOut(WINDOWS *wnd, int x, int y, char mode, char *text);
void far UC_WinOutTextXY(WINDOWS *wnd, int x, int y, char mode, char *text);
void far UC_WindowBar(WINDOWS *wnd, int left, int top, int width, int height);
void far UC_DisplayInputText(WINDOWS *wnd, INPUTLINE *inp, int focus);
void far UC_DoScrollbarButton(WINDOWS *wnd, SCROLLBAR *sbar, int step_dir, int left, int top, int right, int bottom);
int far UC_DoPressButton(WINDOWS *wnd, BUTTON *but, int left, int top, int right, int bottom);
void far UC_BoardDisplay(int left, int top, int right, int bottom);
void far UC_BoardResizeLeftTop(void);
void far UC_BoardResizeRightTop(void);
void far UC_BoardResizeRightBottom(void);
void far UC_BoardResizeLeftBottom(void);
void far UC_BoardResizeUp(void);
void far UC_BoardResizeDown(void);
void far UC_BoardResizeLeft(void);
void far UC_BoardResizeRight(void);
void far UC_Cadi(WINDOWS *wnd, int x, int y);
int far UC_BoardMoveMouse(void);
void far UC_BoardMoveHand(void);
void far UC_CaretBar(WINDOWS *wnd);
void far UC_HideCaret(WINDOWS *wnd);
void far UC_ShowCaret(WINDOWS *wnd);
void far UC_MoveCaret(WINDOWS *wnd, int x, int y);
void far UC_FlashCaret(void);
void far UC_CreateCaret(WINDOWS *wnd, int width, int height);
void far UC_DestroyCaret(WINDOWS *wnd);
int far UC_DestroyObject(WINDOWS *wnd, WORD cure_type, WORD cure_num);

extern void (far *TIMERFUN)(void);

/* 外部引用 */


void far UC_WindowsCentral(void)
{
    WINDOWS *wnd;
    int mouse_event;
    int si, di;
    int key;

    if (UC_CheckTimer() && !BOXPOP) {
        if (TIMERFUN)
            (*TIMERFUN)();
    }

    wnd = wintable_tail;
    if (!UC_WindowVerify(wnd)) {
        UC_Exit("当前窗口无效, 程序中止!");
    }

    if (wnd->hide) {
        UC_WindowEnable(wnd);
    }

    mouse_event = UC_MouseCheck();

    if (wnd->minmax == 1) {
        if (!mouse_event)
            goto CHECK_KEYBOARD;
        else
            goto CHECK_MOUSE_CLICK;
    }

    /* 鼠标形态判断 */
    si = MOUSE_AGI.x;
    di = MOUSE_AGI.y;

    if (!BOXPOP) {
        USER_MOUSE *ms;
        for (ms = wnd->mouse; ms; ms = ms->next) {
            if (UC_InMouseEvenRect(ms, si, di)) {
                UC_MouseSetShape(ms->usermap);
                goto CHECK_MOUSE_EVENT;
            }
        }

        if ((wnd->mode & 6) == 0 && wnd->mode != 0 && wnd->minmax != 2 && !(wnd->mode & 8)) {
            /* 窗口边框光标处理 */
            if ((di >= wnd->top && di <= wnd->top + wnd->vx &&
                 si > wnd->left + wnd->syschar_size + wnd->vx + 2 &&
                 si < wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2) ||
                (di >= wnd->top + wnd->height - wnd->vx && di <= wnd->top + wnd->height - 1 &&
                 si > wnd->left + wnd->syschar_size + wnd->vx + 2 &&
                 si < wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2)) {
                UC_MouseSetShape(&up_down_map);
                goto CHECK_MOUSE_EVENT;
            }

            /* 左上/右下角光标 */
            if ((di >= wnd->top && di <= wnd->top + wnd->vx &&
                 si >= wnd->left && si <= wnd->left + wnd->syschar_size + wnd->vx + 2) ||
                (di >= wnd->top && di <= wnd->top + wnd->syschar_size + wnd->vx + 2 &&
                 si >= wnd->left && si <= wnd->left + wnd->vx) ||
                (di >= wnd->top + wnd->height - 1 - wnd->vx - wnd->syschar_size - 2 &&
                 di <= wnd->top + wnd->height - 1 &&
                 si >= wnd->left + wnd->width - 1 - wnd->vx &&
                 si <= wnd->left + wnd->width - 1) ||
                (di >= wnd->top + wnd->height - 1 - wnd->vx &&
                 di <= wnd->top + wnd->height - 1 &&
                 si >= wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2 &&
                 si <= wnd->left + wnd->width - 1)) {
                UC_MouseSetShape(&left_up_map);
                goto CHECK_MOUSE_EVENT;
            }

            /* 右上/左下角光标 */
            if ((di >= wnd->top && di <= wnd->top + wnd->vx &&
                 si >= wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2 &&
                 si <= wnd->left + wnd->width - 1) ||
                (di >= wnd->top && di <= wnd->top + wnd->syschar_size + wnd->vx + 2 &&
                 si > wnd->left + wnd->width - 1 - wnd->vx &&
                 si <= wnd->left + wnd->width - 1) ||
                (di >= wnd->top + wnd->height - 1 - wnd->vx - wnd->syschar_size - 2 &&
                 di <= wnd->top + wnd->height - 1 &&
                 si >= wnd->left && si <= wnd->left + wnd->vx) ||
                (di >= wnd->top + wnd->height - 1 - wnd->vx &&
                 di <= wnd->top + wnd->height - 1 &&
                 si >= wnd->left && si <= wnd->left + wnd->syschar_size + wnd->vx + 2)) {
                UC_MouseSetShape(&left_down_map);
                goto CHECK_MOUSE_EVENT;
            }

            /* 左右边框光标 */
            if ((di >= wnd->top && di <= wnd->top + wnd->height - 1 &&
                 si >= wnd->left && si <= wnd->left + wnd->vx) ||
                (di >= wnd->top && di <= wnd->top + wnd->height - 1 &&
                 si >= wnd->left + wnd->width - 1 - wnd->vx &&
                 si <= wnd->left + wnd->width - 1)) {
                UC_MouseSetShape(&left_right_map);
                goto CHECK_MOUSE_EVENT;
            }
        }
        UC_MouseSetShape(&move_map);
    }

CHECK_MOUSE_EVENT:
    if (!mouse_event)
        goto CHECK_KEYBOARD;

CHECK_MOUSE_CLICK:
    if ((3 - HAND_LR) & MOUSE_AGI.s) {
        if (!BOXPOP)
            goto CHECK_LEFT_CLICK;
        goto CLOSE_POPBOX;
    }

    if (!(HAND_LR & MOUSE_AGI.s))
        goto CHECK_MOUSE_MOVE_TITLE;

CHECK_LEFT_CLICK:
    UC_HideCaret(wnd);
    si = MOUSE_AGI.x;
    di = MOUSE_AGI.y;

    if (BOXPOP) {
        if (UC_CheckInputBoxPop(wnd, INPOPINP))
            goto CHECK_INPUTLINE_POP;
CLOSE_POPBOX:
        UC_CheckPopBox(wnd, INPOPINP);
        UC_WaitFreeMouse(NULL, 0, 0, 0, 0, NULL);
        return;
    }

    if (BOXPOP)
        goto IN_WINDOW_CLICK;

    if (si < wnd->left || si > wnd->left + wnd->width - 1 ||
        di < wnd->top || di > wnd->top + wnd->height - 1) {
        if (!UC_AllMainOnBack()) {
            if (!(wnd->mode & 1) && wnd->mode != 0) {
                sound(100);
                delay(50);
                nosound();
                goto CLOSE_POPBOX;
            }
        } else {
            WINDOWS *w;
            for (w = wnd->prev; w; w = w->prev) {
                if (si >= w->left && si <= w->left + w->width - 1 &&
                    di >= w->top && di <= w->top + w->height - 1) {
                    if ((w->mode & 1) || w->mode == 0) {
                        UC_WindowEnable(w);
                        UC_MouseBack();
                    }
                    return;
                }
            }
            return;
        }
    }

IN_WINDOW_CLICK:
    if (wnd->minmax == 1) {
        int icon_hit = check_iconwin(wnd);
        if (!icon_hit)
            return;
        if (UC_BoardMoveMouse())
            return;
        goto RESTORE_WINDOW;
    }

    if ((wnd->mode & 6) != 0 || wnd->mode == 0)
        goto CHECK_TITLE_CONTROLS;

    /* 检查边框拖动调整 */
    if (wnd->minmax != 2 && !(wnd->mode & 8)) {
        /* 左上角 */
        if ((di >= wnd->top && di <= wnd->top + wnd->vx &&
             si >= wnd->left && si <= wnd->left + wnd->syschar_size + wnd->vx + 2) ||
            (di >= wnd->top && di <= wnd->top + wnd->syschar_size + wnd->vx + 2 &&
             si >= wnd->left && si <= wnd->left + wnd->vx)) {
            wnd->minmax = 0;
            UC_BoardResizeLeftTop();
            return;
        }

        /* 右上角 */
        if ((di >= wnd->top && di <= wnd->top + wnd->vx &&
             si >= wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2 &&
             si <= wnd->left + wnd->width - 1) ||
            (di >= wnd->top && di <= wnd->top + wnd->syschar_size + wnd->vx + 2 &&
             si >= wnd->left + wnd->width - 1 - wnd->vx &&
             si <= wnd->left + wnd->width - 1)) {
            wnd->minmax = 0;
            UC_BoardResizeRightTop();
            return;
        }

        /* 右下角 */
        if ((di >= wnd->top + wnd->height - 1 - wnd->vx - wnd->syschar_size - 2 &&
             di <= wnd->top + wnd->height - 1 &&
             si >= wnd->left + wnd->width - 1 - wnd->vx &&
             si <= wnd->left + wnd->width - 1) ||
            (di >= wnd->top + wnd->height - 1 - wnd->vx &&
             di <= wnd->top + wnd->height - 1 &&
             si >= wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2 &&
             si <= wnd->left + wnd->width - 1)) {
            wnd->minmax = 0;
            UC_BoardResizeRightBottom();
            return;
        }

        /* 左下角 */
        if ((di >= wnd->top + wnd->height - 1 - wnd->vx - wnd->syschar_size - 2 &&
             di <= wnd->top + wnd->height - 1 &&
             si >= wnd->left && si <= wnd->left + wnd->vx) ||
            (di >= wnd->top + wnd->height - 1 - wnd->vx &&
             di <= wnd->top + wnd->height - 1 &&
             si >= wnd->left && si <= wnd->left + wnd->syschar_size + wnd->vx + 2)) {
            wnd->minmax = 0;
            UC_BoardResizeLeftBottom();
            return;
        }

        /* 上边框 */
        if (di >= wnd->top && di <= wnd->top + wnd->vx &&
            si > wnd->left + wnd->syschar_size + wnd->vx + 2 &&
            si < wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2) {
            wnd->minmax = 0;
            UC_BoardResizeUp();
            return;
        }

        /* 下边框 */
        if (di >= wnd->top + wnd->height - wnd->vx &&
            di <= wnd->top + wnd->height - 1 &&
            si > wnd->left + wnd->syschar_size + wnd->vx + 2 &&
            si < wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2) {
            wnd->minmax = 0;
            UC_BoardResizeDown();
            return;
        }

        /* 左边框 */
        if (di >= wnd->top && di <= wnd->top + wnd->height - 1 &&
            si >= wnd->left && si <= wnd->left + wnd->vx) {
            wnd->minmax = 0;
            UC_BoardResizeLeft();
            return;
        }

        /* 右边框 */
        if (di >= wnd->top && di <= wnd->top + wnd->height - 1 &&
            si >= wnd->left + wnd->width - 1 - wnd->vx &&
            si <= wnd->left + wnd->width - 1) {
            wnd->minmax = 0;
            UC_BoardResizeRight();
            return;
        }
    }

    /* 标题栏按钮: 最大化/最小化/关闭 */
    if (di >= wnd->top + wnd->vx && di <= wnd->top + wnd->vx + wnd->syschar_size) {
        /* 最大化与还原按钮 */
        if (si >= wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2 &&
            si <= wnd->left + wnd->width - 1 - wnd->vx) {
            if (UC_DoPressButton(NULL, NULL,
                                 wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2,
                                 wnd->top + wnd->vx,
                                 wnd->left + wnd->width - wnd->vx,
                                 wnd->top + wnd->vx + wnd->syschar_size + 2)) {
                if (wnd->mode & 8)
                    goto DO_MINIMIZE;
                goto RESTORE_WINDOW;
            }
        }
    }

    /* 最小化按钮 */
    if (!(wnd->mode & 8)) {
        if (di >= wnd->top + wnd->vx && di <= wnd->top + wnd->vx + wnd->syschar_size) {
            if (si >= wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size * 2 - 4 &&
                si <= wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size) {
                if (UC_DoPressButton(NULL, NULL,
                                     wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size * 2 - 4,
                                     wnd->top + wnd->vx,
                                     wnd->left + wnd->width - 1 - wnd->vx - wnd->syschar_size - 2,
                                     wnd->top + wnd->vx + wnd->syschar_size + 2)) {
DO_MINIMIZE:
                    UC_MinWindow(wnd);
                    if (wnd->prev) {
                        UC_WindowEnable(wnd->prev);
                    }
                    goto ACTIVATE_NEXT;
                }
            }
        }
    }

CHECK_TITLE_CONTROLS:
    /* 关闭窗口按钮 */
    if (wnd->mode != 0 && wnd->title && strlen(wnd->title) > 0) {
        if (di >= wnd->top + wnd->vx && di <= wnd->top + wnd->vx + wnd->syschar_size &&
            si >= wnd->left + wnd->vx && si <= wnd->left + wnd->vx + wnd->syschar_size) {
            int ret;
            UC_DrawQuitButton(wnd, 15, 0);
            ret = UC_WaitFreeMouse(NULL,
                                   wnd->left + wnd->vx,
                                   wnd->top + wnd->vx,
                                   wnd->left + wnd->vx + wnd->syschar_size,
                                   wnd->top + wnd->vx + wnd->syschar_size,
                                   NULL);
            UC_DrawQuitButton(wnd, 0, 15);
            if (ret) {
                if (!in_JJ && UC_GetKeyboardState() == 5) {
                    UC_SecretWindow();
                    return;
                }
                if (wnd->CLOSEWIN) {
                    wnd->CLOSEWIN();
                    return;
                }
            }
            return;
        }
    }

    /* 菜单控件 */
    if (wnd->menu) {
        MENUS *mnu = UC_CheckMenu(wnd);
        if (mnu) {
            wnd->curemenu = mnu;
            UC_DoMenuSelect(wnd);
            goto ACTIVATE_NEXT;
        }
    }

    /* 复选框控件 */
    {
        CHECK *chk = UC_CheckCheck(wnd);
        if (chk) {
            if (chk->fun)
                chk->fun(chk->enable);
            return;
        }
    }

    /* 单选框控件 */
    {
        RADIO *rad = UC_CheckRadio(wnd);
        if (rad) {
            if (rad->fun)
                rad->fun(rad->enable);
            return;
        }
    }

    /* 命令按钮控件 */
    {
        BUTTON *but = UC_CheckButton(wnd);
        if (but) {
            if (but->fun)
                but->fun();
            return;
        }
    }

CHECK_INPUTLINE_POP:
    /* 文本输入框控件 */
    {
        INPUTLINE *inp = UC_CheckInputLine(wnd);
        if (inp) {
            if (inp->length == 0)
                return;
        }
    }

    /* 滚动条控件 */
    if (UC_CheckScrollbar(wnd))
        return;

    /* 列表框控件 */
    {
        int lbox_ret = (int)UC_CheckListBox(wnd);
        if (lbox_ret == 2) {
            key = 0x1c0d; /* 回车键 */
            goto DISPATCH_KEY_EVENT;
        }
        if (lbox_ret != 0)
            return;
    }

    /* 拖动标题栏移动窗口 */
    if (!BOXPOP && wnd->title && strlen(wnd->title) > 0 && wnd->mode != 0) {
        if (di >= wnd->top && di <= wnd->top + wnd->vx + wnd->syschar_size &&
            si >= wnd->left && si <= wnd->left + wnd->width) {
            if (UC_BoardMoveMouse())
                return;
            if (wnd->mode & 8)
                return;
            goto RESTORE_WINDOW;
        }
    }

CHECK_MOUSE_MOVE_TITLE:
    /* 用户鼠标事件 */
    if (wnd->minmax != 1) {
        USER_MOUSE *ms;
        for (ms = wnd->mouse; ms; ms = ms->next) {
            if (UC_InMouseEvenRect(ms, MOUSE_AGI.x, MOUSE_AGI.y)) {
                WORD event_state = (ms->state >> 1) & 3;
                if (HAND_LR == 2 && event_state != 3)
                    event_state = 3 - event_state;
                if (ms->state & 0x8000)
                    event_state |= 0x8000;
                if ((ms->state & 1) || (MOUSE_AGI.s & event_state)) {
                    if (ms->fun) {
                        event_state = MOUSE_AGI.s & 3;
                        if (HAND_LR == 2 && event_state != 3)
                            event_state = 3 - event_state;
                        event_state <<= 1;
                        if (MOUSE_AGI.s & 0x8000)
                            event_state |= 0x8000;
                        if (event_state == 0)
                            event_state = 1;
                        ms->fun(MOUSE_AGI.x, MOUSE_AGI.y, event_state);
                        return;
                    }
                }
            }
        }
    }

CHECK_KEYBOARD:
    USER_MAY = 1;
    USER_CHANGE = 0;
    key = UC_CheckKeyboard();
    USER_MAY = 0;
    if (USER_CHANGE)
        return;
    if (key == 0)
        return;

DISPATCH_KEY_EVENT:
    UC_HideCaret(wnd);
    di = wnd->cure_type;
    si = wnd->cure_num;

    if (!BOXPOP && wnd->minmax != 1 && wnd->menu) {
        MENUS *mnu = UC_CheckMenuHotKey(wnd, key);
        if (mnu) {
            wnd->curemenu = mnu;
            UC_DisplayMenu(wnd);
            UC_OpenPopMenu(wnd);
            mnu = UC_GetPopMenuSelect(wnd);
            UC_ClosePopMenu(wnd);
            wnd->curemenu = NULL;
            UC_DisplayMenu(wnd);
            if (mnu) {
                if (mnu->menu_function && mnu->menu_function[mnu->cure_sele])
                    mnu->menu_function[mnu->cure_sele]();
            }
            return;
        }
    }

    /* 系统热键处理 */
    switch (key) {
    case 0x9400: /* Ctrl+Tab */
        if (wnd->mode & 1) {
            if (UC_AllMainOnBack()) {
                WINDOWS *w;
                for (w = wnd->prev; w; w = w->prev) {
                    if ((w->mode & 1) || w->mode == 0) {
                        UC_WindowEnable(w);
                        break;
                    }
                }
            }
        }
        break;

    case 0x011b: /* Esc */
        if (wnd->minmax != 1) {
            if (wnd->mode & 1)
                goto DISPATCH_COMPONENTS_KEY;
            if (wnd->KB_Entry)
                wnd->KB_Entry(key, KEYB_STATE);
            return;
        }
        return;

    case 0x0f00: /* Shift+Tab */
        /* 上一控件 */
        {
            int prev_found = 0;
            switch (di) {
            case TYPE_BUTTON:
                if (si > 1 && UC_GetButton(wnd, si - 1)) {
                    si--;
                    prev_found = 1;
                } else {
                    di = TYPE_LISTBOX;
                    si = UC_GetLastNumber(wnd, TYPE_LISTBOX);
                }
                break;
            case TYPE_LISTBOX:
                if (si > 1 && UC_GetListBox(wnd, si - 1)) {
                    si--;
                    prev_found = 1;
                } else {
                    di = TYPE_INPUTLINE;
                    si = UC_GetLastNumber(wnd, TYPE_INPUTLINE);
                }
                break;
            case TYPE_INPUTLINE:
                if (si > 1 && UC_GetInputBox(wnd, si - 1)) {
                    si--;
                    prev_found = 1;
                } else {
                    di = TYPE_RADIO;
                    si = UC_GetLastNumber(wnd, TYPE_RADIO);
                }
                break;
            case TYPE_RADIO:
                if (si > 1 && UC_GetRadioButton(wnd, si - 1)) {
                    si--;
                    prev_found = 1;
                } else {
                    di = TYPE_CHECK;
                    si = UC_GetLastNumber(wnd, TYPE_CHECK);
                }
                break;
            case TYPE_CHECK:
                if (si > 1 && UC_GetCheckButton(wnd, si - 1)) {
                    si--;
                    prev_found = 1;
                } else {
                    di = TYPE_BUTTON;
                    si = UC_GetLastNumber(wnd, TYPE_BUTTON);
                }
                break;
            }
            if (prev_found) {
                UC_UpdateActive(wnd, 1);
                wnd->cure_type = di;
                wnd->cure_num = si;
                UC_UpdateActive(wnd, 0);
                return;
            }
        }
        break;

    case 0x0f09: /* Tab */
        /* 下一控件 */
        {
            int next_found = 0;
            switch (di) {
            case TYPE_BUTTON:
                if (UC_GetButton(wnd, si + 1)) {
                    si++;
                    next_found = 1;
                } else {
                    di = TYPE_INPUTLINE;
                    si = 1;
                    if (UC_GetInputBox(wnd, si)) next_found = 1;
                }
                break;
            case TYPE_INPUTLINE:
                if (UC_GetInputBox(wnd, si + 1)) {
                    si++;
                    next_found = 1;
                } else {
                    di = TYPE_LISTBOX;
                    si = 1;
                    if (UC_GetListBox(wnd, si)) next_found = 1;
                }
                break;
            case TYPE_LISTBOX:
                if (UC_GetListBox(wnd, si + 1)) {
                    si++;
                    next_found = 1;
                } else {
                    di = TYPE_RADIO;
                    si = 1;
                    if (UC_GetRadioButton(wnd, si)) next_found = 1;
                }
                break;
            case TYPE_RADIO:
                if (UC_GetRadioButton(wnd, si + 1)) {
                    si++;
                    next_found = 1;
                } else {
                    di = TYPE_CHECK;
                    si = 1;
                    if (UC_GetCheckButton(wnd, si)) next_found = 1;
                }
                break;
            case TYPE_CHECK:
                if (UC_GetCheckButton(wnd, si + 1)) {
                    si++;
                    next_found = 1;
                } else {
                    di = TYPE_BUTTON;
                    si = 1;
                    if (UC_GetButton(wnd, si)) next_found = 1;
                }
                break;
            }
            if (next_found) {
                UC_UpdateActive(wnd, 1);
                wnd->cure_type = di;
                wnd->cure_num = si;
                UC_UpdateActive(wnd, 0);
                return;
            }
        }
        break;

    case 0x1c0d: /* Enter */
        if (di == TYPE_BUTTON) {
            BUTTON *but = UC_GetButton(wnd, si);
            if (but && !but->disable) {
                AUTOPRESS = 1;
                UC_DoPressButton(wnd, but, but->left, but->top, but->left + but->width, but->top + but->height);
                if (but->fun)
                    but->fun();
                return;
            }
        }
        break;

    case 0x3800: /* Alt */
    case 0x4400: /* F10 */
        if (wnd->minmax != 1 && wnd->menu) {
            wnd->curemenu = wnd->menu;
            UC_DisplayMenu(wnd);
            UC_DoMenuSelect(wnd);
            goto ACTIVATE_NEXT;
        }
        break;

    case 0x3920: /* Space */
        if (di == TYPE_BUTTON) {
            BUTTON *but = UC_GetButton(wnd, si);
            if (but && !but->disable) {
                AUTOPRESS = 1;
                UC_DoPressButton(wnd, but, but->left, but->top, but->left + but->width, but->top + but->height);
                if (but->fun)
                    but->fun();
                return;
            }
        }
        break;

    case 0x6100: /* Ctrl+F7? */
    case 0x6b00: /* Alt+F4? */
        if (wnd->minmax != 1 && wnd->KB_Entry) {
            wnd->KB_Entry(key, KEYB_STATE);
            return;
        }
        break;

    case 0x6200: /* 键盘移动窗口 */
        if (wnd->minmax != 1) {
            UC_BoardMoveHand();
            return;
        }
        break;

    default:
        break;
    }

DISPATCH_COMPONENTS_KEY:
    if (wnd->minmax == 1)
        return;

    /* 用户自定义按键处理 */
    {
        LABEL *lab;
        for (lab = wnd->label; lab; lab = lab->next) {
            if (lab->hotkey == key) {
                if (lab->fun)
                    lab->fun();
                return;
            }
        }
    }

    /* 按钮热键 */
    {
        BUTTON *but = UC_CheckButtonKey(wnd, key);
        if (but) {
            if (but->fun)
                but->fun();
            return;
        }
    }

    /* 单选框热键 */
    {
        RADIO *rad;
        int num = 1;
        for (rad = wnd->radio; rad; rad = rad->next, num++) {
            if (rad->key == key) {
                UC_UpdateActive(wnd, 1);
                wnd->cure_type = TYPE_RADIO;
                wnd->cure_num = num;
                UC_UpdateActive(wnd, 0);
                break;
            }
        }
    }

    /* 复选框热键 */
    {
        CHECK *chk;
        int num = 1;
        for (chk = wnd->check; chk; chk = chk->next, num++) {
            if (chk->key == key) {
                if (!chk->disable) {
                    UC_UpdateActive(wnd, 1);
                    wnd->cure_type = TYPE_CHECK;
                    wnd->cure_num = num;
                    UC_UpdateActive(wnd, 0);
                    UC_RunCheck(wnd, chk);
                }
                return;
            }
        }
    }

    /* 活动组件键盘处理 */
    switch (wnd->cure_type) {
    case TYPE_INPUTLINE:
        UC_DoInputLine(wnd, key);
        break;
    case TYPE_LISTBOX:
        UC_DoListBox(wnd, key);
        break;
    case TYPE_RADIO:
        UC_DoRadioButton(wnd, key);
        break;
    case TYPE_CHECK:
        UC_DoCheckButton(wnd, key);
        break;
    }
    return;

RESTORE_WINDOW:
    UC_MaxWindow(wnd);
ACTIVATE_NEXT:
    if (wnd->next)
        UC_WindowEnable(wnd->next);
    return;
}

int far UC_SecondViewport(int left, int top, int right, int bottom)
{
    struct viewporttype vp;
    getviewsettings(&vp);
    if (left > vp.right || top > vp.bottom || right < vp.left || bottom < vp.top)
        return 0;
    if (left < vp.left) left = vp.left;
    if (right > vp.right) right = vp.right;
    if (top < vp.top) top = vp.top;
    if (bottom > vp.bottom) bottom = vp.bottom;
    Setviewport(left, top, right, bottom);
    return 1;
}

int far UC_WindowPrintf(WINDOWS *wnd, int x, int y, char mode, char *fmt, ...)
{
    va_list args;
    int len;
    int di = 0, si = 0;
    char linebuf[1024];

    UC_GetRealXY(&x);
    UC_GetRealXY(&y);

    va_start(args, fmt);
    len = vsprintf(PUB_text, fmt, args);
    va_end(args);

    linebuf[0] = 0;
    for (si = 0; si < (int)strlen(PUB_text); si++) {
        char c = PUB_text[si];
        if (c == 0)
            break;
        if (c == 0x0a || c == 0x0d) {
            linebuf[di] = 0;
            NOREAL = 1;
            UC_TextOut(wnd, x, y, mode, linebuf);
            y += textheight(linebuf) + LINESPACE;
            di = 0;
            linebuf[0] = 0;
        } else if ((unsigned char)c >= 0x20) {
            linebuf[di++] = c;
        }
    }
    if (di > 0) {
        linebuf[di] = 0;
        NOREAL = 1;
        UC_TextOut(wnd, x, y, mode, linebuf);
    }
    return len;
}

int far UC_TextLines(char *text, RECT *rect, WORD format)
{
    int lines = 0;
    int di = 0, si = 0;
    char buf[1024];

    while (1) {
        char c = text[di];
        if (c == 0) {
            lines++;
            break;
        }
        if (c == 0x0d || c == 0x0a) {
            lines++;
            si = 0;
            di++;
            continue;
        }
        if (format & 0x20) {
            buf[si++] = c;
            buf[si] = 0;
            if (si >= 2) {
                if (textwidth(buf) > rect->right - rect->left + 1) {
                    if (si == 2 && (unsigned char)buf[0] > 0xa0 && (unsigned char)buf[1] > 0xa0) {
                        /* 保持双字节汉字完整 */
                    } else {
                        int cx, k;
                        unsigned char dl = 0;
                        lines++;
                        si--;
                        cx = si;
                        while (si > 0) {
                            unsigned char ch = (unsigned char)buf[si];
                            if (ch <= 0x2f ||
                                (ch >= 0x3a && ch <= 0x3f) ||
                                (ch >= 0x5b && ch <= 0x60) ||
                                (ch >= 0x7b && ch <= 0x7e))
                                break;
                            si--;
                        }
                        if (si == 0) {
                            for (k = 0; k < cx; k++) {
                                if ((unsigned char)buf[k] > 0xa0)
                                    dl = 1 - dl;
                                else
                                    dl = 0;
                            }
                            si = cx - 1;
                            if (dl != 0)
                                si--;
                        } else {
                            if (buf[si] == '(' || buf[si] == '[' || buf[si] == '{')
                                si--;
                        }
                        di -= (cx - si);
                        si = 0;
                        di++;
                        continue;
                    }
                }
            }
        }
        di++;
    }
    return lines;
}

void far UC_GetLineText(char *text, int line, char *dtext, RECT *rect, WORD format)
{
    int cur_line = 0;
    int di = 0, si = 0;
    char buf[1024];

    if (!text || strlen(text) == 0) {
        dtext[0] = 0;
        return;
    }
    if (UC_TextLines(text, rect, format) <= line) {
        dtext[0] = 0;
        return;
    }

    while (cur_line < line) {
        char c = text[di];
        if (c == 0) {
            di--;
            break;
        }
        if (c == 0x0d || c == 0x0a) {
            cur_line++;
            si = 0;
            di++;
            continue;
        }
        if (format & 0x20) {
            buf[si++] = c;
            buf[si] = 0;
            if (si >= 2) {
                if (textwidth(buf) > rect->right - rect->left + 1) {
                    if (si == 2 && (unsigned char)buf[0] > 0xa0 && (unsigned char)buf[1] > 0xa0) {
                        /* 保持双字节汉字完整 */
                    } else {
                        int cx, k;
                        unsigned char dl = 0;
                        cur_line++;
                        si--;
                        cx = si;
                        while (si > 0) {
                            unsigned char ch = (unsigned char)buf[si];
                            if (ch <= 0x2f ||
                                (ch >= 0x3a && ch <= 0x3f) ||
                                (ch >= 0x5b && ch <= 0x60) ||
                                (ch >= 0x7b && ch <= 0x7e))
                                break;
                            si--;
                        }
                        if (si == 0) {
                            for (k = 0; k < cx; k++) {
                                if ((unsigned char)buf[k] > 0xa0)
                                    dl = 1 - dl;
                                else
                                    dl = 0;
                            }
                            si = cx - 1;
                            if (dl != 0)
                                si--;
                        } else {
                            if (buf[si] == '(' || buf[si] == '[' || buf[si] == '{')
                                si--;
                        }
                        di -= (cx - si);
                        si = 0;
                        di++;
                        continue;
                    }
                }
            }
        }
        di++;
    }

    si = 0;
    while (1) {
        char c = text[di];
        if (c == 0 || c == 0x0d || c == 0x0a)
            break;
        dtext[si] = c;
        dtext[si + 1] = 0;
        if (format & 0x20) {
            if (si >= 1) {
                if (textwidth(dtext) > rect->right - rect->left + 1) {
                    if (si == 1 && (unsigned char)dtext[0] > 0xa0 && (unsigned char)dtext[1] > 0xa0) {
                        /* 保持双字节汉字完整 */
                    } else {
                        int cx, k;
                        unsigned char dl = 0;
                        si--;
                        cx = si;
                        while (si > 0) {
                            unsigned char ch = (unsigned char)dtext[si];
                            if (ch <= 0x2f ||
                                (ch >= 0x3a && ch <= 0x3f) ||
                                (ch >= 0x5b && ch <= 0x60) ||
                                (ch >= 0x7b && ch <= 0x7e))
                                break;
                            if (dtext[si] == '(' || dtext[si] == '[' || dtext[si] == '{') {
                                si--;
                                break;
                            }
                            si--;
                        }
                        if (si == 0) {
                            si = cx + 1;
                        } else {
                            si++;
                        }
                        for (k = 0; k < si; k++) {
                            if ((unsigned char)dtext[k] > 0xa0)
                                dl = 1 - dl;
                            else
                                dl = 0;
                        }
                        if (dl != 0 && si > 1)
                            si--;
                        break;
                    }
                }
            }
        }
        si++;
        di++;
    }
    dtext[si] = 0;
}

int far UC_GetLineSpace(void)
{
    return LINESPACE;
}

void far UC_SetLineSpace(int linespace)
{
    LINESPACE = linespace;
}

int far UC_GetTextAppositeWidth(RECT *rect, WORD format, char *fmt, ...)
{
    va_list args;
    int max_w = 0, num_lines, i;
    char linebuf[1024];

    if (!fmt)
        return 0;

    va_start(args, fmt);
    vsprintf(PUB_text, fmt, args);
    va_end(args);

    if (strlen(PUB_text) == 0)
        return 0;

    num_lines = UC_TextLines(PUB_text, rect, format);
    for (i = 0; i < num_lines; i++) {
        int w;
        UC_GetLineText(PUB_text, i, linebuf, rect, format);
        w = textwidth(linebuf);
        if (w > max_w)
            max_w = w;
    }
    return max_w;
}

int far UC_DrawText(WINDOWS *wnd, RECT *rect, WORD format, char *fmt, ...)
{
    va_list args;
    struct viewporttype vp;
    int x1, y1, x2, y2;
    int total_h = 0, num_lines, i;
    int cur_y, line_x;
    char linebuf[1024];

    if (!fmt)
        return 0;

    va_start(args, fmt);
    vsprintf(PUB_text, fmt, args);
    va_end(args);

    if (strlen(PUB_text) == 0)
        return 0;

    if (rect->right < rect->left) {
        int t = rect->left; rect->left = rect->right; rect->right = t;
    }
    if (rect->top > rect->bottom) {
        int t = rect->top; rect->top = rect->bottom; rect->bottom = t;
    }

    x1 = rect->left;
    y1 = rect->top;
    x2 = rect->right;
    y2 = rect->bottom;

    if (UC_WindowVerify(wnd)) {
        x1 += wnd->left + wnd->vx;
        y1 += wnd->top + wnd->vy;
        x2 += wnd->left + wnd->vx;
        y2 += wnd->top + wnd->vy;
        if (wnd->left + wnd->width - 1 - wnd->vr < x1)
            return 0;
        if (wnd->top + wnd->height - 1 - wnd->vb < y1)
            return 0;
        if (wnd->left + wnd->width - 1 - wnd->vr < x2)
            x2 = wnd->left + wnd->width - 1 - wnd->vr;
        if (wnd->top + wnd->height - 1 - wnd->vb < y2)
            y2 = wnd->top + wnd->height - 1 - wnd->vb;
    }

    getviewsettings(&vp);
    if (!UC_SecondViewport(x1, y1, x2, y2))
        return total_h;

    cur_y = y1;
    num_lines = UC_TextLines(PUB_text, rect, format);
    total_h = num_lines * textheight(PUB_text) + (num_lines - 1) * LINESPACE;

    if (format & 0x04) {
        /* 底部对齐 */
        cur_y += (rect->bottom - rect->top + 1 - total_h);
    } else if (format & 0x08) {
        /* 垂直居中 */
        cur_y += (rect->bottom - rect->top + 1 - total_h) / 2;
    }
    if (cur_y < y1)
        cur_y = y1;

    UC_MouseHide();
    for (i = 0; i < num_lines; i++) {
        UC_GetLineText(PUB_text, i, linebuf, rect, format);
        line_x = x1;
        if (format & 0x01) {
            /* 靠右对齐 */
            line_x += (rect->right - rect->left + 1 - textwidth(linebuf));
        } else if (format & 0x02) {
            /* 居中对齐 */
            line_x += (rect->right - rect->left + 1 - textwidth(linebuf)) / 2;
        }
        if (line_x < x1)
            line_x = x1;

        if (format & 0x10)
            outtextbm(line_x, cur_y, linebuf);
        else
            outtextxy(line_x, cur_y, linebuf);

        cur_y += textheight(linebuf) + LINESPACE;
    }
    Setviewport(vp.left, vp.top, vp.right, vp.bottom);
    UC_MouseShow();
    return total_h;
}

void far UC_TextOut(WINDOWS *wnd, int x, int y, char mode, char *text)
{
    struct viewporttype vp;
    int x1, y1, x2, y2;

    if (NOREAL) {
        UC_WinOutTextXY(wnd, x, y, mode, text);
        NOREAL = 0;
        return;
    }

    if (x == -1) {
        int w;
        if (UC_WindowVerify(wnd))
            w = wnd->width - wnd->vx - wnd->vr;
        else
            w = getmaxx();
        if (w > textwidth(text))
            x = (w - textwidth(text)) / 2;
        else
            x = 0;
    } else {
        UC_GetRealXY(&x);
    }

    if (y == -1) {
        int h;
        if (UC_WindowVerify(wnd))
            h = wnd->height - wnd->vy - wnd->vb;
        else
            h = getmaxy();
        if (h > textheight(text))
            y = (h - textheight(text)) / 2;
        else
            y = 0;
    } else {
        UC_GetRealXY(&y);
    }

    if (UC_WindowVerify(wnd)) {
        x += wnd->left + wnd->vx;
        y += wnd->top + wnd->vy;
        x1 = wnd->left + wnd->vx;
        y1 = wnd->top + wnd->vy;
        x2 = wnd->left + wnd->width - 1 - wnd->vr;
        y2 = wnd->top + wnd->height - 1 - wnd->vb;
    } else {
        x1 = 0;
        y1 = 0;
        x2 = getmaxx() - 1;
        y2 = getmaxy() - 1;
    }

    getviewsettings(&vp);
    if (!UC_SecondViewport(x1, y1, x2, y2))
        return;

    UC_MouseHide();
    if (mode & 0x10)
        outtextbm(x, y, text);
    else
        outtextxy(x, y, text);
    Setviewport(vp.left, vp.top, vp.right, vp.bottom);
    UC_MouseShow();
}

void far UC_WinOutTextXY(WINDOWS *wnd, int x, int y, char mode, char *text)
{
    struct viewporttype vp;
    int x1, y1, x2, y2;

    if (wnd->hide)
        return;

    if (x == -1) {
        int w = wnd->width - wnd->vx;
        if (w > textwidth(text))
            x = (w - textwidth(text)) / 2;
        else
            x = 0;
    }
    if (y == -1) {
        int h = wnd->height - wnd->vx;
        if (h > textheight(text))
            y = (h - textheight(text)) / 2;
        else
            y = 0;
    }

    x += wnd->left + wnd->vx;
    y += wnd->top + wnd->vy;
    x2 = wnd->left + wnd->width - 1;
    y2 = wnd->top + wnd->height - 1;

    getviewsettings(&vp);
    x1 = wnd->left + wnd->vx;
    y1 = wnd->top + wnd->vy;
    x2 -= wnd->vr;
    y2 -= wnd->vb;

    if (!UC_SecondViewport(x1, y1, x2, y2))
        return;

    UC_MouseHide();
    if (mode & 0x10)
        outtextbm(x, y, text);
    else
        outtextxy(x, y, text);
    Setviewport(vp.left, vp.top, vp.right, vp.bottom);
    UC_MouseShow();
}

void far UC_WindowBar(WINDOWS *wnd, int left, int top, int width, int height)
{
    struct viewporttype vp;
    int x1, y1, x2, y2;

    if (width <= 0 || height <= 0)
        return;

    if (!((width > 0) && (height > 0))) {
        UC_Exit("断言失败: %s, 文件 %s, 行号 %d\n", "(width>0) && (height>0)", "AGI_WIN.C", 6802);
    }

    if (!UC_WindowVerify(wnd)) {
        UC_MouseHide();
        bar(left, top, left + width - 1, top + height - 1);
        UC_MouseShow();
        return;
    }

    getviewsettings(&vp);
    x1 = wnd->left + wnd->vx;
    y1 = wnd->top + wnd->vy;
    x2 = wnd->left + wnd->width - 1 - wnd->vr;
    y2 = wnd->top + wnd->height - 1 - wnd->vb;

    if (!UC_SecondViewport(x1, y1, x2, y2))
        return;

    UC_MouseHide();
    bar(wnd->left + wnd->vx + left,
        wnd->top + wnd->vy + top,
        wnd->left + wnd->vx + left + width - 1,
        wnd->top + wnd->vy + top + height - 1);
    Setviewport(vp.left, vp.top, vp.right, vp.bottom);
    UC_MouseShow();
}

void far UC_DisplayInputText(WINDOWS *wnd, INPUTLINE *inp, int focus)
{
    struct viewporttype vp;
    int x1, y1, x2, y2;
    int rx, ry;

    if (!inp)
        return;

    x1 = wnd->left + wnd->vx + inp->left;
    y1 = wnd->top + wnd->vy + inp->top;
    rx = x1 + (int)((long)inp->width * wnd->syschar_size / 2) + 4;
    ry = y1 + wnd->syschar_size + 4;

    if (wnd->left + wnd->width - 1 - wnd->vr < rx)
        rx = wnd->left + wnd->width - 1 - wnd->vr;
    if (wnd->top + wnd->height - 1 - wnd->vx < ry)
        ry = wnd->top + wnd->height - 1 - wnd->vx;

    if (focus) {
        setcolor(0);
        setcolorbm(wnd->title_fgc);
    } else {
        setcolor(wnd->title_fgc);
        setcolorbm(0);
    }
    settextstyle(0, 0, wnd->syschar_size);

    getviewsettings(&vp);
    x1 += 2;
    y1 += 2;
    x2 = rx - 3;
    y2 = ry - 2;

    if (!UC_SecondViewport(x1, y1, x2, y2))
        return;

    UC_MouseHide();
    outtextbm(x1 + wnd->syschar_size / 2, y1, inp->text + inp->curepos);

    if ((int)strlen(inp->text + inp->curepos) < inp->width) {
        setfillstyle(1, wnd->title_fgc);
        bar(x1 + (int)((long)(strlen(inp->text + inp->curepos) + 1) * wnd->syschar_size / 2),
            y1 - 1, rx - 1, ry - 1);
    }

    if (focus) {
        if (inp->curepos > 0)
            outtextbm(x1, y1, CHAR_LEFT);
        else
            outtextbm(x1, y1, CHAR_SPACE);

        if ((int)strlen(inp->text) > inp->curepos + inp->width - 2)
            outtextbm(x1 + (int)((long)(inp->width - 1) * wnd->syschar_size / 2), y1, CHAR_RIGHT);
        else
            outtextbm(x1 + (int)((long)(inp->width - 1) * wnd->syschar_size / 2), y1, CHAR_SPACE);
    }

    Setviewport(vp.left, vp.top, vp.right, vp.bottom);
    UC_MouseShow();
}

void far UC_DoScrollbarButton(WINDOWS *wnd, SCROLLBAR *sbar, int step_dir, int left, int top, int right, int bottom)
{
    void far *buf;
    WORD size;
    int i;
    char changed = 0;
    char first_wait = 1;
    float prev_cure;
    struct time t1, t2;

    size = imagesize(left, top, right, bottom);
    if (size == 0xFFFF)
        size = 256;
    buf = malloc(size);
    if (!buf)
        buf = (void far *)PUBLIC_BUF;

    UC_MouseHide();
    getimage(left + 2, top + 2, right - 2, bottom - 2, buf);
    putimage(left + 3, top + 3, buf, 0);

    setcolor(7);
    for (i = 1; i < 4; i++) {
        line(left + 1, top + i + 1, right - 1, top + i + 1);
        line(left + i + 1, top + 1, left + i + 1, bottom - 1);
    }
    setcolor(8);
    line(left + 1, top + 1, right - 1, top + 1);
    line(left + 1, top + 1, left + 1, bottom - 1);
    UC_MouseShow();

    first_wait = 1;
    while (MOUSE_AGI.s & HAND_LR) {
        prev_cure = sbar->cure;
        if (step_dir) {
            sbar->cure += 1.0f;
            if (sbar->cure > sbar->max - 1.0f) {
                if (sbar->max - 1.0f < 0.0f)
                    sbar->cure = 0.0f;
                else
                    sbar->cure = sbar->max - 1.0f;
                changed = 1;
            }
        } else {
            sbar->cure -= 1.0f;
            if (sbar->cure < 0.0f) {
                sbar->cure = 0.0f;
                changed = 1;
            }
        }
        if (!changed) {
            UC_UpdateScrollbar(wnd, sbar);
            if (sbar->function)
                sbar->function(sbar, prev_cure);
        }
        gettime(&t1);
        while (UC_MouseCheck()) ;
        gettime(&t2);
        if (first_wait) {
            int diff = abs((int)t2.ti_hund - (int)t1.ti_hund);
            if (diff > 40)
                first_wait = 0;
            else if (MOUSE_AGI.s & HAND_LR)
                continue;
        } else {
            int diff = abs((int)t2.ti_hund - (int)t1.ti_hund);
            if (diff <= 3 && (MOUSE_AGI.s & HAND_LR))
                continue;
        }
    }

    UC_MouseHide();
    putimage(left + 2, top + 2, buf, 0);
    killimage(buf);
    UC_DrawButton(1, 1, left, top, right, bottom);
    UC_MouseShow();
}

int far UC_DoPressButton(WINDOWS *wnd, BUTTON *but, int left, int top, int right, int bottom)
{
    void far *buf;
    struct viewporttype vp;
    int x1, y1, x2, y2;
    int shift = 0;
    int ret = AUTOPRESS ? 1 : 0;
    WORD size;

    size = imagesize(left, top, right, bottom);
    if (size == 0xFFFF)
        size = 256;
    buf = malloc(size);
    if (!buf)
        buf = (void far *)PUBLIC_BUF;

    getviewsettings(&vp);
    x1 = wintable_tail->left + wintable_tail->vx;
    y1 = wintable_tail->top + wintable_tail->vx;
    x2 = wintable_tail->left + wintable_tail->width - 1 - wintable_tail->vx;
    y2 = wintable_tail->top + wintable_tail->height - 1 - wintable_tail->vx;

    if (!UC_SecondViewport(x1, y1, x2, y2)) {
        AUTOPRESS = 0;
        return ret;
    }

    UC_MouseHide();
    if (UC_WindowVerify(wnd)) {
        UC_DrawButtonBox(wnd, but, 1);
        getimage(left + 2, top + 2, right - 3, bottom - 3, buf);
    } else {
        getimage(left + 2, top + 2, right - 2, bottom - 2, buf);
    }

    putimage(left + 3 + shift, top + 3 + shift, buf, 0);
    setcolor(8);
    line(left + 1, top + 1, right - 1, top + 1);
    line(left + 1, top + 1, left + 1, bottom - 1);
    setcolor(7);
    line(left + 2, bottom - 1, right - 1, bottom - 1);
    line(right - 1, top + 2, right - 1, bottom - 1);
    UC_MouseShow();

    if (CLICK_SOUND) {
        sound(50);
        delay(2);
        nosound();
    }
    if (AUTOPRESS) {
        delay(100);
    } else {
        ret = UC_WaitFreeMouse(NULL, left, top, right, bottom, NULL);
    }

    UC_MouseHide();
    putimage(left + 2, top + 2, buf, 0);
    killimage(buf);
    UC_DrawButton(1, (char)shift, left, top, right, bottom);

    if (CLICK_SOUND) {
        sound(100);
        delay(2);
        nosound();
    }
    if (AUTOPRESS) {
        delay(100);
    }
    if (UC_WindowVerify(wnd)) {
        UC_DrawButtonBox(wnd, but, 0);
    }
    Setviewport(vp.left, vp.top, vp.right, vp.bottom);
    UC_MouseShow();
    AUTOPRESS = 0;
    return ret;
}

void far UC_BoardDisplay(int left, int top, int right, int bottom)
{
    UC_MouseHide();
    setlinestyle(4, 0x5555, 1);
    if (wintable_tail->mode & 8) {
        rectangle(left, top, right, bottom);
    } else {
        rectangle(left + 1, top + 1, right - 1, bottom - 1);
        setlinestyle(4, 0xaaaa, 1);
        rectangle(left, top, right, bottom);
        rectangle(left + 2, top + 2, right - 2, bottom - 2);
    }
    UC_MouseShow();
}

void far UC_BoardResizeLeftTop(void)
{
    WINDOWS *wnd = wintable_tail;
    int x1 = wnd->left;
    int y1 = wnd->top;
    int x2 = x1 + wnd->width - 1;
    int y2 = y1 + wnd->height - 1;
    int dx = 0, dy = 0;
    int mx, my;

    setwritemode(3);
    setcolor(15);
    while (MOUSE_AGI.s & HAND_LR) {
        x1 += dx;
        y1 += dy;
        if (x1 < 0) x1 = 0;
        if (y1 < 0) y1 = 0;
        if (wnd->minwidth - 1 > x2 - x1)
            x1 = x2 - wnd->minwidth + 1;
        if (wnd->minheight - 1 > y2 - y1)
            y1 = y2 - wnd->minheight + 1;
        UC_BoardDisplay(x1, y1, x2, y2);
        mx = MOUSE_AGI.x;
        my = MOUSE_AGI.y;
        while (!UC_MouseCheck()) ;
        dx = MOUSE_AGI.x - mx;
        dy = MOUSE_AGI.y - my;
        UC_BoardDisplay(x1, y1, x2, y2);
    }
    setlinestyle(0, 0, 1);
    setwritemode(0);
    UC_WindowResize(x1, y1, x2, y2);
}

void far UC_BoardResizeRightTop(void)
{
    WINDOWS *wnd = wintable_tail;
    int x1 = wnd->left;
    int y1 = wnd->top;
    int x2 = x1 + wnd->width - 1;
    int y2 = y1 + wnd->height - 1;
    int dx = 0, dy = 0;
    int mx, my;

    setwritemode(3);
    setcolor(15);
    while (MOUSE_AGI.s & HAND_LR) {
        x2 += dx;
        y1 += dy;
        if (y1 < 0) y1 = 0;
        if (wnd->minwidth - 1 > x2 - x1)
            x2 = x1 + wnd->minwidth - 1;
        if (wnd->minheight - 1 > y2 - y1)
            y1 = y2 - wnd->minheight + 1;
        UC_BoardDisplay(x1, y1, x2, y2);
        mx = MOUSE_AGI.x;
        my = MOUSE_AGI.y;
        while (!UC_MouseCheck()) ;
        dx = MOUSE_AGI.x - mx;
        dy = MOUSE_AGI.y - my;
        UC_BoardDisplay(x1, y1, x2, y2);
    }
    setlinestyle(0, 0, 1);
    setwritemode(0);
    UC_WindowResize(x1, y1, x2, y2);
}

void far UC_BoardResizeRightBottom(void)
{
    WINDOWS *wnd = wintable_tail;
    int x1 = wnd->left;
    int y1 = wnd->top;
    int x2 = x1 + wnd->width - 1;
    int y2 = y1 + wnd->height - 1;
    int dx = 0, dy = 0;
    int mx, my;

    setwritemode(3);
    setcolor(15);
    while (MOUSE_AGI.s & HAND_LR) {
        x2 += dx;
        y2 += dy;
        if (wnd->minwidth - 1 > x2 - x1)
            x2 = x1 + wnd->minwidth - 1;
        if (wnd->minheight - 1 > y2 - y1)
            y2 = y1 + wnd->minheight - 1;
        UC_BoardDisplay(x1, y1, x2, y2);
        mx = MOUSE_AGI.x;
        my = MOUSE_AGI.y;
        while (!UC_MouseCheck()) ;
        dx = MOUSE_AGI.x - mx;
        dy = MOUSE_AGI.y - my;
        UC_BoardDisplay(x1, y1, x2, y2);
    }
    setlinestyle(0, 0, 1);
    setwritemode(0);
    UC_WindowResize(x1, y1, x2, y2);
}

void far UC_BoardResizeLeftBottom(void)
{
    WINDOWS *wnd = wintable_tail;
    int x1 = wnd->left;
    int y1 = wnd->top;
    int x2 = x1 + wnd->width - 1;
    int y2 = y1 + wnd->height - 1;
    int dx = 0, dy = 0;
    int mx, my;

    setwritemode(3);
    setcolor(15);
    while (MOUSE_AGI.s & HAND_LR) {
        x1 += dx;
        y2 += dy;
        if (x1 < 0) x1 = 0;
        if (wnd->minwidth - 1 > x2 - x1)
            x1 = x2 - wnd->minwidth + 1;
        if (wnd->minheight - 1 > y2 - y1)
            y2 = y1 + wnd->minheight - 1;
        UC_BoardDisplay(x1, y1, x2, y2);
        mx = MOUSE_AGI.x;
        my = MOUSE_AGI.y;
        while (!UC_MouseCheck()) ;
        dx = MOUSE_AGI.x - mx;
        dy = MOUSE_AGI.y - my;
        UC_BoardDisplay(x1, y1, x2, y2);
    }
    setlinestyle(0, 0, 1);
    setwritemode(0);
    UC_WindowResize(x1, y1, x2, y2);
}

void far UC_BoardResizeUp(void)
{
    WINDOWS *wnd = wintable_tail;
    int x1 = wnd->left;
    int y1 = wnd->top;
    int x2 = x1 + wnd->width - 1;
    int y2 = y1 + wnd->height - 1;
    int dy = 0;
    int my;

    setwritemode(3);
    setcolor(15);
    while (MOUSE_AGI.s & HAND_LR) {
        y1 += dy;
        if (y1 < 0) y1 = 0;
        if (wnd->minheight - 1 > y2 - y1)
            y1 = y2 - wnd->minheight + 1;
        UC_BoardDisplay(x1, y1, x2, y2);
        my = MOUSE_AGI.y;
        while (!UC_MouseCheck()) ;
        dy = MOUSE_AGI.y - my;
        UC_BoardDisplay(x1, y1, x2, y2);
    }
    setlinestyle(0, 0, 1);
    setwritemode(0);
    UC_WindowResize(x1, y1, x2, y2);
}

void far UC_BoardResizeDown(void)
{
    WINDOWS *wnd = wintable_tail;
    int x1 = wnd->left;
    int y1 = wnd->top;
    int x2 = x1 + wnd->width - 1;
    int y2 = y1 + wnd->height - 1;
    int dy = 0;
    int my;

    setwritemode(3);
    setcolor(15);
    while (MOUSE_AGI.s & HAND_LR) {
        y2 += dy;
        if (wnd->minheight - 1 > y2 - y1)
            y2 = y1 + wnd->minheight - 1;
        UC_BoardDisplay(x1, y1, x2, y2);
        my = MOUSE_AGI.y;
        while (!UC_MouseCheck()) ;
        dy = MOUSE_AGI.y - my;
        UC_BoardDisplay(x1, y1, x2, y2);
    }
    setlinestyle(0, 0, 1);
    setwritemode(0);
    UC_WindowResize(x1, y1, x2, y2);
}

void far UC_BoardResizeLeft(void)
{
    WINDOWS *wnd = wintable_tail;
    int x1 = wnd->left;
    int y1 = wnd->top;
    int x2 = x1 + wnd->width - 1;
    int y2 = y1 + wnd->height - 1;
    int dx = 0;
    int mx;

    setwritemode(3);
    setcolor(15);
    while (MOUSE_AGI.s & HAND_LR) {
        x1 += dx;
        if (x1 < 0) x1 = 0;
        if (wnd->minwidth - 1 > x2 - x1)
            x1 = x2 - wnd->minwidth + 1;
        UC_BoardDisplay(x1, y1, x2, y2);
        mx = MOUSE_AGI.x;
        while (!UC_MouseCheck()) ;
        dx = MOUSE_AGI.x - mx;
        UC_BoardDisplay(x1, y1, x2, y2);
    }
    setlinestyle(0, 0, 1);
    setwritemode(0);
    UC_WindowResize(x1, y1, x2, y2);
}

void far UC_BoardResizeRight(void)
{
    WINDOWS *wnd = wintable_tail;
    int x1 = wnd->left;
    int y1 = wnd->top;
    int x2 = x1 + wnd->width - 1;
    int y2 = y1 + wnd->height - 1;
    int dx = 0;
    int mx;

    setwritemode(3);
    setcolor(15);
    while (MOUSE_AGI.s & HAND_LR) {
        x2 += dx;
        if (wnd->minwidth - 1 > x2 - x1)
            x2 = x1 + wnd->minwidth - 1;
        UC_BoardDisplay(x1, y1, x2, y2);
        mx = MOUSE_AGI.x;
        while (!UC_MouseCheck()) ;
        dx = MOUSE_AGI.x - mx;
        UC_BoardDisplay(x1, y1, x2, y2);
    }
    setlinestyle(0, 0, 1);
    setwritemode(0);
    UC_WindowResize(x1, y1, x2, y2);
}

void far UC_Cadi(WINDOWS *wnd, int x, int y)
{
    RECT rect;
    int save_vx, save_vy, save_vr, save_vb;
    int save_left, save_top, save_width, save_height;

    if (!wnd->DRAWICON)
        return;

    x += (ICO_TITLEWIDTH - wnd->ico_width) / 2;

    save_vx = wnd->vx;
    save_vy = wnd->vy;
    save_vr = wnd->vr;
    save_vb = wnd->vb;
    save_left = wnd->left;
    save_top = wnd->top;
    save_width = wnd->width;
    save_height = wnd->height;

    wnd->vb = 0;
    wnd->vr = 0;
    wnd->vy = 0;
    wnd->vx = 0;
    wnd->left = x;
    wnd->top = y;
    wnd->width = wnd->ico_width;
    wnd->height = wnd->ico_depth;

    rect.left = x - wnd->left - wnd->vx;
    rect.top = y - wnd->top - wnd->vy;
    rect.right = rect.left + wnd->ico_width - 1;
    rect.bottom = rect.top + wnd->ico_depth - 1;

    wnd->DRAWICON(wnd, &rect);

    wnd->vx = save_vx;
    wnd->vy = save_vy;
    wnd->vr = save_vr;
    wnd->vb = save_vb;
    wnd->left = save_left;
    wnd->top = save_top;
    wnd->width = save_width;
    wnd->height = save_height;
}

int far UC_BoardMoveMouse(void)
{
    WINDOWS *wnd = wintable_tail;
    int x1, y1, x2, y2;
    int dx = 0, dy = 0;
    int mx, my;
    void far *buf;
    WORD size;

    if (wnd->mode & 2)
        return 1;
    if (!(wnd->mode & 6) && wnd->mode != 0 && (MOUSE_AGI.s & 0x8000))
        return 0;
    if (wnd->minmax == 2)
        return 1;

    x1 = wnd->left;
    y1 = wnd->top;

    if (wnd->minmax == 1) {
        while (!UC_MouseCheck()) ;
        if (!(MOUSE_AGI.s & HAND_LR))
            return 1;

        windows_hide = 1;
        UC_RedrawBack(wnd->left, wnd->top, wnd->left + wnd->width - 1, wnd->top + wnd->height - 1);
        windows_hide = 0;

        size = imagesize(0, 0, wnd->ico_width - 1, wnd->ico_depth - 1);
        if (size == 0xFFFF)
            size = 256;
        buf = malloc(size);
        if (!buf)
            buf = (void far *)PUBLIC_BUF;

        while (MOUSE_AGI.s & HAND_LR) {
            int cx;
            x1 += dx;
            y1 += dy;
            if (x1 < 0) x1 = 0;
            if (y1 < 0) y1 = 0;
            mx = MOUSE_AGI.x;
            my = MOUSE_AGI.y;
            cx = x1 + (ICO_TITLEWIDTH - wnd->ico_width) / 2;
            UC_MouseHide();
            getimage(cx, y1, cx + wnd->ico_width - 1, y1 + wnd->ico_depth - 1, buf);
            if (wnd->DRAWICON)
                UC_Cadi(wnd, x1, y1);
            else
                UC_ShowIcon(NULL, wnd->ico_handle, wnd->ico_xor, wnd->ico_and, wnd->ico_width, wnd->ico_depth, cx, y1);
            UC_MouseShow();
            while (!UC_MouseCheck()) ;
            UC_MouseHide();
            putimage(cx, y1, buf, 0);
            UC_MouseShow();
            dx = MOUSE_AGI.x - mx;
            dy = MOUSE_AGI.y - my;
        }
        killimage(buf);
    } else {
        setwritemode(3);
        setcolor(15);
        while (MOUSE_AGI.s & HAND_LR) {
            x1 += dx;
            y1 += dy;
            if (x1 < 0) x1 = 0;
            if (y1 < 0) y1 = 0;
            x2 = x1 + wnd->width - 1;
            y2 = y1 + wnd->height - 1;
            UC_BoardDisplay(x1, y1, x2, y2);
            mx = MOUSE_AGI.x;
            my = MOUSE_AGI.y;
            while (!UC_MouseCheck()) ;
            dx = MOUSE_AGI.x - mx;
            dy = MOUSE_AGI.y - my;
            UC_BoardDisplay(x1, y1, x2, y2);
        }
        setlinestyle(0, 0, 1);
        setwritemode(0);
    }
    UC_WindowMove(x1, y1);
    return 1;
}

void far UC_BoardMoveHand(void)
{
    WINDOWS *wnd = wintable_tail;
    int x1, y1, x2, y2;
    int w, h;
    int key;
    char changed = 0;

    if (wnd->mode & 2)
        return;
    if (wnd->minmax == 1 || wnd->minmax == 2)
        return;

    setwritemode(3);
    setcolor(15);

    x1 = wnd->left;
    y1 = wnd->top;
    w = wnd->width;
    h = wnd->height;
    changed = 0;

    UC_MouseHide();
    x2 = x1 + w - 1;
    y2 = y1 + h - 1;

    while (1) {
        UC_BoardDisplay(x1, y1, x2, y2);
        while ((key = UC_CheckKeyboard()) == 0) ;
        UC_BoardDisplay(x1, y1, x2, y2);

        if (key == 0x1c0d) /* Enter */
            break;
        if (key == 0x011b) { /* Esc */
            UC_MouseShow();
            setwritemode(0);
            setlinestyle(0, 0, 1);
            return;
        }

        switch (key) {
        case 0x4800: /* 上移 */
            if (!(wnd->mode & 6) && wnd->mode != 0 && (KEYB_STATE & 3)) {
                if (wnd->minheight + 8 <= h)
                    h -= 8;
                else
                    h = wnd->minheight;
                changed = 1;
            } else {
                int d = (y1 >= 8) ? 8 : y1;
                y1 -= d;
            }
            break;

        case 0x5000: /* 下移 */
            if (!(wnd->mode & 6) && wnd->mode != 0 && (KEYB_STATE & 3)) {
                h += 8;
                changed = 1;
            } else {
                int maxy = getmaxy() - y1;
                int d = (maxy >= 8) ? 8 : maxy;
                y1 += d;
            }
            break;

        case 0x4b00: /* 左移 */
            if (!(wnd->mode & 6) && wnd->mode != 0 && (KEYB_STATE & 3)) {
                if (wnd->minwidth + 8 <= w)
                    w -= 8;
                else
                    w = wnd->minwidth;
                changed = 1;
            } else {
                int d = (x1 >= 8) ? 8 : x1;
                x1 -= d;
            }
            break;

        case 0x4d00: /* 右移 */
            if (!(wnd->mode & 6) && wnd->mode != 0 && (KEYB_STATE & 3)) {
                w += 8;
                changed = 1;
            } else {
                int maxx = getmaxx() - x1;
                int d = (maxx >= 8) ? 8 : maxx;
                x1 += d;
            }
            break;
        }
        x2 = x1 + w - 1;
        y2 = y1 + h - 1;
    }

    UC_MouseShow();
    setwritemode(0);
    setlinestyle(0, 0, 1);
    if (changed)
        UC_WindowResize(x1, y1, x2, y2);
    else
        UC_WindowMove(x1, y1);
}

void far UC_CaretBar(WINDOWS *wnd)
{
    struct viewporttype vp;
    int color = 15;

    UC_MouseHide();
    getviewsettings(&vp);
    setwritemode(3);
    setfillstyle(1, color);
    Setviewport(wnd->left + wnd->vx,
                wnd->top + wnd->vy,
                wnd->left + wnd->width - wnd->vr,
                wnd->top + wnd->height - wnd->vb);
    UC_WindowBar(wnd, wnd->CARET_X, wnd->CARET_Y, wnd->CARET_W, wnd->CARET_H);
    Setviewport(vp.left, vp.top, vp.right, vp.bottom);
    setwritemode(0);
    UC_MouseShow();
}

void far UC_HideCaret(WINDOWS *wnd)
{
    if (UC_GetCurrentWindow() == wnd && wnd->inputhot && !wnd->hide && wnd->CARET_STATE) {
        UC_CaretBar(wnd);
        wnd->CARET_STATE = 0;
    }
}

void far UC_ShowCaret(WINDOWS *wnd)
{
    if (UC_GetCurrentWindow() == wnd && wnd->inputhot && !wnd->hide && !wnd->CARET_STATE) {
        UC_CaretBar(wnd);
        wnd->CARET_STATE = 1;
    }
}

void far UC_MoveCaret(WINDOWS *wnd, int x, int y)
{
    if (wnd->CARET_X == x && wnd->CARET_Y == y)
        return;
    if (wnd->hide || !wnd->CARET_STATE) {
        wnd->CARET_X = x;
        wnd->CARET_Y = y;
        return;
    }
    UC_MouseHide();
    UC_HideCaret(wnd);
    wnd->CARET_X = x;
    wnd->CARET_Y = y;
    UC_ShowCaret(wnd);
    UC_MouseShow();
}

void far UC_FlashCaret(void)
{
    WINDOWS *wnd = UC_GetCurrentWindow();
    if (wnd->inputhot && !wnd->hide && wnd->minmax != 1) {
        if (wnd->CARET_STATE)
            UC_HideCaret(wnd);
        else
            UC_ShowCaret(wnd);
    }
}

void far UC_CreateCaret(WINDOWS *wnd, int width, int height)
{
    if (!UC_WindowVerify(wnd))
        return;
    if (wnd->inputhot) {
        if (wnd->CARET_W == width && wnd->CARET_H == height)
            return;
        UC_HideCaret(wnd);
        wnd->CARET_W = width;
        wnd->CARET_H = height;
    } else {
        wnd->CARET_W = width;
        wnd->CARET_H = height;
        wnd->inputhot = 1;
        wnd->CARET_STATE = 0;
        wnd->CARET_X = 0;
        wnd->CARET_Y = 0;
        UC_SetTimer(UC_FlashCaret, CARET_SPEED);
    }
}

void far UC_DestroyCaret(WINDOWS *wnd)
{
    if (wnd->inputhot) {
        UC_HideCaret(wnd);
        UC_KillTimer(UC_FlashCaret);
        wnd->inputhot = 0;
    }
}

int far UC_DestroyObject(WINDOWS *wnd, WORD cure_type, WORD cure_num)
{
    if (cure_type < 1 || cure_type > 9)
        return 0;

    switch (cure_type) {
    case TYPE_BUTTON: {
        BUTTON *p = UC_GetButton(wnd, cure_num);
        if (!p) return 0;
        if (p->next) p->next->prev = p->prev;
        if (p->prev) p->prev->next = p->next;
        if (wnd->button_head == p) wnd->button_head = p->next;
        if (wnd->button_tail == p) wnd->button_tail = p->prev;
        p->next = NULL;
        p->prev = NULL;
        MyFREE(p);
        break;
    }
    case TYPE_INPUTLINE: {
        INPUTLINE *p = UC_GetInputBox(wnd, cure_num);
        if (!p) return 0;
        if (p->next) p->next->prev = p->prev;
        if (p->prev) p->prev->next = p->next;
        if (wnd->inpline_head == p) wnd->inpline_head = p->next;
        if (wnd->inpline_tail == p) wnd->inpline_tail = p->prev;
        p->next = NULL;
        p->prev = NULL;
        if (p->text) MyFREE(p->text);
        MyFREE(p);
        break;
    }
    case TYPE_LISTBOX: {
        LISTBOX *p = UC_GetListBox(wnd, cure_num);
        if (!p) return 0;
        if (p->next) p->next->prev = p->prev;
        if (p->prev) p->prev->next = p->next;
        if (wnd->box_head == p) wnd->box_head = p->next;
        if (wnd->box_tail == p) wnd->box_tail = p->prev;
        p->next = NULL;
        p->prev = NULL;
        MyFREE(p);
        break;
    }
    case TYPE_RADIO: {
        RADIO *p = UC_GetRadioButton(wnd, cure_num);
        RADIO *cur;
        if (!p) return 0;
        if (wnd->radio == p) {
            wnd->radio = p->next;
        } else {
            for (cur = wnd->radio; cur; cur = cur->next) {
                if (cur->next == p) {
                    cur->next = p->next;
                    break;
                }
            }
        }
        MyFREE(p);
        break;
    }
    case TYPE_CHECK: {
        CHECK *p = UC_GetCheckButton(wnd, cure_num);
        CHECK *cur;
        if (!p) return 0;
        if (wnd->check == p) {
            wnd->check = p->next;
        } else {
            for (cur = wnd->check; cur; cur = cur->next) {
                if (cur->next == p) {
                    cur->next = p->next;
                    break;
                }
            }
        }
        MyFREE(p);
        break;
    }
    case TYPE_LABEL: {
        LABEL *p = UC_GetLabel(wnd, cure_num);
        LABEL *cur;
        if (!p) return 0;
        if (wnd->label == p) {
            wnd->label = p->next;
        } else {
            for (cur = wnd->label; cur; cur = cur->next) {
                if (cur->next == p) {
                    cur->next = p->next;
                    break;
                }
            }
        }
        if (p->text) MyFREE(p->text);
        MyFREE(p);
        break;
    }
    case TYPE_SCROLLBAR: {
        SCROLLBAR *p = UC_GetScrollbar(wnd, cure_num);
        SCROLLBAR *cur;
        if (!p) return 0;
        if (wnd->sbar == p) {
            wnd->sbar = p->next;
        } else {
            for (cur = wnd->sbar; cur; cur = cur->next) {
                if (cur->next == p) {
                    cur->next = p->next;
                    break;
                }
            }
        }
        MyFREE(p);
        break;
    }
    case TYPE_GROUPBOX: {
        GROUPBOX *p = UC_GetGroupBox(wnd, cure_num);
        GROUPBOX *cur;
        if (!p) return 0;
        if (wnd->groupbox == p) {
            wnd->groupbox = p->next;
        } else {
            for (cur = wnd->groupbox; cur; cur = cur->next) {
                if (cur->next == p) {
                    cur->next = p->next;
                    break;
                }
            }
        }
        MyFREE(p);
        break;
    }
    case TYPE_USERRECT: {
        USER_MOUSE *p = UC_GetUserMouse(wnd, cure_num);
        USER_MOUSE *cur;
        if (!p) return 0;
        if (wnd->mouse == p) {
            wnd->mouse = p->next;
        } else {
            for (cur = wnd->mouse; cur; cur = cur->next) {
                if (cur->next == p) {
                    cur->next = p->next;
                    break;
                }
            }
        }
        MyFREE(p);
        break;
    }
    }

    if (UC_BeginPaintOnBack(wnd)) {
        UC_RedrawWindow(wnd);
        UC_EndPaintOnBack(wnd);
    }
    return 1;
}
