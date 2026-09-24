/*---------------------------------------------------------------
  程序: LIST.C
  演示: 如何在窗口中创建列表框, 如何将文字串内容逐条添加入列表框
----------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>

#include "sdk.h"

void My_Begin(void);
void My_Widget(void);
void My_App(void);

WINDOWS *wnd;
LISTBOX *box;
int LISTTOTLE=20;       // 列表框中文字的总条数

// 定义列表框的字串内容
//--------------------------------------------------
char *text[]={ "混沌未分天地乱, 茫茫渺渺无人见.",
               "自从盘古破鸿蒙, 开辟从兹清浊辨.",
               "覆载群生仰至仁, 发明万物皆成善.",
               "欲知造化会元功, 须看西游释厄传.",
               "text 5",
               "text6 ",
               "text7",
               "text8",
               "sdfsdf",
               "w45i7u4iout",
               "计算机汉字",
               "换欢欢换环中还",
               "撒三叁赛赛sdjfdg",
               "sdkfjdfgj",
               "21k3ljlkgjkxvj",
               "sdlkugf54867udkj",
               "sdlkgjdkjlhjh345",
               "drjriojtydfgj",
               "3245ertdfdffdgh",
               "sdkljflkdsgjkldge4iou6eotr",
               NULL,
};

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
                  "c:\\windows\\winlogo.bmp",
                  FUL_SCR);
   UC_InitUCVision(DETECT, DETECT);
}

void My_redraw(WINDOWS *wnd)
{
   wnd=wnd;
}

void My_close(void)
{
   UC_CloseUCVision(NULL, NULL);       // 窗口关闭时，退出SDK
}

void My_resize(WINDOWS *wnd)
{
   wnd=wnd;
}

// 列表框滚动条变化后的回调函数, 也是列表框内容显示的唯一机会
void cbBar(SCROLLBAR *sbar, float pcure)
{
   int ctop;

   pcure=pcure;
   sbar->max=LISTTOTLE;                // 赋予滚动条最大值为列表框内容的最大条数
   ctop=(int)UC_GetBoxTopCount(box);   // 检取当前列表框顶部为第几条开始
   UC_UpdateListBox(wnd, box, text+ctop, LISTTOTLE-ctop);  // 更新从顶部开始的剩余条数
}

void cbSele()
{
}

// 界面元素定义过程
void My_Widget(void)
{

   wnd=UC_DefineWindow(WS_MAIN | WS_NOSIZE,            // 无尺寸变化的主窗口
                       CP_MIDSCR, CP_MIDSCR,           // 窗口居中屏幕
                       500, 400,
                       "List 一个含列表框控制的应用窗口",
                       My_redraw,
                       My_close,
                       My_resize);

   box=UC_DefineListBox(wnd,
                        10, 10,
                        30, 15,
                        100,
                        cbBar,              // 滚动条回调函数
                        cbSele,             // 列表框某条内容被选中后的回调函数
                        1);                 // 定义为非隐藏状态

   UC_DefineActive(wnd, TYPE_LISTBOX, 1);
   UC_WindowEnable(wnd);                    // 显示定义好的窗口
}

// 应用主体
void My_App(void)
{
   UC_MainLoop();        // 直接进入 SDK 事件驱动主循环
}
