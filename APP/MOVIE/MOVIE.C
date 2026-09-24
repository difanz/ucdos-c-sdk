// 采用定时器播放动画演示
// 由于定时器间隔最短时间为50毫秒, 所以可以得出
// 通过这种方法最快的情况下, 每秒可播放动画1000/50=20幅

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <dir.h>
#include "sdk.h"

WINDOWS *movie;       // 动画窗口句柄
int first=1;          // 动画开始标记
int stop=0;           // 动画禁止标记
int min=0;            // 窗口被极小化标记
int step=0;
int stepl=0;
char *path;           // 动画文件名
char Title[]="                       欢迎观看 UCDOS SDK 实现的动画电影! \
UCDOS SDK for C/C++ 定制了一套 DOS 环境下的 GUI 界面规范, \
全部实现仿 Windows 界面, 使 DOS 下的应用程序档次大为提升! 如果您是程序员, \
一定不能错过试试 SDK for C/C++, 可能这就是一次成功的机会! ... ... ";

void My_begin(void)
{
  BUFFER_LEN=65500;
  UC_InitDesktop(1, 15, NULL, NULL);
  UC_InitUCVision(DETECT, VM_640X480X256);
}

void My_end(void)
{
  UC_CloseUCVision(NULL, NULL);
}

// 重画当前的图像
void My_redraw(WINDOWS *wnd)
{
//   struct viewporttype vi;
//   int x, y, r, b;

//   getviewsettings(&vi);
//   x=movie->left+movie->vx+160;
//   y=movie->top+movie->vy+2;
//   r=x+400;
//   b=y+40;

   UC_MouseHide();
   setcolor(0);
   line(wnd->left+wnd->vx, wnd->top+wnd->vy+45,
        wnd->left+wnd->width-wnd->vr, wnd->top+wnd->vy+45);
//   if (UC_SecondViewport(wnd->left+wnd->vx, wnd->top+wnd->vy,
//                wnd->left+wnd->width-1-wnd->vr, wnd->top+wnd->vy+45)) {
//      setfillstyle(0, 8);
//      bar(x, y, r, b);
//      Setviewport(vi.left, vi.top, vi.right, vi.bottom);
//   }

   if (!first) UC_DrawDDB(wnd,
                          NULL,
                          path,
                          0, 45,
                          movie->width-2*movie->vx,
                          movie->height-movie->vy-4,
                          MID_SCR, COPY_PUT);
   UC_MouseShow();
}

void TMvi()
{
   struct viewporttype vi;
   int x, y, r, b;
   char ctext[31];
   char tmp[3];
   int ostep;

   if (stop) return;   // 停止状态, 直接返回
   getviewsettings(&vi);
   x=movie->left+movie->vx+161;
   y=movie->top+movie->vy+3;
   r=x+450;
   b=y+40;
   if (UC_SecondViewport(x, y, r, b)) {
      memmove(ctext, Title+stepl, 30);
      if (stepl>=strlen(Title)) {
         stepl=0;
         memmove(ctext, Title+stepl, 30);
      }
      ctext[30]=NULL;
      setcolorbm(15);
      settextstyle(0, 0, 40);
      setascstyle(3);
      setcolor(0);
//      UC_MouseHide();
      UC_TextOut(movie, 160-step+1, 2+1, DT_OVER, ctext);
//      setcolor(8);
//      setwritemode(XOR_PUT);
//      UC_TextOut(movie, 160-step, 2, DT_NOOVER, ctext);
//      setwritemode(COPY_PUT);
      UC_MouseShow();

      Setviewport(vi.left, vi.top, vi.right, vi.bottom);
   }
   step+=10;          // 字串滚动显示速度

   tmp[0]=ctext[0];
   tmp[1]=NULL;
   if (UC_CheckEditPoint(ctext, 0) == STR_HZLEFT ) {
       tmp[1]=ctext[1];
       tmp[2]=NULL;
   }
   ostep=textwidth(tmp);

   if (step >= ostep) {
      step=0;
      stepl++;
      if (UC_CheckEditPoint(ctext, 0) == STR_HZLEFT ) stepl++;
   }
}

void play()
{
   if (stop) return;   // 停止状态, 直接返回
   if (first) path=UC_GetFileNameByCount("*.ddb", 1, FA_ARCH);
   else path=UC_GetNextFileNameByCount(FA_ARCH);

   first=0;
   if (!path) {      // 找不到 DDB 文件, ???取消定时器???, 返回
//       UC_KillTimer(play);
       first=1;
       return;
   }
//        UC_CreateBitmap(path);
//   UC_SetZoomDDB(movie->width-2*movie->vx,
//                 movie->height-movie->vy-4-45);
        UC_DrawDDB(movie,
                   NULL,
                   path,
                   0, 45,
                   movie->width-2*movie->vx,
                   movie->height-movie->vy-4,
                   MID_SCR, COPY_PUT);
}

// 定时播放开始
void BegPlay()
{
   if (first)                 // 只有第一次显示时, 才设置定时器
      UC_SetTimer(play, 100);  // 默认设定每幅图80毫秒(可根据不同的动画效果修改)
   stop=0;
   UC_DisableObject(movie, TYPE_BUTTON, 1);
   UC_NewActive(movie, TYPE_BUTTON, 2);
}

// 处理窗口的禁止/允许消息, 以决定动画当前的播放状态
int movmsg(WINDOWS *wnd, int msg)
{
   wnd=wnd;
   switch (msg) {
        case WM_MINSIZE:             // 窗口极小化/禁止时, 动画停止
                         min=1;
        case WM_DISABLE:
                         stop=1; break;
        case WM_REVERTSIZE:          // 窗口还原/允许时, 动画允许
                         min=0;
        case WM_ENABLE:
                         if (!min) stop=0;
                         break;
   }
   return NULL;
}

void My_app(void)
{
   movie=UC_DefineWindow(WS_MAIN ,CP_MIDSCR, CP_MIDSCR,
                         400, 400, "UCDOS SDK for C/C++ 设计的动画播放窗口",
                         My_redraw, My_end, NULL);
   UC_DefinePressButton(movie, BegPlay, 10, 10, 60, 0, "播放", 0);
   UC_DefinePressButton(movie, My_end, 80, 10, 60, 0, "退出", 0);
   UC_DefineWindowEnable(movie, movmsg);
   UC_DefineActive(movie, 1, 1);
   UC_MaxWindow(movie);
   UC_SetTimer(TMvi, 0);    // 题头定时器显示
   UC_MainLoop();
}

void main()
{
  My_begin();
  My_app();
}
