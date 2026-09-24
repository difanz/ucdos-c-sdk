/*---------------------------------------------------------------
  程序: RADIO.C
  演示: 如何实现单选钮功能
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

void SoundOnOff(int count)
{
   if (count==0) sound(100);
   else if (count==1) nosound();
}

// 界面元素定义过程
void My_Widget(void)
{
   WINDOWS *wnd;
   static char *rad[]={"打开声音", "关闭声音", NULL};

   wnd=UC_DefineWindow(WS_MAIN | WS_NOSIZE,            // 无尺寸变化的主窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       400, 200,
                       "Radio 含一组单选钮控制的应用窗口",
                       My_redraw,
                       My_close,
                       My_resize);

   UC_DefineGroupBox(wnd,
                     "声音控制[S]",
                     20, 20,
                     120, 100);

   UC_DefineRadioButton(wnd,
                        rad,              // 单选钮标签列表
                        1,                // 默认激活第二个
                        30, 60,
                        SK_ALTS,          // 热键 ALT+S
                        SoundOnOff);      // 回调函数

   UC_DefinePressButton(wnd,
                        My_close,
                        220, 40,
                        100, 30,
                        "退出[X]",
                        SK_ALTX);

   UC_DefineActive(wnd, TYPE_RADIO, 1);   // 默认单选钮激活
   UC_WindowEnable(wnd);                  // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
}
