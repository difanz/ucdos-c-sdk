/*---------------------------------------------------------------
  程序: BOTTON.C
  演示: 如何实现按钮功能
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

// 声音打开回调函数
void SoundOn(void)
{
   WINDOWS *wnd;

   wnd=UC_GetCurrentWindow();
   sound(100);
   UC_EnableObject(wnd, TYPE_BUTTON, 2);
   UC_DisableObject(wnd, TYPE_BUTTON, 1);
   UC_NewActive(wnd, TYPE_BUTTON, 2);
}

// 声音关闭回调函数
void SoundOff(void)
{
   WINDOWS *wnd;

   wnd=UC_GetCurrentWindow();
   nosound();
   UC_EnableObject(wnd, TYPE_BUTTON, 1);
   UC_DisableObject(wnd, TYPE_BUTTON, 2);
   UC_NewActive(wnd, TYPE_BUTTON, 1);
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
   UC_CloseUCVision(NULL, SoundOff);       // 窗口关闭时，退出SDK
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
                       400, 200,
                       "Button 含按钮功能的应用窗口",
                       My_redraw,
                       My_close,
                       My_resize);
   UC_DefinePressButton(wnd,
                        SoundOn,                       // 声音打开回调函数
                        90, 40,
                        200, 40,
                        "打开声音[O]",
                        SK_ALTO);                      // 热键 ALT+O
   UC_DefinePressButton(wnd,
                        SoundOff,                      // 声音关闭回调函数
                        90, 100,
                        200, 40,
                        "关闭声音[C]",
                        SK_ALTC);                      // 热键 ALT+C
   UC_DefineActive(wnd, TYPE_BUTTON, 1);               // 默认第一个按钮激活
   UC_WindowEnable(wnd);                               // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();                                      // 直接进入 SDK 事件驱动主循环
}
