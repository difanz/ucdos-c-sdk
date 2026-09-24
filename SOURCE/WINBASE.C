// WINBASE.C - 窗口基础管理核心

extern void far SETMYVICTOR(void);
extern void far SETOLDVICTOR(void);
extern void far UC_Handler(void);
extern void far UC_FlashCaret(void);


void far nomem(char far *text)
{
    UC_Exit("无足够内存空间, 程序异常中止!\r\n原因: %s", text);
}

void far MyFREE(void far *p)
{
    if (p == GRAPH_BUFFER)
        UC_Exit("异常现象: 释放 GRAPH_BUFFER!\r\n");
    if (p == (void far *)PUBLIC_BUF)
        UC_Exit("异常现象: 释放 PUBLIC_BUF!\r\n");
    if (p == NULL)
        UC_Exit("异常现象: 释放 NULL!\r\n");
    free(p);
    if (heapcheck() < 0)
        UC_Exit("异常现象: 堆被破坏! 请开发者检查应用程序!\r\n");
}

void far EnableSDKint10(void)
{
    SETMYVICTOR();
}

void far DisableSDKint10(void)
{
    SETOLDVICTOR();
}

void far UC_MouseSetHand(char hand)
{
    if (hand == 1 || hand == 2)
        HAND_LR = hand;
}

void far UC_MouseEvent(void)
{
    asm {
        push ds
        push ax
        push bx
        mov ax, seg MOUSE_AGIX
        mov ds, ax
    }
    MOUSE_AGIX = _CX;
    MOUSE_AGIY = _DX;
    MOUSE_AGIB = _BX;

    mouseagi[mouse_tail].x = MOUSE_AGIX;
    mouseagi[mouse_tail].y = MOUSE_AGIY;
    mouseagi[mouse_tail].s = MOUSE_AGIB;
    mouseagi[mouse_tail].t = MOUSETIME;

    if (MOUSE_AGIB != 0) {
        m_prev = mouse_tail;
        if (m_prev == 0) m_prev = 49; else m_prev--;
        if (mouseagi[m_prev].s == 0) {
            for ( ; (m_prev = (m_prev == 0) ? 49 : (m_prev - 1)) != mouse_tail ; ) {
                if (mouseagi[m_prev].s == MOUSE_AGIB) {
                    while (m_prev != mouse_tail) {
                        if (m_prev == 0) m_prev = 49; else m_prev--;
                        if (mouseagi[m_prev].s == MOUSE_AGIB)
                            continue;
                        if (++m_prev == 50)
                            m_prev = 0;
                        break;
                    }
                    if (m_prev == mouse_tail) break;
                    if (mouseagi[m_prev].s != MOUSE_AGIB) break;
                    if (abs(mouseagi[m_prev].x - MOUSE_AGIX) >= 8) break;
                    if (abs(mouseagi[m_prev].y - MOUSE_AGIY) >= 8) break;
                    if (abs((int)(MOUSETIME - mouseagi[m_prev].t)) < DBLTIME) {
                        mouseagi[mouse_tail].s |= 0x8000;
                        break;
                    }
                } else if (mouseagi[m_prev].s != 0) {
                    break;
                }
            }
        }
    }

    mouse_tail++;
    if (mouse_tail == 50)
        mouse_tail = 0;

    asm {
        pop bx
        pop ax
        pop ds
    }
}

void far UC_MouseBack(void)
{
    if (mouse_head > 0)
        mouse_head--;
    else
        mouse_head = 49;
}

int far UC_MouseCheck(void)
{
    if (mouse_head != mouse_tail) {
        MOUSE_AGI.x = mouseagi[mouse_head].x;
        MOUSE_AGI.y = mouseagi[mouse_head].y;
        MOUSE_AGI.s = mouseagi[mouse_head].s;
        mouse_head++;
        if (mouse_head == 50)
            mouse_head = 0;
        return 1;
    }
    return 0;
}

int far UC_MouseReset(void)
{
    asm {
        mov ax, 0x21
        int 0x33
        cmp ax, 0xffff
        jz success
        xor ax, ax
        int 0x33
        cmp ax, 0xffff
        jz success
    }
    return 0;
success:
    return 1;
}

void far UC_MouseInit(void)
{
    void (far *p)();
    int maxy;
    int x, y, b;

    mouse_head = mouse_tail = 0;
    mouseagi[0].x = mouseagi[0].y = mouseagi[0].s = 0;
    p = UC_MouseEvent;
    maxy = getmaxy();

    if (UC_MouseReset()) {
        asm {
            mov ax, 0xc
            mov bx, word ptr p+2
            mov cx, 0x1f
            mov dx, word ptr p
            push es
            mov es, bx
            int 0x33
            mov bx, es
            pop es

            mov ax, 8
            xor cx, cx
            mov dx, maxy
            int 0x33
        }
        UC_MouseSetShape(&move_map);
        UC_MouseShow();
        asm {
            mov ax, 3
            int 0x33
            mov b, bx
            mov x, cx
            mov y, dx
        }
        MOUSE_AGI.x = x;
        MOUSE_AGI.y = y;
        MOUSE_AGI.s = b;
    }
}

void far UC_MouseClose(void)
{
    UC_MouseHide();
    UC_MouseReset();
    mouse_tail = mouse_head = 0;
    mouseagi[0].s = mouseagi[0].y = mouseagi[0].x = 0;
}

void far UC_MouseHide(void)
{
    asm {
        mov ax, 2
        int 0x33
    }
}

void far UC_MouseShow(void)
{
    asm {
        mov ax, 1
        int 0x33
    }
}

void far UC_MouseSetPosition(int x, int y)
{
    asm {
        mov ax, 2
        int 0x33
        mov ax, 4
        mov cx, x
        mov dx, y
        int 0x33
        mov ax, 1
        int 0x33
    }
}

MOUSE_BMP far * far UC_GetIDC(int type)
{
    MOUSE_BMP far *ret;

    switch (type) {
    case 0:  ret = &move_map; break;
    case 1:  ret = &up_down_map; break;
    case 2:  ret = &left_right_map; break;
    case 3:  ret = &left_up_map; break;
    case 4:  ret = &left_down_map; break;
    case 5:  ret = &wait_map; break;
    case 6:  ret = &cross_map; break;
    case 7:  ret = &ibeam_map; break;
    case 8:  ret = &size_map; break;
    case 9:  ret = &no_map; break;
    case 10: ret = &hand_map; break;
    default: ret = &move_map; break;
    }
    return ret;
}

void far UC_MouseShapeType(int type)
{
    UC_MouseSetShape(UC_GetIDC(type));
}

void far UC_MouseSetShape(MOUSE_BMP far *map)
{
    int hoty, hotx;
    MOUSE_BMP far *p;

    if (cure_map != map) {
        cure_map = map;
        hotx = map->hotx;
        hoty = map->hoty;
        UC_MouseHide();
        p = map;
        asm {
            mov bx, hotx
            mov cx, hoty
            mov dx, word ptr p
            mov ax, word ptr p+2
            push es
            mov es, ax
            mov ax, 9
            int 0x33
            pop es
        }
        UC_MouseShow();
    }
}

USER_MOUSE * far UC_DefineUserMouseEvent(WINDOWS *wnd,
                             WORD left, WORD top, WORD right,
                             WORD bottom, WORD state, MOUSE_BMP *usermap,
                             void (*fun)(int x, int y, WORD s))
{
    USER_MOUSE *ms, *p;

    ms = (USER_MOUSE *)malloc(sizeof(USER_MOUSE));
    if (ms == NULL)
        nomem("MOUSE 事件定义");
    ms->left = left;
    ms->top = top;
    ms->right = right;
    ms->bottom = bottom;
    ms->state = state;
    if (usermap == NULL)
        usermap = &move_map;
    ms->usermap = usermap;
    ms->fun = fun;
    ms->next = NULL;

    if (wnd->mouse == NULL) {
        wnd->mouse = ms;
    } else {
        p = wnd->mouse;
        while (p->next != NULL)
            p = p->next;
        p->next = ms;
    }
    return ms;
}

void far UC_InitDesktop(char fillstyle, char fillcolor, char far *bmpname, char bmpmode)
{
    DESKTOP.fillstyle = fillstyle;
    DESKTOP.fillcolor = fillcolor;
    DESKTOP.bmpmode = bmpmode;
    DESKTOP.bmpname = bmpname;
    DESKTOP.handle = 0;
}

void far UC_ShowStateLine(void)
{
    asm {
        mov ax, 0xff07
        int 0x16
        mov ax, 0xff10
        mov bx, 0x7000
        int 0x10
        mov ax, 0xff07
        int 0x16
    }
}

void far UC_System(char far *command)
{
    save_x = MOUSE_AGI.x;
    save_y = MOUSE_AGI.y;
    UC_MouseClose();
    asm {
        mov ax, 0x950f
        int 0x10
        mov old_mode, al
        mov ax, 3
        int 0x10
    }
    DisableSDKint10();
    system(command);
    EnableSDKint10();
    asm {
        mov al, old_mode
        xor ah, ah
        int 0x10
    }
    UC_SetMSWinPalete();
    UC_MouseInit();
    UC_MouseSetPosition(save_x, save_y);
    windows_exec = 1;
    UC_MouseShapeType(5);
    UC_RedrawBack(0, 0, getmaxx(), getmaxy());
    windows_exec = 0;
    UC_RedrawCurrentWindow();
    UC_ShowStateLine();
    UC_MouseShapeType(0);
}

void far UC_ToDosPrompt(void)
{
    UC_System("");
}

void far UC_SetMSWinPalete(void)
{
    struct { unsigned char r, g, b; } pal[16];
    register int i;
    int maxc;

    memcpy(pal, default_win_pal, sizeof(pal));
    maxc = getmaxcolor();
    if (maxc == 15 || maxc == 255) {
        for (i = 0; i < 16; i++) {
            setrgbpalette(i, pal[i].r >> 2, pal[i].g >> 2, pal[i].b >> 2);
        }
    }
}

void far UC_InitUCVision(int GraphDriver, int GraphMode)
{
    RECT rect;

    if (BUFFER_LEN < 0x2800)
        BUFFER_LEN = 0x2800;
    GRAPH_BUFFER = (char far *)malloc(BUFFER_LEN);
    if (GRAPH_BUFFER == NULL) {
        printf("\r\n无足够内存空间启动本程序!\r\n\n");
        exit(0xff);
    }
    if (SYSCHAR_SIZE <= 16)
        IC_SIZE = 12;
    else
        IC_SIZE = 16;

    UC_GetRealXY(&BUT_BORD);
    UC_GetRealXY(&LINESPACE);
    initgraph(GraphDriver, GraphMode);
    UC_ShowStateLine();
    UC_SetMSWinPalete();
    windows_rdbk = 0;
    windows_exec = 0;
    wintable_head = NULL;
    wintable_tail = NULL;
    UC_MouseInit();
    INIT_FIRST = 1;
    UC_MouseShapeType(5);
    setcolor(12);
    rect.left = 0;
    rect.top = 0;
    rect.right = getmaxx() - 1;
    rect.bottom = getmaxy() - 1;
    settextstyle(0, 0, 16);
    UC_DrawText(NULL, &rect, 10, "正在初始化桌面\n请稍等待");
    harderr((int (far *)())UC_Handler);
    if (UC_CreateBitmap(DESKTOP.bmpname))
        DESKTOP.handle = UC_XMSLoadDDB(DESKTOP.bmpname);
    UC_DrawDesktop();
    UC_MouseShapeType(0);
    INIT_FIRST = 0;
}

void far nexitw(int code)
{
    if (RETFUN != NULL)
        RETFUN();
    UC_MouseClose();
    closegraph();
    exit(code);
}

void far UC_CloseFunction(void)
{
    UC_WindowClose();
    nexitw(RETCODE);
}

void far UC_CloseUCVision(int retcode, void (far *fun)())
{
    RETCODE = retcode;
    RETFUN = fun;
    if (NO_EXITWIN != 0) {
        nexitw(RETCODE);
    }
    CLOSEwin = UC_DefineWindow(2, -1, -1, 300, 180, "UC Vision 退出窗口", NULL, UC_WindowClose, NULL);
    UC_DefineLabel(CLOSEwin, 20, 30, 0, NULL, NULL, ICO_EXIT, 1);
    UC_DefineLabel(CLOSEwin, 80, 30, 0, NULL, "此命令将结束本次\nUC Vision 应用程序", NULL, 0);
    UC_DefinePressButton(CLOSEwin, UC_CloseFunction, 75, 100, 60, 0, "确定", 0);
    UC_DefinePressButton(CLOSEwin, UC_WindowClose, 155, 100, 60, 0, "取消", 1);
    UC_DefineActive(CLOSEwin, 1, 1);
    UC_WindowEnable(CLOSEwin);
}

void far UC_SetMessage(WORD msg)
{
    SDK_MSG = msg;
}

WORD far UC_GetMessage(void)
{
    WORD msg = SDK_MSG;
    SDK_MSG = 0;
    return msg;
}

/* 窗口列表与视口管理 */

int far UC_CheckNoOver(WINDOWS far *wnd)
{
    int r_x2, r_y2;
    register int x1, y1;

    if (wnd->hide != 0)
        return 0;
    if (wnd == wintable_tail)
        return 1;
    x1 = wnd->left;
    y1 = wnd->top;
    r_x2 = x1 + wnd->width - 1;
    r_y2 = y1 + wnd->height - 1;
    while ((wnd = wnd->next) != NULL) {
        if (wnd->left <= r_x2 &&
            wnd->top <= r_y2 &&
            wnd->left + wnd->width - 1 >= x1 &&
            wnd->top + wnd->height - 1 >= y1)
            return 0;
    }
    return 1;
}

int far UC_CheckInBack(WINDOWS far *wnd)
{
    int r_x1, r_y1;
    register int r_x2, r_y2;

    if (wnd == wintable_tail)
        return 0;
    r_x1 = wnd->left;
    r_y1 = wnd->top;
    r_x2 = r_x1 + wnd->width - 1;
    if (getmaxx() - 1 < r_x2)
        r_x2 = getmaxx() - 1;
    r_y2 = r_y1 + wnd->height - 1;
    if (getmaxy() - 1 < r_y2)
        r_y2 = getmaxy() - 1;
    while ((wnd = wnd->next) != NULL) {
        if (wnd->left <= r_x1 &&
            wnd->top <= r_y1 &&
            wnd->left + wnd->width - 1 >= r_x2 &&
            wnd->top + wnd->height - 1 >= r_y2)
            return 1;
    }
    return 0;
}

int far UC_CheckInViewPort(WINDOWS far *wnd)
{
    struct viewporttype vi;

    getviewsettings(&vi);
    if (wnd == NULL)
        wnd = wintable_head;
    while (wnd != wintable_tail && wnd != NULL) {
        if (wnd->minmax == 1 || wnd->hide != 0) {
            wnd = wnd->next;
            continue;
        }
        if (wnd->left <= vi.left &&
            wnd->top <= vi.top &&
            wnd->left + wnd->width >= vi.right &&
            wnd->top + wnd->height >= vi.bottom)
            return 1;
        wnd = wnd->next;
    }
    return 0;
}

void far UC_DrawDesktopDefault(void)
{
    UC_MouseHide();
    setfillstyle(DESKTOPDEFAULTSTYLE, DESKTOPDEFAULTCOLOR);
    bar(0, 0, getmaxx(), getmaxy() - 1);
    UC_MouseShow();
}

void far UC_DrawDesktop(void)
{
    WORD zx, zy;

    if (UC_CheckInViewPort(NULL))
        return;
    UC_MouseHide();
    if (DESKTOP.fillstyle > 1 || DESKTOP.bmpmode == 2) {
        if (DESKTOP.bmpname == NULL || strlen(DESKTOP.bmpname) == 0 ||
            (DESKTOP.bmpname != NULL && DESKTOP.bmpmode != 2))
            UC_DrawDesktopDefault();
    }
    if (DESKTOP.fillstyle == 0) {
        if (DESKTOP.bmpname == NULL || strlen(DESKTOP.bmpname) == 0)
            goto done;
    }
    if (DESKTOP.bmpname == NULL || strlen(DESKTOP.bmpname) == 0) {
        if (DESKTOP.fillstyle > 1)
            setwritemode(2);
        setfillstyle(DESKTOP.fillstyle, DESKTOP.fillcolor);
        bar(0, 0, getmaxx(), getmaxy() - 1);
        setwritemode(0);
        goto done;
    }
    if (DESKTOP.bmpmode == 1) {
        if (DESKTOP.fillstyle > 1)
            setwritemode(2);
        setfillstyle(DESKTOP.fillstyle, DESKTOP.fillcolor);
        bar(0, 0, getmaxx(), getmaxy() - 1);
        setwritemode(0);
    }
    UC_GetZoomDDB(&zx, &zy);
    UC_SetZoomDDB(0, 0);
    if (!UC_DrawDDB(NULL, DESKTOP.handle, DESKTOP.bmpname, 0, 0, getmaxx(), getmaxy(), DESKTOP.bmpmode, 0))
        UC_DrawDesktopDefault();
    UC_SetZoomDDB(zx, zy);
done:
    UC_MouseShow();
}

void far UC_DefineKeyboardEvent(WINDOWS far *wnd, int (far *kbenv)(int keycode, char keystate))
{
    if (UC_WindowVerify(wnd)) {
        wnd->KB_Entry = kbenv;
    }
}

WINDOWS far * far UC_GetCurrentWindow(void)
{
    return wintable_tail;
}

int far UC_WindowVerify(WINDOWS far *wnd)
{
    WINDOWS far *p;

    if (wnd == NULL)
        return 0;
    p = wintable_head;
    while (p != NULL) {
        if (p == wnd)
            break;
        p = p->next;
    }
    return (p != NULL) ? 1 : 0;
}

void far UC_WindowAppend(WINDOWS far *wnd)
{
    if (wintable_tail != NULL) {
        wnd->prev = wintable_tail;
        wintable_tail->next = wnd;
    }
    wintable_tail = wnd;
    if (wintable_head == NULL)
        wintable_head = wnd;
}

void far UC_WindowDelete(WINDOWS far *wnd)
{
    if (wnd->next != NULL) {
        wnd->next->prev = wnd->prev;
    }
    if (wnd->prev != NULL) {
        wnd->prev->next = wnd->next;
    }
    if (wintable_head == wnd) {
        wintable_head = wnd->next;
    }
    if (wintable_tail == wnd) {
        wintable_tail = wnd->prev;
    }
    wnd->prev = NULL;
    wnd->next = NULL;
}

void far UC_WindowToFirst(WINDOWS far *wnd)
{
    UC_WindowDelete(wnd);
    UC_WindowAppend(wnd);
}

void far UC_WindowHide(void)
{
    WINDOWS far *wnd = wintable_tail;
    int x1, y1, x2, y2;

    if (!UC_WindowVerify(wnd))
        return;
    if (wnd->hide != 0)
        return;
    if (wnd->inputhot != 0) {
        UC_HideCaret(wnd);
        UC_KillTimer(UC_FlashCaret);
    }
    x1 = wnd->left;
    y1 = wnd->top;
    x2 = x1 + wnd->width - 1;
    y2 = y1 + wnd->height - 1;
    UC_MouseHide();
    windows_hide = 1;
    UC_RedrawBack(x1, y1, x2, y2);
    windows_hide = 0;
    wnd->hide = 1;
    if (wnd->prev != NULL && wnd->prev->hide == 0) {
        UC_WindowToFirst(wnd->prev);
        wnd = wintable_tail;
        UC_DisplayTitle(wnd);
        if (wnd->ENABLE != NULL)
            wnd->ENABLE(wnd, 1);
        if (wnd->minmax != 1 && wnd->inputhot != 0)
            UC_SetTimer(UC_FlashCaret, CARET_SPEED);
    }
    UC_MouseSetShape(&move_map);
    UC_MouseShow();
}

void far UC_RedrawCurrentWindow(void)
{
    UC_RedrawWindow(wintable_tail);
}

void far UC_RedrawWindow(WINDOWS far *wnd)
{
    if (UC_CheckInViewPort(wnd))
        return;
    UC_WindowShow(wnd);
    if (wnd->minmax != 1 && wnd->FUNCTION != NULL)
        wnd->FUNCTION(wnd);
}

void far UC_WindowEnable(WINDOWS far *wnd)
{
    register int over;
    struct viewporttype vi;
    WINDOWS far *old_active;

    if (!UC_WindowVerify(wnd))
        return;
    UC_HideCaret(wintable_tail);
    getviewsettings(&vi);
    if (wnd->minmax != 1) {
        if (wnd->width < wnd->minwidth)
            wnd->width = wnd->minwidth;
        if (wnd->height < wnd->minheight)
            wnd->height = wnd->minheight;
    }
    over = (UC_CheckNoOver(wnd) != 0) ? 1 : 0;
    if (wnd != wintable_tail)
        UC_WindowToFirst(wnd);
    old_active = wnd->prev;
    if (UC_WindowVerify(old_active)) {
        if (old_active->inputhot != 0)
            UC_KillTimer(UC_FlashCaret);
        if (old_active->ENABLE != NULL)
            old_active->ENABLE(old_active, 0);
        if (old_active->hide == 0)
            UC_DisplayTitle(old_active);
    }
    if (wnd->ENABLE != NULL)
        wnd->ENABLE(wnd, 1);
    Setviewport(wnd->left, wnd->top, wnd->left + wnd->width, wnd->top + wnd->height);
    if (over)
        UC_DisplayTitle(wnd);
    else
        UC_RedrawWindow(wnd);
    wnd->hide = 0;
    Setviewport(vi.left, vi.top, vi.right, vi.bottom);
    if (wnd->minmax != 1 && wnd->inputhot != 0)
        UC_SetTimer(UC_FlashCaret, CARET_SPEED);
}

/* 控件定义与分配 */

GROUPBOX far * far UC_DefineGroupBox(WINDOWS far *wnd, char far *text, int left, int top, int width, int height)
{
    GROUPBOX far *box, far *p;

    box = (GROUPBOX far *)malloc(sizeof(GROUPBOX));
    if (box == NULL)
        nomem("定义分组框");
    UC_GetRealXY(&left);
    UC_GetRealXY(&top);
    UC_GetRealXY(&width);
    UC_GetRealXY(&height);
    box->next = NULL;
    box->text = text;
    box->left = left;
    box->top = top;
    box->width = width;
    box->height = height;
    if (UC_WindowVerify(wnd)) {
        if (wnd->groupbox == NULL) {
            wnd->groupbox = box;
        } else {
            p = wnd->groupbox;
            while (p->next != NULL)
                p = p->next;
            p->next = box;
        }
    }
    return box;
}

RADIO far * far UC_DefineRadioButton(WINDOWS far *wnd, char far **list, int enable, int x, int y, int key, void (far *fun)(int count))
{
    RADIO far *rad, far *p;
    int count;

    rad = (RADIO far *)malloc(sizeof(RADIO));
    if (rad == NULL)
        nomem("定义单选钮");
    rad->width = 0;
    count = 0;
    while (list[count] != NULL) {
        if (textwidth(list[count]) > rad->width)
            rad->width = textwidth(list[count]);
        count++;
    }
    rad->width += (wnd->syschar_size * 3) / 2;
    rad->height = (count - 1) * wnd->syschar_size * 3 / 2 + wnd->syschar_size;
    UC_GetRealXY(&x);
    UC_GetRealXY(&y);
    rad->x = x;
    rad->y = y;
    rad->enable = enable;
    rad->list = list;
    rad->key = key;
    rad->fun = fun;
    rad->next = NULL;
    if (UC_WindowVerify(wnd)) {
        if (wnd->radio == NULL) {
            wnd->radio = rad;
        } else {
            p = wnd->radio;
            while (p->next != NULL)
                p = p->next;
            p->next = rad;
        }
    }
    return rad;
}

CHECK far * far UC_DefineCheckButton(WINDOWS far *wnd, char far *text, char enable, int x, int y, int key, void (far *fun)(int state))
{
    CHECK far *chk, far *p;

    UC_GetRealXY(&x);
    UC_GetRealXY(&y);
    chk = (CHECK far *)malloc(sizeof(CHECK));
    if (chk == NULL)
        nomem("定义复选钮");
    chk->disable = 0;
    chk->text = text;
    if (enable != 0)
        enable = 1;
    chk->enable = enable;
    chk->x = x;
    chk->y = y;
    chk->key = key;
    chk->fun = fun;
    chk->next = NULL;
    if (wnd->check == NULL) {
        wnd->check = chk;
    } else {
        p = wnd->check;
        while (p->next != NULL)
            p = p->next;
        p->next = chk;
    }
    return chk;
}

void far UC_DefineActive(WINDOWS far *wnd, WORD cure_type, WORD cure_num)
{
    if (!UC_WindowVerify(wnd))
        return;
    switch (cure_type) {
    case 1:
        if (UC_GetButton(wnd, cure_num) == NULL) return;
        break;
    case 2:
        if (UC_GetInputBox(wnd, cure_num) == NULL) return;
        break;
    case 3:
        if (UC_GetListBox(wnd, cure_num) == NULL) return;
        break;
    case 4:
        if (UC_GetRadioButton(wnd, cure_num) == NULL) return;
        break;
    case 5:
        if (UC_GetCheckButton(wnd, cure_num) == NULL) return;
        break;
    default:
        return;
    }
    wnd->cure_type = cure_type;
    wnd->cure_num = cure_num;
}

void far UC_NewActive(WINDOWS far *wnd, WORD cure_type, WORD cure_num)
{
    UC_UpdateActive(wnd, 1);
    UC_DefineActive(wnd, cure_type, cure_num);
    UC_UpdateActive(wnd, 0);
}

SCROLLBAR far * far UC_DefineScrollbar(WINDOWS far *wnd, char mode, int left, int top, int leng, float max, float cure, float page, void (far *fun)(SCROLLBAR far *sbar, float prevcure), char hide)
{
    SCROLLBAR far *sbar, far *p;

    if (!UC_WindowVerify(wnd))
        return NULL;
    sbar = (SCROLLBAR far *)malloc(sizeof(SCROLLBAR));
    if (sbar == NULL)
        nomem("定义滚动条");
    sbar->mode = mode;
    sbar->hide = hide;
    sbar->left = left;
    sbar->top = top;
    if (left == -1) {
        wnd->vr += wnd->syschar_size + 1;
        wnd->vbar = sbar;
        if (wnd->minheight < wnd->syschar_size * 6)
            wnd->minheight = wnd->syschar_size * 6;
    }
    if (top == -1) {
        wnd->vb += wnd->syschar_size + 1;
        wnd->hbar = sbar;
        if (wnd->minwidth < wnd->syschar_size * 5)
            wnd->minwidth = wnd->syschar_size * 5;
    }
    sbar->ox = -1;
    sbar->oy = -1;
    sbar->leng = leng;
    sbar->max = max;
    if (cure > max)
        cure = max;
    sbar->cure = cure;
    sbar->page = page;
    sbar->next = NULL;
    sbar->function = fun;
    if (wnd->sbar == NULL) {
        wnd->sbar = sbar;
    } else {
        p = wnd->sbar;
        while (p->next != NULL)
            p = p->next;
        p->next = sbar;
    }
    return sbar;
}

float far UC_GetBoxTopCount(LISTBOX far *box)
{
    float cure = box->sbar->cure;

    if (box->top_count > cure)
        box->top_count = cure;
    else if (cure - box->top_count + 1 > box->height)
        box->top_count = cure - (box->height - 1);
    return box->top_count;
}

void far UC_UpdateListBox(WINDOWS far *wnd, LISTBOX far *box, char far **list, int n)
{
    char buf[1024];
    struct viewporttype vi;
    int x1, count, top, line_y, left, right, bottom;
    float top_count;
    register int i;

    top_count = box->top_count;
    x1 = wnd->left + wnd->vx + box->left + 1;
    line_y = wnd->top + wnd->vy + box->top + 2;
    count = box->height;
    left = wnd->left + wnd->vx + box->left;
    top = wnd->top + wnd->vy + box->top;
    right = left + (box->width * wnd->syschar_size) / 2;
    bottom = top + box->height * (wnd->syschar_size + 2);

    getviewsettings(&vi);
    if (!UC_SecondViewport(left, top, right, bottom))
        return;
    settextstyle(0, 0, wnd->syschar_size);
    UC_MouseHide();
    for (i = 0; i < count; i++) {
        strcpy(buf, " ");
        if (i < n)
            strcat(buf, list[i]);
        while (strlen(buf) < box->width)
            strcat(buf, " ");
        if (box->sbar->cure == top_count) {
            setcolor(wnd->title_fgc_dis);
            setcolorbm(wnd->title_bkc_dis);
        } else {
            setcolor(wnd->menubkc);
            setcolorbm(wnd->disablecolor);
        }
        outtextbm(x1, line_y, buf);
        line_y += wnd->syschar_size + 2;
        top_count += 1.0;
    }
    Setviewport(vi.left, vi.top, vi.right, vi.bottom);
    UC_MouseShow();
}

INPUTLINE far * far UC_DefinePopBox(WINDOWS far *wnd, int left, int top, int width, int height, float max, char far *text, void (far *fun)(SCROLLBAR far *sbar, float prevcure), void (far *fun_sele)())
{
    INPUTLINE far *inp;
    LISTBOX far *box;

    inp = UC_DefineInputLine(wnd, left, top, width, text, 0, fun_sele);
    box = UC_DefineListBox(wnd, left, top + 16, width, height, max, fun, fun_sele, 0);
    inp->box = box;
    box->top += 5;
    box->sbar->top += 5;
    return inp;
}

LISTBOX far * far UC_DefineListBox(WINDOWS far *wnd, int left, int top, int width, int height, float max, void (far *fun)(SCROLLBAR far *sbar, float prevcure), void (far *fun_sele)(), char hide)
{
    LISTBOX far *box;
    int sbar_x, sbar_h;

    if (!UC_WindowVerify(wnd))
        return NULL;
    box = (LISTBOX far *)malloc(sizeof(LISTBOX));
    if (box == NULL)
        nomem("定义列表框");
    UC_GetRealXY(&left);
    UC_GetRealXY(&top);
    box->hide = hide;
    box->left = left;
    box->top = top;
    box->width = width;
    box->height = height;
    box->prev = NULL;
    box->next = NULL;
    box->top_count = 0.0;
    box->fun_sele = fun_sele;
    sbar_x = left + (width * wnd->syschar_size) / 2 + 1;
    sbar_h = height * (wnd->syschar_size + 2);
    box->sbar = UC_DefineScrollbar(wnd, 1, sbar_x, top, sbar_h, max, 0.0, (float)(height - 1), fun, hide);
    if (wnd->box_tail != NULL) {
        box->prev = wnd->box_tail;
        wnd->box_tail->next = box;
    }
    wnd->box_tail = box;
    if (wnd->box_head == NULL)
        wnd->box_head = box;
    return box;
}

void far UC_DisplayInputBox(WINDOWS far *wnd, INPUTLINE far *inp, int left, int top, int right, int bottom)
{
    int active;

    UC_MouseHide();
    setfillstyle(1, wnd->disablecolor);
    bar(left, top, right, bottom);
    setcolor(wnd->menubkc);
    rectangle(left, top, right, bottom);
    if (wnd->cure_type == 2 && UC_GetInputBox(wnd, wnd->cure_num) == inp)
        active = 0;
    else
        active = 1;
    UC_DisplayInputText(wnd, inp, active);
    UC_MouseShow();
}

void far UC_UpdateInputLine(WINDOWS far *wnd, INPUTLINE far *inp, char far *text)
{
    int x1, y1, x2, y2;

    if (text != NULL)
        strcpy(inp->text, text);
    else
        inp->text[0] = 0;
    x1 = wnd->left + wnd->vx + inp->left;
    y1 = wnd->top + wnd->vy + inp->top;
    x2 = x1 + (inp->width * wnd->syschar_size) / 2 + 4;
    y2 = y1 + wnd->syschar_size + 4;
    inp->cursx = 0;
    inp->curepos = 0;
    UC_DisplayInputBox(wnd, inp, x1, y1, x2, y2);
    inp_first = 1;
    UC_InputCursorMove(wnd);
}

INPUTLINE far * far UC_DefineInputLine(WINDOWS far *wnd, int left, int top, int width, char far *text, int length, void (far *fun_sele)())
{
    INPUTLINE far *inp;

    if (!UC_WindowVerify(wnd))
        return NULL;
    inp = (INPUTLINE far *)malloc(sizeof(INPUTLINE));
    if (inp == NULL)
        nomem("定义输入框");
    UC_GetRealXY(&left);
    UC_GetRealXY(&top);
    inp->left = left;
    inp->top = top;
    if (width < 3)
        width = 3;
    inp->width = width;
    inp->length = length;
    inp->text = text;
    inp->cursx = 0;
    inp->curepos = 0;
    inp->fun_sele = fun_sele;
    inp->box = NULL;
    inp->prev = NULL;
    inp->next = NULL;
    if (wnd->inpline_tail != NULL) {
        inp->prev = wnd->inpline_tail;
        wnd->inpline_tail->next = inp;
    }
    wnd->inpline_tail = inp;
    if (wnd->inpline_head == NULL)
        wnd->inpline_head = inp;
    return inp;
}

BUTTON far * far UC_DefinePressButton(WINDOWS far *wnd, void (far *fun)(), int left, int top, int width, int height, char far *text, int key)
{
    BUTTON far *btn;

    if (!UC_WindowVerify(wnd))
        return NULL;
    btn = (BUTTON far *)malloc(sizeof(BUTTON));
    if (btn == NULL)
        nomem("定义按钮");
    UC_GetRealXY(&left);
    UC_GetRealXY(&top);
    UC_GetRealXY(&width);
    UC_GetRealXY(&height);
    btn->disable = 0;
    btn->fun = fun;
    btn->left = left;
    btn->top = top;
    btn->width = width;
    if (height == 0)
        height = wnd->syschar_size + BUT_BORD;
    btn->height = height;
    btn->text = text;
    btn->key = key;
    btn->prev = NULL;
    btn->next = NULL;
    if (wnd->button_tail != NULL) {
        btn->prev = wnd->button_tail;
        wnd->button_tail->next = btn;
    }
    wnd->button_tail = btn;
    if (wnd->button_head == NULL)
        wnd->button_head = btn;
    return btn;
}

void far UC_DefineMenu(WINDOWS far *wnd, MENUS far *mnu)
{
    if (UC_WindowVerify(wnd)) {
        wnd->menu = mnu;
        wnd->vy += wnd->syschar_size + 5;
        if (wnd->minheight < wnd->vy)
            wnd->minheight = wnd->vy;
    }
}

void far UC_DefineStateLine(WINDOWS far *wnd, int height)
{
    wnd->stateline_height = height;
    wnd->vb += height;
    wnd->minheight += height;
}

LABEL far * far UC_DefineLabel(WINDOWS far *wnd, int left, int top, int hotkey, void (far *fun)(), char far *text, void (far *ico_addr)(), WORD icn)
{
    LABEL far *lbl, far *p;

    if (!UC_WindowVerify(wnd))
        return NULL;
    lbl = (LABEL far *)malloc(sizeof(LABEL));
    if (lbl == NULL)
        nomem("定义标签");
    lbl->hotkey = hotkey;
    lbl->fun = fun;
    lbl->left = left;
    lbl->top = top;
    lbl->text = text;
    lbl->ico_addr = ico_addr;
    lbl->xor_buf = NULL;
    lbl->and_buf = NULL;
    if (text == NULL) {
        if (!UC_CreateIcon((char far *)ico_addr, &lbl->xor_buf, &lbl->and_buf, &lbl->ico_width, &lbl->ico_depth, icn))
            lbl->ico_addr = NULL;
    }
    lbl->next = NULL;
    if (wnd->label == NULL) {
        wnd->label = lbl;
    } else {
        p = wnd->label;
        while (p->next != NULL)
            p = p->next;
        p->next = lbl;
    }
    return lbl;
}

void far UC_GetRealXY(int far *xy)
{
    float scale, val;

    if (*xy > 0) {
        scale = (float)(long)SYSCHAR_SIZE / 16.0;
        val = (float)(*xy) * scale;
        *xy = (int)val;
        if (val * 10.0 - (float)(*xy * 10) >= 5.0)
            (*xy)++;
    }
}

int far UC_RetReal(int xy)
{
    UC_GetRealXY(&xy);
    return xy;
}

void far UC_DefineWindowMinSize(WINDOWS far *wnd, int width, int height)
{
    if (UC_WindowVerify(wnd)) {
        wnd->minwidth = width + wnd->vx * 2;
        wnd->minheight = height + wnd->vx + wnd->vy;
        if (wnd->width < wnd->minwidth)
            wnd->width = wnd->minwidth;
        if (wnd->height < wnd->minheight)
            wnd->height = wnd->minheight;
    }
}

void far UC_DefineWindowIcon(WINDOWS far *wnd, void (far *ico)(void), WORD count, char far *title)
{
    if (UC_WindowVerify(wnd)) {
        if (ico != NULL)
            wnd->ICON = ico;
        if (title != NULL)
            wnd->ico_text = title;
        wnd->ico_count = count;
    }
}

void far UC_DefineDrawIconRect(WINDOWS far *wnd, void (far *fun)(WINDOWS far *wnd, RECT far *rect))
{
    wnd->DRAWICON = fun;
}

SCROLLBAR far * far UC_DefineWindowHScrollbar(WINDOWS far *wnd, void (far *fun)(SCROLLBAR far *sbar, float prevcure))
{
    return UC_DefineScrollbar(wnd, 0, 0, -1, 0, 0.0, 0.0, 0.0, fun, 1);
}

SCROLLBAR far * far UC_DefineWindowVScrollbar(WINDOWS far *wnd, void (far *fun)(SCROLLBAR far *sbar, float prevcure))
{
    return UC_DefineScrollbar(wnd, 1, -1, 0, 0, 0.0, 0.0, 0.0, fun, 1);
}

void far UC_DefineWindowEnable(WINDOWS far *wnd, int (far *fun)(WINDOWS far *wnd, int msg))
{
    if (UC_WindowVerify(wnd)) {
        wnd->ENABLE = fun;
    }
}

