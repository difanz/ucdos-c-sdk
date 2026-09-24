#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <string.h>
#include "sdk.h"
#include "uccalc.h"

void My_Begin(void);
void My_Widget(void);
void My_App(void);

WINDOWS *mainwnd;
classIB_Calc IB_CalcModule;        //必须声明计算器全局类, 类名不得改动

char resulttext[100] = {"result : "};

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
              "logo16.bmp",
              FUL_SCR);
    UC_InitUCVision( VGA, 0x12);
  }

void My_redraw(WINDOWS * /*wnd*/)
  {
    UC_WinOutTextXY(mainwnd, 20, 20, DT_OVER, resulttext);
    UC_WinOutTextXY(mainwnd, 20, 70, DT_OVER, "Number : 12345.6789");
  }

void My_close(void)
  {
    UC_CloseUCVision(NULL, NULL);       // 窗口关闭时，退出SDK
  }

void My_resize(WINDOWS * /*mainwnd*/)
  {
  }

void OpenCalc(void)
  {
    IB_CalcModule.Run();    //运行计算器
  }

void DosShell(void)
  {
    UC_ToDosPrompt();
  }

void Quit(void)
  {
    My_close();             // 调用与窗口关闭回调函数
  }

void GetCalcResult( void )
  {
    long double result;

    result = IB_CalcModule.GetResult();             //提取计算结果
    sprintf( resulttext, "result : %lg",(double)result);
    UC_RedrawCurrentWindow();
  }

void SetCalcNumber( void )
  {
    IB_CalcModule.SetNumber(12345.6789);            //设置计算数据
  }
// 界面元素定义过程
void My_Widget(void)
  {
    static char *dmem[]={
                  "O 打开计算器 [O] ",
                  "D DOS SHELL  [D] ",
                  "-",
                  "Q 退出[Q]...  ",
                  NULL
                };

    static void (*dfun[])()={
                     OpenCalc,
                     DosShell,
                     NULL,
                     Quit
                   };

    static MENUS tmenu[]= {
              { "M 菜单功能[M] ", dmem, dfun, 0 },
              { NULL,         NULL, NULL, NULL }
            };

    mainwnd=UC_DefineWindow(WS_MAIN | WS_NOSIZE,     // 无尺寸变化的主窗口
                            CP_MIDSCR, CP_MIDSCR,          // 窗口居中屏幕
                            400, 150,  // 用默认尺寸
                            "计算器应用实例",
                            My_redraw,
                            My_close,
                            My_resize);
    UC_DefineMenu(mainwnd, tmenu);
    UC_DefinePressButton( mainwnd,
                          GetCalcResult,                // 回调函数
                          270, 10,
                          90, 25,
                          "获取结果",
                          0);               // 热键 ALT+O
    UC_DefinePressButton( mainwnd,
                          SetCalcNumber,                // 回调函数
                          270, 60,
                          90, 25,
                          "置入数据",
                          0);               // 热键 ALT+O

    UC_WindowEnable(mainwnd);                        // 显示定义好的窗口
  }

// 应用主体
void My_App(void)
  {
    UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
  }

