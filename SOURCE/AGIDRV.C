// AGIDRV.C - 显示驱动接口与图形环境

#include <alloc.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <math.h>
#include <mem.h>
#include "agidrv.h"
#include <conio.h>
#include "struct.h"
#include "agi_win.h"
#include "agi_bmp.h"

// 内部底层调用函数原型
void EngTextOut(TEXTINFO *textinfo);
int EngTextExtent(TEXTINFO *textinfo);
int TESTADP(void);
int GDI(void);
void EnableSDKint10(void);
void DisableSDKint10(void);
void setint8(void);
void biosint8(void);
void VGAFADEIN(int delay, char far *pal);
void VGAFADEOUT(int delay, char far *pal);
void get_grpfun(void);

// 外部驱动及退出管理指针
void (far *grp_entry)();
void (far *xms_entry)();
void (far *UCSDK_EXIT)();
char PUBLIC_BUF[4096];

// XMS 移动与句柄表
static struct {
   DWORD Length;
   WORD SourceHandle;
   DWORD SourceOffset;
   WORD DestHandle;
   DWORD DestOffset;
} xms_move;

static struct {
   WORD offset_k;
   WORD size_k;
} xms_table[1000];

// 线型与调色板缓冲
WORD mylinetype[3];
char TMP_tpal[768];
void far *agi_arcdata;
WORD agi_hg_off;
WORD agi_hg_segm;
static WORD imgsize;

// 远端弧线数据缓冲 (位于 AGIDRV5_DATA)
char far agi_arcbuf[16];

// 包含 XMS 内存管理程序
#include "xmsmm.c"

// 文本与显示环境状态
TEXTINFO textinfo = { 0, 0, NULL, 0, 0, 8, 16, 15, 0, 0, 0, NULL, 0, 1 };
char *GRAPH_BUFFER = NULL;
WORD BUFFER_LEN = 10240;
int agi_drawcolor = 15;
int agi_fillcolor = 15;
int agi_fillpattern = 1;
int agi_writemode = 0;
int agi_position_x = 0;
int agi_position_y = 0;
int agi_viewport_left = 0;
int agi_viewport_right = -1;
int agi_viewport_top = 0;
int agi_viewport_bottom = -1;
int agi_viewport_clip = 1;

int getcolor(void)
{
   return agi_drawcolor;
}

void settextextra(int extra)
{
   textinfo.CharSpace = extra;
}

void setcolor(int color)
{
   agi_drawcolor = color;
   textinfo.FrColor = color;
}

void setcolorbm(int color)
{
   textinfo.BgColor = color;
}

unsigned getpixel(int x, int y)
{
   int c;
   asm {
      mov ax, 90f0h
      mov bh, 1
      mov cx, x
      mov dx, y
      int 48h
      xor bh, bh
      mov c, bx
   }
   if (getmaxcolor() == 1 && c != 0)
      c = 1;
   return c;
}

void putpixel(int x, int y, int color)
{
   if (color < 0)
      color = 15;
   asm {
      mov ax, 90f0h
      mov bx, color
      mov bh, 0
      mov cx, x
      mov dx, y
      int 48h
   }
}

void line(int x1, int y1, int x2, int y2)
{
   asm {
      mov ax, 90f1h
      mov bx, agi_drawcolor
      mov bh, 0
      mov cx, x1
      mov dx, y1
      mov si, x2
      mov di, y2
      int 48h
   }
}

void linerel(int dx, int dy)
{
   line(agi_position_x, agi_position_y, agi_position_x + dx, agi_position_y + dy);
   agi_position_x += dx;
   agi_position_y += dy;
}

void lineto(int x, int y)
{
   line(agi_position_x, agi_position_y, x, y);
   agi_position_x = x;
   agi_position_y = y;
}

void moveto(int x, int y)
{
   agi_position_x = x;
   agi_position_y = y;
}

void outtext(char far *textstring)
{
   outtextxy(agi_position_x, agi_position_y, textstring);
}

void outtextbm(int x, int y, char far *textstring)
{
   textinfo.x = x;
   textinfo.y = y;
   textinfo.String = textstring;
   textinfo.icon = 0;
   EngTextOut(&textinfo);
}

void outtextxy(int x, int y, char far *textstring)
{
   textinfo.x = x;
   textinfo.y = y;
   textinfo.String = textstring;
   textinfo.icon = 1;
   EngTextOut(&textinfo);
}

void setascstyle(int font)
{
   textinfo.AscFontNo = font;
}

void settextstyle(int font, int direction, int charsize)
{
   textinfo.ChiFontNo = font;
   if (font == 0)
      textinfo.AscFontNo = 0;
   textinfo.FontHeight = charsize;
   textinfo.FontWidth = charsize / 2;
   (void)direction;
}

void settextheight(int charsize)
{
   textinfo.FontHeight = charsize;
}

void settextwidth(int charsize)
{
   textinfo.FontWidth = charsize;
}

int textwidth(char far *textstring)
{
   if (textstring == NULL)
      return 0;
   textinfo.String = textstring;
   return EngTextExtent(&textinfo);
}

int textheight(char far *textstring)
{
   if (textstring == NULL)
      return 0;
   textinfo.String = textstring;
   return textinfo.FontHeight;
}

void rectangle(int left, int top, int right, int bottom)
{
   asm {
      mov ax, 90f4h
      mov cx, left
      mov dx, top
      mov si, right
      mov di, bottom
      mov bx, agi_drawcolor
      mov bh, 0
      int 48h
   }
}

void bar(int left, int top, int right, int bottom)
{
   asm {
      push bp
      mov cx, left
      mov dx, top
      mov si, right
      mov di, bottom
      mov bh, 84h
      mov bp, agi_fillcolor
      mov ax, 90f4h
      int 48h
      pop bp
   }
}

void bar3d(int left, int top, int right, int bottom, int depth, int topflag)
{
   bar(left, top, right, bottom);
   rectangle(left, top, right, bottom);
   line(left, top, left + depth, top - depth);
   line(right, top, right + depth, top - depth);
   line(left + depth, top - depth, right + depth, top - depth);
   line(right, bottom, right + depth, bottom - depth);
   line(right + depth, bottom - depth, right + depth, top - depth);
   (void)topflag;
}

void setwritemode(int mode)
{
   agi_writemode = mode;
   asm {
      mov ax, 90f5h
      mov dl, byte ptr mode
      mov bh, dl
      mov bl, 5
      int 48h
   }
}

void getlinesettings(struct linesettingstype far *lineinfo)
{
   lineinfo->linestyle = mylinetype[0];
   lineinfo->upattern = mylinetype[1];
   lineinfo->thickness = mylinetype[2];
}

void setlinestyle(int linestyle, unsigned upattern, int thickness)
{
   switch (linestyle) {
      case SOLID_LINE:   upattern = 0xffff; break;
      case DOTTED_LINE:  upattern = 0xaaaa; break;
      case CENTER_LINE:  upattern = 0xe4e4; break;
      case DASHED_LINE:  upattern = 0xcccc; break;
   }
   mylinetype[0] = linestyle;
   mylinetype[1] = upattern;
   mylinetype[2] = thickness;
   asm {
      mov ax, 90f5h
      mov bl, 4
      mov cx, upattern
      int 48h
   }
}

void setfillpattern(char far *upattern, int color)
{
   if (color < 0)
      color = 15;
   asm {
      push ds
      mov ax, 90eeh
      mov bh, 4
      mov si, word ptr upattern
      mov ds, word ptr upattern+2
      int 48h
      pop ds
   }
   setfillstyle(USER_FILL, color);
}

void setfillstyle(int pattern, int color)
{
   if (color < 0)
      color = 15;
   agi_fillcolor = color;
   agi_fillpattern = pattern;
   asm {
      mov ax, 90f5h
      mov bx, pattern
      mov bh, bl
      dec bh
      mov bl, 6
      int 48h
   }
}

void Setviewport(int left, int top, int right, int bottom)
{
   if (getmaxx() - 1 < right || right < 0)
      right = getmaxx() - 1;
   if (getmaxy() - 1 < bottom || bottom < 0)
      bottom = getmaxy() - 1;

   agi_viewport_left = left;
   agi_viewport_right = right;
   agi_viewport_top = top;
   agi_viewport_bottom = bottom;
   agi_viewport_clip = 1;

   asm {
      mov ax, 90f6h
      mov cx, left
      mov dx, top
      mov si, right
      mov di, bottom
      int 48h
   }
}

void setviewport(int left, int top, int right, int bottom, int clip)
{
   Setviewport(left, top, right, bottom);
   agi_position_x = left;
   agi_position_y = top;
   (void)clip;
}

unsigned imagesize(int left, int top, int right, int bottom)
{
   int width, height;
   unsigned size;
   WORD high;

   width = abs(right - left) + 1;
   height = abs(bottom - top) + 1;

   asm {
      mov ah, 4
      mov bh, 1
      mov cx, width
      call far ptr [grp_entry]
      mul height
      add ax, 4
      adc dx, 0
      mov size, ax
      mov high, dx
   }

   if (high != 0 || size == 0 || high > 0 || size > 65530U)
      size = 0xffff;

   return size;
}

void getimage(int left, int top, int right, int bottom, void far *bitmap)
{
   WORD xms_h;
   char cwd[80];
   char buf1[16], buf2[16];

   imgsize = imagesize(left, top, right, bottom);
   agi_hg_off = FP_OFF(bitmap);
   agi_hg_segm = FP_SEG(bitmap);

   if (imgsize != 0xffff && bitmap != (void far *)PUBLIC_BUF) {
      *(WORD far *)bitmap = right - left + 1;
      *(((WORD far *)bitmap) + 1) = bottom - top + 1;
      UC_GetScreenBlock((char far *)bitmap + 4, right - left + 1, bottom - top + 1, left, top);
      return;
   }

   xms_h = UC_XMSgetscreen(left, top, right - left + 1, bottom - top + 1);
   if (xms_h != 0) {
      *(WORD far *)bitmap = 0xffff;
      *(((WORD far *)bitmap) + 1) = xms_h;
   } else {
      *(WORD far *)bitmap = 0xfffe;
      itoa(FP_OFF(bitmap), buf1, 16);
      itoa(FP_SEG(bitmap), buf2, 16);
      strcat(buf2, buf1);
      getcwd(cwd, 80);
      if (cwd[strlen(cwd) - 1] != '\\')
         strcat(cwd, "\\");
      strcat(cwd, buf2);
      strcat(cwd, ".DDB");
      memmove((char far *)bitmap + 2, cwd, 80);
      UC_CreateScreenDDB(cwd, left, top, right - left + 1, bottom - top + 1);
   }
}

void killimage(char *bitmap)
{
   WORD type;
   char path[80];
   WORD xms_h;

   agi_hg_off = FP_OFF(bitmap);
   agi_hg_segm = FP_SEG(bitmap);
   type = *(WORD far *)MK_FP(agi_hg_segm, agi_hg_off);

   if (type < (WORD)-2) {
      if (bitmap != (char far *)PUBLIC_BUF)
         MyFREE(bitmap);
      return;
   }
   if (type == (WORD)-2) {
      memmove(path, (char far *)bitmap + 2, 80);
      unlink(path);
      if (bitmap != (char far *)PUBLIC_BUF)
         MyFREE(bitmap);
      return;
   }

   xms_h = *(WORD far *)MK_FP(agi_hg_segm, agi_hg_off + 2);
   UC_FreeXMS(xms_h);
   if (bitmap != (char far *)PUBLIC_BUF)
      MyFREE(bitmap);
}

void putimage(int left, int top, void far *bitmap, int op)
{
   int old_mode;
   WORD width, height;
   void far *temp_buf = NULL;
   WORD mem_size;
   char cwd[80];

   agi_hg_off = FP_OFF(bitmap);
   agi_hg_segm = FP_SEG(bitmap);

   width = *(WORD far *)bitmap;
   height = *(((WORD far *)bitmap) + 1);

   if (width < (WORD)-2) {
      BYTE depth = (BYTE)UC_GetColourDepth();
      if (depth == 1 || depth == 3) {
         mem_size = ((width + 7) >> 3) * height;
         if (depth == 1)
            mem_size <<= 2;
         mem_size += 4;
         temp_buf = malloc(mem_size);
         if (temp_buf != NULL)
            memmove(temp_buf, bitmap, mem_size);
      }

      old_mode = agi_writemode;
      setwritemode(op);
      UC_PutScreenBlock((char far *)bitmap + 4, width, height, left, top);
      setwritemode(old_mode);

      if (temp_buf != NULL) {
         memmove(bitmap, temp_buf, mem_size);
         MyFREE(temp_buf);
      }
   } else if (width == (WORD)-2) {
      memmove(cwd, (char far *)bitmap + 2, 80);
      UC_DrawDDB(NULL, 0, cwd, left, top, getmaxx() - left, getmaxy() - top, 0, op);
   } else {
      old_mode = agi_writemode;
      setwritemode(op);
      UC_XMSputscreen(height, left, top);
      setwritemode(old_mode);
   }
}

void arc(int x, int y, int stangle, int endangle, int radius)
{
   agi_arcdata = agi_arcbuf;
   *(((WORD far *)agi_arcdata) + 1) = x;
   *(((WORD far *)agi_arcdata) + 2) = y;
   *(((WORD far *)agi_arcdata) + 3) = radius;
   *(((WORD far *)agi_arcdata) + 4) = radius;
   *(((WORD far *)agi_arcdata) + 5) = stangle;
   *(((WORD far *)agi_arcdata) + 6) = endangle;
   *(((char far *)agi_arcdata) + 1) = (char)agi_drawcolor;
   *(char far *)agi_arcdata = 1;

   asm {
      push ds
      mov si, word ptr agi_arcdata
      mov bh, 2
      mov ds, word ptr agi_arcdata+2
      mov ax, 90fah
      int 48h
      pop ds
   }
}

void circle(int x, int y, int radius)
{
   arc(x, y, 0, 360, radius);
}

void pieslice(int x, int y, int stangle, int endangle, int radius)
{
   agi_arcdata = agi_arcbuf;
   *(((WORD far *)agi_arcdata) + 1) = x;
   *(((WORD far *)agi_arcdata) + 2) = y;
   *(((WORD far *)agi_arcdata) + 3) = radius;
   *(((WORD far *)agi_arcdata) + 4) = radius;
   *(((WORD far *)agi_arcdata) + 5) = stangle;
   *(((WORD far *)agi_arcdata) + 6) = endangle;
   *(((char far *)agi_arcdata) + 1) = (char)agi_drawcolor;
   *(char far *)agi_arcdata = 2;
   *(((WORD far *)agi_arcdata) + 7) = agi_fillcolor;

   asm {
      push ds
      mov si, word ptr agi_arcdata
      mov bh, 2
      mov ds, word ptr agi_arcdata+2
      mov ax, 90fah
      int 48h
      pop ds
   }
}

void sector(int x, int y, int stangle, int endangle, int xradius, int yradius)
{
   agi_arcdata = agi_arcbuf;
   *(((WORD far *)agi_arcdata) + 1) = x;
   *(((WORD far *)agi_arcdata) + 2) = y;
   *(((WORD far *)agi_arcdata) + 3) = xradius;
   *(((WORD far *)agi_arcdata) + 4) = yradius;
   *(((WORD far *)agi_arcdata) + 5) = stangle;
   *(((WORD far *)agi_arcdata) + 6) = endangle;
   *(((char far *)agi_arcdata) + 1) = (char)agi_drawcolor;
   *(char far *)agi_arcdata = 2;
   *(((WORD far *)agi_arcdata) + 7) = agi_fillcolor;

   asm {
      push ds
      mov si, word ptr agi_arcdata
      mov bh, 2
      mov ds, word ptr agi_arcdata+2
      mov ax, 90fah
      int 48h
      pop ds
   }
}

void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius)
{
   agi_arcdata = agi_arcbuf;
   *(((WORD far *)agi_arcdata) + 1) = x;
   *(((WORD far *)agi_arcdata) + 2) = y;
   *(((WORD far *)agi_arcdata) + 3) = xradius;
   *(((WORD far *)agi_arcdata) + 4) = yradius;
   *(((WORD far *)agi_arcdata) + 5) = stangle;
   *(((WORD far *)agi_arcdata) + 6) = endangle;
   *(((char far *)agi_arcdata) + 1) = (char)agi_drawcolor;
   *(char far *)agi_arcdata = 0;

   asm {
      push ds
      mov si, word ptr agi_arcdata
      mov bh, 2
      mov ds, word ptr agi_arcdata+2
      mov ax, 90fah
      int 48h
      pop ds
   }
}

void fillellipse(int x, int y, int xradius, int yradius)
{
   agi_arcdata = agi_arcbuf;
   *(((WORD far *)agi_arcdata) + 1) = x;
   *(((WORD far *)agi_arcdata) + 2) = y;
   *(((WORD far *)agi_arcdata) + 3) = xradius;
   *(((WORD far *)agi_arcdata) + 4) = yradius;
   *(((WORD far *)agi_arcdata) + 5) = 0;
   *(((WORD far *)agi_arcdata) + 6) = 360;
   *(((char far *)agi_arcdata) + 1) = (char)agi_drawcolor;
   *(char far *)agi_arcdata = 2;
   *(((WORD far *)agi_arcdata) + 7) = agi_fillcolor;

   asm {
      push ds
      mov si, word ptr agi_arcdata
      mov bh, 2
      mov ds, word ptr agi_arcdata+2
      mov ax, 90fah
      int 48h
      pop ds
   }
}

int UC_GetColourDepth(void)
{
   char depth;
   asm {
      push di
      mov ax, 95feh
      push di
      int 10h
      pop di
      mov depth, dl
   }
   return depth;
}

void initgraph(int graphdriver, int graphmode)
{
   char drv_path[80] = "C:\\UCDOS\\DRV\\SDK.DRV";
   char drv_map[24] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 5, 12, 13, 7, 14, 15, 16, 17, 12, 18, 20, 19, 3 };
   int adp;
   char drv_name[80];
   int pt;
   char boot_drive;
   int fd;
   char drv_tag[80];
   WORD total_drivers;
   long drv_offset;
   WORD drv_len;
   void far *drv_buf;
   int gdi_ret;

   if (graphdriver == 20 || graphdriver == 21 || graphdriver == 22) {
      adp = graphdriver;
      if ((TESTADP() & 0xff00) == 0) {
         printf("Can not Found VESA BIOS!\r\n");
         exit(0xff);
      }
      graphdriver = adp;
   } else if (graphdriver == 0) {
      adp = TESTADP();
      graphdriver = adp & 0xff;
      if (graphdriver == 0 || graphdriver > 23) {
         if (adp & 0xff00) {
            graphdriver = 20;
         } else {
            printf("Can not support current display adpter!\r\n");
            exit(0xff);
         }
      } else {
         graphdriver = drv_map[graphdriver];
      }
   }

   strcpy(drv_name, "UCDOS 5.0 SDK/UCVision 驱动程序库");

   asm {
      mov ax, 0db00h
      xor bx, bx
      int 2fh
      mov pt, bx
   }
   if (pt != 0x5450) {
      printf("UCDOS 5.0/6.0 system not found!\r\n");
      exit(0xff);
   }

   asm {
      mov dx, 0ffffh
      int 7fh
      mov boot_drive, dl
      mov ax, 1
      int 79h
   }
   if (_FLAGS & 0x40) {
   } else {
      printf("请先执行曲线字库管理模块 RDPS.COM 或 RDFNT.COM !\r\n");
      exit(0xff);
   }

   drv_path[0] = boot_drive;
   fd = open(drv_path, O_RDONLY | O_BINARY);
   if (fd == -1) {
      printf("没有找到驱动程序库: %s!\r\n", drv_path);
      exit(0xff);
   }

   read(fd, drv_tag, strlen(drv_name));
   drv_tag[strlen(drv_name)] = 0;
   if (strcmp(drv_name, drv_tag) != 0) {
      printf("驱动程序库 %s 可能不正确!\r\n请重新安装文件 SDK.DRV.\r\n", drv_path);
      exit(0xff);
   }

   read(fd, &total_drivers, 2);
   if (graphdriver + 1 > total_drivers) {
      printf("指定的驱动程序不在库中!\r\n");
      exit(0xff);
   }

   drv_offset = strlen(drv_name) + (long)(graphdriver - 1) * 6 + 2;
   lseek(fd, drv_offset, SEEK_SET);
   read(fd, &drv_offset, 4);
   read(fd, &drv_len, 2);
   lseek(fd, drv_offset, SEEK_SET);

   drv_buf = malloc(drv_len + 0x200);
   if (drv_buf == NULL) {
      printf("无足够内存安装驱动程序!\r\n");
      exit(0xff);
   }

   drv_buf = MK_FP(FP_SEG(drv_buf) + 1, 0x100);
   read(fd, drv_buf, drv_len);
   close(fd);

   ((void (far *)())drv_buf)();
   gdi_ret = GDI();
   if (gdi_ret == -2) {
      printf("驱动程序可能是错误的!\r\n");
      exit(0xff);
   }

   EnableSDKint10();
   get_grpfun();
   UC_CheckXMS();
   UC_InitTimer();
   setint8();

   if (graphmode == 0) {
      asm {
         mov ax, 9514h
         mov bx, 800h
         int 10h
      }
   } else {
      asm {
         mov ax, 9514h
         mov bx, 500h
         mov cx, graphmode
         int 10h
         or ah, ah
         jnz mode_fallback
         int 10h
         jmp mode_done
      }
   mode_fallback:
      asm {
         mov ax, 9514h
         mov bx, 800h
         int 10h
      }
   mode_done:;
   }

   setlinestyle(SOLID_LINE, 0xffff, 1);
   Setviewport(0, 0, getmaxx() - 1, getmaxy() - 1);
   UC_MouseSetPosition(getmaxx() / 2, (getmaxy() + 20) / 2);

   textinfo.Buf = GRAPH_BUFFER;
   textinfo.BufLen = BUFFER_LEN;
}

void cleardevice(void)
{
   int l, r, t, b;

   l = agi_viewport_left;
   r = agi_viewport_right;
   t = agi_viewport_top;
   b = agi_viewport_bottom;

   Setviewport(0, 0, getmaxx() - 1, getmaxy() - 1);
   asm {
      mov ax, 90eeh
      mov bx, 0
      int 48h
   }
   Setviewport(l, t, r, b);
   agi_position_x = 0;
   agi_position_y = 0;
}

void clearviewport(void)
{
   asm {
      mov ax, 90eeh
      mov bx, 0
      int 48h
   }
   agi_position_x = agi_viewport_left;
   agi_position_y = agi_viewport_top;
}

void floodfill(int x, int y, int border)
{
   asm {
      mov bh, 3
      mov cx, agi_fillcolor
      mov ch, cl
      mov ax, border
      mov cl, al
      mov si, x
      mov di, y
      mov ax, 90eeh
      int 48h
   }
}

int getmaxcolor(void)
{
   int c;
   asm {
      mov ax, 90efh
      int 48h
      mov c, dx
   }
   if (c == 1)
      return 15;
   if (c == 2)
      return 255;
   if (c == 3)
      return 1;
   if (c > 11)
      return 65535U;
   return 0;
}

int getmaxx(void)
{
   int x;
   asm {
      mov ax, 90efh
      int 48h
      mov x, cx
   }
   return x;
}

int getmaxy(void)
{
   int y;
   asm {
      mov ax, 90efh
      int 48h
      sub bx, 20
      mov y, bx
   }
   return y;
}

void setpalette(int colornum, int color)
{
   if (color < 0)
      color = 15;
   if (getmaxcolor() == 15) {
      asm {
         mov ax, 1000h
         mov bx, colornum
         mov dx, color
         mov bh, dl
         int 10h
      }
   }
}

void UC_VGAFadeIn(int delay, char *pal)
{
   int depth = UC_GetColourDepth();
   if (depth == 1 || depth == 2)
      VGAFADEIN(delay, pal);
}

void UC_VGAFadeOut(int delay)
{
   int depth = UC_GetColourDepth();
   if (depth == 1 || depth == 2) {
      UC_ReadBiosPalette(TMP_tpal);
      VGAFADEOUT(delay, TMP_tpal);
   }
}

void setvgapal(char *pal)
{
   char far *p;

   if (UC_GetColourDepth() != 2)
      return;

   movmem(pal, TMP_tpal, 0x300);
   p = (char far *)TMP_tpal;

   asm {
      mov ax, word ptr p+2
      mov dx, word ptr p
      push es
      mov es, ax
      mov si, dx
      mov cx, 300h
   }
pal_shr:
   asm {
      shr byte ptr es:[si], 2
      inc si
      loop pal_shr
      mov si, dx
      mov cx, 100h
      xor bl, bl
      cli
   }
pal_out:
   asm {
      mov dx, 3c8h
      mov al, bl
      out dx, al
      inc dx
      mov al, es:[si]
      out dx, al
      mov al, es:[si+1]
      out dx, al
      mov al, es:[si+2]
      out dx, al
      add si, 3
      inc bl
      loop pal_out
      pop es
   }
}

void setsavepalette(char *spal, char *dpal, int beg, int count, WORD csize)
{
   int i, idx;

   if (UC_GetColourDepth() != 2)
      return;

   movmem(spal, TMP_tpal, 0x300);
   if (beg + count > 256)
      count = 256 - beg;

   asm {
      mov ax, 0affh
      mov bx, beg
      mov cx, count
      call far ptr [grp_entry]
   }

   idx = beg * 3;
   for (i = 0; i < count; i++) {
      TMP_tpal[idx] = dpal[idx];
      TMP_tpal[idx+1] = dpal[idx+1];
      TMP_tpal[idx+2] = dpal[idx+2];
      idx += 3;
   }

   movmem(TMP_tpal, dpal, csize);
   dpal[0x2fd] = 0xff;
   dpal[0x2fe] = 0xff;
   dpal[0x2ff] = 0xff;
}

void setrgbpalette(int colornum, int red, int green, int blue)
{
   int maxc = getmaxcolor();
   if (maxc == 255 || maxc == 15) {
      asm {
         mov ax, colornum
         cli
         mov dx, 3c8h
         out dx, al
         inc dx
         mov ax, red
         out dx, al
         mov ax, green
         out dx, al
         mov ax, blue
         out dx, al
         sti
      }
   }
}

void UC_atexit(void (far *func)())
{
   UCSDK_EXIT = func;
}

void UC_Exit(char *fmt, ...)
{
   va_list arg;

   if (UCSDK_EXIT != NULL)
      (*UCSDK_EXIT)();

   UC_MouseClose();
   closegraph();

   va_start(arg, fmt);
   vprintf(fmt, arg);
   va_end(arg);

   exit(0xff);
}

void closegraph(void)
{
   UC_VGAFadeOut(100);
   if (xms_entry != NULL) {
      asm {
         mov ah, 0ah
         mov dx, global_xms_handle
         call far ptr [xms_entry]
      }
   }
   asm {
      mov ax, 3
      int 10h
   }
   DisableSDKint10();
   biosint8();
}

int getx(void)
{
   return agi_position_x;
}

int gety(void)
{
   return agi_position_y;
}

void getviewsettings(struct viewporttype far *viewport)
{
   viewport->left = agi_viewport_left;
   viewport->right = agi_viewport_right;
   viewport->top = agi_viewport_top;
   viewport->bottom = agi_viewport_bottom;

   if (agi_viewport_bottom == -1)
      viewport->bottom = getmaxy() - 1;
   if (agi_viewport_right == -1)
      viewport->right = getmaxx() - 1;

   viewport->clip = agi_viewport_clip;
}

void get_grpfun(void)
{
   asm {
      push es
      mov ax, 9535h
      mov bx, 400h
      int 10h
      mov ax, es
      pop es
      mov word ptr [grp_entry+2], ax
      mov word ptr [grp_entry], dx
   }
}
