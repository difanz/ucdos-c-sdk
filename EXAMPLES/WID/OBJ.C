/*---------------------------------------------------------------
  程序: OBJ.C
  演示: 如何在窗口中创建构件后又将其删除; 删除后又将其重新定义
----------------------------------------------------------------*/

#include <stdlib.h>
#include <dos.h>
#include "sdk.h"

void My_Begin(void);
void My_Widget(void);
void My_App(void);

WORD fq=0;

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

// 窗口重画函数, 以大字显示当前的音量值
void My_redraw(WINDOWS *wnd)
{
   RECT rect;

   UC_GetClientRect(wnd, &rect);
   settextstyle(0, 0, 48);
   setcolor(1);
   setcolorbm(15);
   UC_DrawText(wnd,
               &rect,
               DT_CENTER | DT_VCENTER |DT_OVER,
               " 音量=%i ", fq);
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

// 滚动条变化后的回调函数, 根据滚动条当前值发出不同的声音
void cbVbar(SCROLLBAR *sbar, float prevcure)
{
   WINDOWS *wnd;

   prevcure=prevcure;
   fq=sbar->cure*10;
   if (fq) sound(fq);
   else nosound();
   My_redraw(UC_GetCurrentWindow());
}

// 删除后重新定义滚动条与标签
void isyes()
{
   WINDOWS *wnd;

   wnd=UC_GetCurrentWindow();
   UC_DefineLabel(wnd,
                  30, 240,
                  NULL,             // 无热键
                  NULL,             // 无回调函数
                  "音量控制",
                  NULL,             // 无图标
                  NULL);

   UC_DefineScrollbar(wnd,
                      V_BAR,        // 垂直滚动
                      50, 30,       // 左上角坐标
                      200,          // 滚动条长度
                      100,          // 逻辑最大值
                      0,            // 逻辑当前值
                      10,           // 逻辑页变化值
                      cbVbar,       // 滚动条变化后的回调函数
                      1);           // 非隐藏状态

   UC_RedrawWindow(wnd);  // 重新刷新窗口
}

void ObjDelete()
{
   if (UC_DestroyObject(UC_GetCurrentWindow(), TYPE_SCROLLBAR, 1)) {
       UC_DestroyObject(UC_GetCurrentWindow(), TYPE_LABEL, 1);
   }
   else UC_DialogYesNo(isyes, NULL, NULL,
                       "滚动条部件已经删除!\n是否重新定义?");
}

// 界面元素定义过程
void My_Widget(void)
{
   WINDOWS *wnd;

   wnd=UC_DefineWindow(WS_MAIN | WS_NOSIZE,            // 无尺寸变化的主窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       CW_USEDEFAULT, CW_USEDEFAULT,   // 用默认尺寸
                       "Object 一个部件的定义与删除控制",
                       My_redraw,
                       My_close,
                       My_resize);

   UC_DefineLabel(wnd,
                  30, 240,
                  NULL,             // 无热键
                  NULL,             // 无回调函数
                  "音量控制",
                  NULL,             // 无图标
                  NULL);

   UC_DefineScrollbar(wnd,
                      V_BAR,        // 垂直滚动
                      50, 30,       // 左上角坐标
                      200,          // 滚动条长度
                      100,          // 逻辑最大值
                      0,            // 逻辑当前值
                      10,           // 逻辑页变化值
                      cbVbar,       // 滚动条变化后的回调函数
                      1);           // 非隐藏状态

   UC_DefinePressButton(wnd,
                        ObjDelete,              // 回调函数
                        100, 200,
                        120, 30,
                        "删除滚动条[D]",
                        SK_ALTD);               // 热键 ALT+D

   UC_DefineActive(wnd, TYPE_BUTTON, 1);
   UC_WindowEnable(wnd);                               // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
}
