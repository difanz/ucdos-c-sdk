// DOS 关键错误处理程序 (INT 24h)

#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <alloc.h>
#include "sdk.h"

int DOS_CHECKDISK = 0;
int DOS_CHECKRETERR = -1;

static char *err_msg[] = {
   "写保护",
   "有未知的单位",
   "没有准备好, 请检查磁盘是否插入",
   "有不认识的命令",
   "数据校验和错(CRC)",
   "错误的请求",
   "寻道出错",
   "有不能识别的介质",
   "扇区没有找到",
   "打印机缺纸",
   "写操作出现故障",
   "读操作出现故障",
   "出现一般性故障",
   "发生共享冲突",
   "发生加锁冲突",
   "更换了无效磁盘",
   "没有可用的文件控制块(FCB)",
   "共享缓冲区溢出"
};

static int get_key(void)
{
   return bdos(7, 0, 0) & 0xff;
}

int UC_DialogRetry(char *msg)
{
   static int x1, y1, x2, y2;
   static int ret_val;
   static int xms_handle;
   static struct viewporttype save_vp;

   getviewsettings(&save_vp);
   Setviewport(0, 0, getmaxx() - 1, getmaxy() - 1);
   x1 = (getmaxx() - 360) / 2;
   y1 = (getmaxy() - 140) / 2;
   x2 = x1 + 359;
   y2 = y1 + 139;

   UC_MouseHide();
   xms_handle = UC_XMSgetscreen(x1, y1, 360, 140);
   setfillstyle(1, 15);
   bar(x1, y1, x2, y2);
   setfillstyle(1, 4);
   bar(x1 + 1, y1 + 1, x2 - 1, y1 + 19);
   setcolor(0);
   rectangle(x1, y1, x2, y2);
   setcolor(15);
   settextstyle(0, 0, 16);
   outtextxy(x1 + 100, y1 + 2, "*** DOS 严重错误 *** ");
   setcolor(1);
   outtextxy(x1 + 20, y1 + 50, msg);
   setcolor(0);
   outtextxy(x1 + 20, y1 + 100, "请选择→  [R]重试  [I]忽略  [F]取消");

   while (1) {
      ret_val = tolower(get_key());
      switch (ret_val) {
      case 'r':
         ret_val = 0x442;
         break;
      case 'i':
         ret_val = 0x443;
         break;
      case 'f':
      case 27:
         ret_val = 0x444;
         break;
      default:
         continue;
      }
      break;
   }

   if (xms_handle) {
      UC_XMSputscreen(xms_handle, x1, y1);
      UC_FreeXMS(xms_handle);
   }
   Setviewport(save_vp.left, save_vp.top, save_vp.right, save_vp.bottom);
   UC_MouseShow();
   return ret_val;
}

int ErrorWin(char *msg)
{
   register int s;

   switch (s = UC_DialogRetry(msg)) {
   case 0x442: s = 1; break;
   case 0x443: s = 0; break;
   case 0x444: s = 3; break;
   }
   return s;
}

void UC_Handler(int deverr)
{
   int errval = _DI;
   int drive;
   int err_idx;
   static char buf[50];
   static int ret;

   DOS_CHECKRETERR = -1;
   if (DOS_CHECKDISK == 1) {
      DOS_CHECKRETERR = 3;
      DOS_CHECKDISK = 0;
      _hardretn(3);
   }

   if (deverr & 0x8000) {
      ErrorWin("设备错误! (无法访问)");
      DOS_CHECKRETERR = 5;
      _hardretn(5);
   } else {
      drive = deverr & 0xff;
      err_idx = errval & 0xff;
      sprintf(buf, "驱动器 %c: %s", drive + 'A', err_msg[err_idx]);
      ret = ErrorWin(buf);
      if (ret != 1)
         DOS_CHECKRETERR = ret;
      if (ret == 3)
         _hardretn(ret);
      _hardresume(ret);
   }
}
