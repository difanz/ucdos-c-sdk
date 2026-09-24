/*---------------------------------------------------------------
  程序: LABEL.C
  演示: 如何建立窗口中的标签，如何将图标读入内存.
  说明: 当图形标签与按钮结合后，将产生图标按钮
----------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include "sdk.h"

void My_Begin(void);
void My_Widget(void);
void My_App(void);

void (*ICO_SOUND)(void);
void (*ICO_NOSOUND)(void);

void main()
{
   My_Begin();           // 调用SDK初始化标准过程
   My_Widget();          // 调用自己的屏幕元素定义
   My_App();             // 进入自己的应用
}

// 应用程序初始化过程
// 自己把两个静态图标文件读入内存当中, 也可以将图标用 BGIOBJ 转为 OBJ
// 文件, 加入工程, 直接链入最终的 EXE 程序文件中.
void My_Begin(void)
{
   if ((ICO_SOUND=UC_ReadIconFile("label1.ico"))==NULL) {
       printf ("label1.ico 文件未找到或出错!\n");
       exit(1);     // SDK 未初始化前, 用 exit 退出程序
   }
   if ((ICO_NOSOUND=UC_ReadIconFile("label2.ico"))==NULL) {
       printf ("label1.ico 文件未找到或出错!\n");
       exit(1);     // SDK 未初始化前, 用 exit 退出程序
   }
   UC_InitDesktop(SOLID_FILL,
                  WHITE,
                  "c:\\windows\\winlogo.bmp",
                  FUL_SCR);
   UC_InitUCVision(DETECT, DETECT);
}

void SoundOn()
{
   sound(30);
}

void SoundOff()
{
   nosound();
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
                       CW_USEDEFAULT, CW_USEDEFAULT,   // 用默认尺寸
                       "Label 一个含有标签及图标按钮的应用窗口",
                       My_redraw,
                       My_close,
                       My_resize);

   UC_DefineLabel(wnd,
                  54, 54,
                  SK_ALTO,              // 热键 ALT+O
                  NULL,
                  NULL,                 // 没有文字
                  ICO_SOUND,            // 打开声音图标
                  0);

   UC_DefinePressButton(wnd,
                        SoundOn,
                        50, 50,
                        40, 40,
                        NULL,           // 没有按钮文字
                        SK_ALTO);       // 热键ALT+O

   UC_DefineLabel(wnd,
                  94, 58,
                  NULL,                 // 没有热键
                  NULL,                 // 没有回调函数
                  "打开声音[O]",
                  NULL,                 // 没有图标
                  NULL);

   UC_DefineLabel(wnd,
                  54, 104,
                  NULL,                 // 热键 ALT+C
                  NULL,
                  NULL,                 // 没有文字
                  ICO_NOSOUND,          // 声音关闭图标
                  NULL);

   UC_DefinePressButton(wnd,
                        SoundOff,
                        50, 100,
                        40, 40,
                        NULL,               // 没有按钮文字
                        SK_ALTC);           // 热键 ALT+C
   UC_DefineLabel(wnd,
                  94, 108,
                  NULL,                     // 没有热键
                  NULL,
                  "关闭声音[C]",
                  NULL,                     // 没有图标
                  NULL);

   UC_DefineActive(wnd, TYPE_BUTTON, 1);    // 默认激活第一个按钮
   UC_WindowEnable(wnd);                    // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();                           // 直接进入 SDK 事件驱动主循环
}
