/*---------------------------------------------------------------
  程序: WIN3.C
  演示: 如何建立一个含状态行的窗口
----------------------------------------------------------------*/

#include <stdlib.h>
#include "sdk.h"

void My_Begin(void);
void My_Widget(void);
void My_App(void);

void main()
{
   My_Begin();           // 调用SDK初始化标准过程
   My_Widget();          // 调用自己的屏幕元素定义
   My_App();             // 进入自己的应用
}

// 应用程序初始化过程
void My_Begin(void)
{
   UC_InitDesktop(SOLID_FILL,
                  WHITE,
                  "c:\\windows\\winlogo.bmp",
                  FUL_SCR);
   UC_InitUCVision(DETECT, DETECT);
}

void My_redraw(WINDOWS *wnd)
{
   RECT rect;

   if (UC_BeginPaintStateLine(wnd)){        // 开始状态行的绘制工作
       UC_GetStateLineRect(wnd, &rect);     // 之后可以使用任何SDK绘制函数
       UC_DrawText(wnd,
                   &rect,
                   DT_CENTER | DT_VCENTER,
                   "窗口状态行正文");
       UC_EndPaintStateLine(wnd);           // 结束状态行绘制
   }
}

void My_close(void)
{
   UC_CloseUCVision(NULL, NULL);       // 窗口关闭时，退出SDK
}

void My_resize(WINDOWS *wnd)
{
   wnd=wnd;
}

// 界面元素定义过程
void My_Widget(void)
{
   WINDOWS *wnd;

   wnd=UC_DefineWindow(WS_MAIN | WS_NOSIZE,            // 无尺寸变化的主窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       CW_USEDEFAULT, CW_USEDEFAULT,   // 用默认尺寸
                       "Window3 一个含状态行显示的窗口",
                       My_redraw,
                       My_close,
                       My_resize);
   UC_DefineStateLine(wnd, 20);                        // 状态行高度定义为20点
   UC_WindowEnable(wnd);                               // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
}
