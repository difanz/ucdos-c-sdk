#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <string.h>
#include "sdk.h"
#include "opendlg.h"

void My_Begin(void);
void My_Widget(void);
void My_App(void);

WINDOWS *wnd;
classOpenDlg* ibOpenDlg = NULL;    //必须声明 FileOpen 对话框全局类指针

char pathname[MAXPATH+20] =           {"Full path name : "};
char filetitle[MAXFILE+1+MAXEXT+20] = {"File title     : "};
char filename[MAXFILE+20] =           {"Only filename  : "};
char fileext[MAXEXT+20] =             {"Only ext       : "};
char title[20] = {"Owner Title"};

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
              "ucmain.bmp",
              FUL_SCR);
    UC_InitUCVision( VGA, 0x12);
  }

void My_redraw(WINDOWS * /*wnd*/)
  {
    UC_WinOutTextXY(wnd, 20, 20, DT_OVER, pathname);
    UC_WinOutTextXY(wnd, 20, 40, DT_OVER, filetitle);
    UC_WinOutTextXY(wnd, 20, 60, DT_OVER, filename);
    UC_WinOutTextXY(wnd, 20, 80, DT_OVER, fileext);
  }

void My_close(void)
  {
    UC_CloseUCVision(NULL, NULL);       // 窗口关闭时，退出SDK
  }

void My_resize(WINDOWS * /*wnd*/)
  {
  }

void OpenFile(void)
  {
    LPSTR fn;

    ibOpenDlg = new classOpenDlg( TRUE, NULL, NULL, 0,         //初始化
                                    "C files (*.cpp)|*.cpp|"
                                    "H files   (*.h)|*.h  |"
                                    "All files (*.*)|*.*  ||");

    ibOpenDlg->file_attr |= _A_NORMAL;      //设置文件列表框中文件的属性
    ibOpenDlg->dir_attr  |= _A_NORMAL;      //设置目录列表框中目录的属性
    ibOpenDlg->a_ofFileMustExist = TRUE;    //设置所选择文件必须存在
    ibOpenDlg->DlgTitle = title;            //设置自定义对话框标题

    if(ibOpenDlg->DoModal()==ID_OK)         //显示对话框并允许用户作出选择
      {
        putchar('\007');

        fn = ibOpenDlg->GetPathName();      //提取选择文件的完全路径
        _fmemmove( pathname+17, fn, _fstrlen(fn)+1 );

        fn = ibOpenDlg->GetFileTitle();     //提取选择文件的标题
        _fmemmove( filetitle+17, fn, _fstrlen(fn)+1 );

        fn = ibOpenDlg->GetFileName();      //提取选择文件的文件名
        _fmemmove( filename+17, fn, _fstrlen(fn)+1 );

        fn = ibOpenDlg->GetFileExt();       //提取选择文件的扩展名
        _fmemmove( fileext+17, fn, _fstrlen(fn)+1 );

        UC_RedrawWindow(wnd);
      }

    delete ibOpenDlg;
    ibOpenDlg = NULL;
  }

void SaveFile(void)
  {
    LPSTR fn;

    ibOpenDlg = new classOpenDlg( FALSE, NULL, NULL, 0,         //初始化
                                    "C files (*.cpp)|*.cpp|"
                                    "H files   (*.h)|*.h  |"
                                    "All files (*.*)|*.*  ||");

    ibOpenDlg->file_attr |= _A_NORMAL;      //设置文件列表框中文件的属性
    ibOpenDlg->dir_attr  |= _A_NORMAL;      //设置目录列表框中目录的属性
//    ibOpenDlg->DlgTitle = title;            //设置自定义对话框标题

    if(ibOpenDlg->DoModal()==ID_OK)         //显示对话框并允许用户作出选择
      {
        putchar('\007');

        fn = ibOpenDlg->GetPathName();      //提取选择文件的完全路径
        _fmemmove( pathname+17, fn, _fstrlen(fn)+1 );

        fn = ibOpenDlg->GetFileTitle();     //提取选择文件的标题
        _fmemmove( filetitle+17, fn, _fstrlen(fn)+1 );

        fn = ibOpenDlg->GetFileName();      //提取选择文件的文件名
        _fmemmove( filename+17, fn, _fstrlen(fn)+1 );

        fn = ibOpenDlg->GetFileExt();       //提取选择文件的扩展名
        _fmemmove( fileext+17, fn, _fstrlen(fn)+1 );

        UC_RedrawWindow(wnd);
      }

    delete ibOpenDlg;
    ibOpenDlg = NULL;
  }

void DosShell(void)
  {
    UC_ToDosPrompt();
  }

void Quit(void)
  {
    My_close();             // 调用与窗口关闭回调函数
  }

// 界面元素定义过程
void My_Widget(void)
  {
    static char *dmem[]={
                  "O 打开文件 [O] ",
                  "S 存储文件 [S] ",
                  "D DOS SHELL[D] ",
                  "-",
                  "Q 退出[Q]...  ",
                  NULL
                };

    static void (*dfun[])()={
                     OpenFile,
                     SaveFile,
                     DosShell,
                     NULL,
                     Quit
                   };

    static MENUS tmenu[]= {
              { "M 菜单功能[M] ", dmem, dfun, 0 },
              { NULL,         NULL, NULL, NULL }
            };

    wnd=UC_DefineWindow(WS_MAIN | WS_NOSIZE,     // 无尺寸变化的主窗口
                  CP_MIDSCR, CP_MIDSCR,          // 窗口居中屏幕
                  CW_USEDEFAULT, CW_USEDEFAULT,  // 用默认尺寸
                  "FileOpen 应用窗口实例",
                  My_redraw,
                  My_close,
                  My_resize);
    UC_DefineMenu(wnd, tmenu);
    UC_WindowEnable(wnd);                        // 显示定义好的窗口
  }

// 应用主体
void My_App(void)
  {
    UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
  }

