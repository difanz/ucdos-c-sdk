/*---------------------------------------------------------------
  程序: WIN2.C
  演示: 如何建立一个含有标准滚动条的窗口
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
   wnd=wnd;
}

void My_close(void)
{
   UC_CloseUCVision(NULL, NULL);       // 窗口关闭时，退出SDK
}

void My_resize(WINDOWS *wnd)
{
   wnd=wnd;
}

// 横向滚动条回调函数
void cbHbar(SCROLLBAR *sbar, float prevcure)
{
   sbar=sbar;
   prevcure=prevcure;
}

// 纵向滚动条回调函数
void cbVbar(SCROLLBAR *sbar, float prevcure)
{
   sbar=sbar;
   prevcure=prevcure;
}

// 界面元素定义过程
void My_Widget(void)
{
   WINDOWS *wnd;

   wnd=UC_DefineWindow(WS_MAIN,                        // 建立标准模式的主窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       CW_USEDEFAULT, CW_USEDEFAULT,   // 用默认尺寸
                       "Window2 含标准滚动条窗口",
                       My_redraw,
                       My_close,
                       My_resize);
   UC_DefineWindowHScrollbar(wnd, cbHbar);
   UC_DefineWindowVScrollbar(wnd, cbVbar);
   UC_WindowEnable(wnd);                               // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
}
