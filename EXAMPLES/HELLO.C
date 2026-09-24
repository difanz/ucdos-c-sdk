/*-----------------------------------------------------------------
  程序: HELLO.C
  演示: 经典的 "Hello, World!" 是如何制作出来的.
  说明: 本程序开一个标准应用程序窗口, 在窗口中间显示 "Hello, World"
-------------------------------------------------------------------*/

#include <stdlib.h>
#include "sdk.h"

void My_Begin(void);
void My_Widget(void);
void My_App(void);

void main()
{
   My_Begin();
   My_Widget();
   My_App();
}

// SDK 应用程序初始化标准过程
void My_Begin(void)
{
   UC_InitDesktop(SOLID_FILL,                       // 桌面填充图案
                  WHITE,                            // 颜色
                  "c:\\windows\\leaves.bmp",        // 桌面贴图
                  FUL_SCR);                         // 贴图模式
   UC_InitUCVision(DETECT, DETECT);                 // 初始化 SDK
}

void main_redraw(WINDOWS *wnd)
{
   UC_WindowPrintf(wnd, -1, -1, DT_NOOVER, "Hello, World!");
}

void main_close()
{
   UC_CloseUCVision(NULL, NULL);    // SDK 应用程序退出
}

void main_resize(WINDOWS *wnd)
{
  wnd=wnd;
}

// 界面定义过程
void My_Widget(void)
{
   WINDOWS *wnd;

   wnd=UC_DefineWindow(WS_MAIN,               // 窗口风格
                       CP_MIDSCR,             // X坐标居中
                       CP_MIDSCR,             // Y坐标居中
                       CW_USEDEFAULT,         // 默认宽度
                       CW_USEDEFAULT,         // 默认高度
                       "Hello",               // 窗口标题
                       main_redraw,           // 窗口重画的回调函数
                       main_close,            // 窗口关闭的回调函数
                       main_resize            // 窗口尺寸变化的回调函数
        );
   UC_WindowEnable(wnd);                      // 显示定义好的窗口
}

// 应用主体过程
void My_App(void)
{
   UC_MainLoop();                 // 进入事件处理循环
}
