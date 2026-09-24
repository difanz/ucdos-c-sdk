/*---------------------------------------------------------
    围棋        1995,10,10 起写                        简晶
                1996,5  用 SDK 改写
---------------------------------------------------------*/

#define ALT_X      0x2D           // 结束游戏
#define UP_KEY    0x48            // 向上移动光标
#define DOWN_KEY  0x50            // 向下移动光标
#define LEFT_KEY  0x4B            // 向左移动光标
#define RIGHT_KEY 0x4d            // 向右移动光标
#define F1_KEY    0x3b            // 帮助
#define SPACE     0x20            // 落子
#define ENTER     0x0D            // 回车
#define BACKSPACE 0x8             // 悔棋
#define TAB       0x9             // 写数字

#include <dir.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sdk.h"

extern void myico(void);                // 系统ICO
extern void go_man1(void);              // 黑子ICO
extern void go_man2(void);              // 白子ICO
extern void go_cursor1(void);           // 黑光标ICO
extern void go_cursor2(void);           // 白光标ICO

void initgo(void);
void Main_menu(void);
void Draw_GoBoard(void);                // 画棋盘
int Press_key(char x,char y);           // 等待按键子程序
void Redraw_GoBoard(void);              // 重画当前棋盘内容
int Play_Go(int c, char s);             // 进行游戏键盘事件处理函数
void End_game(void);
void Draw_xxo(char x,char y);           // 画主点
void Draw_X(char x, char y);            // 画十字
void Draw_Cursor(char x ,char y);       // 画移动光标
void Draw_man(char x, char y);          // 画棋子
char TZ_call (char m, char x, char y);
char TZ_man(char m, char x, char y);    // 提子自动判别

void draw_winname1(void);
void draw_winname2(void);
void draw_wingoboard(WINDOWS *wnd);
void ok_key(void);
void put_pre(void);
void About_game(void);
void Draw_num(void);
void premove(int x, int y, WORD s);
void mousemove(int x, int y, WORD s);
void mouseplay(int x, int y, WORD s);
void putgo(void);
void new_game(void);
void save_game(void);
void saveok(char *pathfile);

char name1[40], name2[40];
char Cure_man, J_X, J_Y, Go_Board[20][20];        // 棋盘黑白矩阵
char jx, jy, ppp;
char draw_number, tmpdn, tmphm, handmove;    // 是否写数字标记(1=Yes)
int Go_count[362],TZ_count[181];             // 棋子计数矩阵
int need_count=1, count,tz_count,r,err_sound,put_sound, argw=0;
char Cure_X, Cure_Y;
char tmp[MAXPATH];                           // 临时文件名
char numbuf[4]="1";                              // 第几手数字输入缓冲区
char first_man=1;

char *man1_xor_buf, *man1_and_buf;
char *man2_xor_buf, *man2_and_buf;
char *cur1_xor_buf, *cur1_and_buf;
char *cur2_xor_buf, *cur2_and_buf;
WORD man1_ico_width, man1_ico_depth;
WORD man2_ico_width, man2_ico_depth;
WORD cur1_ico_width, cur1_ico_depth;
WORD cur2_ico_width, cur2_ico_depth;

WINDOWS *win_name1, *win_name2, *win_goboard, *win_text, *win_about;
WINDOWS *win_opti;

CHECK *num_sele, *move_sele;
BYTE buf[3000];

void xchg_snum()
{
        tmpdn=!tmpdn;
}

void xchg_hmov()
{
        tmphm=!tmphm;
}

void todos(void)
{
        put_pre();
        UC_ToDosPrompt();
}

void cucv(void)
{
        UC_CloseUCVision(NULL, NULL);
}

void yq(void)
{
        saveok(tmp);
}

void quitsave(char *pathfile)
{
        strcpy(tmp, pathfile);
        UC_CloseUCVision(NULL, yq);
}

void quityes(void)
{
        static char *arg[]={"*.go", "*.*", NULL};

        UC_DialogFileOpen(SAVEFILE, &argw, arg, "退出存盘", quitsave, cucv);
}

void quitno(void)
{
        UC_CloseUCVision(NULL, NULL);
}

void End_game()
{
  if (count) {
          UC_DialogYesNo(quityes, quitno, NULL, "注意:\n\n当前棋局未存盘, 在退出游戏前\n是否将棋局存盘?");
          return;
  }
  UC_CloseUCVision(NULL, NULL);
}

void Draw_num()
{
          draw_number=!draw_number;
          UC_MouseHide();
          put_pre();
          Redraw_GoBoard();
          ok_key();
          UC_MouseShow();
}

void draw_about(WINDOWS *wnd)
{
  setcolor(12);
  setcolorbm(15);
  settextstyle(0, 0, wnd->syschar_size);
  UC_TextOut(win_about, 60, 10, DT_NOOVER, "围棋游戏程序");
  UC_TextOut(win_about, 60, 30, DT_NOOVER, "版本 1.0");
  setcolor(2);
  setcolor(9);
  UC_TextOut(wnd, -1, 100, DT_NOOVER, "作者: 简晶 1996.5-7");
}

void About_game(void)
{
  win_about=UC_DefineWindow(WS_NOSIZE,-1,-1,300,200,"关于本程序",
                                draw_about, UC_WindowClose, NULL);
  UC_DefineLabel(win_about, 10, 10, NULL, NULL, NULL, myico, 1);
  UC_DefinePressButton(win_about, UC_WindowClose, 120,130,60,0,"确定", CMD_OK);
  UC_DefineActive(win_about,TYPE_BUTTON,1);
  UC_WindowEnable(win_about);
}

void draw_wingoboard(WINDOWS *wnd)
{
  wnd=wnd;
        Draw_GoBoard();
        Redraw_GoBoard();
        if (ppp) {             // 如果是 RESIZE 后调用, 需要重新保存手背景
                ok_key();
                ppp=0;
                return;
        }
        UC_MouseHide();       // 否则只需重画即可
        Draw_Cursor(Cure_X, Cure_Y);
        UC_MouseShow();
}

void name_ok(void)
{
        UC_WindowClose();
        new_game();
}

void opti_ok(void)
{
  UC_WindowClose();
  draw_number=!tmpdn;
  handmove=tmphm;
  need_count=atoi(numbuf);
  Draw_num();
}

void num_chg()
{
  tmpdn=!tmpdn;
}

void move_chg()
{
  tmphm=!tmphm;
}

void option()
{
  tmpdn=draw_number;
  tmphm=handmove;
  win_opti=UC_DefineWindow(WS_NOSIZE,-1,-1,350,160,"选择项",
                                                                  NULL, UC_WindowClose, NULL);
  move_sele=UC_DefineCheckButton(win_opti, "跟随鼠标[F]", handmove,
                                                                                        80, 20, SK_ALTF, move_chg);
  num_sele=UC_DefineCheckButton(win_opti, "显示数字[N]", draw_number,
                                                                                  80, 40, SK_ALTN, num_chg);
  UC_DefineLabel(win_opti, 100, 60, NULL, NULL, "从第", 0, 0);
  UC_DefineInputLine(win_opti, 100+win_opti->syschar_size*2, 60,
                                                 5, numbuf, 3, opti_ok);
  UC_DefineLabel(win_opti,100+win_opti->syschar_size*5, 60, NULL, NULL,
                                        "手开始显示数字", 0, 0);

  UC_DefinePressButton(win_opti, opti_ok, 100, 100, 60, 0, "确定", CMD_OK); //K
  UC_DefinePressButton(win_opti, UC_WindowClose, 200, 100, 60, 0, "取消", CMD_CANCEL); //K
  UC_DefineActive(win_opti,1,1);
  UC_WindowEnable(win_opti);
}

void first_set(void)
{
        int i, j;

        UC_MouseHide();
        if (Cure_X!=10&&Cure_Y!=10) put_pre();
        Cure_man=first_man; Cure_X=10; Cure_Y=10;
        count=0; J_X=0; J_Y=0; jx=0; jy=0;
        for (i=1;i<20; i++) {
                 for (j=1; j<20; j++) Go_Board[i][j]=0;
        }
        Draw_GoBoard();
        Redraw_GoBoard();
        ok_key();
        UC_MouseShow();
}

void new_game()
{
  first_man=1;
  first_set();
}

void new_game_sub()
{
        WORD i;

        if (count) {
                i=UC_DialogYesNo(NULL, NULL, NULL, "注意:\n是否确定退出当前棋局?");
                if (i!=ID_YES) return;
        }
        new_game();
}

void resize_borad(WINDOWS *wnd)
{
  wnd=wnd;
        ppp=1;            // resize 标记
}

void load_newgo(void)
{
  FILE * new_go;
  char goname[3];
  int i, count1;

  first_set();
  new_go=fopen(tmp, "rb");
  fread(goname, 2, 1, new_go);
  goname[2]=NULL;
  if (strcmp(goname, "GO")) {          // 不是围棋谱档案, 出错提示
          UC_DialogWarning(NULL, NULL, "%s\n该文件不是棋谱档案!", tmp);
          fclose(new_go);
          return;
  }
  fread(&count1, 2, 1, new_go);        // 读入手数
  fread(Go_count, 2, count1+1, new_go); // 读入走棋记录
  fclose(new_go);

  for (i=1; i<=count1; i++) {          // 自动走入棋盘
                put_pre();
                Cure_Y=Go_count[i]/20;
                Cure_X=Go_count[i]-Cure_Y*20;
                ok_key();
                putgo();
  }
}

void loadok(char *pathfile)
{
         WORD i;

         strcpy(tmp, pathfile);
         if (count) {
                 i=UC_DialogYesNo(NULL, NULL, NULL, "注意:\n是否确定退出当前棋局?"); //yesnew, nonew
                 if (i!=ID_YES) return;
         }
         load_newgo();
}

void saveok(char *pathfile)
{
         FILE *new_file;

         strcpy(tmp, pathfile);
         unlink(tmp);     // 删除指定文件
         if ((new_file = fopen(tmp, "wb")) == NULL) return;
         fwrite("GO", 2, 1, new_file);
         fwrite(&count, 2, 1, new_file);
         fwrite(Go_count, 2, count+1, new_file);
         fclose(new_file);
}

void savefail()
{
}

void save_game(void)
{
  static char *arg[]={"*.go", "*.*", NULL};

  UC_DialogFileOpen(SAVEFILE, &argw, arg, NULL, saveok, savefail);
}

void load_game()
{
  static char *arg[]={"*.go", "*.*", NULL};

  UC_DialogFileOpen(LOADFILE, &argw, arg, NULL, loadok, savefail);
}


void Draw_xxo(char x,char y)
{
  UC_MouseHide();
  circle(win_goboard->left+win_goboard->vx+x*20,
                        win_goboard->top+win_goboard->vy+y*20,1);
  circle(win_goboard->left+win_goboard->vx+x*20,
                        win_goboard->top+win_goboard->vy+y*20,2);
  circle(win_goboard->left+win_goboard->vx+x*20,
                        win_goboard->top+win_goboard->vy+y*20,3);
  UC_MouseShow();
}

void Draw_X(char x, char y)
{
  UC_MouseHide();
  setfillstyle(1,BROWN);
  bar (win_goboard->left+win_goboard->vx+x*20-9,
                 win_goboard->top+win_goboard->vy+y*20-9,
                 win_goboard->left+win_goboard->vx+x*20+9,
                 win_goboard->top+win_goboard->vy+y*20+9);
  setcolor(0);
  if (x>1) line (win_goboard->left+win_goboard->vx+x*20-9,
                                          win_goboard->top+win_goboard->vy+y*20,
                                          win_goboard->left+win_goboard->vx+x*20,
                                          win_goboard->top+win_goboard->vy+y*20);
  if (x<19) line (win_goboard->left+win_goboard->vx+x*20,
                                                win_goboard->top+win_goboard->vy+y*20,
                                                win_goboard->left+win_goboard->vx+x*20+9,
                                                win_goboard->top+win_goboard->vy+y*20);
  if (y>1) line (win_goboard->left+win_goboard->vx+x*20,
                                          win_goboard->top+win_goboard->vy+y*20-9,
                                          win_goboard->left+win_goboard->vx+x*20,
                                          win_goboard->top+win_goboard->vy+y*20);
  if (y<19) line (win_goboard->left+win_goboard->vx+x*20,
                                                win_goboard->top+win_goboard->vy+y*20,
                                                win_goboard->left+win_goboard->vx+x*20,
                                                win_goboard->top+win_goboard->vy+y*20+9);
  if (x==4||x==10||x==16) {
                if (y==4||y==10||y==16) Draw_xxo(x,y);
          }
  UC_MouseShow();
}

void Draw_Cursor(char x ,char y)
{
  UC_MouseHide();
  if (Cure_man==1)
     UC_ShowIcon (win_goboard, NULL, cur1_xor_buf, cur1_and_buf,
                  cur1_ico_width, cur1_ico_depth, 4+x*20-12,3+y*20-8);
  else UC_ShowIcon (win_goboard, NULL, cur2_xor_buf, cur2_and_buf,
                    cur2_ico_width, cur2_ico_depth, 4+x*20-12,3+y*20-8);
  UC_MouseShow();
}

void d_circle (int x, int y)
{
  if (Cure_man==1)
     UC_ShowIcon (win_goboard, NULL, man1_xor_buf, man1_and_buf,
                  man1_ico_width, man1_ico_depth, 4+x*20-13,4+y*20-14);
  else UC_ShowIcon (win_goboard, NULL, man2_xor_buf, man2_and_buf,
                    man2_ico_width, man2_ico_depth, 4+x*20-13,4+y*20-14);
}

void Draw_man(char x, char y)
{
  char c_str[10];
  int ox, dcount;

  dcount=count-need_count+1;

  UC_MouseHide();
  d_circle(x,y);              // 显示棋子
  if (draw_number&&dcount>0) {
                settextstyle(0,0,13);
                if (Cure_man==1) setcolor(15);
                        else setcolor(0);
                if (dcount>99) ox=x*20-9;
                        else {
                                  if (dcount<10) ox=x*20-3;
                                          else ox=x*20-6;
                                  }
                outtextxy(win_goboard->left+win_goboard->vx+ox,
                                         win_goboard->top+win_goboard->vy+y*20-6,
                                         itoa(dcount,c_str,10));
  }
  UC_MouseShow();
}

void Draw_GoBoard(void)
{
  char x,y;

  UC_MouseHide();
  setfillstyle(1,BROWN);
  bar(win_goboard->left+win_goboard->vx+5,
                win_goboard->top+win_goboard->vy+5,
                win_goboard->left+win_goboard->vx+395,
                win_goboard->top+win_goboard->vy+395);
  setcolor(14);
  rectangle(win_goboard->left+win_goboard->vx+4,
                                win_goboard->top+win_goboard->vy+4,
                                win_goboard->left+win_goboard->vx+396,
                                win_goboard->top+win_goboard->vy+396);
  setcolor(7);
  rectangle(win_goboard->left+win_goboard->vx+5,
                                win_goboard->top+win_goboard->vy+5,
                                win_goboard->left+win_goboard->vx+395,
                                win_goboard->top+win_goboard->vy+395);
  rectangle(win_goboard->left+win_goboard->vx+3,
                                win_goboard->top+win_goboard->vy+3,
                                win_goboard->left+win_goboard->vx+397,
                                win_goboard->top+win_goboard->vy+397);

  setcolor(0);
  for (y=1; y<20; y++)
                line (win_goboard->left+win_goboard->vx+20,
                                win_goboard->top+win_goboard->vy+y*20,
                                win_goboard->left+win_goboard->vx+20*19,
                                win_goboard->top+win_goboard->vy+y*20);
  for (x=1; x<20; x++)
                line (win_goboard->left+win_goboard->vx+x*20,
                                win_goboard->top+win_goboard->vy+20,
                                win_goboard->left+win_goboard->vx+x*20,
                                win_goboard->top+win_goboard->vy+20*19);

  setfillstyle(1,BLACK);
  Draw_xxo(4,4);
  Draw_xxo(16,16);
  Draw_xxo(4,16);
  Draw_xxo(16,4);
  Draw_xxo(10,10);
  Draw_xxo(4,10);
  Draw_xxo(10,4);
  Draw_xxo(16,10);
  Draw_xxo(10,16);
  UC_MouseShow();
}

char TZ_man(char m, char x, char y)
{
  char d;

  tz_count=0;
  d=3-m;
  Go_Board[y][x]=m;

  if (y>1 && Go_Board[y-1][x]==d) TZ_call(d,x,y-1);
  if (x<19 && Go_Board[y][x+1]==d) TZ_call(d,x+1,y);
  if (y<19 && Go_Board[y+1][x]==d) TZ_call(d,x,y+1);
  if (x>1 && Go_Board[y][x-1]==d) TZ_call(d,x-1,y);
  if (TZ_call(m,x,y)) {
          Cure_man=3-Cure_man;
          sound(err_sound);delay(100);nosound();
          return 1;    // 落子出错, 返回1
          }
  return 0;       // 落子无错, 返回0
}

char TZ_call (char m, char x, char y)
{
  int r;
  char p,q;

rep_TZ:

  r=0;
  tz_count++; TZ_count[tz_count]=y*20+x; Go_Board[y][x]=5; //m+.5

rep_TZ1:

  if ((x>1 && Go_Board[y][x-1]==0)||
          (y>1 && Go_Board[y-1][x]==0)||
          (y<19 && Go_Board[y+1][x]==0)||
          (x<19 && Go_Board[y][x+1]==0))
          {
                 for (r=1;r<=tz_count;r++) {
                          p=TZ_count[r]/20; q=TZ_count[r]-p*20;
                          Go_Board[p][q]=m;
                          }
                 tz_count=0;
                 return 0;
          }
  if (x>1 && Go_Board[y][x-1]==m) { x--; goto rep_TZ; }
  if (y>1 && Go_Board[y-1][x]==m) { y--; goto rep_TZ; }
  if (x<19 && Go_Board[y][x+1]==m) { x++; goto rep_TZ; }
  if (y<19 && Go_Board[y+1][x]==m) {y++; goto rep_TZ; }

  r++;
  if (r<tz_count) {
                y=TZ_count[tz_count-r]/20; x=TZ_count[tz_count-r]-y*20;
                goto rep_TZ1;
                }
  for (r=1;r<=tz_count;r++) {
                p=TZ_count[r]/20; q=TZ_count[r]-p*20;
                Go_Board[p][q]=0;
                UC_MouseHide();
                put_pre();
                Draw_X(q,p);
                ok_key();
                UC_MouseShow();
                Go_Board[p][q]=0;
                }
  if (tz_count==1) {J_X=q; J_Y=p; }         // 记录劫争位置
  tz_count=0;
  return 1;
}

void Redraw_GoBoard()
{
        int i, j, ct;
        char x, y, man;

        man=Cure_man;
        ct=count;
        count=0;

        UC_MouseHide();
        for (i=1;i<=ct;i++) {
                  y=Go_count[i]/20; x=Go_count[i]-y*20;
                  Draw_X(x,y);
                  count++;
                  if (Go_Board[y][x]==1) { Cure_man=1; Draw_man(x,y); }
                  else if (Go_Board[y][x]==2) { Cure_man=2; Draw_man(x,y); }
        }
        UC_MouseShow();
        Cure_man=man;
        count=ct;
}

void put_pre()
{
  UC_MouseHide();
  putimage(win_goboard->left+win_goboard->vx+Cure_X*20-5,
                          win_goboard->top+win_goboard->vy+Cure_Y*20-5,
                          buf,COPY_PUT);
  UC_MouseShow();
}

void ok_key()
{
  UC_MouseHide();
  getimage(win_goboard->left+win_goboard->vx+Cure_X*20-5,
                          win_goboard->top+win_goboard->vy+Cure_Y*20-5,
                          win_goboard->left+win_goboard->vx+Cure_X*20+27,
                          win_goboard->top+win_goboard->vy+Cure_Y*20+27,buf);
  Draw_Cursor(Cure_X, Cure_Y);
  UC_MouseShow();
  return;
}

void premove(int x, int y, WORD s)
{
   if (handmove) mousemove(x, y, s);
}

void mousemove(int x, int y, WORD s)
{
  s=s;
        x=(x-10-win_goboard->left-win_goboard->vx)/20+1;
        y=(y-10-win_goboard->top-win_goboard->vy)/20+1;
        if (x<1) x=1;
        if (y<1) y=1;
        if (x>19) x=19;
        if (y>19) y=19;
        if ((Cure_X==x)&&(Cure_Y==y)) return;
        put_pre();
        Cure_X=x; Cure_Y=y;
        ok_key();
}

void mouseplay(int x, int y, WORD s)
{
   mousemove(x, y, s);
   if (UC_WaitFreeMouse(win_goboard, 10, 10, 380, 380, mousemove)) putgo();  //拖动事件
}

void putgo()
{
          if ((Cure_X==J_X && Cure_Y==J_Y ) || (count>360))  // 总共有361手
                  { sound(err_sound); delay(100); nosound(); }
          else
                {
                 if (!Go_Board[Cure_Y][Cure_X])                 // 如果本位置已经落子,则不再允许落子
                  {
                         jx=J_X; jy=J_Y;
                         J_X=0; J_Y=0;
                         count++;
                         Go_count[count]=Cure_Y*20+Cure_X;

                         UC_MouseHide();
                         putimage(win_goboard->left+win_goboard->vx+Cure_X*20-5,
                                                 win_goboard->top+win_goboard->vy+Cure_Y*20-5,
                                                 buf,COPY_PUT);
                         Draw_man(Cure_X,Cure_Y);              // 显示当前棋子
                         Cure_man=3-Cure_man;                  // 交换黑白子(轮对方走棋)
                         ok_key();
                         UC_MouseShow();

                         sound(put_sound); delay(100); nosound();
                         Cure_man=3-Cure_man;                  // 交换黑白子(轮对方走棋)
                         if (TZ_man(Cure_man,Cure_X,Cure_Y))    // 提子自动判别
                                 { J_X=jx; J_Y=jy; count--;}        // 落子出错,则恢复原劫争状态
                         Cure_man=3-Cure_man;                  // 交换黑白子(轮对方走棋)
                  }
                  else { sound(err_sound); delay(100); nosound(); }
                 }
}

int Play_Go(int c, char s)
{
  int extended;

  s=s;
  extended=(c&0xff00)>>8; c=c&0x00ff;

  if (extended==ALT_X) { End_game(); return 1; }
  if (extended==F1_KEY) { About_game(); return 1; }
  if (c==TAB) {
                Draw_num();
                return 1;
  }
  if ((c==SPACE)||(c==ENTER))
          {
          putgo();
          return 1;
          }
  if (c==BACKSPACE)
          {
                 if (count > 0)
                         {
                                put_pre();
                                Cure_Y=Go_count[count]/20;
                                Cure_X=Go_count[count]-Cure_Y*20;
                                Draw_X(Cure_X,Cure_Y);        // 取消前面一子
                                Go_Board[Cure_Y][Cure_X]=0;
                                Cure_man=3-Cure_man;          // 交换黑白子
                                ok_key();
                                count--;
                         }
                 else { sound(err_sound); delay(100); nosound(); }
          return 1;
          }
  if (extended==UP_KEY)
          {
                 if (Cure_Y>1) {
                          put_pre();
                          Cure_Y--;
                          ok_key();
                 }
                 else { sound(err_sound);delay(100);nosound(); }
                 return 1;
          }
  if (extended==DOWN_KEY)
          {
                 if (Cure_Y<19) {
                          put_pre();
                          Cure_Y++;
                          ok_key();
                 }
                 else { sound(err_sound);delay(100);nosound(); }
                 return 1;
          }
  if (extended==LEFT_KEY)
          {
                 if (Cure_X>1) {
                          put_pre();
                          Cure_X--;
                          ok_key();
                 }
                 else {sound(err_sound);delay(100);nosound();}
                 return 1;
          }
  if (extended==RIGHT_KEY)
          {
                 if (Cure_X<19) {
                          put_pre();
                          Cure_X++;
                          ok_key();
                 }
                 else {sound(err_sound);delay(100);nosound();}
                 return 1;
          }
  return NULL;            // 不处理的按键
}

// 以下部份为围棋系统框架
//=====================================================================
// 程序总初始化
void initgo(void)
{
        err_sound=1500;
        put_sound=200;
        draw_number=0;
        handmove=1;
        SYSCHAR_SIZE=16;

        BUFFER_LEN=65500;
        UC_InitDesktop(1, 8, "c:\\windows\\honey.bmp", FUL_SCR);
        UC_InitUCVision(VGA, VM_640X480X16);

        UC_CreateIcon((char *)go_man1, &man1_xor_buf, &man1_and_buf,
                                 &man1_ico_width, &man1_ico_depth, 1);     // 建立黑子图标
        UC_CreateIcon((char *)go_man2, &man2_xor_buf, &man2_and_buf,
                                 &man2_ico_width, &man2_ico_depth, 1);     // 建立白子图标
        UC_CreateIcon((char *)go_cursor1, &cur1_xor_buf, &cur1_and_buf,
                                 &cur1_ico_width, &cur1_ico_depth, 1);     // 建立黑子光标
        UC_CreateIcon((char *)go_cursor2, &cur2_xor_buf, &cur2_and_buf,
                                 &cur2_ico_width, &cur2_ico_depth, 1);     // 建立白子光标

        Cure_man=1;              // 起手黑子先行
        Cure_X=10; Cure_Y=10;    // 默认初始化坐标为棋盘中间
        count=0;                 // 手数初始化0
        J_X=0; J_Y=0;
        ppp=0;                   // 棋盘 RESIZE 重画标记
}

/*-------------------------------------------------------
        完成系统窗口/菜单/事件等的定义工作, 并进入Vision主控
---------------------------------------------------------*/
void Main_menu()
{
        static void (*dfun[])()={new_game_sub,
                                 load_game,
                                 save_game,
                                 NULL,
                                 option,
                                 NULL,
                                 todos,
                                 End_game
        };
        static void (*pfun[])()={
                                 NULL,
                                 About_game
        };

        static char *dmem[]={
                              "N 新局[N]           ",
                              "O 取出存档[O]...    ",
                              "S 保存棋局[S]...    ",
                              "-",
                              "P 选择项[P]...      ",
                              "-",
                              "D 进入DOS[D]        ",
                              "X 退出[X]...  ALT+X ",
                              NULL
        };

        static char *pmem[]={
                              "H 系统帮助[H]      ",
                              "A 关于[A]...    F1 ",
                              NULL
        };

        static MENUS tmenu[]= {
                               { "F 文件[F] ", dmem, dfun, 0 },
                               { "H 帮助[H] ", pmem, pfun, 0 },
                               { NULL,         NULL, NULL, NULL }
        };


  strcpy(name1,"中国聂卫平");
  strcpy(name2,"日本吴清源");

  win_goboard=UC_DefineWindow(WS_MAIN, 0, 0, -1, -1, "围棋双人游戏",
                              draw_wingoboard, End_game, resize_borad);
  win_goboard->menubkc=7;
  UC_DefineWindowIcon(win_goboard, myico, 0, "双人围棋");
  UC_DefineMenu(win_goboard, tmenu);
  UC_DefineWindowMinSize(win_goboard, 401, 401);

  win_goboard->boardcolor=3;
//--------------------------------------
// 定义游戏键盘事件处理函数
  UC_DefineKeyboardEvent(win_goboard, Play_Go);
//--------------------------------------
  // 定义点中以后的事件
  UC_DefineUserMouseEvent(win_goboard, 10, 10, 380, 380, USM_LEFT, NULL, mouseplay);
//--------------------------------------
  // 定义移动时的事件
  UC_DefineUserMouseEvent(win_goboard, 10, 10, 380, 380, USM_MOVE, NULL, premove);

  win_name1=UC_DefineWindow(WS_NOSIZE, -1, -1, 340, 150,
                                "输入双方名字", NULL, NULL, NULL);
  UC_DefinePressButton(win_name1, name_ok, 10, 50, 60, 0, "确定[O]",CMD_OK);
  UC_DefineInputLine(win_name1, 90, 40, 20, name1, 30, name_ok);
  UC_DefineInputLine(win_name1, 90, 70, 20, name2, 30, name_ok);
  UC_DefineActive(win_name1, TYPE_INPUTLINE, 1);

  UC_MaxWindow(win_goboard);
  UC_WindowEnable(win_name1);
  UC_MainLoop();                // 进入 Vision 主控
}

/*-----------------------------------------------------------
        围棋游戏主程序
-------------------------------------------------------------*/
void main()
{
  initgo();              // 程序总初始化
  Main_menu();           // 系统窗口/菜单定义
}
