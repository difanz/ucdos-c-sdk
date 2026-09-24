/* ---------------------------------------------------------------
   UCDOS 5.0 SDK for C/C++    UC Vision 版本 1.0         1996.10.3

   出品: 北京希望高技术集团

   作者: 简 晶

   声明: 请使用正版的 SDK 编写您的软件, 这样可以获得良好的技术支持
         和完善的售后服务; 这一点对于商品化软件而言更为重要。
         未经授权的以下任何一种行为，都可能引起民事或经济法律纠纷:

         (1) 对本系统随意拷贝或出租;
         (2) 对本系统源代码或目标代码进行修改;
         (3) 对本系统进行反编译、反汇编及进行注释;
         (4) 抄袭局部或全部的相关文档;
         (5) 用拷贝来的 SDK 进行商业或公用软件开发.
-----------------------------------------------------------------*/
#ifndef __JJSDK__
#define __JJSDK__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __LARGE__
#error Use LARGE mode to compile UCDOS 5.0 SDK application!
#endif  // 请使用大模式编译 SDK 应用程序

#include <conio.h>

// SDK 预定义的变量类型标识符
typedef unsigned char        BYTE;
typedef unsigned int         WORD;
typedef unsigned long        DWORD;
typedef int                  BOOL;

typedef short                SHORT;
typedef long                 LONG;

typedef unsigned char        UCHAR;
typedef unsigned int         UINT;
typedef unsigned short       USHORT;
typedef unsigned long        ULONG;

typedef char _far           *PCHAR;
typedef int _far            *PINT;
typedef short _far          *PSHORT;
typedef long _far           *PLONG;

typedef unsigned char _far  *PUCHAR;
typedef unsigned int _far   *PUINT;
typedef unsigned short _far *PUSHORT;
typedef unsigned long _far  *PULONG;

typedef unsigned short       SEL;
typedef unsigned short _far *PSEL;

typedef unsigned long        REALPTR;

/*================================================================*/

// SDK 提供的布尔值标识符
#define TRUE    1
#define FALSE   0

// SDK 支持显示卡的标识符
#define  DETECT  0      // 自动探测显示卡，或使用默认的图形模式
#define  VGA     1      // 标准 VGA 驱动
#define  TVGA    2      // Trident 8800,8900,9000
#define  PVGA    3      // Paradise / CL-GD5/GD7 系列
#define  ATI     4      // ATI 188xx
#define  ET4000  5      // Tseng ET4000/W32
#define  CHIPS   6      // Chips & Tech 82C453/64300/655xx
#define  S3      7      // S3 全系列
#define  ACER    8      // Acer View M3135
#define  OAK     9      // Oak OTI-0xx
#define  REALTEK 10     // Realtek RTG-31xx
#define  ET3000  11     // Tseng ET3000
#define  TGUI    12     // Trident GUI 92xx/94xx/96xx
#define  ARK     13     // ARK Logic VGA-1000PV/2000PV
#define  MACH64  14     // ATI MACH64 Graphics Pro Turbo
#define  COMPAQ  15     // COMPAQ QVision 1024
#define  CLGD6   16     // Cirrus Logic GD-6xxx
#define  QV2000  17     // MATROX / COMPAQ QVision 2000
#define  MACH32  18     // ATI 68800 MACH32
#define  IGA1680 19     // InteGraphics System IGA1680
#define  VESA    20     // VESA BIOS/Driver (可使用UNIVBE/UNIVESA等软件仿真)
#define  VESA32K 21     // VESA BIOS/Driver 32K 真彩色
#define  VESA64K 22     // VESA BIOS/Driver 64K 真彩色

// 显示卡的等同标识符
#define  CLGD5   PVGA
#define  CLGD7   PVGA
#define  QVISION COMPAQ
#define  MATROX  QV2000
#define  PM6410  VESA

// SDK 支持的屏幕色彩深度标识符
#define CRD_MONO          3         // 单色模式(MONO/HGC)
#define CRD_COLOR16       1         // EGA/VGA/SVGA 16色
#define CRD_COLOR256      2         // VGA/SVGA 256色
#define CRD_HICOLOR15B    0x0C      // SVGA 15位真彩色(32K)
#define CRD_HICOLOR16B    0x0D      // SVGA 16位真彩色(64K)
#define CRD_TURECOLOR24B  0x0E      // SVGA 24位真彩色(16.8M)
#define CRD_TURECOLOR32B  0x0F      // SVGA 32位真彩色(16.8M+A)

// SDK 支持的常用图形模式标识符 (VESA代码一律以UNIVBE软件支持为标准)

// 16色模式
#define VM_320X200X16     0x0D
#define VM_640X200X16     0x0E
#define VM_640X350X16     0x10
#define VM_640X480X16     0x12
#define VM_800X600X16     0x102
#define VM_1024X768X16    0x104
#define VM_1280X1024X16   0x106
#define VM_1600X1200X16   0x123

// 256色模式
#define VM_320X200X256    0x13
#define VM_640X350X256    0x11C
#define VM_640X400X256    0x100
#define VM_640X480X256    0x101
#define VM_800X600X256    0x103
#define VM_1024X768X256   0x105
#define VM_1280X1024X256  0x107
#define VM_1600X1200X256  0x124

// 32K真彩色模式
#define VM_320X200X32K    0x10D
#define VM_640X350X32K    0x11D
#define VM_640X400X32K    0x11E
#define VM_640X480X32K    0x110
#define VM_800X600X32K    0x113
#define VM_1024X768X32K   0x116
#define VM_1280X1024X32K  0x119
#define VM_1600X1200X32K  0x125

// 64K真彩色模式
#define VM_320X200X64K    0x10E
#define VM_640X350X64K    0x11F
#define VM_640X400X64K    0x120
#define VM_640X480X64K    0x111
#define VM_800X600X64K    0x114
#define VM_1024X768X64K   0x117
#define VM_1280X1024X64K  0x11A
#define VM_1600X1200X64K  0x126

// 其余特殊的某种显示卡专用显示模式(只能在该种显示卡上使用)
#define VM_TVGA_768X1024X16 0x61
#define VM_S3_1152X864X256  0x76
#define VM_S3_1600X1200X256 0x120

// SDK 预定义的键盘状态字节屏蔽位
#define KS_INSERTFREE        0x80
#define KS_CAPSLOCKFREE      0x40
#define KS_NUMLOCKFREE       0x20
#define KS_SCROLLLOCKFREE    0x10
#define KS_ALTPUSH           0x08
#define KS_CTRLPUSH          0x04
#define KS_LSHIFTPUSH        0x02
#define KS_RSHIFTPUSH        0x01

// SDK 预定义的部分扩展键盘扫描码
#define SK_BACKSPACE         0x0e08
#define SK_TAB               0x0f09
#define SK_ENTER             0x1c0d
#define SK_ESC               0x011b
#define SK_SPACE             0x3920

#define SK_CTRLBREAK         0x0000
#define SK_ALTESC            0x0100
#define SK_ALTBACKSPACE      0x0e00
#define SK_SHIFTTAB          0x0f00
#define SK_ALTQ              0x1000
#define SK_ALTW              0x1100
#define SK_ALTE              0x1200
#define SK_ALTR              0x1300
#define SK_ALTT              0x1400
#define SK_ALTY              0x1500
#define SK_ALTU              0x1600
#define SK_ALTI              0x1700
#define SK_ALTO              0x1800
#define SK_ALTP              0x1900
#define SK_ALTLSQ            0x1a00       // [
#define SK_ALTRSQ            0x1b00       // ]
#define SK_ALTENTER          0x1c00

#define SK_ALTA              0x1e00
#define SK_ALTS              0x1f00
#define SK_ALTD              0x2000
#define SK_ALTF              0x2100
#define SK_ALTG              0x2200
#define SK_ALTH              0x2300
#define SK_ALTJ              0x2400
#define SK_ALTK              0x2500
#define SK_ALTL              0x2600
#define SK_ALTPOINT2         0x2700      // ;
#define SK_ALTRPOINT         0x2800      // '
#define SK_ALTLPOINT         0x2900      // `

#define SK_ALTIDIV           0x2b00      // \
#define SK_ALTZ              0x2c00
#define SK_ALTX              0x2d00
#define SK_ALTC              0x2e00
#define SK_ALTV              0x2f00
#define SK_ALTB              0x3000
#define SK_ALTN              0x3100
#define SK_ALTM              0x3200
#define SK_ALTPOINT1         0x3300      // <,
#define SK_ALTPOINT          0x3400      // >.
#define SK_ALTDIV            0x3500      // ?/
#define SK_ALTMUL            0x3700      // *
#define SK_ALTKEY            0x3800
#define SK_F1                0x3b00
#define SK_F2                0x3c00
#define SK_F3                0x3d00
#define SK_F4                0x3e00
#define SK_F5                0x3f00
#define SK_F6                0x4000
#define SK_F7                0x4100
#define SK_F8                0x4200
#define SK_F9                0x4300
#define SK_F10               0x4400

#define SK_HOME              0x4700
#define SK_ARROWUP           0x4800
#define SK_PAGEUP            0x4900
#define SK_ALTSUBGRAY        0x4a00      // - 小键盘
#define SK_ARROWLEFT         0x4b00
#define SK_CLEAR             0x4c00      // Num 5 with Num Lock OFF
#define SK_ARROWRIGHT        0x4d00
#define SK_ALTPLUS           0x4e00      // + pad
#define SK_END               0x4f00
#define SK_ARROWDOWN         0x5000
#define SK_PAGEDOWN          0x5100
#define SK_INSERT            0x5200
#define SK_DELETE            0x5300
#define SK_SHIFTF1           0x5400
#define SK_SHIFTF2           0x5500
#define SK_SHIFTF3           0x5600
#define SK_SHIFTF4           0x5700
#define SK_SHIFTF5           0x5800
#define SK_SHIFTF6           0x5900
#define SK_SHIFTF7           0x5a00
#define SK_SHIFTF8           0x5b00
#define SK_SHIFTF9           0x5c00
#define SK_SHIFTF10          0x5d00
#define SK_CTRLF1            0x5e00
#define SK_CTRLF2            0x5f00
#define SK_CTRLF3            0x6000
#define SK_CTRLF4            0x6100
#define SK_CTRLF5            0x6200
#define SK_CTRLF6            0x6300
#define SK_CTRLF7            0x6400
#define SK_CTRLF8            0x6500
#define SK_CTRLF9            0x6600
#define SK_CTRLF10           0x6700
#define SK_ALTF1             0x6800
#define SK_ALTF2             0x6900
#define SK_ALTF3             0x6a00
#define SK_ALTF4             0x6b00
#define SK_ALTF5             0x6c00
#define SK_ALTF6             0x6d00
#define SK_ALTF7             0x6e00
#define SK_ALTF8             0x6f00
#define SK_ALTF9             0x7000
#define SK_ALTF10            0x7100
#define SK_CTRLPRINTSCREEN   0x7200
#define SK_CTRLARROWLEFT     0x7300
#define SK_CTRLARROWRIGHT    0x7400
#define SK_CTRLEND           0x7500
#define SK_CTRLPAGEDOWN      0x7600
#define SK_CTRLHOME          0x7700
#define SK_ALT1              0x7800
#define SK_ALT2              0x7900
#define SK_ALT3              0x7a00
#define SK_ALT4              0x7b00
#define SK_ALT5              0x7c00
#define SK_ALT6              0x7d00
#define SK_ALT7              0x7e00
#define SK_ALT8              0x7f00
#define SK_ALT9              0x8000
#define SK_ALT0              0x8100
#define SK_ALTSUB            0x8200      // - 大键盘
#define SK_ALTEQU            0x8300      // =
#define SK_CTRLPAGEUP        0x8400
#define SK_F11               0x8500
#define SK_F12               0x8600
#define SK_SHIFTF11          0x8700
#define SK_SHIFTF12          0x8800
#define SK_CTRLF11           0x8900
#define SK_CTRLF12           0x8a00
#define SK_ALTF11            0x8b00
#define SK_ALTF12            0x8c00
#define SK_CTRLARROWUP       0x8d00
#define SK_CTRLSUBGRAY       0x8e00      // - pad
#define SK_CTRL5PAD          0x8f00      // 5 pad
#define SK_CTRLPLUSGRAY      0x9000      // + pad
#define SK_CTRLARROWDOWN     0x9100
#define SK_CTRLINSERT        0x9200
#define SK_CTRLDELETE        0x9300
#define SK_CTRLTAB           0x9400
#define SK_CTRLDIV           0x9500      // / pad
#define SK_CTRLMUL           0x9600      // * pad
#define SK_ALTHOME           0x9700
#define SK_ALTARROWUP        0x9800
#define SK_ALTPAGEUP         0x9900

#define SK_ALTARROWLEFT      0x9b00

#define SK_ALTARROWRIGHT     0x9d00

#define SK_ALTEND            0x9f00
#define SK_ALTARROWDOWN      0xa000
#define SK_ALTPAGEDOWN       0xa100
#define SK_ALTINSERT         0xa200
#define SK_ALTDELETE         0xa300
#define SK_ALTDIVGRAY        0xa400      // / (pad)
#define SK_ALTTAB            0xa500
#define SK_ALTCR             0xa600      // enter (pad)

#define CMD_OK            SK_ALTO
#define CMD_CANCEL        SK_ALTC
#define CMD_YES           SK_ALTY
#define CMD_NO            SK_ALTN

// SDK 提供的几种标准自定义消息码
#define ID_NULL           0
#define ID_OK             1000
#define ID_YES            1001
#define ID_NO             1002
#define ID_CANCEL         1003
#define ID_RETRY          1090
#define ID_IGNORE         1091
#define ID_FAIL           1092

// 与 BGI 一致的其他标识符
#define COPY_PUT          0          // 直接作图
#define AND_PUT           1          // 与屏幕内容相与
#define OR_PUT            2          // 与屏幕内容相或
#define XOR_PUT           3          // 与屏幕内容异或
#define NOT_PUT           3

#define SOLID_LINE        0          // 实直线 ──
#define DOTTED_LINE       1          // 点虚线 ┄┄
#define CENTER_LINE       2          // 对中线 -·-
#define DASHED_LINE       3          // 线虚线 - - -
#define USERBIT_LINE      4

#define EMPTY_FILL        0          // 实填充
#define SOLID_FILL        1          // 实填充
#define LINE_FILL         10         // 砖块
#define LTSLASH_FILL      4          // 细///
#define SLASH_FILL        5          // 粗///
#define BKSLASH_FILL      3          // 粗\\\
#define LTBKSLASH_FILL    2          // 细\\\
#define HATCH_FILL        6          // 横纵网格
#define XHATCH_FILL       7          // 交叉网格
#define INTERLEAVE_FILL   9          // 网点
#define WIDE_DOT_FILL     8          // 米字
#define CLOSE_DOT_FILL    11         // 菱形
#define USER_FILL         12         // 用户自定义

// SDK 预定义的标准光标类型标识符
#define IDC_ARROW         0          // 箭头光标(默认)
#define IDC_SIZENS        1          // 上下箭头
#define IDC_SIZEWE        2          // 左右箭头
#define IDC_SIZENWSE      3          // 左上右下箭头
#define IDC_SIZENESW      4          // 左下右上箭头
#define IDC_WAIT          5          // 砂漏(等待)
#define IDC_CROSS         6          // 十字
#define IDC_IBEAM         7          // 输入点
#define IDC_SIZE          8          // 上下左右
#define IDC_NO            9          // 禁止
#define IDC_HAND          10         // 手形

// 窗口模式标识符
#define WS_MAIN           1          // 主窗口(可相互切换)
#define WS_NOMOVE         2          // 非移动窗口(尺寸亦不可变)
#define WS_NOSIZE         4          // 尺寸不可变窗口
#define WS_SINGLELINE     8          // 单线窗口(仅有极小化按钮)
#define WS_MINKEYBOARD    0x8000     // 极小化窗口时可处理键盘事件

// 窗口当前属性标识符
#define SW_SHOWNOR        0          // 正常尺寸窗口
#define SW_SHOWMIN        1          // 极小化窗口
#define SW_SHOWMAX        2          // 极大化窗口

// 窗口位置/尺寸标识符
#define CP_MIDSCR         -1         // 窗口位置居中
#define CP_USEDEFAULT     -2         // 窗口位置默认
#define CW_MAX            -1         // 窗口尺寸最大(带边框)
#define CW_USEDEFAULT     -2         // 窗口尺寸默认

// 窗口消息事件标识符
#define WM_DISABLE        0          // 窗口将被禁止
#define WM_ENABLE         1          // 窗口将被激活
#define WM_MINSIZE        2          // 窗口缩小为图标
#define WM_MAXSIZE        3          // 窗口极大化
#define WM_REVERTSIZE     4          // 窗口还原
#define WM_RESIZE         5          // 窗口尺寸变化
#define WM_MOVE           6          // 窗口移动
#define WM_CLOSE          7          // 窗口关闭

// 鼠标左右手标识符
#define LEFT_HAND         2          // 左手鼠标
#define RIGHT_HAND        1          // 右手鼠标

// 自定义鼠标事件消息标识符
#define USM_MOVE          1          // 移动码
#define USM_LEFT          2          // 左键按下
#define USM_RIGHT         4          // 右键按下
#define USM_DBLCLK        0x8000     // 双击(高位置1)

// DDB 图像显示模式标识符
#define XY_SCR            0          // 按(X,Y)定位
#define MID_SCR           1          // 按居中定位
#define FUL_SCR           2          // 铺满全屏

// 文字显示标识符
#define DT_RIGHT          1          // 右对齐
#define DT_CENTER         2          // 左右居中
#define DT_BOTTOM         4          // 下对齐
#define DT_VCENTER        8          // 上下居中
#define DT_NOOVER         0          // 字符显示不覆盖背景
#define DT_OVER           0x10       // 覆盖文字背景
#define DT_WORDBREAK      0x20       // 自动截断单词

// 滚动条模式标识符
#define H_BAR             0          // 水平滚动条
#define V_BAR             1          // 垂直滚动条

// 窗口标准构件类型标识符
#define TYPE_BUTTON       1          // 按钮部件
#define TYPE_INPUTLINE    2          // 输入行部件
#define TYPE_LISTBOX      3          // 列表框部件
#define TYPE_RADIO        4          // 单选钮部件
#define TYPE_CHECK        5          // 复选钮部件
#define TYPE_LABEL        6          // 标签部件
#define TYPE_SCROLLBAR    7          // 滚动条部件
#define TYPE_GROUPBOX     8          // 分组框部件
#define TYPE_USERRECT     9          // 自定义控制框

// 驱动器属性标识符
#define DRV_360K          1          // 360K 5寸大软驱
#define DRV_12M           2          // 1.2M 5寸大软驱
#define DRV_720K          3          // 720K 3寸小软驱
#define DRV_144M          4          // 1.44M 3寸小软驱
#define DRV_HARD          0x10       // 硬盘(含网络及各种映射盘符)
#define DRV_CDROM         0x11       // 光驱

// 文件打开窗口功能标识符
#define SAVEFILE          0          // 文件存盘调用
#define LOADFILE          1          // 文件装入调用

// 字串指定位置检查返回标识符
#define STR_NULL          0          // 当前是NULL字符(或超出字串长度)
#define STR_HZLEFT        1          // 当前是汉字左半部分
#define STR_HZRIGHT       2          // 当前是汉字右半部分
#define STR_ASCII         3          // 当前是英文字符

/*================================================================*/

// SDK 预定义的标准图标数据指针
extern void ICO_QUESTION(void);      // 问号(?)
extern void ICO_EXIT(void);          // 退出
extern void ICO_ASTERISK(void);      // 注意(i)
extern void ICO_APPLICATION(void);   // 应用程序(Window)
extern void ICO_HAND(void);          // 停止(Stop)
extern void ICO_EXCLAMATION(void);   // 感叹号(!)

// 一些有用的 SDK 全局变量
extern int IC_SIZE;                  // 图标文字尺寸
extern char AUTOPRESS;               // 鼠标自动按下并弹起
extern int CLICK_SOUND;              // 按钮卡嗒声是否支持(默认0=不)
extern int DESKTOPDEFAULTCOLOR;      // 默认的桌面背景色
extern int DESKTOPDEFAULTSTYLE;      // 默认的桌面背景填充模式
extern char HAND_LR;                 // 当前的鼠标功能键(默认左键)
extern char NO_EXITWIN;              // 退出窗口
extern WORD DBLTIME;                 // 鼠标双击时间间隔(默认9, 即半秒之内)
extern char PUBLIC_BUF[4096];        // 公用缓冲区(应用程序一般不能使用)
extern char *GRAPH_BUFFER;           // 系统自动分配的图像处理缓冲区
extern WORD BUFFER_LEN;              // 图像缓冲区大小(单位字节, 默认10K)
extern WORD XMSBUFFER_LEN;           // 系统可使用的扩展内存大小(单位KB, 默认自动搜索全部使用)
extern WORD XMSHANDLE;               // XMS 全局大句柄
extern WORD SYSCHAR_SIZE;            // 系统标准字符尺寸
extern BYTE SYSCHAR_CSTYLE;
extern BYTE SYSCHAR_ASTYLE;
extern WORD CARET_SPEED;             // 插入光标闪烁速度
extern char windows_exec, windows_rdbk;
extern char windows_hide;
extern int DOS_CHECKDISK;            // DOS 严重错误控制变量
extern int DOS_CHECKRETERR;          // DOS 严重错误检测变量

/*================================================================*/

// 文字显示结构
typedef struct tagTEXTINFO{
  int x,y;                       // 显示坐标
  char  *String;                   // 字串指针
  WORD  AscFontNo;                 // 英文字体号
  WORD  ChiFontNo;                 // 中文字体号
  WORD  FontWidth,FontHeight;      // 英文字符宽度/高度
  WORD  FrColor,BgColor;           // 前/背景色
  WORD  CharSpace;                 // 字符间距
  WORD  EqualWidth;                // 英文是否等宽处理(1=是)
  char  *Buf;                      // 可使用的缓冲区
  WORD  BufLen;                    // 缓冲区长度(字节)
  BYTE  icon;                      // 是否透明显示(1=是)
}TEXTINFO,*LPTEXTINFO;

extern TEXTINFO textinfo;          // 全局的文字显示结构

// 桌面结构
typedef struct desk{
  char fillstyle;    // 填充图案
  char fillcolor;    // 填充颜色
  char *bmpname;     // 图像文件名(NULL=仅填充)
  WORD handle;       // 位图XMS句柄
  char bmpmode;      // 图像显示方式0=直接显示,1=居中,2=填充
}DESKTOPINFO;

extern DESKTOPINFO DESKTOP;     // 全局的桌面结构

// 矩形结构
typedef struct _re{
  int left;                     // 左上角
  int top;
  int right;                    // 右下角
  int bottom;
}RECT;

// 当前可视区域结构
struct viewporttype {
  int  left;                    // 区域左上角
  int  top;
  int  right;                   // 区域右下角
  int  bottom;
  int  clip;                    // 无效
};

// 当前直线造型结构
struct linesettingstype {
  int  linestyle;               // 线型代号
  WORD upattern;                // 造型字
  int  thickness;               // 无效
};

// 鼠标位图结构
typedef struct{
  WORD screen_mask[16];         // 屏幕屏蔽图
  WORD cursor_mask[16];         // 光标屏蔽图
  int hotx;                     // 鼠标热点
  int hoty;
}MOUSE_BMP;

// MOUSE 事件信息结构
typedef struct{
  int x;          // 发生位置
  int y;
  WORD s;          // 发生状态
  WORD t;          // 事件发生时间
}MOUSE_INFO;

extern MOUSE_INFO MOUSE_AGI;     // 全局的鼠标消息结构

// 用户自定义MOUSE事件结构
typedef struct _ms{
  int left;            // 事件矩形左上右下坐标范围
  int top;             // 4个值均定义为0时, 表示整个客户区范围
  int right;
  int bottom;
  WORD state;           // 事件激发条件
  MOUSE_BMP *usermap;   // 自定义MOUSE图标(NULL=标准箭头形状)
  void (*fun)           // 用户鼠标事件驱动回调函数
       (int x, int y, WORD s);
  struct _ms *next;
}USER_MOUSE;

// 复选钮结构
typedef struct _check{
  char disable;                 // 按钮禁止使用状态
  char *text;                   // 正文指针
  char enable;                  // 默认的选中状态
  int x;                        // 左上角坐标
  int y;
  int key;                      // 热键
  void (*fun)(int state);       // 选中后的回调函数
  struct _check *next;
}CHECK;

// 单选钮组结构
typedef struct _groprad{
  char **list;                  // 各项正文
  int enable;                   // 当前激活是哪一个
  int x;                        // 左上角坐标
  int y;
  int width;                    // 宽度(取最宽的一个字符串)
  int height;                   // 高度
  int key;                      // 热键
  void (*fun)(int count);       // 选择项执行回调函数
  struct _groprad *next;
}RADIO;

// 标签结构
typedef struct _label{
  char *text;                   // 字串(为NULL时显示图标)
  void (*ico_addr)(void);       // ICO 图标数据地址
  char *xor_buf;                // 彩色XOR位图指针
  char *and_buf;                // 单色AND位图指针
  WORD ico_width;               // 图标宽度
  WORD ico_depth;               // 图标高度
  int left;                     // 坐标
  int top;
  int hotkey;                   // 热键代码
  void (*fun)(void);            // 热键功能函数指针
  struct _label *next;
}LABEL;

// 按钮结构
typedef struct _but {
        char disable;           // 按钮禁止使用状态
        int left;               // 左上角坐标
        int top;
        int width;              // 宽度
        int height;             // 高度(0=自动计算合适高度)
        char *text;             // 按钮显示内容
        int key;                // 热键
        void (*fun)(void);      // 按下后回调函数指针
        struct _but *prev;
        struct _but *next;
}BUTTON;

// 滚动条结构
typedef struct _scrollbar {
        char mode;                 // 横/竖模式
        int left;                  // 左上角(X,Y)坐标
        int top;
        int ox, oy;                // 移动前的坐标
        int leng;                  // 宽/高
        float max;                 // 逻辑最大值
        float cure;                // 当前逻辑值(以1为单位变化)
        float page;                // 页变化的逻辑值
        void (*function)           // 滚动后回调函数指针
             (struct _scrollbar *sbar, float prevcure);
        char hide;                 // 隐藏状态
        struct _scrollbar *next;   // 下一个滚动条指针
}SCROLLBAR;

// 列表框结构
typedef struct _box {
  int left;                     // 左上角(X,Y)
  int top;
  int width;                    // 宽度(字符个数)
  int height;                   // 高度(字符个数)
  float top_count;              // 顶部为第几个
  char hide;                    // 隐藏状态
  SCROLLBAR *sbar;              // 包含的滚动条句柄
  void (*fun_sele)(void);       // 选中以后函数指针
  struct _box *prev;
  struct _box *next;
}LISTBOX;

// 输入行结构
typedef struct _inp {
  int left;                     // 左上角坐标
  int top;
  int width;                    // 可显示的字符个数(宽度)
  WORD length;                  // 输入串最大长度
  WORD curepos;                 // 当前显示时从第几个字符开始
  WORD cursx;                   // 输入行中的光标位置
  char *text;                   // 输入串存放地址指针
  void (*fun_sele)(void);       // 选中后回调函数指针
  LISTBOX *box;                 // 包含的下拉列表框句柄
  struct _inp *prev;
  struct _inp *next;
}INPUTLINE;

// 菜单结构
typedef struct _menu {
  char *menu_name;              // 窗口横向菜单内容(第一个字母为简洁键)
  char **menu_selects;          // 菜单选择项(每项第一个字母为简洁键)
  void (**menu_function)(void); // 选择项执行回调函数指针
  WORD cure_sele;               // 当前选择项
}MENUS;

// 分组框机构
typedef struct _groupbox{
  char *text;                   // 分组框的标题指针
  int left;                     // 左上角坐标
  int top;
  int width;                    // 宽度
  int height;                   // 高度
  struct _groupbox *next;
}GROUPBOX;

// 窗口结构
typedef struct _wnd {
  struct _wnd *next;          // 窗口链表
  struct _wnd *prev;
  char hide;                  // 隐藏标记
  WORD mode;                  // 窗口模式
  int minmax;                 // 最大化/最小化标记
  int oldminmax;              // 缩小化后还原时使用的原来的标记
  void (*ICON)(void);         // 窗口图标原始数据(缩小化时才创建)
  WORD ico_count;             // 第几个图标
  WORD ico_handle;            // 图标的句柄保存
  char *ico_xor;              // 图标异或图数据指针
  char *ico_and;              // 图标相与图数据指针
  WORD ico_width;             // 图标宽度
  WORD ico_depth;             // 图标高度
  char *ico_text;             // 窗口图标说明文字(默认窗口标题)
  int ico_x;                  // 窗口图标的显示位置
  int ico_y;
  int old_left;               // 重排时保存原始位置/尺寸
  int old_top;
  int old_width;
  int old_height;
  int left;                   // 窗口当前左上角坐标
  int top;
  int width;                  // 窗口宽度
  int height;                 // 窗口高度
  int minwidth;               // 最小宽度
  int minheight;              // 最小高度
  int stateline_height;       // 状态行高度
  char *title;                // 标题(如果定义为NULL,则不显示标题)
  int vx, vy;                 // 窗口中内容开始显示的相对距离
  int vr, vb;                 // 窗口右边应减去距离
  WORD syschar_size;          // 窗口字符尺寸
  BYTE syschar_cstyle;        // 窗口汉字字体
  BYTE syschar_astyle;        // 窗口英文字体
  BYTE boardstyle;            // 窗口填充图案
  BYTE boardcolor;            // 窗口底板颜色
  BYTE linecolor;             // 边框颜色
  BYTE disablecolor;          // 部件禁止使用颜色
  BYTE menubkc;               // 菜单背景色
  BYTE title_bkc;             // 标题背景色
  BYTE title_fgc;             // 标题前景色
  BYTE title_bkc_dis;         // 标题禁止时背景色
  BYTE title_fgc_dis;         // 标题激活时前景色
  WORD cure_type;             // 当前激活部件代号
  WORD cure_num;              // 当前激活部件中的第几个
  GROUPBOX *groupbox;         // 分组框
  LABEL *label;               // 标签指针
  CHECK *check;               // 复选钮
  RADIO *radio;               // 单选钮
  SCROLLBAR *sbar;            // 滚动条指针
  MENUS *menu;                // 菜单指针
  MENUS *curemenu;            // 当前菜单(横向)
  BUTTON *button_head;        // 按钮链表
  BUTTON *button_tail;
  INPUTLINE *inpline_head;    // 输入框链表
  INPUTLINE *inpline_tail;
  LISTBOX *box_head;          // 列表框链表
  LISTBOX *box_tail;
  SCROLLBAR *hbar;            // 横向滚动条属性
  SCROLLBAR *vbar;            // 纵向滚动条属性
  char inputhot;              // 是否具有输入焦点
  char CARET_STATE;                             // 脱字符状态
  int CARET_X, CARET_Y, CARET_W, CARET_H;       // 脱字符的位置及尺寸记录
  USER_MOUSE *mouse;                            // MOUSE 用户事件定义回调函数
  void (*FUNCTION)(struct _wnd *wnd);           // 重画回调函数指针(重画窗口标准内容外的信息)
  void (*CLOSEWIN)(void);                       // 窗口关闭回调函数
  void (*RESIZEWIN)(struct _wnd *wnd);          // 尺寸变化后回调函数
  int (*KB_Entry)(int keycode, char keystate);  // 窗口的键盘处理回调函数
  void (*DRAWBOARD)(struct _wnd *wnd);          // 画窗口背景回调函数
  void (*DRAWICON)(struct _wnd *wnd, RECT *rect);  // 图标窗口绘制回调函数
  int (*ENABLE)(struct _wnd *wnd, int msg);     // 窗口变化事件回调函数
}WINDOWS;

extern WINDOWS *wintable_head, *wintable_tail;  // 全局的窗口首尾链表指针

// BMP 文件头结构
typedef struct {
        BYTE id[2];               // "BM" 标记
        DWORD filesize;           //  文件总长度
        WORD reserved[2];
        DWORD headersize;         // 头大小(位图数据实际偏移)
        DWORD infosize;           // 40
        DWORD width;              // 宽度
        DWORD depth;              // 高度
        WORD biplanes;            // 1
        WORD bits;                // 1,4,8,24
        DWORD bicompression;      // 0
        DWORD bisizelmage;        // 位图数据字节数
        DWORD bixpelspermeter;
        DWORD biypelspermeter;
        DWORD biclrused;          // 实际用颜色(0=全部)
        DWORD biclrimportant;     // 重要颜色数(0=全部)
}BMPHEAD;

// ICO 文件头结构
typedef struct {
        WORD icoresv;    // 0 标记
        WORD icotype;    // 1 标记
        WORD icocount;   // 个数
}ICOHEAD;

// ICO 目录结构
typedef struct {
        BYTE width;         // ICO 宽点(16,32,64)
        BYTE height;        // ICO 高度(16,32,64)
        BYTE colorcount;    // ICO 使用颜色数 2, 8, 16
        BYTE resv;          // 0 保留
        WORD resv1;         // 0 保留
        WORD resv2;         // 0 保留
        DWORD icodibsize;   // 图标DIB总字节
        DWORD icodiboffset; // 图标DIB数据的偏移
}ICODIR;

// ICO DIB结构
typedef struct {
        DWORD infosize;           // 40
        DWORD width;              // 宽度
        DWORD depth;              // 高度
        WORD biplanes;            // 1
        WORD bits;                // 1,4,8,24
        DWORD bicompression;      // 0
        DWORD bisizelmage;        // 位图数据字节数
        DWORD bixpelspermeter;
        DWORD biypelspermeter;
        DWORD biclrused;          // 实际用颜色(0=全部)
        DWORD biclrimportant;     // 重要颜色数(0=全部)
}ICODIB;

// 相关位图(DDB)结构
typedef struct {
        BYTE id[2];        // DDB 文件头标记 "DB"
        BYTE acmode;       // DDB 适应色彩区分号
        WORD width;        // 宽点
        WORD depth;        // 高点
        WORD bytes;        // 每行字节数
}DDBHEAD;

// 相关位图(DDB)转换结构
typedef struct {
        WORD sbmp_addoff;         // 数据源地址
        WORD sbmp_addseg;
        WORD dbmp_addoff;         // 目标地址
        WORD dbmp_addseg;
        WORD bmp_width;           // 点宽度
        WORD bmp_color;           // 前背景色(单色BMP用)
        WORD spal_addoff;         // 源调色板地址(16/256色BMP用)
        WORD spal_addseg;
        WORD dpal_addoff;         // 目标调色板地址(16/256色BMP用)
        WORD dpal_addseg;
        BYTE bmp_model;           // 源与目标数据操作模式
                                  // (0=COPY,1=NOT,2=AND,3=OR,4=XOR)
        BYTE bmp_bits;            // 1,4,8,24 源格式(BMP数据的一行)
}IMGSTRUCT;

//=========================================================================

/*-----------------------------------------------------------------------*/
/* 以下为 BGI 的仿真函数, 无特殊说明者同 BGI                             */
/*-----------------------------------------------------------------------*/
void initgraph(int graphdriver, int graphmode);
void closegraph(void);
void cleardevice(void);
void clearviewport(void);
int getmaxcolor(void);
int getmaxx(void);
int getmaxy(void);
int getx(void);
int gety(void);
int getcolor(void);
void setcolor(int color);
void setwritemode(int mode);
/*-----------------------------------------------------------------------*/
// 参数 clip 无效
void setviewport(int left, int top, int right, int bottom, int clip);
// 作用与 setviewport 一致, 但是当前点的坐标不变化
void Setviewport(int left, int top, int right, int bottom);
/*-----------------------------------------------------------------------*/
void getviewsettings (struct viewporttype far *viewport);
void setpalette(int colornum, int color);
void setrgbpalette(int colornum, int red, int green, int blue);
unsigned getpixel(int x, int y);
void putpixel(int x, int y, int color);
void moveto(int x, int y);
void setlinestyle(int linestyle, unsigned upattern, int thickness);
void getlinesettings(struct linesettingstype far *lineinfo);
void line(int x1, int y1, int x2, int y2);
void linerel(int dx, int dy);
void lineto(int x, int y);
void rectangle(int left, int top, int right, int bottom);
void bar(int left, int top, int right, int bottom);
void bar3d(int left, int top, int right, int bottom, int depth, int topflag);
void setfillstyle(int pattern, int color);
void setfillpattern(char far *upattern, int color);
void arc(int x, int y, int stangle, int endangle, int radius);
void circle(int x, int y, int radius);
void pieslice(int x, int y, int stangle, int endangle, int radius);
void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius);
void fillellipse(int x, int y, int xradius, int yradius);
void sector(int x, int y, int stangle, int endangle,int xradius, int yradius);
void floodfill(int x, int y, int border);
/*-----------------------------------------------------------------------*/
/* 参数 direction 无效
   本命令将使英文及汉字使用相同的字体代号及相同的字符高度.
   汉字宽度与高度相同, 英文宽度是汉字的一半                              */
void settextstyle(int font, int direction, int charsize);
// 设置英文字体
void setascstyle(int font);
// 设置字符间距
void settextextra(int extra);
/*-----------------------------------------------------------------------*/
void settextheight(int charsize);
void settextwidth(int charsize);
int textheight(char far *textstring);
int textwidth(char far *textstring);
/*-----------------------------------------------------------------------*/
/* 设定文字显示时的背景色                                                */
void setcolorbm(int color);
/* 显示有背景的文字, 背景色由 setcolorbm 设定                            */
void outtextbm(int x, int y, char far *textstring);
/*-----------------------------------------------------------------------*/
void outtext(char far *textstring);
void outtextxy(int x, int y, char far *textstring);
/*-----------------------------------------------------------------------*/
/* 返回0xffff表示image尺寸超出64K, 此时使用getimage时只需要
   分配256个字节缓冲区即可.*/
unsigned imagesize(int left, int top, int right, int bottom);
/*-----------------------------------------------------------------------*/
/* 当image尺寸超出64K时, getimage 可自动将image保存至XMS或磁盘空间,
   但此时缓冲区 bitmap 需给出 256 个字节的空间保存其他信息 */
void getimage(int left, int top, int right, int bottom, void far *bitmap);
void putimage(int left, int top, void far *bitmap, int op);
/*-----------------------------------------------------------------------*/
/* 释放image所占用的内存/磁盘空间
   一定要注意, 使用getimage后的image空间必须通过killimage函数释放. */
void killimage(char *bitmap);
/*-----------------------------------------------------------------------*/
/* 设置VGA256色模式下的调色板
   参数 pal 指向768字节的调色板数据, 注意调色板数据是8位格式             */
void setvgapal(char *pal);
/*-----------------------------------------------------------------------*/
/* 保护VGA256色模式下的调色板, 生成一个保护后的调色板
        参数 spal= 源调色板
             dpal= 目标调色板(一般是当前调色板)
             beg= 保护开始的DAC号
             count= 连续保护几个DAC
             csize= 源调色板实际的DAC字节数(每个DAC占用3字节)            */
void setsavepalette(char *spal, char *dpal, int beg, int count, WORD csize);


/*-----------------------------------------------------------------------*/
/* 其他的调色板、图像处理函数                                            */
/*-----------------------------------------------------------------------*/
// 检取当前屏幕的颜色深度
int UC_GetColourDepth(void);
/*-----------------------------------------------------------------------*/
// 渐显指定的调色板
// delay= 延时
// pal= 显示的调色板(一般是当前调色板数据)
void UC_VGAFadeIn(int delay, char *pal);
/*-----------------------------------------------------------------------*/
// 渐黑当前的屏幕
// delay= 延时
void UC_VGAFadeOut(int delay);
/*-----------------------------------------------------------------------*/
// 16/256 色模式下, 读出当前 BIOS 调色板到指针 pal,
//      RGB 数据均已左移2位, 形成8位格式
void UC_ReadBiosPalette(char *pal);
/*-----------------------------------------------------------------------*/
// Windows BMP 格式的4字节RGB值转换为BIOS的3字节RGB方式
// 参数 pal= BMP调色板指针
//      count= 颜色个数计数(处理几组RGB)
void UC_SetBMPPalette(char *pal, int count);
/*-----------------------------------------------------------------------*/
// 设定256色图像显示时的调色板保护
// 参数 dac_begin= 起始DAC
//      dac_count= 连续保护DAC的个数
void UC_Safe256palette(int dac_begin, int dac_count);
/*-----------------------------------------------------------------------*/
/* 将 Windows BMP 格式的一行DIB数据转换成DDB位图格式
   本命令以BMP格式为外部标准, 可将任何格式的图像数据转换成DDB格式
        参数 sbuf= 源数据缓冲
             dbuf= 目标缓冲
             spal= 源RGB指针(即图像的RGB)
             dpal= 目标RGB指针(即保护后的RGB)
             color= 单色用前背景色(低位是前景)
             model= 源数据和目标数据操作模式(0=COPY,1=NOT,2=AND,3=OR,4=XOR)
             width= 源点宽
             depth= 源点高
             bits= 源数据的颜色深度(1,4,8,24)                            */
void UC_DIB2DDB(char *sbuf, char *dbuf, char *spal, char *dpal,
                int color, char model, int width, int depth, int bits);
/*-----------------------------------------------------------------------*/
/* 创建 ICO 图标, 将ICO文件转换出两个DDB缓冲区(可支持任何色彩及尺寸的ICO)
   并自动申请 xor_buf 及 and_buf 空间
        返回: TRUE  = 成功
              FALSE = 失败
        参数 ico_addr= ICO 图标数据存放地址
             xor_buf= 彩色DDB(XOR图)缓冲区
             and_buf= 单色DDB(AND图)缓冲区
             ico_width, ico_depth= 返回调用者的ICO宽度及高度
             icn= 转换图标数据中第几个ICO图标                            */
int UC_CreateIcon(char *ico_addr, char **xor_buf, char **and_buf,
                  WORD *ico_width, WORD *ico_depth, WORD icn);
/*-----------------------------------------------------------------------*/
// 将一创建的ICO图标装入XMS
// 返回 0 失败, 否则返回 XMS 句柄
// xor_buf/and_buf= 图标彩色/单色数据指针
// width,depth= ICO 图标的宽度/高度
WORD UC_XMSLoadIcon(char *xor_buf, char *and_buf, int width, int depth);
/*-----------------------------------------------------------------------*/
// 将XMS中的ICO图标显示到屏幕上, 一般不必使用本命令
// 正常情况下显示图标请用 ShowIcon 函数
// 参数 handle= 图标的 XMS 句柄
//        x, y= 显示坐标
void UC_XMSputicon(WORD handle, int x, int y);
/*-----------------------------------------------------------------------*/
// 显示一创建的ICO图标
// 参数 handle= 装入XMS中的图标句柄(等于NULL时, 自动使用xor_buf/and_buf)
//      xor_buf= XOR位图缓冲区
//      and_buf= AND位图缓冲区
//      width,depth= ICO宽度及高度点
//      x,y= 屏幕(窗口)坐标
void UC_ShowIcon(WINDOWS *wnd, WORD handle, char *xor_buf, char *and_buf,
                 int width, int depth, int x, int y);
/*-----------------------------------------------------------------------*/
// 创建一个 Bitmap 位图, 并生成一个对应的DDB格式位图
// 返回0失败, 返回非0成功
// 参数 path= BMP 文件命及其路径
int UC_CreateBitmap(char *path);
/*-----------------------------------------------------------------------*/
// 建立小型BMP位图, 无调色板/无级缩放处理(原BMP文件及转换结果均在64K以内)
// 入口: bmp_addr= BMP 文件数据地址
// 返回: 0 表示建立失败, 否则成功
// 参数: ddb_buf= 结果DDB数据指针(装入XMS成功后为NULL, 此时使用handle)
//       width, depth= BMP 宽高
//       handle= XMS 句柄(0=装入失败), 若有, 则 ddb_buf=NULL
int UC_CreateMiniBMP(char *bmp_addr, char **ddb_buf, WORD *width,
                     WORD *depth, WORD *handle);
/*-----------------------------------------------------------------------*/
// 显示小型BMP位图
// 参数: wnd= 窗口句柄
//       handle= XMS 句柄(=0时使用ddb_buf)
//       ddb_buf= handle=0时ddb数据地址
//       width, depth= BMP宽高
//       x, y= 屏幕坐标
void UC_ShowMiniBMP(WINDOWS *wnd, WORD handle, char *ddb_buf,
                    WORD width, WORD depth, int x, int y);
/*-----------------------------------------------------------------------*/
// 将一 DDB 文件装入 XMS 空间
// 返回 NULL 失败, 否则为装入成功后的 XMS 句柄
// 参数 path= DDB 文件名及路径(扩展名无效)
WORD UC_XMSLoadDDB(char *path);
/*-----------------------------------------------------------------------*/
// 显示一幅DDB格式位图文件
// 参数 wnd= 窗口句柄 (NULL时以全屏幕为背景)
//      handle= XMS句柄(NULL时, 使用path)
//      path= BMP文件名及路径
//      x, y= 左上角坐标
//      width,height= 从起始(x,y)坐标开始的限制显示高宽
//      smode=显示位置(居中/铺满等)
//      wmode=显示方式(异或/相与等)
int UC_DrawDDB(WINDOWS *wnd, WORD handle, char *path, int x, int y,
               int width, int height, int smode, int wmode);
/*-----------------------------------------------------------------------*/
//将buf中的扫描线放入屏幕
//width=点宽度
//depth=点高度
//x,y=屏幕坐标
void UC_PutScreenBlock(char *buf, int width, int depth, int x, int y);
/*-----------------------------------------------------------------------*/
// 从屏幕上取出扫描线->buf
//width=点宽度
//depth=点高度
//x,y=屏幕坐标
void UC_GetScreenBlock(char *buf, int width, int depth, int x, int y);
/*-----------------------------------------------------------------------*/
// 从当前屏幕上建立DDB文件
// path=文件名及其路径
// x,y=屏幕坐标
// width, depth=点宽度及高度
int UC_CreateScreenDDB(char *path, int x, int y, int width, int depth);
/*-----------------------------------------------------------------------*/
// 从屏幕取图形 -> XMS
// 成功返回XMS句柄, 失败返回NULL
// x,y=屏幕坐标
// width, depth=点宽度及高度
WORD UC_XMSgetscreen(int x, int y, int width, int depth);
/*-----------------------------------------------------------------------*/
// 从XMS -> 屏幕
// x,y=屏幕坐标
// width, depth=点宽度及高度
void UC_XMSputscreen(WORD handle, int x, int y);
/*-----------------------------------------------------------------------*/
// 检查DDB格式文件的一致性, 若当前屏幕可显示, 返回TRUE,
// 若失败, 返回 FALSE
// path=DDB文件命及其路径
int UC_CheckDDB(char *path);
/*-----------------------------------------------------------------------*/
// 对一指定的DDB文件进行无级缩放, 产生一个新的DDB文件
// SrcFile= 源文件名
// DstFile= 目标文件名
// DstWidth= 目标宽度
// DstHeight= 目标高度
// Buf= 缩放可使用的缓冲区
// BufLen= 缓冲区长度字节
int UC_ZoomDDB(char *SrcFile, char *DstFile, WORD DstWidth,
               WORD DstHeight, char *Buf, WORD BufLen);
/*-----------------------------------------------------------------------*/
// 缩放内存中的DDB, 结果存放到一个新的DDB文件中
// ddbbuf= DDB图像缓冲区
// width,depth= DDB 宽度/高度
int UC_ZoomIcon(char *ddbbuf, WORD width, WORD depth,
                char *DstFile, WORD DstWidth, WORD DstHeight,
                char *buf, WORD BufLen);
/*-----------------------------------------------------------------------*/
// 设定DDB显示时的缩放尺寸
// dstwidth, dstheight= 目标宽度/高度
// 若设置为0, 则保留源尺寸
void UC_SetZoomDDB(WORD dstwidth, WORD dstheight);
/*-----------------------------------------------------------------------*/
// 取得当前DDB放大的尺寸
// 返回: dstwidth, dstheight 为当前宽度/高度
void UC_GetZoomDDB(WORD *dstwidth, WORD *dstheight);
/*-----------------------------------------------------------------------*/
// 取一BMP文件头 -> 结构 bmp 中
// 返回NULL, 表示BMP文件打开失败
// 是否是BMP格式文件, 需要用户自己识别
void UC_GetBMPInfo(char *path, BMPHEAD *bmp);
/*-----------------------------------------------------------------------*/
// 返回ICO图标信息, 返回NULL时非ICO图标格式
// ico_addr= ICO图标数据
// icn= 计数
// width= 返回宽度
// depth= 返回高度
// bits= 返回每点占用位数
int UC_GetIconInfo(char *ico_addr, WORD icn, WORD *width, WORD *depth,
                   WORD *bits);
/*-----------------------------------------------------------------------*/
// 从一 ICO 文件数据中按需要找出合适的序号
// 返回 0 表示没有合适的. 必要的话可以使用第一个
// 参数: cpIconAddr   图标数据指针
//       wWidth       需要的图标宽度
//       wDepth       需要的图标高度
//       wBits        需要的图标颜色深度
WORD UC_GetIconApposite(void *cpIconAddr,
                        WORD wWidth, WORD wDepth, WORD wBits);
/*-----------------------------------------------------------------------*/
// 将图标读入内存的子程序
// 返回分配好内存的图标数据指针
// 返回 NULL 则表示失败
// filename= 图标文件名及其路径
void *UC_ReadIconFile(char *filename);


/*----------------------------------------------------------------------*/
/*  以下为XMS扩展内存的管理函数                                         */
/*----------------------------------------------------------------------*/
// 分配XMS (返回NULL失败, 1成功)
// size=以KB计的内存块大小
// handle= 成功后为 XMS 句柄(同时使用的句柄经内部扩充后可达1000个)
int UC_AllocXMS(WORD size, WORD *handle);
/*----------------------------------------------------------------------*/
// 取总共及剩余XMS空间(单位KB)
// totalk=XMS总空间
// freek=XMS剩余空间
void UC_GetFreeXMS(WORD *totalk, WORD *freek);
/*----------------------------------------------------------------------*/
// 释放XMS空间, handle=已分配的句柄
void UC_FreeXMS(WORD handle);
/*----------------------------------------------------------------------*/
// 把数据送往XMS前进行初始化工作
// handle= 分配的XMS句柄
// buf= 数据的缓冲区指针
// offset= 送往XMS的开始偏移字节(SEEK)
void UC_InitToXMS(WORD handle, void *buf, DWORD offset);
/*----------------------------------------------------------------------*/
// 把XMS中的数据取回前进行初始化工作
// handle= 分配的XMS句柄
// buf= 数据的缓冲区指针
// offset= 要取回数据在XMS的开始偏移字节(SEEK)
void UC_InitFromXMS(WORD handle, void *buf, DWORD offset);
/*----------------------------------------------------------------------*/
// 数据传送往 XMS
// count= 传送字节数
void UC_MoveToXMS(WORD count);
/*----------------------------------------------------------------------*/
// XMS 数据传送往内存
// count= 传送字节数
void UC_MoveFromXMS(WORD count);
/*----------------------------------------------------------------------*/
// 校验XMS句柄handle的合法性
// 返回0不合法, 返回1合法
int UC_VerifyXMSHandle(WORD handle);


/*-----------------------------------------------------------------------*/
/*  SDK 初始化及系统退出类                                               */
/*-----------------------------------------------------------------------*/
// 桌面初始化函数
// fillstyle= 填充图案代号
// fillcolor= 填充颜色
// bmpname= BMP图像文件名称及其路径(为NULL时仅填充)
// bmpmode= BMP图像显示时的方式
void UC_InitDesktop (char fillstyle, char fillcolor,
                     char *bmpname, char bmpmode);

// 窗口管理初始化函数(含自动模式设定)
void UC_InitUCVision (int GraphDriver, int GraphMode);

// 系统退出窗口确认
// retcode= 返回码
// fun= 确定退出时的回调函数
void UC_CloseUCVision(int retcode, void (*fun)());

// 执行DOS命令
// command= DOS命令及其路径
void UC_System(char *command);

// 进入DOS提示符子程序
void UC_ToDosPrompt(void);

// 内存不够异常退出函数
// text= 用于说明原因
void nomem(char *text);

// 替代 free 函数的内存释放函数
// 增加安全性的检查
// block= 欲释放的地址
void MyFREE(void *block);

/* 异常退出口
   当出现异常情况时的退出不能使用 exit(), 此时请使用本函数
   参数 (*fmt, ...) 表示退出后的屏幕提示信息, 可以说明退出的原因 */
void UC_Exit(char *fmt, ...);

// 设置应用程序的退出回调函数
// sdkexit= 异常退出时的应用程序回调函数
void UC_atexit(void(*sdkexit)());

/*-------------------------------------------------------------------------
                  构件定义类(即创建类)
-------------------------------------------------------------------------*/
// 定义一个窗口, 返回窗口句柄
WINDOWS *UC_DefineWindow(WORD mode,              // 窗口模式
                         int left, int top,      // 窗口左上角
                         int width, int height,  // 窗口高/宽
                         char *title,            // 窗口标题
                         void (*fun_redraw)(WINDOWS *wtmp),   // Redraw 重画函数
                         void (*fun_close)(),                 // Close 关闭函数
                         void (*fun_resize)(WINDOWS *wtmp)    // Resize 调用函数
                         );

// 定义窗口激活/禁止事件处理函数
// wnd= 窗口句柄
// fun= 事件处理函数(msg=传递的消息标识符)
void UC_DefineWindowEnable(WINDOWS *wnd, int(*fun)(WINDOWS *wnd, int msg));

// 定义窗口底滚动条
SCROLLBAR *UC_DefineWindowHScrollbar(WINDOWS *wnd,
                         void (*fun)(SCROLLBAR *sbar, float prevcure));

// 定义窗口右滚动条
SCROLLBAR *UC_DefineWindowVScrollbar(WINDOWS *wnd,
                         void (*fun)(SCROLLBAR *sbar, float prevcure));

// 定义窗口的状态行
// wnd= 窗口句柄
// height= 状态行高度
void UC_DefineStateLine(WINDOWS *wnd, int height);

// 定义窗口背景绘制函数
// wnd= 窗口句柄
// function= 绘制回调函数
void UC_DefineDrawBackground(WINDOWS *wnd, void(*function)(WINDOWS *wnd));

// 定义窗口缩小图标的自绘制函数
void UC_DefineDrawIconRect(WINDOWS *wnd,
                           void (*fun)(WINDOWS *wnd, RECT *rect));

// 自绘制窗口图标调用
// x, y= 矩形的左上角(一般是窗口的左上角坐标)
// 然后传递给用户X居中的坐标(按图标宽度计算)
void UC_Cadi(WINDOWS *wnd, int x, int y);

// 定义窗口对应的图标
// wnd= 窗口句柄
// ico= 图标数据
// count= 第几个图标
// 对应的文字
void UC_DefineWindowIcon(WINDOWS *wnd, void (*ico)(void), WORD count,
                         char *text);

// 取得当前窗口的 HANDLE (即最前面的窗口)
WINDOWS *UC_GetCurrentWindow(void);

// 校验一窗口代码合法性函数, 返回非0表示合法
// w1=窗口句柄
int UC_WindowVerify(WINDOWS *w1);

// 定义窗口最小尺寸
// wnd= 窗口句柄
// width= 最小宽度(0失效)
// height= 最小高度(0失效)
void UC_DefineWindowMinSize(WINDOWS *wnd, int width, int height);

// 定义主窗口的菜单项, 无返回
// wnd= 窗口句柄
// mnu= 菜单结构指针(见 STRUCT.H 说明)
void UC_DefineMenu(WINDOWS *wnd, MENUS *mnu);

// 删除窗口中的部件
// 返回 NULL 失败, 返回1删除成功并引起窗口重新刷新
// wnd= 窗口句柄
// cure_type= 部件句柄
// cure_num= 部件号码
int UC_DestroyObject(WINDOWS *wnd, WORD cure_type, WORD cure_num);

// 定义分组框
// wnd= 窗口句柄
// text= 分组框标题
// left, top= 左上角
// width, height= 分组框宽/高
GROUPBOX *UC_DefineGroupBox(WINDOWS *wnd, char *text, int left, int top,
                            int width, int height);

// 定义单选钮
// wnd= 窗口句柄
// rad= 一组单选钮定义结构
// fun= 单选钮选中后函数指针
// cound= 传递给用户当前点中第几个
RADIO *UC_DefineRadioButton(WINDOWS *wnd, char **list, int enable,
                            int x, int y, int key, void (*fun)(int count));

// 定义复选钮(显示信息应与标签定义结合)
// wnd= 窗口句柄
// text= 标签
// enable= 激活状态
// x,y= 左上角坐标
// key= 热键代码
// fun()= 选择后的函数回调函数
CHECK *UC_DefineCheckButton(WINDOWS *wnd, char *text, char enable,
                            int x, int y, int key, void(*fun)(int state));

// 定义一个标签, 无返回
// wnd= 窗口句柄
// left,top= 左上角坐标
// hotkey= 热键代码
// fun= 热键函数指针
// text= 标签字串指针(定义为NULL, 则显示ICO图标)
// ico_addr()= ICO图标指针
// icn= 显示ICO图标中的第几个
LABEL *UC_DefineLabel(WINDOWS *wnd, int left, int top,
                      int hotkey, void (*fun)(),
                      char *text, void (*ico_addr)(), WORD icn);

// 定义一个按钮, 无返回
// wnd= 窗口句柄
// fun()= 按钮按下后的函数回调函数
// left,top,width,height=按钮左上角坐标及宽高
// text= 按钮中显示的字串指针
// key= 激活按钮的热键代码(鼠标检测以外)
BUTTON *UC_DefinePressButton(WINDOWS *wnd, void (*fun)(), int left, int top,
                             int width, int height, char *text, int key);

// 定义一个输入框, 返回输入框句柄
// wnd= 窗口句柄
// left,top= 输入框左上角坐标
// width= 输入框宽度(字符计数)
// text= 输入缓冲区指针
// length= 最长输入字符个数
// fun_sele()= 按回车确认后的函数回调函数
INPUTLINE *UC_DefineInputLine(WINDOWS *wnd, int left, int top, int width,
                              char *text, int length, void (*fun_sele)());

// 定义一个滚动条, 返回滚动条句柄
// wnd= 窗口句柄
// mode= H_BAR(水平), V_BAR(垂直)
// left,top= 左上角坐标
// leng= 滚动条长度
// max= 滚动条最大表示值(以1为单位变化)
// cure= 滚动条定义时默认当前值(以1为单位变化)
// page= 以页为单位变化的逻辑值
// fun()= 滚动条变化后的函数回调函数
// prevcure= 滚动条变化前的滚动条逻辑值(由此判别方向及滚动次数)
// hide= 初始定义时是否处于隐藏状态(0=YES)
SCROLLBAR *UC_DefineScrollbar(WINDOWS *wnd, char mode, int left, int top,
                              int leng, float max, float cure, float page,
                              void(*fun)(SCROLLBAR *sbar, float prevcure),
                              char hide);

// 定义一个列表框, 返回列表框句柄
// wnd= 窗口句柄
// left,top= 左上角坐标
// width= 输入框宽度(字符个数)
// height= 列表框高度(字符行数)
// max= 列表框滚动条的最大值
// fun()= 列表框滚动条变化后的函数回调函数
// fun_sele()= 列表框选中后的函数回调函数
// hide= 是否定义为隐藏性列表框(可弹出使用, 0=是)
LISTBOX *UC_DefineListBox(WINDOWS *wnd, int left, int top, int width,
                          int height, float max,
                          void (*fun)(SCROLLBAR *sbar, float prevcure),
                          void(*fun_sele)(), char hide);

// 定义一个组合框(弹出列表框), 返回输入框句柄(以输入框句柄定义)
// wnd= 窗口句柄
// left,top= 左上角坐标
// width= 输入框宽度(字符个数)
// height= 弹出列表框高度(字符行数)
// max= 弹出列表框滚动条的最大值
// text= 弹出列表框显示行内容的指针
// fun()= 弹出列表框滚动条变化后的函数回调函数
// fun_sele()= 弹出列表框选中后的函数回调函数
INPUTLINE *UC_DefinePopBox(WINDOWS *wnd, int left, int top, int width,
                           int height, float max, char *text,
                           void(*fun)(SCROLLBAR *sbar, float prevcure),
                           void(*fun_sele)());

// 定义默认当前激活类
// wnd=窗口句柄
// cure_type=组件类别句柄
// cure_num=设定组件中的第几个激活
void UC_DefineActive(WINDOWS *wnd, unsigned cure_type, unsigned cure_num);

// 激活新的部件(自动禁止原来的部件)
void UC_NewActive(WINDOWS *wnd, WORD cure_type, WORD cure_num);

// 禁止部件
void UC_DisableObject(WINDOWS *wnd, WORD cure_type, WORD cure_num);

// 允许部件
void UC_EnableObject(WINDOWS *wnd, WORD cure_type, WORD cure_num);

/*-------------------------------------------------------------------------
  消息事件处理定义类
-------------------------------------------------------------------------*/
// 定义键盘事件驱动函数指针
// wnd= 窗口句柄
// kbenv()= 用户自定义键盘驱动指针(处理后返回TRUE, 不处理的键返回FALSE)
// keycode= 调用键盘事件驱动时 kbenv 传递给用户的当前键码
// keystate= 键盘状态字节
void UC_DefineKeyboardEvent(WINDOWS *wnd,
                           int (*kbenv)(int keycode, char keystate));

// 返回当前键盘状态信息
char UC_GetKeyboardState(void);

// 模拟按键
// keycode= 按键扫描码
void UC_KeyBack(const int keycode);

// 定义用户自定义鼠标事件矩形驱动
// wnd= 窗口句柄
// left,top,right,bottom= 事件矩形左上角/右下角坐标, (0,0,0,0)为整个窗口框
// state= 事件激活条件状态
// usermap= 自定义MOUSE图标(NULL=标准)
// fun()= 用户事件函数回调函数
USER_MOUSE *UC_DefineUserMouseEvent(WINDOWS *wnd,
                             WORD left, WORD top, WORD right,
                             WORD bottom, WORD state, MOUSE_BMP *usermap,
                             void (*fun)(int x, int y, WORD s));

// 鼠标消息回退
void UC_MouseBack(void);

// 从鼠标事件缓冲区中顺序取出一个信息, 存于全局结构 MOUSE_AGI 中
int UC_MouseCheck(void);

// 检测当前的键盘按键信息, 并调用用户的键盘事件处理接口
int UC_CheckKeyboard(void);

// 等待鼠标按键放开(可在此处理鼠标拖动工作)
// wnd= 窗口句柄(NULL时, 坐标为屏幕绝对坐标)
// left,top,right,bottom= 允许拖动返回的范围(在此范围之外放开返回NULL)
// fun()= NULL则无拖动处理, 否则为拖动函数指针
int UC_WaitFreeMouse(WINDOWS *wnd, int left, int top, int right, int bottom,
                     void (*fun)(int x, int y, WORD s));

// 事件消息处理中心
void UC_WindowsCentral(void);

// 事件处理主死循环
void UC_MainLoop(void);

// 设置当前消息码
void UC_SetMessage(WORD MsgID);

// 获取当前消息码
WORD UC_GetMessage(void);

/*-------------------------------------------------------------------------
        MOUSE 功能处理类
---------------------------------------------------------------------------*/
// 鼠标初始化工作函数
void UC_MouseInit(void);

// 鼠标管理退出函数
void UC_MouseClose(void);

// 设置 MOUSE 左右手
void UC_MouseSetHand(char hand);

// 关闭鼠标显示
void UC_MouseHide(void);

// 显示鼠标函数
void UC_MouseShow(void);

// 强行设定鼠标位置函数
// (x, y)=鼠标新的位置
void UC_MouseSetPosition(int x, int y);

// 设定 MOUSE 图标数据
// *bitmap= 指向鼠标 BITMAP 数据
void UC_MouseSetShape(MOUSE_BMP *bitmap);

// 设置当前的鼠标形状类型
void UC_MouseShapeType(int type);

// 取得标准光标对应的指针
MOUSE_BMP *UC_GetIDC(int type);

/*-------------------------------------------------------------------------
        窗口应用类
---------------------------------------------------------------------------*/
// 激活某一窗口
// wnd=欲激活的窗口句柄
void UC_WindowEnable(WINDOWS *wnd);

// 窗口最大化显示及还原显示
void UC_MaxWindow(WINDOWS *wnd);

// 窗口极小化显示
void UC_MinWindow(WINDOWS *wnd);

// 显示窗口标题函数(若窗口标题定义为NULL, 则直接返回)
// wnd= 窗口句柄
void UC_DisplayTitle(WINDOWS *wnd);

// 取出当前窗口的客户区矩形范围
void UC_GetClientRect(WINDOWS *wnd, RECT *rect);

// 关闭当前窗口
void UC_WindowClose(void);

// SDK 帮助窗口
// pcHelpTitle= 帮助窗口标题
// pcHelpFile= 帮助文件名及其路径
// pcKeyWord= 帮助主题关键词
// iLeft, iTop, iWidth, iHigh= 窗口的左上角/右下角坐标(0, 0, 0, 0 为默认)
void UC_WinHelp( char * pcHelpTitle, char * pcFileName, char * pcKeyWord
                , int iLeft, int iTop, int iWidth, int iHigh );

// 显示一警告窗口函数
// funok= 关闭后的回调函数
// title= 窗口标题
// fmt= 显示内容
void UC_DialogWarning(void (*funok)(void), char *title, char *fmt,...);

// 显示一选择确认窗口函数
// isyes= 选择YES的回调函数
// isno= 选择NO的回调函数
// title= 窗口标题
// fmt= 显示内容
WORD UC_DialogYesNo(void (*isyes)(void), void (*isno)(void),
                          char *title, char *fmt,...);

// 显示一 About 窗口函数
// funok= 按确定按钮后的回调函数
// ico=关于窗口的图标指针
// icn=图标计数
// title= 自定义标题
// fmt= 显示内容
void UC_DialogAbout(void (*funok)(void), void (*ico)(), int icn,
                    char *title, char *fmt,...);

void UC_SecretWindow(void);

// 开始在背景窗口中显示
// 返回 NULL 不必显示, 否则需要显示
int UC_BeginPaintOnBack(WINDOWS *wnd);

// 结束背景窗口中的显示工作
void UC_EndPaintOnBack(WINDOWS *wnd);

// 初始化窗口提示行的显示
// 返回 NULL 表示失败, 提示行不能写, 否则可以.
// 写提示行的坐标仍然是窗口客户区的相对坐标
int UC_BeginPaintStateLine(WINDOWS *wnd);

// 返回窗口状态行矩形区域(相对窗口的坐标)
void UC_GetStateLineRect(WINDOWS *wnd, RECT *rect);

// 结束窗口提示行写
void UC_EndPaintStateLine(WINDOWS *wnd);

//使窗口中的客户区域无效(将自动重画)
void UC_InvalidataRect(WINDOWS *wnd, RECT *rect);

// 重画当前窗口后面的所有窗口(当前窗口位置/尺寸变化后调用)
// left,top,right,bottom= 需要重画的左上角/右下角范围(是矩形)
void UC_RedrawBack(int left, int top, int right, int bottom);

// 重画当前窗口前面的所有窗口(在被覆盖的窗口中显示内容后调用)
// wnd= 当前窗口句柄
// left,top,right,bottom= 需要重画的左上角/右下角范围(是矩形)
void UC_RedrawFore(WINDOWS *wnd, int left, int top,
                   int right, int bottom);

// 检查指定窗口上面是否被其他窗口全部覆盖, 返回1是, 否则没有覆盖
int UC_CheckInBack(WINDOWS *wnd);

// 如果重画范围被上层窗口全部遮住, 则不需再画(此时返回1), 否则返回NULL可重画
// 入口: wnd= 需重画的窗口句柄
int UC_CheckInViewPort(WINDOWS *wnd);

// 更新显示指定窗口
// wnd=欲刷新的窗口句柄
void UC_RedrawWindow(WINDOWS *wnd);

// 重新刷新当前窗口
void UC_RedrawCurrentWindow(void);

// 屏幕图形区域移动
// wnd= 窗口句柄(NULL时, 坐标为绝对坐标)
// oldx, oldy= 原左上角坐标
// oldw, oldh= 原宽度/高度
// newx, newy= 新的左上角坐标
void UC_ScrollBlock(WINDOWS *wnd, int oldx, int oldy, int oldw, int oldh,
                    int newx, int newy);

// 移动当前窗口位置
// newx, newy= 新的窗口左上角坐标
void UC_WindowMove(int newx, int newy);

// 变化当前窗口的尺寸
// newx,newy,newr,newb= 新的窗口左上角/右下角坐标
void UC_WindowResize(int newx, int newy, int newr, int newb);

// 鼠标移动窗口
int UC_BoardMoveMouse(void);

// 重新排列屏幕的所有窗口图标
void UC_ArrangeIcons(void);

// 标题式窗口
void UC_CascadeWindows(int left, int top, int right, int bottom);

// 横向平铺窗口
void UC_HTileWindows(int left, int top, int right, int bottom);

// 纵向平铺窗口
void UC_VTileWindows(int left, int top, int right, int bottom);

/*-------------------------------------------------------------------
        屏幕构件显示内容更新类
--------------------------------------------------------------------*/
// 更新当前的激活类型
// wnd= 窗口句柄
// color= 0激活, 1非激活
void UC_UpdateActive(WINDOWS *wnd, char color);

// 显示单选钮组
// wnd= 窗口句柄
// rad= 单选钮组句柄
void UC_UpdateRadioButton(WINDOWS *wnd, RADIO *rad);

// 根据单选钮YES状态画单选钮图案
// wnd= 窗口句柄
// check= 单选钮句柄
// x,y= 左上角坐标
void UC_UpdateCheckButton(WINDOWS *wnd, CHECK *check);

// 更新显示一个列表框中的内容(一般在滚动条变化后调用, 更新屏幕列表信息)
// wnd= 窗口句柄
// box= 列表框句柄
// text= 更新显示的字串地址指针
// total= 字串有多少条
void UC_UpdateListBox(WINDOWS *wnd, LISTBOX *box, char **text, int total);

// 根据当前列表框滚动条的当前值计算列表框顶部对应显示内容应从第几行开始
// 并返回该行数
float UC_GetBoxTopCount(LISTBOX *box);

// 更新输入框内容的显示
// wnd= 窗口句柄
// inp= 输入框句柄
// text= 欲更新的字串
void UC_UpdateInputLine(WINDOWS *wnd, INPUTLINE *inp, char *text);

// 检察一个字串的当前位置处是否汉字
int UC_CheckEditPoint(char *buf, int curepos);

// 显示滚动条上的移动按钮, 并清除移动后的痕迹
// wnd= 窗口句柄
// sbar= 滚动条句柄
void UC_UpdateScrollbar(WINDOWS *wnd, SCROLLBAR *sbar);

// 显示完整的滚动条函数
// wnd= 窗口句柄
// sbar= 滚动条句柄
void UC_DisplayScrollbar(WINDOWS *wnd, SCROLLBAR *sbar);

// 计算滚动条实际位置及长度
// wnd= 窗口句柄
// sbar= 滚动条句柄
// left= 实际X坐标(-1为窗口最右边)
// top= 实际Y坐标(-1为窗口最下边)
// leng= 实际长度(0为撑满窗口高宽, -1为撑满窗口高宽但留个角)
void UC_GetScrollbarXY(WINDOWS *wnd, SCROLLBAR *sbar,
                       int *left, int *top, int *leng);

// 根据系统字符尺寸(SYSCHAR_SIZE)计算实际的坐标
// xy= 欲计算变量的指针
void UC_GetRealXY(int *xy);

// 直接返回实际坐标
int UC_RetReal(const int val);

// 按钮点中后的效果处理, 返回非0表示在按钮范围内释放鼠标键
// wnd, but= 必须都定义为 NULL
// left,top,right,bottom= 按钮左上角/右下角
int UC_DoPressButton(WINDOWS *wnd, BUTTON *but,
                     int left, int top, int right, int bottom);

// 显示按钮子程序
// BHBH= 弹起/按下类型
// BLBL= 厚度
// x,y,right,bottom= 左上角/右下角坐标
void UC_DrawButton(char BHBH, char BLBL, int x, int y, int right, int bottom);

// 为背景重画准备的第二个视窗设置过程
// left,top,right,bottom= 新的视窗范围
int UC_SecondViewport(int left, int top, int right, int bottom);

// 窗口中的变量字串显示函数
// wnd= 窗口句柄
// x,y= 显示起始坐标
// fmt= 与 printf 函数一致的显示变量
int UC_WindowPrintf(WINDOWS *wnd, int x, int y, char mode, char *fmt, ...);

// 窗口中的固定字串显示函数, 按比例修正(x,y)坐标
// wnd= 窗口句柄
// x,y= 显示起始坐标
// text= 字串指针
void UC_TextOut(WINDOWS *wnd, int x, int y, char mode, char*text);

// 窗口中的固定字串显示函数, 不修正(x,y)坐标
// 一般不提供外部使用, 用于已经修正的坐标显示
// wnd= 窗口句柄
// x,y= 显示起始坐标
// text= 字串指针
void UC_WinOutTextXY(WINDOWS *wnd, int x, int y, char mode, char*text);

// 矩形区域的文字显示函数
// wnd= 窗口句柄
// rect= 矩形结构
// format= 显示格式
// fmt= 字串
int UC_DrawText(WINDOWS *wnd, RECT *rect, WORD format, char *fmt,...);

// 给出一个字串和要求宽度, 计算自动分词后的字串总高度(含行间距)
// text= 字串
// width= 给出的限制宽度点
int UC_TextPlines(char *text, int width);

// 计算字串有多少行, 返回字串的总行数
int UC_TextLines(char *text, RECT *rect, WORD format);

// 取出第几行字串的内容
// text= 源字串
// line= 第几行(从0开始)
// dtext= 目标字串的缓冲区
void UC_GetLineText(char *text, int line, char *dtext, RECT *rect, WORD format);

// 以当前字型检取字串在一个矩形中的最合适宽度
int UC_GetTextAppositeWidth(RECT *rect, WORD format, char *fmt,...);

// 返回当前的行间距
int UC_GetLineSpace(void);

// 设置行间距
void UC_SetLineSpace(int linespace);

// 窗口 BAR
// wnd= 窗口句柄
// left, top= 欲清除区域左上角坐标
// width, height= 区域宽度和高度
void UC_WindowBar(WINDOWS *wnd, int left, int top, int width, int height);

/*-------------------------------------------------------------------
        定时器控制类
--------------------------------------------------------------------*/
// 检查是否有定时函数出现
// 返回0没有, 返回1有, 全局函数指针TIMERFUN=轮到的函数指针
int UC_CheckTimer(void);

// 初始化定时器工作, 定时器中的所有工作全部取消
void UC_InitTimer(void);

// 设置定时器, 返回NULL设置失败
// fun= 定时调用的函数指针
// msec= 毫秒计数的定时时间
int UC_SetTimer(void (*fun)(), WORD msec);

// 取消定时器
// fun= 要取消的定时器指针
void UC_KillTimer(void (*fun)());

/*-------------------------------------------------------------------
        插入光标类
--------------------------------------------------------------------*/
// 创建脱字符
void UC_CreateCaret(WINDOWS *wnd, int width, int height);

// 销毁脱字符
void UC_DestroyCaret(WINDOWS *wnd);

// 移动脱字符位置
void UC_MoveCaret(WINDOWS *wnd, int x, int y);

// 隐藏脱字符
void UC_HideCaret(WINDOWS *wnd);

// 显示脱字符
void UC_ShowCaret(WINDOWS *wnd);

/*-------------------------------------------------------------------
        磁盘、文件类
--------------------------------------------------------------------*/
// 检察指定的驱动器属性
// 返回NULL表示驱动器不存在, 否则存在(返回标识符代表的驱动器类型)
// 参数 drv=0... (从A:开始)
int UC_GetDiskAttribute(int drv);

// 返回指定属性与匹配字串的文件总数
int UC_GetFileTotal(const char *spec, int attr);

// 返回指定的第几个文件名称, (NULL=失败)
// 参数: spec=  查找文件名匹配字串
//       count= 找出第几个(必须从1开始)
//       attr=  需要的文件属性
char *UC_GetFileNameByCount(const char *spec, int count, int attr);

// 继续按照指定属性找下一匹配文件
char *UC_GetNextFileNameByCount(int attr);

// 文件打开窗口函数
// option= 0存盘, 1装入
// argw= 默认第几个列出类型
// arg= 列出类型串指针
// funok(char *pathfile)= 打开成功后的函数指针,pathfile=传递出来的文件名指针
// funfail()= 打开失败后的函数指针
char *UC_DialogFileOpen(char option, int *argw, char **arg, char *title,
                        void (*funok)(char *pathfile), void (*funfail)());

// 获得当前执行程序的启动路径
// path= 返回路径字串(长度请定义为MAXPATH)
void UC_GetExecPath(char *path);

/*-------------------------------------------------------------------
// SDK 断言检测函数 (不需要断言时, 定义 UC_NDEBUG)
// 使用方法： UC_Assert(表达式)
//            表达式如果不成立, 断言失败, 程序将终止并显示失败信息
--------------------------------------------------------------------*/
#  undef UC_Assert
#       ifdef UC_NDEBUG
#               define UC_Assert(p)   ((void)0)
#       else
#               define UC_Assert(p) ((p) ? (void)0 : (void) UC_Exit( \
                                "断言失败: %s, 文件 %s, 行号 %d\n", \
                                #p, __FILE__, __LINE__ ) )
#       endif

#ifdef __cplusplus
}
#endif

#endif // __JJSDK__
