/**************+/
    UC Calc
 **************
   I Believe
   1996-7-11
/+*************/

#if __BCPLUSPLUS__< 0x0340
#error Use Borland C++ 4.5 or later releases mode to compile FileOpen Module!
#endif

#ifndef SDK_UCCALC_CLASS
#define SDK_UCCALC_CLASS

#include <stdlib.h>
#include "sdk.h"

const int MenuNum = 2;
const int Menu1Num = 4;
const int Menu2Num = 3;
const int ButtonNum = 28;
const int ShowStringNum = 25;
const int AvailNum = 13;

const int Key_OFF   = 0;
const int Key_Back  = 1;
const int Key_CE    = 2;
const int Key_C     = 3;
const int Key_MC    = 4;
const int Key_7     = 5;
const int Key_8     = 6;
const int Key_9     = 7;
const int Key_Div   = 8;
const int Key_Sqrt  = 9;
const int Key_MR    = 10;
const int Key_4     = 11;
const int Key_5     = 12;
const int Key_6     = 13;
const int Key_Mul   = 14;
const int Key_Prec  = 15;
const int Key_MS    = 16;
const int Key_1     = 17;
const int Key_2     = 18;
const int Key_3     = 19;
const int Key_Sub   = 20;
const int Key_Inve  = 21;
const int Key_MP    = 22;
const int Key_0     = 23;
const int Key_SYM   = 24;
const int Key_Point = 25;
const int Key_Plus  = 26;
const int Key_Equ   = 27;
const int KeyPosLeft= 20;
const int KeyPosTop = 60;
const int KeyWidth1 = 50;
const int KeyWidth2 = 64;
const int KeyHeight = 30;
const int KeyClceH  = 4;
const int KeyClceV  = 6;
const int KeyCt     = 20;
const int OpeNULL   = 28;

const int ShowBarLeft   = KeyPosLeft+4;
const int ShowBarTop    = 10+4;
const int ShowBarWidth  = KeyWidth1*6 + KeyClceH*5 + KeyCt - 4*2 ;
const int ShowBarHeight = 30 - 4*2;

const int ClientWidth = KeyPosLeft*2 + KeyWidth1*6 + KeyClceH*5 + KeyCt;
const int ClientHeight= KeyPosTop + KeyHeight*5 + KeyClceV*4 ;
const int CalcW = ClientWidth + 10;
const int CalcH = ClientHeight + 60;

const int CalcMessage = 6;
const int NORMAL = 0;
const int OVERLARGE = 1;
const int OVERSMALL = 2;
const int FUN_UNDEF = 3;
const int GEN_ERROR = 4;
const int DIV_ZERO  = 5;

struct KeyData {
         char caption[10];
         WORD name;
         RECT pos;
       };

extern void RedrawWin( WINDOWS * wnd );
extern void ResizeWin( WINDOWS * wnd );
extern void CloseWin( void );
extern void MenuQuit( void );
extern void CalcKeyPress( void );

class classIB_Calc
  {
     private:
       char *CapcMenu1[ Menu1Num+1];
       void(*CapfMenu1[ Menu1Num+1])();
       char *CapcMenu2[ Menu2Num+1];
       void(*CapfMenu2[ Menu2Num+1])();
       char *EapcMenu1[ Menu1Num+1];
       void(*EapfMenu1[ Menu1Num+1])();
       char *EapcMenu2[ Menu2Num+1];
       void(*EapfMenu2[ Menu2Num+1])();

       static  BOOL RunFlag;

       MENUS CaMenu[ MenuNum+1];
       MENUS EaMenu[ MenuNum+1];
       WINDOWS * wnd;
       WINDOWS* win_about;
       KeyData  keyd[28];
       char ShowBuffer[50];
       char ShadowBuffer[50];
       char * EResultMess[CalcMessage];
       char * CResultMess[CalcMessage];
       char * ECaption;
       char * CCaption;
       char * CAboutText[3];
       char * EAboutText[3];
       char * CAboutTitle;
       char * EAboutTitle;
       char * CAboutBtn;
       char * EAboutBtn;

       int  PrvOperator;
       int  CurCharNum;
       int  CalcMessage;
       long double Memreg;
       long double Temreg;
       long double Curreg;
       long double MidregS;
       long double MidregT;
       long double Invereg1;
       long double Invereg2;

       BOOL MemFlag;
       BOOL TargOpeExist;
       BOOL InitBuffer;
       BOOL EquFlag;
       BOOL SysUpdate;
       BOOL BackEnable;
       BOOL PointExist;
       BOOL InveFlag1;
       BOOL Elanguage;

       void DefComponent( void );    // 界面元素定义过程
       void CloseCalc( void );
       void InitMenu( void );
       void InitMessageString( void );
       void InitButton( void );
       void InitCalc( void );
       void HideInit( void );
       int  Calculate( void );
       void KeyPress( int KeyNum );

       void UpdateShow ( void );
       BOOL AddChar( char );
       BOOL DelChar( void );
       void ClearMem( void );
       void StoreMem( void );
       void RestoreMem( void );
       void PlusMem( void );
       void CE( void );
       void C( void );
       void Sym( void );
       void Operators( int OpeKey );


       friend void UCcalc_RedrawWin( WINDOWS * wnd );
       friend void UCcalc_ResizeWin( WINDOWS * wnd );
       friend void UCcalc_draw_about(WINDOWS *wnd);
       friend void UCcalc_CloseWin( void );
       friend void UCcalc_CalcTimer( void );
       friend void UCcalc_MenuQuit( void );
       friend void UCcalc_AboutCalc( void );
       friend void UCcalc_AppExit( void );
       friend void UCcalc_AppNoExit( void );
       friend void UCcalc_SelectEnglish( void );
       friend void UCcalc_SelectChinese( void );
       friend void UCcalc_CalcKeyPress( void );
       friend int  UCcalc_CalcKeyBoardPress( int keycode, char keystate);

     public:
       classIB_Calc();
       ~classIB_Calc()  { };

       void Run();                            //运行计算器
       long double GetResult( void );         //提取计算结果
       void SetNumber( double groperator );   //设置计算数据
  };

#endif
