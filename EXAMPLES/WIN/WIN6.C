/*---------------------------------------------------------------
  程序: WIN6.C
  演示: 如何建立一个含窗口菜单的窗口
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

void SoundOn(void)
{
   sound(30);
}

void SoundOff(void)
{
   nosound();
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

void Quit(void)
{
   My_close();             // 调用与窗口关闭回调函数
}

// 界面元素定义过程
void My_Widget(void)
{
   WINDOWS *wnd;

   static char *dmem[]={ "O 打开声音[O] ",
                         "C 关闭声音[C] ",
                         "-",
                         "X 退出[X]...  ",
                         NULL
   };

   static void (*dfun[])()={ SoundOn,
                             SoundOff,
                             NULL,
                             Quit
   };

   static MENUS tmenu[]= {
                           { "A 菜单功能[A] ", dmem, dfun, 0 },
                           { NULL,         NULL, NULL, NULL }
   };

   wnd=UC_DefineWindow(WS_MAIN | WS_NOSIZE,            // 无尺寸变化的主窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       CW_USEDEFAULT, CW_USEDEFAULT,   // 用默认尺寸
                       "Window6 一个含有菜单功能的应用窗口",
                       My_redraw,
                       My_close,
                       My_resize);
   UC_DefineMenu(wnd, tmenu);
   UC_WindowEnable(wnd);                               // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
}
