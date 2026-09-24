/* ------------------------------------------------------------------
   程序: TIMER.C
   演示: 通过定时器的使用, 建立一个后台窗口, 每隔一秒显示一次当前系统
         内存与堆状态(heap)的报告; 该窗口极小化时也可在图标中显示堆的
         状态.
         仿照本例子, 可以编制出一个图标状态的时钟.
   补充: 如果将本程序修改为一个函数, 则可在应用程序中进行调用, 随时监
         测并报告系统状态, 在程序设计阶段起到很好的辅助调试效果.
-------------------------------------------------------------------*/
#include <alloc.h>
#include <dos.h>
#include <string.h>
#include "sdk.h"

WINDOWS *mi_win=NULL;

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
   char HEAPSTATE[10];

   s=heapcheck();
   if (s<0) strcpy(HEAPSTATE, "出错");
   else if (s==1) strcpy(HEAPSTATE, "空堆");
   else strcpy(HEAPSTATE, "正常");
   setfillstyle(1, 15);
   UC_WindowBar(wnd, 0, 0, wnd->width, wnd->height);  // 首先清除图标区域背景
   settextstyle(0, 0, wnd->syschar_size);  // 用标准字体
   setcolor(0);                            // 黑色
   UC_DrawText(wnd, rect, DT_CENTER | DT_VCENTER, HEAPSTATE);
}

// 定时器函数
void mis()
{
        if (UC_BeginPaintOnBack(mi_win)) {   // 开始背景绘制
           if (mi_win->minmax==SW_SHOWMIN) { // 图标状态时
         UC_Cadi(mi_win, mi_win->left, mi_win->top);
      } else mi_redraw(mi_win);
                UC_EndPaintOnBack(mi_win);         // 结束背景绘制
        }
}

void mi_close()
{
   UC_CloseUCVision(NULL, NULL);
}

void main(void)
{
   UC_InitDesktop(SOLID_FILL,
                  WHITE,
                  "c:\\windows\\winlogo.bmp",
                  FUL_SCR);
   UC_InitUCVision(DETECT, DETECT);

        mi_win=UC_DefineWindow(WS_MAIN,
                          CP_MIDSCR, CP_MIDSCR,
                          350, 100, "Timer 定时器实现的实时系统状态报告",
                          mi_redraw, mi_close, NULL);
   UC_DefineDrawIconRect(mi_win,          // 定义图标的自绘回调函数
                         cbDrawIcon);
        UC_WindowEnable(mi_win);
        UC_SetTimer(mis, 1000);          // 定义定时器, 每秒钟显示一次状态
   UC_MainLoop();
}
