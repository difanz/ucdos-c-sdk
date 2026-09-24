// 说明: 本程序使用 64K 真彩色屏幕,
//       可以使各幅图像之间的调色板互不干扰

#include <dir.h>
#include <conio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sdk.h"

void resize_bar(WINDOWS *wnd);
void draw_win(WINDOWS *wnd);

typedef struct _mb {
        WINDOWS *wnd;
        BMPHEAD bmp;
        char path[MAXPATH];
        WORD handle;
        int bmp_x;
        int bmp_y;
        char showmode;          // 显示方式
        struct _mb *prev;
        struct _mb *next;
} MulBmp;

MulBmp *mb_tail;
WINDOWS *menu_win, *win_about, *win_info, *win_opti;
int argw=0, radsele;
int revert=0;

void closemywin(void);
MulBmp *foundmb(WINDOWS *wnd);

void My_begin(void)
{
   BUFFER_LEN=65500;                        // 图像缓冲区开到最大
   UC_InitDesktop(1, 7, NULL, FUL_SCR);
   UC_InitUCVision(VESA64K, VM_640X480X64K);
   mb_tail=NULL;
}

void syshelp()
{
//   UC_WinHelp("c:\\test.ush", NULL);
}

void closeend()
{
   UC_WindowClose();
}

void ifzoom(MulBmp *tmb)
{
  int leftv, toph;

  if (revert && tmb->showmode!=FUL_SCR) {         // 显示模式变换后, 重新装入BMP
      UC_FreeXMS(tmb->handle);
      tmb->handle=UC_XMSLoadDDB(tmb->path);
      revert=0;
  }

  if (tmb->showmode==FUL_SCR) {
      leftv=tmb->wnd->width-tmb->wnd->vx-tmb->wnd->vr;
      toph=tmb->wnd->height-tmb->wnd->vy-tmb->wnd->vb;
      UC_MouseShapeType(IDC_WAIT);
      UC_ZoomDDB(tmb->path, "tttt", leftv, toph, GRAPH_BUFFER, BUFFER_LEN);
      UC_FreeXMS(tmb->handle);
      tmb->handle=UC_XMSLoadDDB("tttt");
      unlink("tttt.ddb");
      UC_MouseShapeType(IDC_ARROW);
  }
}

void radok()
{
  WINDOWS *wnd;
  MulBmp *tmb;

  UC_WindowClose();
  wnd=UC_GetCurrentWindow()->prev;
  tmb=foundmb(wnd);
  if (radsele==0) tmb->showmode=XY_SCR;
  else if (radsele==1) tmb->showmode=MID_SCR;
  else tmb->showmode=FUL_SCR;
  if (tmb->showmode!=FUL_SCR) {         // 显示模式变换后, 重新装入BMP
      UC_FreeXMS(tmb->handle);
      tmb->handle=UC_XMSLoadDDB(tmb->path);
  }
  resize_bar(wnd);
  UC_RedrawWindow(wnd);
}

void clickradio(int count)
{
   radsele=count;
}

void option()
{
        MulBmp *tmb;

  static char *rad[]={ "标准", "居中", "充满窗口", NULL};

  if ((tmb=foundmb(UC_GetCurrentWindow()->prev))==NULL) return;
  if (tmb->wnd->minmax==SW_SHOWMIN) return;

  win_opti=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR, 280, 170, "选择项",
                           NULL, closeend, NULL);
  UC_DefineGroupBox(win_opti, "显示方式[P]", 20, 10, 110, 110);
  UC_DefineRadioButton(win_opti, rad, tmb->showmode, 30, 40, SK_ALTP, clickradio);
  UC_DefinePressButton(win_opti, radok, 160, 50, 80, 0, "确定", SK_ENTER); //K
  UC_DefinePressButton(win_opti, closeend, 160, 90, 80, 0, "取消", SK_ESC); //K
  UC_DefineActive(win_opti,1,1);
  UC_WindowEnable(win_opti);
}

void draw_info()
{
  MulBmp *tmb;
  char i[6];

  tmb=foundmb(win_info->prev->prev);
  if (!tmb) return;

  if (tmb->bmp.bits==1) strcpy(i,"2");
  if (tmb->bmp.bits==4) strcpy(i,"16");
  if (tmb->bmp.bits==8) strcpy(i,"256");
  if (tmb->bmp.bits==24) strcpy(i,"16.8M");

  setcolor(0);
  settextstyle(0, 0, win_info->syschar_size);
  UC_WindowPrintf(win_info, 10, 20, DT_NOOVER, "文件: %s", tmb->path);
  UC_WindowPrintf(win_info, 10, 50, DT_NOOVER, "宽度: %ld", tmb->bmp.width);
  UC_WindowPrintf(win_info, 10, 70, DT_NOOVER, "高度: %ld", tmb->bmp.depth);
  UC_WindowPrintf(win_info, 10, 90, DT_NOOVER, "颜色: %s", i);
}

void infowin(void)
{
  win_info=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR, 350,160,"当前图像信息窗口",
                           draw_info, closeend, NULL);
  UC_DefinePressButton(win_info, closeend, 200, 80, 80, 0, "确定", NULL); //K
  UC_DefineActive(win_info,1,1);
  UC_WindowEnable(win_info);
}

void draw_about(WINDOWS *wnd)
{
  setcolor(12);
  setcolorbm(15);
  settextstyle (0, 0, wnd->syschar_size);
  UC_TextOut(wnd, 60, 10, DT_NOOVER, "Windows BMP 文件浏览器");
  UC_TextOut(wnd, 60, 30, DT_NOOVER, "版本 1.0");
  setcolor(2);
  UC_TextOut(wnd, 60, 60, DT_NOOVER, "北京希望高科技集团, 1996.8");
  setcolor(9);
  UC_TextOut(wnd, 60, 80, DT_NOOVER, "作者: 简晶");
}

void closeabout()
{
  UC_WindowClose();
}

void about(void)
{
  win_about=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR, 300, 200,
                            "关于本程序", draw_about, closeabout, NULL);
  UC_DefineLabel(win_about, 10, 10, NULL, NULL, NULL, ICO_APPLICATION, 1);
  UC_DefinePressButton(win_about, closeabout, 120,130,60,0,"退出",SK_ENTER);
  UC_DefineActive(win_about,1,1);
  UC_WindowEnable(win_about);
}

int WMsub(WINDOWS *wnd, int msg)
{
   MulBmp *tmb;
   int width, height;

   tmb=foundmb(wnd);
   switch (msg) {
          case WM_MINSIZE:
               UC_ZoomDDB(tmb->path, "tttt", 32, 32, GRAPH_BUFFER, BUFFER_LEN);
               UC_FreeXMS(tmb->handle);
               tmb->handle=UC_XMSLoadDDB("tttt");
               unlink("tttt.ddb");
               break;
          case WM_REVERTSIZE:
               revert=1;
               break;
          default: break;
   }
   return NULL;
}

void MiniBmp(WINDOWS *wnd, RECT *rect)
{
   MulBmp *tmb;
   int width, height;

   tmb=foundmb(wnd);
   width=rect->right-rect->left+1;
   height=rect->bottom-rect->top+1;

   UC_SetZoomDDB(width, height);
   UC_DrawDDB(wnd, tmb->handle, tmb->path, rect->left, rect->top,
              width, height, XY_SCR, COPY_PUT);
   UC_SetZoomDDB(0, 0);
}

void draw_win(WINDOWS *wnd)
{
   MulBmp *tmb;
   int leftv, topv, lengv;
   int lefth, toph, lengh;
   int i; //, ovb, ovr;
   char cr[6];

   tmb=foundmb(wnd);
   if (!tmb) return;
   UC_GetScrollbarXY(wnd, wnd->vbar, &leftv, &topv, &lengv);
   UC_GetScrollbarXY(wnd, wnd->hbar, &lefth, &toph, &lengh);

   setcolor(0);
   if (tmb->showmode==FUL_SCR) {
      UC_SetZoomDDB(leftv, toph);
   }
   i=UC_DrawDDB(wnd, tmb->handle, tmb->path, tmb->bmp_x, tmb->bmp_y, leftv, toph,
                tmb->showmode, COPY_PUT);
   UC_SetZoomDDB(0, 0);
   if (!i) closemywin();

   if (UC_BeginPaintStateLine(wnd)) {
          if (tmb->bmp.bits==1) strcpy(cr,"2");
          if (tmb->bmp.bits==4) strcpy(cr,"16");
          if (tmb->bmp.bits==8) strcpy(cr,"256");
          if (tmb->bmp.bits==24) strcpy(cr,"16.8M");
          i=wnd->height-wnd->vy-wnd->vr-20+1;
          setfillstyle(SOLID_FILL, LIGHTGRAY);
          UC_WindowBar(wnd, 0, i+1, wnd->width-wnd->vr, i+20);
          setcolor(0);
          settextstyle(0, 0, 16);
          UC_WindowPrintf(wnd, -1, i+2, DT_NOOVER, "图像信息: %ldx%ld-%s",
                          tmb->bmp.width, tmb->bmp.depth, cr);
          UC_EndPaintStateLine(wnd);
    }
}

void movlr(SCROLLBAR *sbar, float pcure)
{
  WINDOWS *wnd;
  MulBmp *tmb;
  int lefth, toph, lengh;

  pcure=pcure;
  wnd=UC_GetCurrentWindow();
  tmb=foundmb(wnd);
  UC_GetScrollbarXY(wnd, sbar, &lefth, &toph, &lengh);

  if (!sbar->cure) tmb->bmp_x=0;
  else tmb->bmp_x=(sbar->cure+1)*20;
  if (tmb->bmp_x>=sbar->max*20) tmb->bmp_x=tmb->bmp.width-lengh; //+1;
  tmb->bmp_x=-tmb->bmp_x;
  draw_win(wnd);
}

void movud(SCROLLBAR *sbar, float pcure)
{
  WINDOWS *wnd;
  MulBmp *tmb;
  int leftv, topv, lengv;

  pcure=pcure;
  wnd=UC_GetCurrentWindow();
  tmb=foundmb(wnd);
  UC_GetScrollbarXY(wnd, sbar, &leftv, &topv, &lengv);

  if (!sbar->cure) tmb->bmp_y=0;
  else tmb->bmp_y=(sbar->cure+1)*20;
  if (tmb->bmp_y>=sbar->max*20) tmb->bmp_y=tmb->bmp.depth-lengv; //+1;
  tmb->bmp_y=-tmb->bmp_y;
  draw_win(wnd);
}

/*
void over()
{
  while (mb_tail) {
        UC_FreeXMS(mb_tail->handle);
        mb_tail=mb_tail->prev;
  }
}
*/

void My_end(void)
{
   UC_CloseUCVision(NULL, NULL);
}

void resize_bar(WINDOWS *wnd)
{
   MulBmp *tmb;
   int leftv, topv, lengv;
   int lefth, toph, lengh;

   tmb=foundmb(wnd);
   UC_GetScrollbarXY(wnd, wnd->vbar, &leftv, &topv, &lengv);
   UC_GetScrollbarXY(wnd, wnd->hbar, &lefth, &toph, &lengh);

   if (tmb->showmode==XY_SCR) {
      if (tmb->bmp.depth>toph) {
         wnd->vbar->max=(tmb->bmp.depth-toph)/20+1;
         if (wnd->vbar->max==1) wnd->vbar->max++;
      } else wnd->vbar->max=1;
      if (tmb->bmp.width>leftv) {
         wnd->hbar->max=(tmb->bmp.width-leftv)/20+1;
         if (wnd->hbar->max==1) wnd->hbar->max++;
      } else wnd->hbar->max=1;
    } else {
            wnd->vbar->max=1;
            wnd->hbar->max=1;
    }
    if (tmb->bmp.width<leftv) {
       tmb->bmp_x=0;
       wnd->hbar->cure=0;
    }

    if (tmb->bmp.depth<toph) {
       tmb->bmp_y=0;
       wnd->vbar->cure=0;
    }

    if (tmb->bmp_x < 0 &&
        (tmb->bmp_x+tmb->bmp.width < leftv)) {
       tmb->bmp_x=leftv-tmb->bmp.width;
       wnd->hbar->cure=-tmb->bmp_x/20+1;
    }
    if (tmb->bmp_y < 0 &&
        (tmb->bmp_y+tmb->bmp.depth < toph)) {
       tmb->bmp_y=toph-tmb->bmp.depth;
       wnd->vbar->cure=-tmb->bmp_y/20+1;
    }

    wnd->vbar->page=lengv/20;
    wnd->hbar->page=lengh/20;
    ifzoom(tmb);
}

// 找出与当前窗口对应的BMP管理结构
MulBmp *foundmb(WINDOWS *wnd)
{
  MulBmp *tmb;

  if (!wnd) return NULL;
  tmb=mb_tail;
  while (tmb) {
        if (wnd==tmb->wnd) return tmb;
        tmb=tmb->prev;
  }
  return NULL;
}

void closemywin()
{
  MulBmp *tmb;

  tmb=foundmb(UC_GetCurrentWindow());
  if (tmb) {
          if (tmb->next) tmb->next->prev=tmb->prev;
          if (tmb->prev) tmb->prev->next=tmb->next;
          if (mb_tail==tmb) mb_tail=tmb->prev;
          UC_FreeXMS(tmb->handle);
          free(tmb);
  }
  UC_WindowClose();
}

void openok(char *tmp)
{
        WINDOWS *wnd;
        MulBmp *newbmp;
//      int i;

        newbmp=(MulBmp *)malloc(sizeof(MulBmp));
        if (newbmp==NULL) {
                 UC_DialogWarning(NULL, NULL, "内存不够,请关闭几个图像!");
                 return;
        }
        strcpy(newbmp->path, tmp);
        UC_GetBMPInfo(newbmp->path, &newbmp->bmp);
        newbmp->handle=0;
        UC_MouseShapeType(IDC_WAIT);
        if (UC_CreateBitmap(newbmp->path)) {
                newbmp->handle=UC_XMSLoadDDB(newbmp->path);
                UC_MouseShapeType(IDC_ARROW);
        } else {
                free(newbmp);
                UC_MouseShapeType(IDC_ARROW);
                return;
        }
        newbmp->bmp_x=0;
        newbmp->bmp_y=0;
        wnd=UC_DefineWindow(WS_MAIN, CP_USEDEFAULT, CP_USEDEFAULT, 400, 300,
                            newbmp->path, draw_win, closemywin, resize_bar);
//      i=strlen(newbmp->path);
//      while (newbmp->path[i]!='\\') i--;
//      UC_DefineWindowIcon(wnd, NULL, 0, newbmp->path+i+1);
        UC_DefineDrawIconRect(wnd, MiniBmp);
        UC_DefineWindowEnable(wnd, WMsub);
        newbmp->wnd=wnd;
        newbmp->showmode=XY_SCR;
        newbmp->prev=NULL;
        newbmp->next=NULL;
        if (mb_tail) {
            newbmp->prev=mb_tail;
            mb_tail->next=newbmp;
        }
        mb_tail=newbmp;

        UC_DefineStateLine(wnd, 20);
        UC_DefineWindowHScrollbar(wnd, movlr);
        UC_DefineWindowVScrollbar(wnd, movud);
        resize_bar(wnd);
        UC_WindowEnable(wnd);
}

void file_open(void)
{
  static char *arg[]={"*.bmp", "*.dib", "*.*", NULL};
  char *tfile;

  tfile=UC_DialogFileOpen(LOADFILE, &argw, arg, NULL, NULL, NULL); //openok, openfail);
  if (tfile==NULL) return;   // 打开失败, 返回
  openok(tfile);
}

void vbCascadeWindow()
{
        WINDOWS *tw;

        tw=menu_win->prev;
        if (!tw) return;
        UC_CascadeWindows(menu_win->left, menu_win->height,
                          getmaxx(), getmaxy());
        if (tw->minmax!=SW_SHOWMIN) UC_WindowEnable(tw);
}

void My_app(void)
{
        static void (*dfun[])()={file_open,
                                 infowin,
                                 NULL,
                                 UC_ToDosPrompt,
                                 My_end
        };
        static void (*ofun[])()={
                                 vbCascadeWindow,
                                 UC_ArrangeIcons,
                                 NULL,
                                 option
        };
        static void (*pfun[])()={
                                 syshelp,
                                 about
        };

        static char *dmem[]={
                             "O [O]打开...      ",
                             "I [I]图像信息...  ",
                             "-",
                             "D [D]进入DOS      ",
                             "X [X]退出...      ",
                             NULL
        };

        static char *omem[]={
                             "C [C]标题窗口     ",
                             "A [A]重排图标     ",
                             "-",
                             "P [P]选择项...    ",
                             NULL
        };

        static char *pmem[]={
                             "H [H]系统帮助 ",
                             "A [A]关于...  ",
                             NULL
        };

        static MENUS tmenu[]= {
                               { "F [F]文件 ", dmem, dfun, 0 },
                               { "O [O]选择项 ", omem, ofun, 0 },
                               { "H [H]帮助 ", pmem, pfun, 0 },
                               { NULL,         NULL, NULL, NULL }
        };

        menu_win=UC_DefineWindow(WS_MAIN|WS_NOMOVE,0,0,300,0,"Windows BMP 文件浏览器",
                                 NULL, My_end, NULL);
        UC_DefineMenu(menu_win, tmenu);

        UC_WindowEnable(menu_win);
        UC_MainLoop();
}

void main ()
{
        My_begin();
        My_app();
}
