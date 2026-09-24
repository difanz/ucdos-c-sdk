// 默认在 256 色模式下显示图标, 若考虑兼容性, 可修改为16色模式

#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <dir.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alloc.h>
#include "sdk.h"

extern void myico();

WINDOWS *main_win, *win_about, *win_info;

WORD ico_count;              // ICO 当前目录下所有ICO图标个数
int ico_cure;                // 窗口显示第一个的序号
char ico_name[2000][9];
int ico_cnt[2000];
WORD ico_handle[2000];
char path[MAXPATH], tmp[MAXPATH];
int argw=0, old_argw=0;
int V_cnt, H_cnt;
WORD info_wide, info_dept, info_bits;
void movud(SCROLLBAR *sbar, float pcure);

void My_begin(void)
{
   UC_InitDesktop(1, 7, NULL, FUL_SCR);
   UC_InitUCVision(DETECT, VM_640X480X256);
   ico_count=0;
   ico_cure=1;
}

void draw_about()
{
  setcolor(12);
  setcolorbm(15);
  settextstyle(0, 0, win_about->syschar_size);
  UC_TextOut(win_about, 60, 10, DT_NOOVER, "Windows ICO/CUR 图标浏览器");
  UC_TextOut(win_about, 60, 30, DT_NOOVER, "版本 1.0");
  setcolor(2);
  UC_TextOut(win_about, 60, 60, DT_NOOVER, "北京希望高科技集团, 1996,8");
  setcolor(9);
  UC_TextOut(win_about, 60, 80, DT_NOOVER, "作者: 简晶");
}

void closeabout()
{
	UC_WindowClose();
}

void about(void)
{
  win_about=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR, 300, 200,
                            "关于本程序", draw_about, closeabout, NULL);
  UC_DefineLabel(win_about, 10, 10, NULL, NULL, NULL, myico, 1);
  UC_DefinePressButton(win_about, closeabout, 110,130,80,0,"退出", SK_ENTER);
  UC_DefineActive(win_about,1,1);
  UC_WindowEnable(win_about);
}

// 显示一行ICO图标
// 为提高REDRAW速度, 只显示在当前视窗范围内的图标
int showline(WINDOWS *wnd, int i, int k )
{
	int handle, x, y, j, l;
	char *ico_xor, *ico_and, *filebuf;
	char filename[MAXPATH];
	WORD ico_w, ico_h;
	struct viewporttype viewinfo;

	getviewsettings(&viewinfo);                     // 得到当前重画视窗范围
	setfillstyle(wnd->boardstyle, wnd->boardcolor);
	settextstyle(0,0,16);
	setcolor(0);
	y=10+i*60;
	if ((y+60+wnd->top+wnd->vy)<viewinfo.top) return k+H_cnt;
	if ((y+wnd->top+wnd->vy)>viewinfo.bottom) return k+H_cnt;
	for (j=0; j<H_cnt; j++) {
       x=24+j*80;
       if (((x-16+80+wnd->left+wnd->vx)>=viewinfo.left)&&
          ((x-16+wnd->left+wnd->vx)<=viewinfo.right)) {
          UC_WindowBar(wnd, x-16, y, 80, 60);
          if (k<ico_count) {
             if (ico_handle[k]) {// 如果有XMS装入过, 则只需显示XMS图标
                UC_WinOutTextXY(wnd, x-16+(64-textwidth(ico_name[k]))/2,
                                y+34, DT_NOOVER, ico_name[k]);
                UC_ShowIcon(wnd, ico_handle[k], ico_xor, ico_and, ico_w, ico_h, x, y);
             }
             else {
                   strcpy(filename, path);
                   strcat(filename, ico_name[k]);
                   if (old_argw==0) strcat(filename, ".ICO");
		   else strcat(filename, ".CUR");
                   handle=open(filename, O_RDONLY | O_BINARY);
                   if (handle!=-1) {
                       l=filelength(handle);
                       if ((filebuf=malloc(l))!=NULL) {
                           read(handle, filebuf, l);
                           close(handle);
                           if (UC_CreateIcon(filebuf, &ico_xor, &ico_and,
                                             &ico_w, &ico_h, ico_cnt[k])) {
                               ico_handle[k]=UC_XMSLoadIcon(ico_xor, ico_and, ico_w, ico_h);
                               UC_WinOutTextXY(wnd, x-16+(64-textwidth(ico_name[k]))/2,
                                               y+34, DT_NOOVER, ico_name[k]);
                               UC_ShowIcon(wnd, ico_handle[k], ico_xor, ico_and, ico_w, ico_h, x, y);
                               free(ico_xor);
                               free(ico_and);
                           }
                           free(filebuf);
                       } else close(handle);
                   }
             }
	  }
       }
       k++;
   }
   return k;
}


void main_redraw(WINDOWS *wnd)
{
   int i, k;

   if (!ico_count) return;          // 没有ICO 图标可显示, 返回

   k=ico_cure-1;
   for (i=0; i<V_cnt; i++) {
       k=showline(wnd, i, k);
   }
}

void main_close()
{
   UC_CloseUCVision(NULL, NULL); //over);
}

void main_resize(WINDOWS *wnd)
{
   H_cnt=(wnd->width-wnd->vx-wnd->vr)/80;
   V_cnt=(wnd->height-wnd->vy-wnd->vb)/60;
   if (H_cnt<1) H_cnt=1;
   if (V_cnt<1) V_cnt=1;
   if (ico_count>H_cnt*V_cnt) {
       wnd->vbar->max=(ico_count+H_cnt-1)/H_cnt-V_cnt+1;
   } else wnd->vbar->max=1;

   if (ico_cure+H_cnt*V_cnt-1 > ico_count) {
      if (ico_count>H_cnt*V_cnt) {
	  ico_cure=ico_count-H_cnt*V_cnt+1;
      } else {
	  ico_cure=1;
      }
   }
   wnd->vbar->cure=(ico_cure-1+H_cnt-1)/H_cnt;
   if (V_cnt>1) wnd->vbar->page=V_cnt-1;
   else wnd->vbar->page=1;
}

void openok(char *ptmp)
{
   struct ffblk ffblk;
   int done;
   WORD i, j, k; //, w, h;
   int handle;
   char *filebuf;
   WORD ico_w, ico_h, ico_bits;

   strcpy(tmp, ptmp);
   UC_MouseShapeType(IDC_WAIT);
   while (ico_count) {
	ico_count--;
	if (ico_handle[ico_count]) {
		UC_FreeXMS(ico_handle[ico_count]);
		ico_handle[ico_count]=0;
	}
   }
   UC_RedrawWindow(main_win);     // 刷新新的窗口
   UC_MouseShapeType(IDC_ARROW);

   i=strlen(tmp);
   for (j=i; j>0; j--) {
       if (tmp[j-1]=='\\') {
	  tmp[j]=NULL;
	  break;              // tmp=当前ICO图标所在的路径
       }
   }
   strcpy(path, tmp);

   if (argw==0) done=findfirst("*.ico", &ffblk, 0);
   else done=findfirst("*.cur", &ffblk, 0);

   UC_MouseShapeType(IDC_WAIT);
   while (!done) {
	   for (j=0; j<9; j++) {
	       if (ffblk.ff_name[j]=='.') break;
	       ico_name[ico_count][j]=ffblk.ff_name[j];
	   }
	   ico_name[ico_count][j]=NULL;
	   handle=open(ffblk.ff_name, O_RDONLY | O_BINARY);
	   if (handle!=-1) {
	       k=filelength(handle);
	       if ((filebuf=malloc(k))!=NULL) {
		  read(handle, filebuf, k);
		  close(handle);
		  j=1; i=ico_count;
		  while (UC_GetIconInfo(filebuf, j, &ico_w, &ico_h, &ico_bits)) {
		      ico_handle[ico_count]=0;
		      ico_cnt[ico_count]=j;
		      ico_count++;
		      strcpy(ico_name[ico_count], ico_name[i]);
		      j++;
		  }
		  free(filebuf);
	       } else close(handle);
	   }
	   done = findnext(&ffblk);
   }
   UC_MouseShapeType(IDC_ARROW);

   main_resize(main_win);
   UC_DisplayScrollbar(main_win, main_win->vbar);
   old_argw=argw;
   main_redraw(main_win);
}

void openfail()
{
}

void fileopen(void)
{
  static char *arg[]={"*.ico", "*.cur", NULL};

  UC_DialogFileOpen(LOADFILE, &argw, arg, NULL, openok, openfail);
}

// 键盘事件处理函数
int cbKey(int keycode, char keystate)
{
   WINDOWS *wnd;
   SCROLLBAR *sbar;
   float pcure;

   keystate=keystate;
   wnd=UC_GetCurrentWindow();
   sbar=wnd->vbar;
   pcure=sbar->cure;
   switch (keycode) {
	case SK_ARROWUP:
	     if (pcure) {
		     sbar->cure--;
	     } else return 1;
	     break;
	case SK_ARROWDOWN:
	     if (pcure+1<sbar->max) {
		     sbar->cure++;
	     } else return 1;
	     break;
	case SK_PAGEUP:
	     if (pcure>=sbar->page) {
		     sbar->cure-=sbar->page;
	     } else {
		     if (sbar->cure==0) return 1;
		     sbar->cure=0;
	     }
	     break;
	case SK_PAGEDOWN:
	     if (pcure+sbar->page+1<sbar->max) {
		     sbar->cure+=sbar->page;
	     } else {
		     if (sbar->cure==sbar->max-1) return 1;
		     sbar->cure=sbar->max-1;
	     }
	     break;
	case SK_HOME:
	     if (sbar->cure==0) return 1;
	     sbar->cure=0;
	     break;
	case SK_END:
	     if (sbar->cure==sbar->max-1) return 1;
	     sbar->cure=sbar->max-1;
	     break;
	default: return NULL;
   }
   UC_UpdateScrollbar(wnd, sbar);
   movud(sbar, pcure);
   return 1;
}

void movud(SCROLLBAR *sbar, float pcure)
{
   int ic, oldx, oldy, newx, newy, i;

   pcure=pcure;
   ic=ico_cure;
   ico_cure=sbar->cure*H_cnt+1;
   if (ico_cure<1) ico_cure=1;
   oldx=newx=0;
   if (ic<ico_cure) {
      oldy=60; newy=0; i=V_cnt-1;
   }
   if (ic>ico_cure) {
      oldy=0; newy=60; i=0;
   }
   if (ic!=ico_cure) {
      if (abs(ic-ico_cure)!=H_cnt) main_redraw(main_win);      // 非滚动时全部刷新
      else {
	   UC_ScrollBlock(main_win, oldx, oldy, H_cnt*80, V_cnt*60-60,
			  newx, newy);
	   showline(main_win, i, ico_cure-1+i*H_cnt);           // 否则仅显示滚动后的一行
      }
   }
}

void drawbkg(WINDOWS *wnd)
{
    setfillstyle(1, 2);
    UC_WindowBar(wnd, 0, 0, wnd->width, wnd->height);
    setfillstyle(9, 4);
    setwritemode(OR_PUT);
    UC_WindowBar(wnd, 0, 0, wnd->width, wnd->height);
    setwritemode(COPY_PUT);
    setfillstyle(0,0);
}

void draw_info()
{
  char st[10];
  char str[64];

  setcolor(0);
  settextstyle(0, 0, win_info->syschar_size);
  strcpy(str, "宽度: ");
  itoa(info_wide, st, 10);
  strcat(str, st);
  UC_TextOut(win_info, 261, 21, DT_NOOVER, str);
  strcpy(str, "高度: ");
  itoa(info_dept, st, 10);
  strcat(str, st);
  UC_TextOut(win_info, 261, 51, DT_NOOVER, str);
  strcpy(str, "颜色: ");
  if (info_bits==1) strcat(str, "单色");
  if (info_bits==4) strcat(str, "16色");
  if (info_bits==8) strcat(str, "256色");
  UC_TextOut(win_info, 261, 81, DT_NOOVER, str);
  setcolor(15);
  settextstyle(0, 0, win_info->syschar_size);
  strcpy(str, "宽度: ");
  itoa(info_wide, st, 10);
  strcat(str, st);
  UC_TextOut(win_info, 260, 20, DT_NOOVER, str);
  strcpy(str, "高度: ");
  itoa(info_dept, st, 10);
  strcat(str, st);
  UC_TextOut(win_info, 260, 50, DT_NOOVER, str);
  strcpy(str, "颜色: ");
  if (info_bits==1) strcat(str, "单色");
  if (info_bits==4) strcat(str, "16色");
  if (info_bits==8) strcat(str, "256色");
  UC_TextOut(win_info, 260, 80, DT_NOOVER, str);

  UC_DrawDDB(win_info, NULL, "zoomicoa", 20, 20, 200, 200, XY_SCR, AND_PUT);
  UC_DrawDDB(win_info, NULL, "zoomico", 20, 20, 200, 200, XY_SCR, XOR_PUT);
}

void closeinfo()
{
  unlink("zoomico.ddb");
  unlink("zoomicoa.ddb");
  UC_WindowClose();
}

void moveinfo(int x, int y, WORD s)
{
  x=x;
  y=y;
  s=s;
  UC_BoardMoveMouse();            // 点中信息窗口中间, 自定义移动该窗口
}

void showinfo(int x, int y, WORD s)
{
   int k, l, handle;
   char filename[MAXPATH];
   char *filebuf, *ico_xor, *ico_and;

   s=s;
   x-=main_win->left+main_win->vx;
   y-=main_win->top+main_win->vy;

   x=x/80; if (x>=H_cnt) return;
   y=y/60; if (y>=V_cnt) return;
   k=y*H_cnt+x+ico_cure-1;
   if (k<ico_count) {
      setfillstyle(0,15);
      setwritemode(XOR_PUT);
      UC_WindowBar(main_win, 24+x*80-16, 10+y*60, 80, 60);
      l=UC_WaitFreeMouse(main_win, 24+x*80-16, 10+y*60,
			 24+x*80-16+80, 10+y*60+60, NULL);
      UC_WindowBar(main_win, 24+x*80-16, 10+y*60, 80, 60);
      setwritemode(COPY_PUT);
      if (!l) return;
      strcpy(filename, path);
      strcat(filename, ico_name[k]);
      if (argw==0) strcat(filename, ".ICO");
      else strcat(filename, ".CUR");
      handle=open(filename, O_RDONLY | O_BINARY);
      if (handle==-1) return;
      l=filelength(handle);
      if ((filebuf=malloc(l))==NULL);
      read(handle, filebuf, l);
      close(handle);
      UC_GetIconInfo(filebuf, ico_cnt[k], &info_wide, &info_dept, &info_bits);
      if (UC_CreateIcon(filebuf, &ico_xor, &ico_and,
			&info_wide, &info_dept, ico_cnt[k])) {
	 UC_MouseShapeType(IDC_WAIT);
	 UC_ZoomIcon(ico_xor, info_wide, info_dept, "zoomico", 200, 200,
		     GRAPH_BUFFER, BUFFER_LEN);
	 UC_ZoomIcon(ico_and, info_wide, info_dept, "zoomicoa", 200, 200,
		     GRAPH_BUFFER, BUFFER_LEN);
	 free(ico_xor);
	 free(ico_and);
	 strcat(filename, " 信息");
	 strcpy(tmp, filename);
	 win_info=UC_DefineWindow(WS_NOSIZE, CP_MIDSCR, CP_MIDSCR, 400, 270,
				  tmp, draw_info, closeinfo, NULL);
	 win_info->boardstyle=NULL;
	 UC_DefineDrawBackground(win_info, drawbkg);
	 UC_DefinePressButton(win_info, closeinfo, 260,180,80,0,"确定",SK_ENTER);
	 UC_DefineActive(win_info,1,1);
	 UC_DefineUserMouseEvent(win_info, 0, 0, 0, 0, USM_LEFT, UC_GetIDC(IDC_SIZE), moveinfo);
	 UC_WindowEnable(win_info);
	 UC_MouseShapeType(IDC_ARROW);
      }
      free(filebuf);
   }
}

void My_app(void)
{
   static char *menu_text1[]={ "O 打开文件[O]... ",
			       "-",
			       "A 关于[A]...     ",
			       "D [D]osShell     ",
			       "X 退出[X]...     ",
			       NULL
   };
   static void (*menu_func1[])()={ fileopen,
				   NULL,
				   about,
				   UC_ToDosPrompt,
				   main_close,
				   NULL
   };
   static MENUS tmenu[]={
			 { "F 文件[F] ", menu_text1, menu_func1, 0    },
			 { NULL,       NULL,       NULL,       NULL }
   };

   main_win=UC_DefineWindow(WS_MAIN,
                            CP_MIDSCR, CP_MIDSCR,
			    CW_USEDEFAULT, CW_USEDEFAULT,
			    "Windows ICO/CUR 图标浏览器",
			    main_redraw, main_close, main_resize);
   UC_DefineWindowIcon(main_win, myico, 0, NULL);
   UC_DefineMenu(main_win, tmenu);
   UC_DefineWindowVScrollbar(main_win, movud);
   UC_DefineUserMouseEvent(main_win, 0, 0, 0, 0, USM_LEFT, UC_GetIDC(IDC_HAND), showinfo);
   UC_DefineKeyboardEvent(main_win, cbKey);

   UC_WindowEnable(main_win);
   UC_MainLoop();
}

void main()
{
   My_begin();
   My_app();
}
