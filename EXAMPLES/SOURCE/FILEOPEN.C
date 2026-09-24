// SDK 内部文件打开窗口函数 UC_DialogFileOpen 的部分源代码,
// 其中含有磁盘处理的秘诀!! 可提供高级程序员参考、修改使用
// 本 C 程序不可编译为 EXE 文件, 仅可联接为 OBJ 进入工程文件使用.

#include <dir.h>
#include <dos.h>
#include <string.h>
#include <stdio.h>
#include <alloc.h>
#include <ctype.h>
#include "sdk.h"

static int getdtotal(void);
static void draw_filename(SCROLLBAR *sbar, float pcure);
static void draw_direc(SCROLLBAR *sbar, float pcure);
static void change_file(void);
static void todir(void);
static void newdir(void);
static void change_driver(void);
static void draw_drv(SCROLLBAR *sbar, float pcure);
static void draw_arg(SCROLLBAR *sbar, float pcure);

WINDOWS *FILEOPEN;
LISTBOX *filebox, *dirbox;
INPUTLINE *fileinp, *dirinp, *arginp, *drvinp;
int *warg;
char cdtmp[MAXPATH];
char filename[13];
char filepath[MAXPATH];
char tmppath[MAXPATH];
char **ALL_FILE_ARG; //, **TOTALDRV;
char *totalDRV[27]={ "    ", "    ", "    ", "    ",
                     "    ", "    ", "    ", "    ",
                     "    ", "    ", "    ", "    ",
                     "    ", "    ", "    ", "    ",
                     "    ", "    ", "    ", "    ",
                     "    ", "    ", "    ", "    ",
                     "    ", "    ", "    "
};
char *DRVBOX_CHAR="X:";
char *DRVHBOX_CHAR="X:";
char *DRVCDBOX_CHAR="X:";
char *UPPATH_CHAR="";
int total_FILE_ARG, DRVtotal;
struct ffblk ffblk;
char arge[13];
char drv[5];
char if_load;
WORD totalf, totald; //, i;
int savedisk;
char *name[8]={ "            ",
                "            ",
                "            ",
                "            ",
                "            ",
                "            ",
                "            ",
                "            "
};

// 检取顺序号(num从0开始)对应的驱动器代号
static int ChkDrvInNum(int num)
{
   int disk;

   num++;
   for (disk=0; disk<30; disk++) {
       if (UC_GetDiskAttribute(disk)) num--;
       if (!num) break;
   }
   return disk;
}

// 检查指定的驱动器是否在列表范围中
// dr=驱动器代号(0开始)
// 返回0不在, 否则返回在列表中的第几项(1开始)
static int ChkDrvInLst(int dr)
{
    WORD i;

    dr=dr+'A';
    i=1;
    while (i<=DRVtotal) {
          if (dr==totalDRV[i-1][2]) return i;
          i++;
    }
    return NULL;
}

static int mygetdisk(void)
{
   return ChkDrvInLst(getdisk())-1;
}

// 返回驱动器总数
static int getdtotal(void)
{
        int total, disk;

        total=0;
        for (disk = 0; disk < 30; disk++)  // 检查硬盘
        {
                if (UC_GetDiskAttribute(disk)) total++;
        }
        return total;
}

static void draw_filename(SCROLLBAR *sbar, float pcure)
{
  int i, j;
  char carg[13];
  char fname[13];
  char *tname;
  char buf[MAXPATH];

  pcure=pcure;
  strcpy(carg,ALL_FILE_ARG[arginp->box->sbar->cure]);
  j=UC_GetBoxTopCount(filebox);
  tname=UC_GetFileNameByCount(carg, j+1, FA_ARCH);
  for (i=0; i<8; i++) {
          if (tname) {
                        tname[12]=NULL;
                        strcpy(name[i], tname);
                        tname=UC_GetNextFileNameByCount(FA_ARCH);
          }
          else strcpy(name[i], "            ");
          j++;
  }
  tname=UC_GetFileNameByCount(carg, sbar->cure+1, FA_ARCH);
  if (tname) {
          tname[12]=NULL;
          strcpy(fname,tname);
  }
  else fname[0]=NULL;
  DOS_CHECKDISK=1;
  DOS_CHECKRETERR=-1;
  getcwd(buf, MAXPATH);
  if (DOS_CHECKRETERR!=-1) {    // 如果是DOS错误取消状态, 直接返回
          dirbox->sbar->max=1;
          dirbox->sbar->cure=0;
          dirbox->top_count=0;
          UC_DisplayScrollbar(FILEOPEN, dirbox->sbar);
          filebox->sbar->max=1;
          filebox->sbar->cure=0;
          filebox->top_count=0;
          UC_DisplayScrollbar(FILEOPEN, filebox->sbar);
  }
  UC_UpdateInputLine(FILEOPEN, fileinp, fname);
  UC_UpdateListBox(FILEOPEN, filebox, name, 8);
}

static void draw_direc(SCROLLBAR *sbar, float pcure)
{
  int i, j;
  char buf[MAXPATH];
  char *fname;

  pcure=pcure;
  j=UC_GetBoxTopCount(dirbox);
  fname=UC_GetFileNameByCount("*.*", j+1, FA_DIREC);
  for (i=0; i<8; i++) {
      if (fname) {
         fname[12]=NULL;
         strcpy(name[i],fname);
         fname=UC_GetNextFileNameByCount(FA_DIREC);
      }
      else strcpy(name[i], "            ");
      if (j==sbar->cure) strcpy(cdtmp, name[i]);     // 当前文件名亦存放在 cdtmp 中
      j++;
  }
  DOS_CHECKDISK=1;
  DOS_CHECKRETERR=-1;
  getcwd(buf, MAXPATH);
  if (DOS_CHECKRETERR!=-1) {    // 如果是DOS错误取消状态, 直接返回
          dirbox->sbar->max=1;
          dirbox->sbar->cure=0;
          dirbox->top_count=0;
          UC_DisplayScrollbar(FILEOPEN, dirbox->sbar);
          filebox->sbar->max=1;
          filebox->sbar->cure=0;
          filebox->top_count=0;
          UC_DisplayScrollbar(FILEOPEN, filebox->sbar);
    buf[0]=NULL;
  }
  UC_UpdateInputLine(FILEOPEN, dirinp, buf);
  UC_UpdateListBox(FILEOPEN, dirbox, name, 8);
}

static void change_file(void)
{
  UC_UpdateInputLine(FILEOPEN, arginp, ALL_FILE_ARG[arginp->box->sbar->cure]);
  filebox->sbar->max=UC_GetFileTotal(ALL_FILE_ARG[arginp->box->sbar->cure], FA_ARCH);
  filebox->sbar->cure=0;
  filebox->top_count=0;
  draw_filename(filebox->sbar, 0);
  UC_DisplayScrollbar(FILEOPEN, filebox->sbar);
}

static void todir(void)
{
   char buf[MAXPATH];

   DOS_CHECKDISK=0;
   DOS_CHECKRETERR=-1;
   getcwd(buf, MAXPATH); // 检查当前路径是否有效
   if (DOS_CHECKRETERR!=-1) {    // 如果是DOS错误取消状态, 直接返回
      setdisk(savedisk);   // 恢复原始驱动器
      savedisk=ChkDrvInLst(savedisk);
      if (savedisk) {
         savedisk--;
         drvinp->box->sbar->cure=savedisk;
         UC_UpdateInputLine(FILEOPEN, drvinp, totalDRV[savedisk]); // 更新驱动器列表框
      }
   }

   dirbox->sbar->max=UC_GetFileTotal("*.*", FA_DIREC);
   dirbox->sbar->cure=0;
   dirbox->top_count=0;
   draw_direc(dirbox->sbar, 0);
   UC_DisplayScrollbar(FILEOPEN, dirbox->sbar);
   change_file();
}

static void inpdir(void)
{
  strcpy(cdtmp, tmppath);
  newdir();
}

static void newdir(void)
{
   int dr, i;

   savedisk=getdisk();
   if (!strcmp(cdtmp, UPPATH_CHAR)) strcpy(cdtmp, "..");
   if (cdtmp[1]==':') {
      dr=cdtmp[0];
      dr=toupper(dr)-'A';
      if (!(dr=ChkDrvInLst(dr))) {
          DOS_CHECKDISK=0;
          DOS_CHECKRETERR=-1;
          getcwd(cdtmp, MAXPATH);
          if (DOS_CHECKRETERR!=-1) cdtmp[0]=0;        // 取当前目录出错, 默认路径名为空
          UC_UpdateInputLine(FILEOPEN, dirinp, cdtmp);
          return;  // 输入了不在驱动器列表中的字母, 重新更新为当前路径后, 返回
      }
      dr--;
      drvinp->box->sbar->cure=dr;
      UC_UpdateInputLine(FILEOPEN, drvinp, totalDRV[dr]);
      setdisk(totalDRV[dr][2]-'A');
   }
   chdir(cdtmp);
   todir();
}

static void change_driver(void)
{
  int disk;

  UC_UpdateInputLine(FILEOPEN, drvinp, totalDRV[drvinp->box->sbar->cure]);
  savedisk=getdisk();
  disk=totalDRV[drvinp->box->sbar->cure][2]-'A';
  setdisk(disk);
  todir();
}

static void draw_drv(SCROLLBAR *sbar, float pcure)
{
  int t;

  sbar=sbar;
  pcure=pcure;
  t=(int)UC_GetBoxTopCount(drvinp->box);
  UC_UpdateListBox(FILEOPEN, drvinp->box,
                   totalDRV+t, DRVtotal-t);
}

static void draw_arg(SCROLLBAR *sbar, float pcure)
{
  int t;

  sbar=sbar;
  pcure=pcure;
  t=(int)UC_GetBoxTopCount(arginp->box);
  UC_UpdateListBox(FILEOPEN, arginp->box,
                   ALL_FILE_ARG+t, total_FILE_ARG-t);
}

static void file_over(void)
{
  UC_WindowClose();
  *warg=(int)arginp->box->sbar->cure;
  UC_SetMessage(ID_OK);
}

static void file_ok()
{
  FILE *in;

  strcpy(filepath, tmppath);
  if (filepath[strlen(filepath)-1]!='\\') strcat(filepath, "\\");
  filename[12]=NULL;
  strcat(filepath, filename);
  in=fopen(filepath,"r");
  if (if_load) {    // 装入的情况
     if (in==NULL) {  // 如果文件不存在, 不能打开
        UC_NewActive(FILEOPEN, TYPE_INPUTLINE, 1);
        UC_DialogWarning(NULL, NULL, "%s\n不能够找到该文件\n\n请检查给出文件名及其路径的正确性.", filename);
        return;
     }
  }
  else {                        // 存盘的情况
       if (in!=NULL) {  // 如果文件存在, 提示覆盖
          fclose(in);
          UC_NewActive(FILEOPEN, TYPE_INPUTLINE, 1);
          UC_DialogYesNo(file_over, NULL, NULL, "%s\n该文件已经存在\n\n请确认是否将该文件覆盖掉?", filename); //path);
          return;
       }
  }
  fclose(in);
  file_over();
}

static void file_cancle()
{
  filepath[0]=NULL;
  UC_WindowClose();
  UC_SetMessage(ID_CANCEL);
}

static void ip_fname()
{
   UC_NewActive(FILEOPEN, TYPE_INPUTLINE, 1);
}

static void ip_fpath()
{
   UC_NewActive(FILEOPEN, TYPE_INPUTLINE, 2);
}

static void ip_ftype()
{
   UC_NewActive(FILEOPEN, TYPE_INPUTLINE, 3);
}

static void ip_fdrv()
{
   UC_NewActive(FILEOPEN, TYPE_INPUTLINE, 4);
}

//===================================================================
// 文件打开窗口函数
// option= 0存盘, 1装入
// argw= 默认第几个列出类型
// arg= 列出类型串指针
// title= 窗口标题(定义为NULL=使用默认标题)
// funok(char *pathfile)= 打开成功后的函数指针,pathfile=传递出来的文件名指针
// funfail()= 打开失败后的函数指针
char *UC_DialogFileOpen(char option, int *argw, char **arg, char *title,
                        void (*funok)(char *pathfile), void (*funfail)(void))
{
  int ID_RET, i, j;

  if_load=option;
  ALL_FILE_ARG=arg;
  total_FILE_ARG=0;
  while (arg[total_FILE_ARG]) total_FILE_ARG++;
  if ((*argw)>total_FILE_ARG) *argw=0;
  warg=argw;
  strcpy(arge, arg[*argw]);
  strcpy(filename, arge);

  DRVtotal=getdtotal();
  if (!DRVtotal) return NULL;

  for (i=0;i<DRVtotal;i++) {
    ID_RET=ChkDrvInNum(i);    // 按照顺序号找到相应的驱动器字母
    j=UC_GetDiskAttribute(ID_RET);    // 检取驱动器类型
    switch (j) {
         case DRV_CDROM: strcpy(totalDRV[i], DRVCDBOX_CHAR);  // CD 驱符号
                         break;
         case DRV_HARD:  strcpy(totalDRV[i], DRVHBOX_CHAR);
                         break;
         default:        strcpy(totalDRV[i], DRVBOX_CHAR); // 软驱符号
    }
          totalDRV[i][2]='A'+ID_RET;
  }

  asm {
     push ds
     xor ax,ax
     mov ds,ax
     mov byte ptr ds:[504h],0    // 强行置 A: B: 驱动器不交换
     pop ds
  }

  strcpy(drv, totalDRV[mygetdisk()]);    // 当前驱动器作为默认激活使用

  totalf=UC_GetFileTotal(arge, FA_ARCH);
  totald=UC_GetFileTotal("*.*", FA_DIREC);

  tmppath[0]=NULL;

  if (title) FILEOPEN=UC_DefineWindow(WS_NOSIZE,
                CP_USEDEFAULT, CP_USEDEFAULT,
                450, 300, title, NULL,
                file_cancle, NULL);
  else {
                 if (if_load) FILEOPEN=UC_DefineWindow(WS_NOSIZE,
                                        CP_USEDEFAULT, CP_USEDEFAULT,
                                        450, 300, "打开文件", NULL,
                                        file_cancle, NULL);
                 else FILEOPEN=UC_DefineWindow(WS_NOSIZE,
                                CP_USEDEFAULT, CP_USEDEFAULT,
                                420, 300, "文件存盘",NULL,
                                file_cancle, NULL);
  }
  fileinp=UC_DefineInputLine(FILEOPEN, 20, 30, 14, filename, 12, file_ok);
  dirinp=UC_DefineInputLine(FILEOPEN, 170, 30, 14, tmppath, MAXPATH-1, inpdir);

  arginp=UC_DefinePopBox(FILEOPEN, 20, 240, 14, 4, total_FILE_ARG, arge, draw_arg, change_file);
  drvinp=UC_DefinePopBox(FILEOPEN, 170, 240, 14, 4, DRVtotal, drv, draw_drv, change_driver);

  drvinp->box->sbar->cure=mygetdisk();
  arginp->box->sbar->cure=*argw;

  UC_DefinePressButton(FILEOPEN, file_ok, 320, 30, 90, 0, "确定[O]", CMD_OK);
  UC_DefinePressButton(FILEOPEN, file_cancle, 320, 70, 90, 0, "取消[C]", CMD_CANCEL);

  filebox=UC_DefineListBox(FILEOPEN, 20, 60, 14, 8, totalf, draw_filename, file_ok, 1);
  dirbox=UC_DefineListBox(FILEOPEN, 170, 60, 14, 8, totald, draw_direc, newdir, 1);

  UC_DefineLabel(FILEOPEN, 20, 10, 0x2100, ip_fname, "文件名[F]", NULL, NULL);
  UC_DefineLabel(FILEOPEN, 170, 10, 0x2000, ip_fpath, "目录[D]", NULL, NULL);
  UC_DefineLabel(FILEOPEN, 20, 220, 0x1400, ip_ftype, "列出类型[T]", NULL, NULL);
  UC_DefineLabel(FILEOPEN, 170, 220, 0x1300, ip_fdrv, "驱动器[R]", NULL, NULL);
  UC_DefineActive(FILEOPEN, 2, 1);        // 定义文件名输入框为激活
  UC_WindowEnable(FILEOPEN);
  while(1) {
                UC_WindowsCentral();
                ID_RET=UC_GetMessage();
                if (ID_RET==ID_OK) {
                        if (funok) funok(filepath);
                        return filepath;
                }
                if (ID_RET==ID_CANCEL) {
                        if (funfail) funfail();
                        return NULL;
                }
  }
}
