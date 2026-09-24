// 说明: 本程序具有编辑器的基本特征, 但并非一个实用的编辑器.
//       仅作为编程例子提供, 若有兴趣, 可发展.

extern void mulpad();

#include <stdlib.h>
#include <stdio.h>
#include <dir.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>
#include <mem.h>
#include <dos.h>
#include <alloc.h>
#include "sdk.h"
#include "edit1.c"
#include "edit2.c"

WINDOWS *main_win;


void My_begin(void)
{
   UC_InitDesktop(1, 3, NULL, NULL); //"c:\\UCDOS\\marble.bmp", FUL_SCR);
   UC_InitUCVision(VGA, VM_640X480X16);
   edw_head=NULL;
   edw_tail=NULL;
}

void main_redraw(WINDOWS *wnd)
{
   wnd=wnd;
}

void main_close()
{
   ED_Quit();
}

void main_resize(WINDOWS *wnd)
{
  wnd=wnd;
}

void My_app(void)
{
   static char *menu_text1[]={
                              "N 新文件[N]         ",
                              "O 打开[O]...     F3 ",
                              "S 存盘[S]        F2 ",
                              "R 换名存盘[R]...    ",
                              "L 全部存盘[L]       ",
                              "-",
                              "P 打印[P]...        ",
                              "D DOS命令[D]        ",
                              "-",
                              "A 重排图标[A]       ",
                              "X 退出[X]...  ALT+X ",
                              NULL
        };
        static void (*menu_func1[])()={
                                       ED_NewFile,
                                       ED_OpenFile,
                                       ED_Save,
                                       ED_SaveAs,
                                       ED_SaveAll,
                                       NULL,
                                       ED_Print,
                                       ED_DosShell,
                                       NULL,
                                       UC_ArrangeIcons,
                                       ED_Quit,
                                       NULL
        };
        static MENUS tmenu[]={
                              { "F 文件[F]  ",   menu_text1, menu_func1, 0   },
                              { NULL,          NULL,       NULL,        NULL }
        };

        main_win=UC_DefineWindow(WS_MAIN,
                                 0, 0, 300, 0, "简易编辑器",
                                 main_redraw, main_close, main_resize);
        UC_DefineWindowIcon(main_win, mulpad, 1, NULL);
        UC_DefineMenu(main_win, tmenu);
        UC_WindowEnable(main_win);
        UC_MainLoop();
}

void main()
{
   My_begin();
   My_app();
}
