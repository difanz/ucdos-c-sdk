// 本文件提供几种对话框的实现方法, 供程序员参考以编写其他类型的对话框

#include <stdio.h>
#include <stdarg.h>
#include <alloc.h>
#include <string.h>
#include "sdk.h"

WINDOWS *diawnd;
char dwtxt[1024];

void UC_SetID_OK(void)
{
   UC_SetMessage(ID_OK);
}

void UC_SetID_YES(void)
{
   UC_SetMessage(ID_YES);
}

void UC_SetID_NO(void)
{
   UC_SetMessage(ID_NO);
}

void UC_DrawAbout(WINDOWS *wnd)
{
   int l, t, x, y, x1;
        WORD tx, fx;
        DWORD fc;
   int s;
   char HEAPSTATE[5];

   l=wnd->left+wnd->vx;
   t=wnd->top+wnd->vy;
   x=UC_RetReal(60);
   y=UC_RetReal(150);
   x1=UC_RetReal(300);

   UC_MouseHide();
   setcolor(0);
   line(l+x, t+y, l+x1, t+y);
   line(l+x, t+y+UC_RetReal(90), l+x1, t+y+UC_RetReal(90));
   setcolor(7);
   line(l+x, t+y+1, l+x1, t+y+1);
   line(l+x, t+y+UC_RetReal(90)-1, l+x1, t+y+UC_RetReal(90)-1);
   settextstyle(0, 0, wnd->syschar_size);
   y+=UC_RetReal(10);
   setcolor(0);

   UC_GetFreeXMS(&tx, &fx);
   fc=coreleft();
   s=heapcheck();
   if (s<0) strcpy(HEAPSTATE, "出错");
   else if (s==1) strcpy(HEAPSTATE, "空堆");
   else strcpy(HEAPSTATE, "正常");
   setcolor(0);
   setcolorbm(15);
   settextstyle(0, 0, wnd->syschar_size);
   UC_WindowPrintf(wnd, 80, 160, DT_NOOVER,
                   "可用常规内存 : %lu 字节\n总共扩展内存 : %i KB\n可用扩展内存 : %i KB\n  系统堆状态 : %s",
                   fc, tx, fx, HEAPSTATE);
   UC_MouseShow();
}

void AboutMouse(WORD x, WORD y, WORD s)
{
   x=x;
   y=y;
   if (s&USM_LEFT) return;
   if (!(s&USM_DBLCLK)) return;
   UC_SecretWindow();
}

// 显示一About窗口函数
// funok= 按确定按钮后的回调函数
// ico=关于窗口的图标指针
// icn=图标计数
// title= 自定义标题
void UC_DialogAbout(void (*funok)(void), void (*ico)(), int icn,
                    char *title, char *fmt,...)
{
  va_list argptr;

  va_start(argptr, fmt);
  vsprintf(dwtxt, fmt, argptr);
  va_end(argptr);

  if (!title) diawnd=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR,
                                     400, 300, "关  于",
                                     UC_DrawAbout, UC_SetID_OK, NULL);
  else diawnd=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR, 400, 300,
                                title, UC_DrawAbout, UC_SetID_OK, NULL);
  UC_DefineLabel(diawnd, 20, 20, NULL, NULL, NULL, ico, icn);
  UC_DefineLabel(diawnd, 60, 20, NULL, NULL, dwtxt, NULL, NULL);
  UC_DefinePressButton(diawnd, UC_SetID_OK, 320, 20, 60, 0, "退出", SK_ENTER);
  UC_DefineActive(diawnd, TYPE_BUTTON, 1);
  UC_DefineUserMouseEvent(diawnd, 0, 0, 0, 0,
                          USM_DBLCLK, NULL, AboutMouse);
  UC_WindowEnable(diawnd);
  while(1) {
       UC_WindowsCentral();
       if (UC_GetMessage()==ID_OK) break;
  }
  UC_WindowClose();
  if (funok) funok();
}

// 显示一警告窗口函数
// funok= 按确定按钮后的回调函数
// title= 自定义标题
void UC_DialogWarning(void (*funok)(void), char *title, char *fmt,...)
{
  va_list argptr;

  va_start(argptr, fmt);
  vsprintf(dwtxt, fmt, argptr);
  va_end(argptr);

  if (!title) diawnd=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR,
                                      400, 210, "警 告", NULL, UC_SetID_OK, NULL);
  else diawnd=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR, 400, 210,
                                title, NULL, UC_SetID_OK, NULL);
  UC_DefineLabel(diawnd, 20, 20, NULL, NULL, NULL, ICO_EXCLAMATION, 1);
  UC_DefineLabel(diawnd, 60, 20, NULL, NULL, dwtxt, NULL, NULL);
  UC_DefinePressButton(diawnd, UC_SetID_OK, 160, 140, 60, 0, "确认", SK_ENTER);
  UC_DefineActive(diawnd, 1, 1);
  UC_WindowEnable(diawnd);
  while(1) {
       UC_WindowsCentral();
       if (UC_GetMessage()==ID_OK) break;
  }
  UC_WindowClose();
  if (funok) funok();
}

// 显示一选择确认窗口函数
// isyes()= 选择YES的函数
// isno()= 选择NO的函数
// title= 自定义标题
WORD UC_DialogYesNo(void (*isyes)(void), void (*isno)(void), char *title, char *fmt,...)
{
  va_list argptr;

  va_start(argptr, fmt);
  vsprintf(dwtxt, fmt, argptr);
  va_end(argptr);

  if (!title) diawnd=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR,
                                     400, 210, "提 示", NULL, UC_SetID_NO, NULL);
  else diawnd=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR,
                              400, 210, title, NULL, UC_SetID_NO, NULL);
  UC_DefineLabel(diawnd, 20, 20, NULL, NULL, NULL, ICO_QUESTION, 1);
  UC_DefineLabel(diawnd, 60, 20, NULL, NULL, dwtxt, NULL, NULL);
  UC_DefinePressButton(diawnd, UC_SetID_YES, 110, 140, 60, 0, "是[Y]", SK_ALTY);
  UC_DefinePressButton(diawnd, UC_SetID_NO, 210, 140, 60, 0, "否[N]", SK_ALTN);
  UC_DefineActive(diawnd, 1, 1);
  UC_WindowEnable(diawnd);
  while(1) {
       UC_WindowsCentral();
       switch (UC_GetMessage()) {
              case ID_YES:
                          UC_WindowClose();
                          if (isyes) isyes();
                          return ID_YES;
              case ID_NO:
                          UC_WindowClose();
                          if (isno) isno();
                          return ID_NO;
       }
  }
}
