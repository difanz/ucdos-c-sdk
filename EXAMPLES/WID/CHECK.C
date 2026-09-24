/*---------------------------------------------------------------
  程序: CHECK.C
  演示: 如何实现复选钮功能
----------------------------------------------------------------*/

#include <stdlib.h>
#include <dos.h>

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
   UC_CloseUCVision(NULL, nosound);       // 窗口关闭时，退出SDK
}

void My_resize(WINDOWS *wnd)
{
   wnd=wnd;
}

// 单选钮选中后回调函数处理过程, 传递进来的 state 为单选钮激活状态标记
void SoundOnOff(int state)
{
   switch (state) {
          case WM_ENABLE:
               sound(100);           // 激活时发声
               break;
          case WM_DISABLE:
               nosound();            // 禁止时无声
        }
}

// 界面元素定义过程
void My_Widget(void)
{
   WINDOWS *wnd;

   wnd=UC_DefineWindow(WS_MAIN | WS_NOSIZE,            // 无尺寸变化的主窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       400, 200,
                       "Check 含复选钮的控制的应用程序窗口",
                       My_redraw,
                       My_close,
                       My_resize);

   UC_DefineCheckButton(wnd,
                        "声音开关[S]",
                        WM_DISABLE,         // 默认关闭
                        130, 20,
                        SK_ALTS,
                        SoundOnOff);        // 单选钮选中回调函数

   UC_DefinePressButton(wnd,
                        My_close,
                        150, 120,
                        100, 30,
                        "退出[X]",
                        SK_ALTX);

   UC_DefineActive(wnd, TYPE_BUTTON, 1);   // 默认单选钮激活
   UC_WindowEnable(wnd);                   // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();                          // 直接进入 SDK 事件驱动主循环
}
