/*---------------------------------------------------------------
  程序: WIN5.C
  演示: 如何建立一个自己绘制背景的窗口
  说明: 本程序使用 win5.bmp 铺满一个自己绘制的窗口背景
        同时定义一个图标自绘制函数, 使窗口极小化图标的显示也可自绘制
----------------------------------------------------------------*/

#include <stdlib.h>
#include <dir.h>

#include "sdk.h"

void My_Begin(void);
void My_Widget(void);
void My_App(void);

WORD bmphandle=0;
char bmpname[MAXPATH]="win5.bmp";

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
   if (UC_CreateBitmap(bmpname)) {         // 创建 win5.bmp 的设备无关图
       bmphandle=UC_XMSLoadDDB(bmpname);   // 为提高速度, 将设备无关图装入 XMS
   }
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

// 背景绘制回调函数
void cbDrawBK(WINDOWS *wnd)
{
   UC_DrawDDB(wnd,
              bmphandle,                // 装入 XMS 中的 BMP 句柄
              bmpname,                  // BMP 文件名
              0, 0,                     // 左上角
              wnd->width, wnd->height,  // 允许显示的宽度/高度
              FUL_SCR,                  // 显示模式为铺满窗口
              COPY_PUT);                // COPY 显示方式
}

// 极小化图标自绘制回调函数
void cbDrawIBK(WINDOWS *wnd, RECT *rect)
{
   int width, height;

   width=rect->right-rect->left+1;
   height=rect->bottom-rect->top+1;
   UC_DrawDDB(wnd,
              bmphandle,                // 装入 XMS 中的 BMP 句柄
              bmpname,                  // BMP 文件名
              rect->left, rect->top,    // 左上角
              width, height,            // 允许显示的宽度/高度
              FUL_SCR,                  // 显示模式为铺满窗口
              COPY_PUT);                // COPY 显示方式
}

// 界面元素定义过程
void My_Widget(void)
{
   WINDOWS *wnd;

   wnd=UC_DefineWindow(WS_MAIN,                        // 创建一个标准应用窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       CW_USEDEFAULT, CW_USEDEFAULT,   // 用默认尺寸
                       "Window5 一个自己绘制背景的应用窗口(含图标自绘制)",
                       My_redraw,
                       My_close,
                       My_resize);
   wnd->boardstyle=NULL;                               // 取消默认的窗口背景填充
   UC_DefineDrawBackground(wnd, cbDrawBK);             // 定义背景绘制回调函数
   UC_DefineDrawIconRect(wnd, cbDrawIBK);              // 定义极小化图标绘制回调函数
   UC_WindowEnable(wnd);                               // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
}
