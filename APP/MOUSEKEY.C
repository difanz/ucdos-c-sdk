/*---------------------------------------------------------------
  程序: MOUSEKEY.C
  演示: 如何在窗口中进行键盘及鼠标事件处理
        同时也演示了文本插入光标的使用方法

  本程序建立一个最原始的文字录入窗口, 通过键盘事件的控制用箭头键
  移动光标, 并可显示接收的ASCII英文字符, 但汉字不能显示(请分析为
  什么?); 另外, 自定义鼠标事件可以支持将文本光标移动到用鼠标点下
  的地方, 从新位置接着输入; 而鼠标在客户区双击, 可以产生退出窗口.
----------------------------------------------------------------*/

#include <stdlib.h>
#include "sdk.h"

int cxPos, cyPos;        // 当前的文字光标位置
int AscWidth, AscHeight; // 当前的文字宽度和高度点
int MaxWidth, MaxHeight; // 当前窗口的最宽和最高行列

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

// 重画函数没有处理
// 所以一旦出现 redraw, 屏幕内容将消除
void My_redraw(WINDOWS *wnd)
{
   cxPos=cyPos=0;
   UC_MoveCaret(wnd, 0, 0);            // 光标位置回0
}

void My_close(void)
{
   UC_CloseUCVision(NULL, NULL);       // 窗口关闭时，退出SDK
}

// 窗口尺寸变化时, 重新计算最大的行列
// 并将光标位置回0
void My_resize(WINDOWS *wnd)
{
   MaxWidth=(wnd->width-wnd->vx-wnd->vr)/AscWidth-1;
   MaxHeight=(wnd->height-wnd->vy-wnd->vb)/AscHeight-1;
   cxPos=cyPos=0;
   UC_MoveCaret(wnd, 0, 0);   // 光标位置回0
}

// 键盘事件回调函数
int cbKeyboard(int keycode, char keystate)
{
   WINDOWS *wnd;
   BYTE ascii;
   int x, y;

   keystate=keystate;
   wnd=UC_GetCurrentWindow(); // 键盘事件总是在当前窗口中进行

   switch (keycode) {         // 处理光标移动键
          case SK_ARROWUP:
                  if (cyPos) cyPos--;
                  goto tonewpos;
          case SK_ARROWDOWN:
                  if (cyPos<MaxHeight) cyPos++;
                  goto tonewpos;
          case SK_ARROWLEFT:
                  if (cxPos) cxPos--;
                  goto tonewpos;
          case SK_ARROWRIGHT:
                  if (cxPos<MaxWidth) cxPos++;
                  goto tonewpos;
   }
   ascii=keycode&0x00ff;      // 得到ASCII码
   if (!ascii) return NULL;   // 扩展按键不处理
   x=cxPos*AscWidth;
   y=cyPos*AscHeight;
   setcolor(0);               // 字符用黑色显示
   setcolorbm(15);            // 白色背景
   settextstyle(0, 0, AscHeight);
   UC_HideCaret(wnd);            // 窗口客户区绘制前, 应先将插入光标关闭
   UC_WindowPrintf(wnd, x, y, DT_OVER, "%c", ascii);
   UC_ShowCaret(wnd);            // 绘制后, 可将插入光标显示
   cxPos++;
   if (cxPos>MaxWidth) {
      cxPos=0;
      cyPos++;
      if (cyPos>MaxHeight) cyPos=0;
   }

tonewpos:

   x=cxPos*AscWidth;          // 计算新的位置
   y=cyPos*AscHeight;
   UC_MoveCaret(wnd, x, y);   // 移动光标到新位置
   return 1;                  // 函数返回非0, 表示按键已经处理
}

// 鼠标事件回调函数
// 因为要处理左键双击动作, 所以应该首先识别左键消息,
// 以免右键的双击消息也进行处理.
void cbMouse(int x, int y, WORD s)
{
   WINDOWS *wnd;

   if ( !(s & USM_LEFT) ) return;   // 非左键消息, 返回不处理

   if (s & USM_DBLCLK) {      // 如果有双击位, 表示当前消息是左键双击
      My_close();             // 执行退出函数
      return;
   }
   wnd=UC_GetCurrentWindow(); // 否则左键单击时
   x-=wnd->left+wnd->vx;      // 得到鼠标相对窗口的坐标
   y-=wnd->top+wnd->vy;
   cxPos=x/AscWidth;          // 得到文字坐标
   cyPos=y/AscHeight;
   x=cxPos*AscWidth;          // 计算新的位置
   y=cyPos*AscHeight;
   UC_MoveCaret(wnd, x, y);   // 移动光标到新位置
}

// 界面元素定义过程
void My_Widget(void)
{
   WINDOWS *wnd;

   wnd=UC_DefineWindow(WS_MAIN,                        // 普通应用的主窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       CW_USEDEFAULT, CW_USEDEFAULT,   // 用默认尺寸
                       "MouseKey 原始录入窗口-->窗口的键盘及鼠标事件处理",
                       My_redraw,
                       My_close,
                       My_resize);

   cxPos=cyPos=0;                 // 初始化光标位置
   AscWidth=wnd->syschar_size/2;  // 字符宽度点
   AscHeight=wnd->syschar_size;   // 字符高度点
   MaxWidth=(wnd->width-wnd->vx-wnd->vr)/AscWidth-1;
   MaxHeight=(wnd->height-wnd->vy-wnd->vb)/AscHeight-1;

   UC_DefineKeyboardEvent(wnd, cbKeyboard);           // 定义窗口键盘事件回调函数
   UC_DefineUserMouseEvent(wnd,                       // 定义窗口鼠标事件回调函数
                           0, 0, 0, 0,
                           USM_LEFT | USM_DBLCLK,     // 左键按下/双击时调用
                           UC_GetIDC(IDC_IBEAM),      // 文字光标形状
                           cbMouse);
   UC_WindowEnable(wnd);                              // 显示定义好的窗口
   UC_CreateCaret(wnd,                                // 创建文字插入光标
                  AscWidth,
                  AscHeight);
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
}
