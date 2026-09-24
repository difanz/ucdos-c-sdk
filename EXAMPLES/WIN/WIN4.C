/*---------------------------------------------------------------
  程序: WIN4.C
  演示: 如何建立一个自定义图标的窗口
  说明: 本程序使用 win4.ico 作为窗口自定义的最小化图标,
        如果窗口中不包含 WS_MAIN 属性, 那么将无法最大化和最小化,
        由此图标定义也就无效了.
----------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <alloc.h>
#include <fcntl.h>
#include <io.h>

#include "sdk.h"

void My_Begin(void);
void My_Widget(void);
void My_App(void);

void (*MyICO)(void);       // 图标数据指针

void main()
{
   My_Begin();           // 调用SDK初始化标准过程
   My_Widget();          // 调用自己的屏幕元素定义
   My_App();             // 进入自己的应用
}

/*---------------------------------------------------------------
应用程序初始化过程
    自己将 ICO 文件读入内存当中;
    除了临时将图标文件读入内存的方法外, 也可以使用 Borland 提供的
BGIOBJ 程序将图标文件转换成 OBJ 文件, 加入到工程中, 直接链入最后
的 EXE 程序文件中.
---------------------------------------------------------------*/
void My_Begin(void)
{

   if ((MyICO=UC_ReadIconFile("win4.ico"))==NULL) {
       printf("win4.ico 文件找不到或者文件出错!\n");
       exit(1);   // 尚未初始化SDK时使用exit退出
   }

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

// 界面元素定义过程
void My_Widget(void)
{
   WINDOWS *wnd;

   wnd=UC_DefineWindow(WS_MAIN,                        // 建立标准模式的主窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       CW_USEDEFAULT, CW_USEDEFAULT,   // 用默认尺寸
                       "Window4 自定义图标的窗口(最小化时显示)",
                       My_redraw,
                       My_close,
                       My_resize);
   UC_DefineWindowIcon(wnd, MyICO, 0, "自定义图标及说明文字");
   UC_WindowEnable(wnd);                               // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
}
