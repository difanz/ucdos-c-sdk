// TIMER.C - 内部时钟中断驱动

typedef struct tagTIMER {
    void (far *fun)();
    WORD reload;
    WORD count;
    struct tagTIMER far *prev;
    struct tagTIMER far *next;
} TIMER;

char timeron = 0;
WORD MOUSETIME = 0;

int timer_head;
int timer_tail;
TIMER far *tif_head;
TIMER far *tif_tail;
void (far *TIMERFUN)();
void (far *timerlist[50])();
void (interrupt far *old_int8)();

void interrupt far Myint8(void);
void far MyFREE(void far *p);

void far setint8(void)
{
    old_int8 = _dos_getvect(8);
    _dos_setvect(8, Myint8);
}

void far biosint8(void)
{
    _dos_setvect(8, old_int8);
}

void far sendmsg(TIMER far *t)
{
    for (_CX = timer_head; _CX != timer_tail; ) {
        if (t->fun == timerlist[_CX])
            return;
        _CX++;
        if (_CX == 50)
            _CX = 0;
    }
    timerlist[timer_tail] = t->fun;
    timer_tail++;
    if (timer_tail == 50)
        timer_tail = 0;
}

void interrupt far Myint8(void)
{
    TIMER far *p;
    (*old_int8)();
    MOUSETIME++;
    if (timeron == 0)
        return;
    timeron = 0;
    for (p = tif_head; p != NULL; p = p->next) {
        if (p->count == 0) {
            sendmsg(p);
            p->count = p->reload;
        } else {
            p->count--;
        }
    }
    timeron = 1;
}

int far UC_CheckTimer(void)
{
    if (timer_head != timer_tail) {
        TIMERFUN = timerlist[timer_head];
        timer_head++;
        if (timer_head == 50)
            timer_head = 0;
        if (TIMERFUN != NULL)
            return 1;
    }
    return 0;
}

void far UC_InitTimer(void)
{
    tif_head = tif_tail = NULL;
    timer_head = timer_tail = 0;
}

int far UC_SetTimer(void (*fun)(), WORD msec)
{
    TIMER far *p;
    for (p = tif_head; p != NULL; p = p->next) {
        if (p->fun == fun) {
            p->reload = msec / 55;
            p->count = msec / 55;
            timeron = 1;
            return 1;
        }
    }
    if ((p = (TIMER far *)malloc(sizeof(TIMER))) == NULL)
        return 0;
    p->fun = fun;
    p->reload = msec / 55;
    p->count = msec / 55;
    p->next = p->prev = NULL;
    if (tif_tail != NULL) {
        p->prev = tif_tail;
        tif_tail->next = p;
    }
    tif_tail = p;
    if (tif_head == NULL)
        tif_head = p;
    timeron = 1;
    return 1;
}

void far UC_KillTimer(void (*fun)())
{
    TIMER far *p;
    timeron = 0;
    for (p = tif_head; p != NULL; p = p->next) {
        if (p->fun == fun) {
            if (p->next != NULL)
                p->next->prev = p->prev;
            if (p->prev != NULL)
                p->prev->next = p->next;
            if (tif_head == p)
                tif_head = p->next;
            if (tif_tail == p)
                tif_tail = p->prev;
            p->next = p->prev = NULL;
            for (_CX = timer_head; _CX != timer_tail; _CX++) {
                if (p->fun == timerlist[_CX])
                    timerlist[_CX] = NULL;
            }
            MyFREE(p);
            break;
        }
    }
    timeron = 1;
}
