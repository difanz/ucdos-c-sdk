// 本程序是一定时器实现的系统信息函数, 可在程序编写调试过程中使用

#include <alloc.h>
#include <dos.h>
#include <time.h>
#include <string.h>
#include "sdk.h"

static WINDOWS *mi_win=NULL;
static intime=0;

void mi_redraw(WINDOWS *wnd)
{
   WORD tx, fx;
   DWORD fc;
   RECT rect;
   int s;
   char HEAPSTATE[10];

   UC_GetClientRect(wnd, &rect);      // 取出客户区范围
   UC_GetFreeXMS(&tx, &fx);
   fc=coreleft();
   s=heapcheck();
   if (s<0) strcpy(HEAPSTATE, "出错");
   else if (s==1) strcpy(HEAPSTATE, "空堆");
   else strcpy(HEAPSTATE, "正常");
   setcolor(0);
   setcolorbm(15);
   settextstyle(0, 0, 16);
   UC_DrawText(wnd, &rect, DT_OVER|DT_CENTER,
               "常规内存剩余: %lu 字节\n总共扩展内存: %i KB\n剩余扩展内存: %i KB\nHeap 状态: %s",
               fc, tx, fx, HEAPSTATE);
}

// 图标绘制回调函数
// 在图标区域显示当前堆的状态, 可方便程序的调试
void cbDrawIcon(WINDOWS *wnd, RECT *rect)
{
   int s;
   struct  time tm;

   setcolor(0);
   if (intime==0) {   // 定时器调用时, 不必画背景
      setfillstyle(1, 15);
      UC_WindowBar(wnd, 0, 0, wnd->width, wnd->height);  // 首先清除图标区域背景
      rectangle(wnd->left+rect->left, wnd->top+rect->top,
                wnd->left+rect->right, wnd->top+rect->bottom);
   }
   setcolorbm(15);
   settextstyle(0, 0, 12);
   gettime(&tm);
   UC_DrawText(wnd, rect, DT_CENTER | DT_VCENTER | DT_WORDBREAK | DT_OVER,
               "%02d:%02d:%02d", tm.ti_hour, tm.ti_min, tm.ti_sec);
}

// 定时器函数
void mis()
{
   intime=1;
   if (UC_BeginPaintOnBack(mi_win)) {   // 开始背景绘制
      if (mi_win->minmax==SW_SHOWMIN) { // 图标状态时
         UC_Cadi(mi_win, mi_win->left, mi_win->top);
      } else mi_redraw(mi_win);
      UC_EndPaintOnBack(mi_win);         // 结束背景绘制
   }
   intime=0;
}

void mi_close()
{
   UC_KillTimer(mis);
   UC_WindowClose();
   mi_win=NULL;
}


// 信息窗口函数
//===================
void UC_InfoWindow()
{
   if (mi_win) {  // 已经启动过, 只需激活即可
      UC_WindowEnable(mi_win);
      return;
   }

   mi_win=UC_DefineWindow(WS_MAIN,
                          CP_USEDEFAULT, CP_USEDEFAULT,
                          200, 100, "系统状态窗口",
                          mi_redraw, mi_close, NULL);
   UC_DefineDrawIconRect(mi_win,          // 定义图标的自绘回调函数
                         cbDrawIcon);
   UC_WindowEnable(mi_win);
   UC_SetTimer(mis, 1000);          // 定义定时器, 每秒钟显示一次状态
}
