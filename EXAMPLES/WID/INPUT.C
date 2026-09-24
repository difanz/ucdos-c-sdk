/*---------------------------------------------------------------
  程序: INPUT.C
  演示: 如何在窗口中创建输入框
----------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include "sdk.h"

void My_Begin(void);
void My_Widget(void);
void My_App(void);

char InpBuf[61];         // 预定义的输入缓冲区
char DrwBuf[61];         // 实际显示缓冲区

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
   setfillstyle(SOLID_FILL, 15);
   UC_WindowBar(wnd, 5, 5, wnd->width-wnd->vx-wnd->vr, 32);
   if (!strlen(DrwBuf)) return;        // 没有实际显示内容, 清除后返回
   setcolor(0);
   setcolorbm(15);
   settextstyle(0, 0, 32);
   UC_WindowPrintf(wnd, 5, 5, DT_OVER, DrwBuf);
   settextstyle(0, 0, 16);
}

void My_close(void)
{
   UC_CloseUCVision(NULL, NULL);       // 窗口关闭时，退出SDK
}

void My_resize(WINDOWS *wnd)
{
   wnd=wnd;
}

// 输入框中按回车后的回调函数
void cbInpok()
{
   strcpy(DrwBuf, InpBuf);
   My_redraw(UC_GetCurrentWindow());
}

// 界面元素定义过程
void My_Widget(void)
{
   WINDOWS *wnd;

   wnd=UC_DefineWindow(WS_MAIN | WS_NOSIZE,            // 无尺寸变化的主窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       CW_USEDEFAULT, CW_USEDEFAULT,   // 用默认尺寸
                       "InputBox 一个含输入框控制的应用窗口",
                       My_redraw,
                       My_close,
                       My_resize);

   UC_DefineInputLine(wnd,
                      10, 100,
                      50,            // 50个字符宽度
                      InpBuf,        // 预定义的输入缓冲区
                      60,            // 最长允许输入60个字符
                      cbInpok);      // 按回车后的回调函数

   UC_DefineActive(wnd, TYPE_INPUTLINE, 1);
   UC_WindowEnable(wnd);                               // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
}
