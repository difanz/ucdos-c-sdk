/*---------------------------------------------------------------
  程序: LIFE.C
  演示: 生命繁衍
----------------------------------------------------------------*/

extern void Life(void);

#include <mem.h>
#include <string.h>
#include <stdlib.h>
#include "sdk.h"

#define LV_LIVE    1             // 存活
#define LV_DEAD    0             // 死亡

#define ID_NEXTYEAR  2000        // 下一年消息码
#define ID_CONTINUE  2001        // 连续发展消息码
#define ID_RESET     2002        // 重新开始消息码

int RX=50;
int MAT[100][100], TMAT[100][100];
int NOCROSS=1;
int Gen, Year, Men, OMen, stop;
WINDOWS *Lwnd;
SCROLLBAR *MATbar;

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
                  NULL,          // 不需要背景位图
                  FUL_SCR);
   UC_InitUCVision(VGA, DETECT);
   Men=OMen=0;
   Gen=Year=0;
   stop=0;
}

void lvDrawTotal(WINDOWS *wnd)
{
   if (wnd->minmax==SW_SHOWMIN) return;
//   if (UC_BeginPaintOnBack(wnd)) {
      settextstyle(0, 0, 16);
      setcolor(0);
      setcolorbm(15);
      UC_WindowPrintf(wnd, 20, 415, DT_OVER, "生命个数: %i ", Men);
      UC_WindowPrintf(wnd, 200, 415, DT_OVER, "年代: %i ", Gen);
//      UC_EndPaintOnBack(wnd);
//   }
}

void lvDrawLive(WINDOWS *wnd, int x, int y, int live)
{
  int l;

  l=400/RX;
  if (live==LV_LIVE) {
     setfillstyle(0, 2);
     MAT[y][x]=LV_LIVE;
  }
  else {
     setfillstyle(0, 15);
     MAT[y][x]=LV_DEAD;
  }
  if (wnd->minmax!=SW_SHOWMIN)
//   if (UC_BeginPaintOnBack(wnd)) {
      UC_WindowBar(wnd, 10+x*l+1, 10+y*l+1, l-1, l-1);
//      UC_EndPaintOnBack(wnd);
//   }
}

void My_redraw(WINDOWS *wnd)
{
   int x, y, r, b, i, j, l;
   struct viewporttype vi;

   x=wnd->left+wnd->vx;
   y=wnd->top+wnd->vy;
   r=wnd->left+wnd->width-wnd->vr;
   b=wnd->top+wnd->height-wnd->vb;

   getviewsettings(&vi);
   if (UC_SecondViewport(x, y, r, b)) {
     l=400/RX;
     if (NOCROSS) setcolor(0);
     else setcolor(15);
     UC_MouseHide();
     for (i=0; i<=RX; i++) {
        line(x+10, y+10+i*l, x+10+l*RX, y+10+i*l);
        line(x+10+i*l, y+10, x+10+i*l, y+10+l*RX);
     }
     setcolor(0);
     if (!NOCROSS) rectangle(x+10, y+10, x+10+RX*l, y+10+RX*l);

     for (i=0; i<RX; i++) {
       for (j=0; j<RX; j++) {
          if (MAT[i][j]) {
             lvDrawLive(wnd, j, i, LV_LIVE);
          }
       }
     }
     lvDrawTotal(wnd);
     UC_MouseShow();
     Setviewport(vi.left, vi.top, vi.right, vi.bottom);
   }
}

void cbBegin()
{
   UC_SetMessage(ID_NEXTYEAR);
}

void cbPause()
{
   UC_SetMessage(ID_CONTINUE);
}

void cbReset()
{
   UC_SetMessage(ID_RESET);
}

void lvMATClose()
{
   UC_WindowClose();
}

void lvMATOK()
{
   UC_WindowClose();
   RX=MATbar->cure+1;
   UC_RedrawWindow(Lwnd);  // 重新刷新生命窗口
}

void cbMATRedraw(WINDOWS *wnd)
{
  setcolor(0);
  setcolorbm(15);
  settextstyle(0, 0, wnd->syschar_size);
  UC_WindowPrintf(wnd, 20, 20, DT_OVER,
                  "当前生命空间: %i  ", (int)MATbar->cure+1);
}

void cbMATbar(SCROLLBAR *sbar, float pc)
{
  sbar=sbar;
  pc=pc;
  cbMATRedraw(UC_GetCurrentWindow());
}

void cbSetMAT()
{
   WINDOWS *wnd;

   wnd=UC_DefineWindow(WS_NOSIZE,
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       340, 130,
                       "设置生命空间",
                       cbMATRedraw,
                       lvMATClose,
                       NULL);
   MATbar=UC_DefineScrollbar(wnd, H_BAR,
                             20, 40, 290,
                             100, RX-1, 10,
                             cbMATbar, 1);
   UC_DefinePressButton(wnd, lvMATOK,
                        125, 70, 80, 0,
                        "确定", SK_ENTER);
   UC_WindowEnable(wnd);
}

void cbCross()
{
   NOCROSS=1-NOCROSS;
   My_redraw(Lwnd);
}

int cbMessage(WINDOWS *wnd, int msg)
{
   wnd=wnd;
   switch (msg) {
//       case WM_MINSIZE:
//       case WM_REVERTSIZE:
//            return 1;
       case WM_DISABLE:
            stop=1;          // 窗口被禁止时, 生命不起作用
            break;
       case WM_ENABLE:
            stop=0;
            break;
   }
   return NULL;
}

// Mouse 点出初次的生命
void cbMouse(int x, int y, WORD s)
{
  WINDOWS *wnd;
  int l, t, ls;

  wnd=UC_GetCurrentWindow();
  l=wnd->left+wnd->vx+10;
  t=wnd->top+wnd->vy+10;
  ls=400/RX;
  l=(x-l)/ls;
  t=(y-t)/ls;
  if (l>=RX || t>=RX) return;
  if (s&USM_RIGHT) {                 // 右键死亡
     if (MAT[t][l]) {
        lvDrawLive(wnd, l, t, LV_DEAD);
        TMAT[t][l]=LV_DEAD;
        Men--;
     }
  }
  else {
     if (!MAT[t][l]) {
        lvDrawLive(wnd, l, t, LV_LIVE);
        TMAT[t][l]=LV_LIVE;
        Men++;
     }
  }
  lvDrawTotal(wnd);
}

void My_About(void)
{
   UC_DialogAbout(NULL, Life, 1, "关于生命",
                  "生命繁衍 Life 1.0\n\n北京希望高技术集团 1996.9\n简晶");
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
   Lwnd=UC_DefineWindow(WS_MAIN | WS_SINGLELINE,
                       0, 0,           // 窗口居中屏幕
                       CW_MAX, CW_MAX,   // 用默认尺寸
                       "Life 生命繁衍",
                       My_redraw,
                       My_close,
                       My_resize);
   UC_DefinePressButton(Lwnd, cbBegin,
                        450, 20, 100, 0,
                        "[N]下一年", SK_ALTN);
   UC_DefinePressButton(Lwnd, cbPause,
                        450, 60, 100, 0,
                        "[S]连续发展", SK_ALTS);
   UC_DefinePressButton(Lwnd, cbSetMAT,
                        450, 100, 100, 0,
                        "[C]生命空间", SK_ALTC);
   UC_DefinePressButton(Lwnd, cbCross,
                        450, 140, 100, 0,
                        "[W]网格", SK_ALTW);
   UC_DefinePressButton(Lwnd, cbReset,
                        450, 180, 100, 0,
                        "[R]重新开始", SK_ALTR);
   UC_DefinePressButton(Lwnd, My_About,
                        450, 220, 100, 0,
                        "[A]关于生命", SK_ALTA);
   UC_DefinePressButton(Lwnd, My_close,
                        450, 260, 100, 0,
                        "[X]退出", SK_ALTX);

   UC_DefineActive(Lwnd, TYPE_BUTTON, 1);
   UC_DefineUserMouseEvent(Lwnd, 10, 10, 410, 410,
                           USM_LEFT | USM_RIGHT, NULL, cbMouse);
   UC_DefineWindowEnable(Lwnd, cbMessage);
   UC_WindowEnable(Lwnd);                               // 显示定义好的窗口
}

// 检测指定位置周围的邻居个数
// 注意: 边界之外无邻居
int lvGetNeighbor(int x, int y)
{
   int total;

   total=0;

   if (x>0) {
      if (TMAT[y][x-1]) total++;     // 左边邻居
   }
   if (y>0) {
      if (TMAT[y-1][x]) total++;     // 上边邻居
   }
   if (x<RX-1) {
      if (TMAT[y][x+1]) total++;     // 右边邻居
   }
   if (y<RX-1) {
      if (TMAT[y+1][x]) total++;     // 下边邻居
   }

   if (x>0 && y>0) {
      if (TMAT[y-1][x-1]) total++;   // 左上邻居
   }
   if (x<RX-1 && y<RX-1) {
      if (TMAT[y+1][x+1]) total++;   // 右下邻居
   }
   if (x>0 && y<RX-1) {
      if (TMAT[y+1][x-1]) total++;   // 左下邻居
   }
   if (x<RX-1 && y>0) {
      if (TMAT[y-1][x+1]) total++;   // 右上邻居
   }

   return total;
}

// 一个年度的生命增长及死亡变化
void lvDoLive(void)
{
   int x, y, neighbor, msg;

   Gen++;
   memmove(&TMAT, &MAT, 100*100*sizeof(int));
   for (y=0; y<RX; y++) {             // 以纵向坐标为外循环
      for (x=0; x<RX; x++) {

         UC_WindowsCentral();
         if (stop) goto dlret; //return;
         msg=UC_GetMessage();
         if (msg) {                   // 有按钮按下, 返回
            UC_SetMessage(msg);       // 重置消息码
            goto dlret;
         }

         neighbor=lvGetNeighbor(x, y);
         if (TMAT[y][x]==LV_LIVE) {              // 当前位置有生命的情况下
            if (neighbor<2 || neighbor>3) {      // 邻居大于3个或小于2个, 死亡
               lvDrawLive(Lwnd, x, y, LV_DEAD);
               Men--;
            }
         } else {
            if (neighbor==3) {                   // 当前位置空白, 且邻居为3个, 繁殖
               lvDrawLive(Lwnd, x, y, LV_LIVE);
               Men++;
            }
         }
      }
   }
dlret:
   lvDrawTotal(Lwnd);
   if (!Men) {
        UC_DialogWarning(NULL, "生命消亡",
                         "生命全部死亡! 无法进一步发展 ...\n请选择重新开始, 认真决定新的初期模式.");
   }

   if (OMen==Men) Year++;        // 生命稳定的年度计数
   else Year=0;
   if (OMen!=Men) OMen=Men;
}

// 应用主体
void My_App(void)
{
   int msg, x, y;

   Gen=0;

   while (1) {
     UC_WindowsCentral();
     if (!stop) {
        msg=UC_GetMessage();
//        if (Men && msg==ID_NULL) msg=ID_CONTINUE;
smsg:
        switch (msg) {
            case ID_CONTINUE:
                 if (!Men) {
                    UC_DialogWarning(NULL, "没有生命", "   当前生命个数为 0,\n\n   无法继续发展 ...");
                    break;            // 没有生命, 返回
                 }
                 while (1) {
                   lvDoLive();
                   if (!Men) break;   // 没有生命, 返回
                   if (Year>=10) {
                      UC_DialogWarning(NULL, "生命发展稳定",
                                       "生命发展的过去 10 年中,\n数量已趋于稳定!\n\n可选择重新开始, 发展新的模式.");
                      Year=0;
                      break;
                   }
                   msg=UC_GetMessage();
                   if (msg!=ID_NULL) goto smsg; // 有重置消息码, 直接处理
                   if (stop) break;
                 }
                 break;
            case ID_RESET:
                 Gen=0;
                 Year=0;
                 Men=0;
                 OMen=0;
                 for (x=0; x<100; x++) {
                   for (y=0; y<100; y++) {
                      MAT[y][x]=LV_DEAD;
                   }
                 }
                 UC_RedrawWindow(Lwnd);
                 break;
            case ID_NEXTYEAR:
                 if (!Men) {
                    UC_DialogWarning(NULL, "没有生命", "   当前生命个数为 0,\n\n   无法继续发展 ...");
                    break;   // 没有生命, 返回
                 }
                 lvDoLive();
                 if (Year>=10) {
                    UC_DialogWarning(NULL, "生命发展稳定",
                                     "生命发展的过去 10 年中,\n数量已趋于稳定!\n\n可选择重新开始, 发展新的模式.");
                    Year=0;
                 }
                 break;
        }
      }
   }
}
