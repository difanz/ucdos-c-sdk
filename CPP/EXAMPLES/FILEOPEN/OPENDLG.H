/**  *************  +/
      UC OpenDlg
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

#if __BCPLUSPLUS__< 0x0340
#error Use Borland C++ 4.5 or later releases mode to compile FileOpen Module!
#endif

#ifndef SDK_OPENDLG_CLASS
#define SDK_OPENDLG_CLASS

#include <stdlib.h>
#include <dir.h>
#include "sdk.h"
#include "edit.h"
#include "filelist.h"
#include "filter.h"
#include "dirlist.h"
#include "drives.h"
#include "static.h"


const int OpenDlgW = 480;
const int OpenDlgH = 300;

class classOpenDlg:AObject
  {
    private:
      WINDOWS *  ParentWnd;
      WINDOWS *  open_wnd;
      BUTTON *   open_ok_wnd;
      BUTTON *   open_cancel_wnd;
      AEdit   *  ODfileedit;
      AFileList * ODfilelist;
      AFilter *  ODfilter;
      ADirList * ODdirlist;
      ADrives *  ODdrives;
      AStatic *  ODstatic1;
      AStatic *  ODstatic2;
      AStatic *  ODstatic3;
      AStatic *  ODstatic4;
      AStatic *  ODstatic5;

      BOOL SysUpdate;
      BOOL OpenFileDialog;

    public:
      // Attributes
      // open file parameter block
      OPENFILENAME a_ofn;             //保留未使用

      BOOL a_ofFileMustExist;         //选择文件是否必须存在标置
      UINT file_attr;                 //选择文件列表框中文件的属性
                                      //属性值同 MS-DOS 文件的属性
      UINT dir_attr;                  //选择目录列表框中目录的属性
                                      //属性值同 MS-DOS 目录的属性
      LPSTR DlgTitle;                 //对话框标题字符串指针
                                      //用户可自定义对话框标题

      // Constructors
      classOpenDlg( BOOL bOpenFileDialog = TRUE,    //对话框类型
                    LPCSTR lpszDefExt = NULL,       //保留未使用
                    LPCSTR lpszFileName = NULL,     //保留未使用
                    DWORD dwFlags = NULL,           //保留未使用
                    LPCSTR lpszFilter = NULL,       //文件名过滤器
                    WINDOWS * pParentWnd = NULL);   //父窗口指针

                    // bOpenFileDialog 值为
                    // TRUE: File Open; FALSE: File SaveAs;

                    // 文件名过滤器用法同 WINDOWS FileOpen 对话框,
                    // 但只支持每项单个扩展名

      ~classOpenDlg();
      // Operations
      virtual int DoModal();      //显示对话框并允许用户作出选择

      // Helpers for parsing file name after successful return
      LPSTR GetPathName() const;  //返回选择文件的完全路径
      LPSTR GetFileName() const;  //返回选择文件的文件名
      LPSTR GetFileExt() const;   //返回选择文件的扩展名
      LPSTR GetFileTitle() const; //返回选择文件的标题


    protected:
      virtual void OnOK();
      virtual void OnCancel();

      virtual void OnChange(UINT Type, UINT IDcom);

      LPSTR DlgTitleSour;

      char* a_strFilter;          // filter string
                                  // separate fields with '|',
                                  // terminate with '||\0'
      char a_szFileTitle[64];     // contains file title after return
      char a_szFileName[MAXPATH]; // contains full path name after return

      char cur_path[MAXPATH];
      char old_path[MAXPATH];
      int  cur_drive;

      char a_drive[MAXDRIVE];
      char a_dir[MAXDIR];
      char a_file[MAXFILE];
      char a_ext[MAXEXT];

      int  Message;

      void DefComponent( void );    // 界面元素定义过程
      void EnterApp( void );        // 应用主体

      void InitOpenDlg( void );
      void UpdateShow( void );
      BOOL FinalProcess( void );

      void ChDirect();

      friend void IB_RedrawWin( WINDOWS * wnd );
      friend void IB_ResizeWin( WINDOWS * wnd );
      friend void IB_CloseWin( void );

      friend void CB_Open_OK( void );
      friend void CB_Open_Cancel( void );

      friend void CB_filelist_scroll( SCROLLBAR *sbar, float prevcure );
      friend void CB_filelist_sele( void );

      friend void CB_fileedit_sele( void );

      friend void CB_filter_scroll( SCROLLBAR *sbar, float prevcure );
      friend void CB_filter_sele( void );

      friend void CB_drives_scroll(SCROLLBAR *sbar, float prevcure);
      friend void CB_drives_sele(void);

      friend void CB_dirlist_scroll( SCROLLBAR *sbar, float prevcure );
      friend void CB_dirlist_sele( void );
  };

#endif
