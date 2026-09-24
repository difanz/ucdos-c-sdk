/**  *************  +/
      UC OpenDlg
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

#include "opendlg.h"
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <dos.h>

/********** ClassOpenDlg Implementation *********/

extern unsigned _stklen = 0x4000;

classOpenDlg::classOpenDlg( BOOL   bOpenFileDialog,
                            LPCSTR lpszDefExt,
                            LPCSTR lpszFileName,
                            DWORD  /*dwFlags*/,
                            LPCSTR lpszFilter,
                            WINDOWS * pParentWnd)
  {
    char defaultFilter[20] = {"All files(*.*)|*.*||"};
    char defaultTitle1[] = {"File Open"};
    char defaultTitle2[] = {"File SaveAs"};

    ParentWnd = pParentWnd;
    OpenFileDialog = bOpenFileDialog;
    a_ofn.lpstrInitialDir = NULL;
    a_ofFileMustExist = TRUE;
    file_attr = _A_NORMAL;
    dir_attr  = _A_SUBDIR|_A_NORMAL;

    if(OpenFileDialog)
      {
        DlgTitleSour = new char[_fstrlen(defaultTitle1)+1];
        _fmemmove( DlgTitleSour, defaultTitle1, _fstrlen(defaultTitle1)+1 );
      }
    else
      {
        DlgTitleSour = new char[_fstrlen(defaultTitle2)+1];
        _fmemmove( DlgTitleSour, defaultTitle2, _fstrlen(defaultTitle2)+1 );
      }

    DlgTitle = DlgTitleSour;

    if(lpszFilter!=NULL)
      {
        a_strFilter = new char[_fstrlen(lpszFilter)+1];
        _fmemmove( a_strFilter, lpszFilter, _fstrlen(lpszFilter)+1 );
      }
    else
      {
        a_strFilter = new char[_fstrlen(defaultFilter)+1];
        _fmemmove( a_strFilter, defaultFilter, _fstrlen(defaultFilter)+1 );
      }

    if(lpszFileName!=NULL)
      _fmemmove( a_szFileTitle, lpszFileName, _fstrlen(lpszFileName)+1 );
    else
      a_szFileTitle[0] = '\0';

    if(lpszDefExt!=NULL)
      _fmemmove( a_ext, lpszDefExt, _fstrlen(lpszDefExt)+1 );

    a_szFileName[0] = '\0';
  }

int classOpenDlg::DoModal()
  {
    InitOpenDlg();
    DefComponent();          // 调用自己的屏幕元素定义
    EnterApp();             // 进入自己的应用
    UC_WindowClose();
    return Message;
  }

classOpenDlg::~classOpenDlg()
  {
    delete DlgTitleSour;
    delete a_strFilter;

    delete ODstatic5;
    delete ODstatic4;
    delete ODstatic3;
    delete ODstatic2;
    delete ODstatic1;
    delete ODdrives;
    delete ODdirlist;
    delete ODfilter;
    delete ODfilelist;
    delete ODfileedit;

    setdisk(old_path[0]-'A');
    chdir(old_path);

  }

void classOpenDlg::OnChange( UINT Type, UINT IDcom)
  {
    if((Type==TYPE_LISTBOX)&&(IDcom==1))
      {
        char FileNameTmp[MAXFILE+MAXEXT+1];
        ODfilelist->GetFileName(FileNameTmp);
        ODfileedit->SetText(FileNameTmp);
      }
    if((Type==TYPE_INPUTLINE)&&(IDcom==2))
      {
        char PathNameTmp1[MAXPATH+20];
        char PathNameTmp2[MAXPATH+20];
        char FileNameTmp[MAXFILE+MAXEXT+2];

        ODfileedit->GetText(PathNameTmp1);
        strupr(PathNameTmp1);
        strcpy(PathNameTmp2,PathNameTmp1);

        for(int i=strlen(PathNameTmp1);i>0;i--)
          if((PathNameTmp1[i-1]=='\\')||(PathNameTmp1[i-1]==':'))
            {
              PathNameTmp2[i] = '\0';
              strcat( PathNameTmp2, "*.*");

              if(PathNameTmp2[1]==':')
                if(!ODdrives->DetectDisk(PathNameTmp2[0]-'A'))
                  break;

              DOS_CHECKDISK=1;
              DOS_CHECKRETERR=-1;

              struct find_t  list;
              if(_dos_findfirst( PathNameTmp2, _A_NORMAL|_A_RDONLY|_A_HIDDEN
                               |_A_SYSTEM|_A_VOLID|_A_SUBDIR|_A_ARCH,&list)!=0)
                break;

              if(PathNameTmp2[1]==':')
                {
                  int disk = PathNameTmp2[0]-'A';
                  ODdrives->SetDisk(disk);
                  setdisk(disk);
                }

              if(i>3)
                PathNameTmp2[i-1] = '\0';
              else
                PathNameTmp2[i] = '\0';

              chdir(PathNameTmp2);
              ChDirect();

              break;
            }

        memmove( FileNameTmp, PathNameTmp1+i, strlen(PathNameTmp1)-i+1 );
        ODfileedit->SetText(FileNameTmp);
        ODfilelist->Dir( file_attr, FileNameTmp );
      }
    if((Type==TYPE_COMBOBOX)&&(IDcom==3))
      {
        char FileNameTmp[MAXFILE+MAXEXT+1];
        ODfilter->GetFileName(FileNameTmp);
        ODfileedit->SetText(FileNameTmp);
        ODfilelist->Dir( file_attr, FileNameTmp );
      }
    if((Type==TYPE_LISTBOX)&&(IDcom==4))
      {
        {
          char dir[MAXPATH+5];

          ODdirlist->GetDirName(dir);
          chdir(dir);
        }

        char FileNameTmp[MAXFILE+MAXEXT+1];
        ODfileedit->GetText(FileNameTmp);
        ODfilelist->Dir( file_attr, FileNameTmp );

        ChDirect();
      }

    if((Type==TYPE_COMBOBOX)&&(IDcom==5))
      {

        char DriveTmp[MAXDRIVE];

        DOS_CHECKDISK=0;
        DOS_CHECKRETERR=-1;
        int disk = getdisk();
        ODdrives->GetDrivesName(DriveTmp);

        setdisk(DriveTmp[0]-'A');

        struct find_t  list;
        _dos_findfirst( "*.*", _A_NORMAL, &list);
        if (DOS_CHECKRETERR!=-1)
          {
            UC_RedrawCurrentWindow();
            setdisk(disk);
            do
              {
                DOS_CHECKDISK=0;
                DOS_CHECKRETERR=-1;
                _dos_findfirst( "*.*", _A_NORMAL, &list);
              }
            while(DOS_CHECKRETERR!=-1);
            ODdrives->SetDisk(disk);
            return;
          }

        char FileNameTmp[MAXFILE+MAXEXT+1];
        ODfileedit->GetText(FileNameTmp);
        ODfilelist->Dir( file_attr, FileNameTmp );

        ChDirect();
      }
  }

void classOpenDlg::ChDirect( void )
  {
    char dir[MAXPATH+5];

    getcwd(cur_path, MAXPATH);
    char text[20];
    memmove( text, cur_path, 19 );
    text[19] = '\0';
    if(strlen(cur_path)>19)
      {
        for(int tp=18, cp=strlen(cur_path)-1;cp>6;cp--,tp--)
          if(cur_path[cp]!='\\')
            text[tp] = cur_path[cp];
          else
            break;
        memmove( text+tp-3, "...\\", 4 );
      }
    ODstatic4->SetText(text);

    strcpy( dir, cur_path );
    if(dir[strlen(dir)-1]=='\\')
      dir[strlen(dir)-1] = '\0';
    strcat( dir, "\\*.*" );
    ODdirlist->Dir( dir_attr, dir);
  }

BOOL classOpenDlg::FinalProcess( void )
  {
    getcwd(cur_path, MAXPATH);

    strcpy( a_szFileName, cur_path );
    if(a_szFileName[strlen(a_szFileName)-1]!='\\')
      {
        int sl = strlen(a_szFileName);
        a_szFileName[sl] = '\\';
        a_szFileName[sl+1] = '\0';
      }
    char FileNameTmp[MAXFILE+MAXEXT+1];
    ODfileedit->GetText(FileNameTmp);
    strcat( a_szFileName, FileNameTmp );
    strcpy( a_szFileTitle, FileNameTmp );

    int flags=fnsplit( a_szFileName,a_drive, a_dir, a_file, a_ext);

    if((!OpenFileDialog)||a_ofFileMustExist)
      {
        int handle;
        int openflag = _dos_open(a_szFileTitle, O_RDONLY, &handle);
        switch (openflag)
          {
            case EACCES   :                //Permission denied
            case EINVACC :               //Invalid access code
            case EMFILE   :               //Too many open files
            case ENOENT   :               //Path or file not found
            default      :
                           _dos_close(handle);
                           return FALSE;
            case 0       :
                           break;
          }

        _dos_close(handle);
      }

    if(!(flags & FILENAME))
      a_file[0] = '\0';

    if(!(flags & EXTENSION))
      a_ext[0] = '\0';
    return TRUE;
  }

void classOpenDlg::InitOpenDlg( void )
  {
    getcwd(old_path, MAXPATH);
    if(a_ofn.lpstrInitialDir!=NULL)
      if (chdir(a_ofn.lpstrInitialDir))
        chdir(old_path);

    getcwd(cur_path, MAXPATH);
    cur_drive = getdisk() + 1;
  }

void classOpenDlg::UpdateShow( void )
  {
  }

// 界面元素定义过程
void classOpenDlg::DefComponent( void )
  {
    LISTBOXRECT LBrect = { 20,60, 17, 8 };
    POINT Spoint;
    NO_EXITWIN = 1;
    open_wnd=UC_DefineWindow( WS_NOSIZE,
                              CP_MIDSCR,
                              CP_MIDSCR,
                              OpenDlgW,
                              OpenDlgH,
                              DlgTitle,
                              IB_RedrawWin,
                              IB_CloseWin,
                              IB_ResizeWin );

    open_ok_wnd = UC_DefinePressButton( open_wnd,
                                        CB_Open_OK,
                                        375, 20,
                                        80,  25,
                                        "确定",
                                        0);

    open_cancel_wnd = UC_DefinePressButton( open_wnd,
                                            CB_Open_Cancel,
                                            375, 60,
                                            80,  25,
                                            "取消",
                                            0);

    ODfilelist = new AFileList( CB_filelist_scroll, CB_filelist_sele, this );
    ODfilelist->Create( LBrect, open_wnd, 1 );

    ODfileedit = new AEdit( CB_fileedit_sele, NULL, this);
    Spoint.x = 20;
    Spoint.y = 30;
    ODfileedit->Create( Spoint, open_wnd, 2);

    LBrect.top  = 230;
    LBrect.height = 3;
    ODfilter = new AFilter( CB_filter_scroll, CB_filter_sele,
                            a_strFilter, this );
    ODfilter->Create( LBrect, open_wnd, 3 );

    LBrect.left = 200;
    LBrect.top  = 60;
    LBrect.height = 8;
    ODdirlist = new ADirList( CB_dirlist_scroll, CB_dirlist_sele, this );
    ODdirlist->Create( LBrect, open_wnd, 4 );
    char dir[MAXPATH+5];
    strcpy( dir, cur_path );
    if(dir[strlen(dir)-1]=='\\')
      dir[strlen(dir)-1] = '\0';
    strcat( dir, "\\*.*" );
    ODdirlist->Dir( dir_attr, dir);

    LBrect.left = 200;
    LBrect.top  = 230;
    LBrect.height = 3;
    ODdrives = new ADrives( CB_drives_scroll, CB_drives_sele, this );
    ODdrives->Create( LBrect, open_wnd, cur_drive, 5 );

    ODstatic1 = new AStatic("文件名:",strlen("文件名:"));
    Spoint.x = 20;
    Spoint.y = 12;
    ODstatic1->Create( Spoint, open_wnd, 2);

    ODstatic2 = new AStatic("列出以下类型文件:",strlen("列出以下类型文件:"));
    Spoint.x = 20;
    Spoint.y = 210;
    ODstatic2->Create( Spoint, open_wnd, 3);

    ODstatic3 = new AStatic("目录:",strlen("目录:"));
    Spoint.x = 200;
    Spoint.y = 12;
    ODstatic3->Create( Spoint, open_wnd, 2);

    getcwd(cur_path, MAXPATH);
    char text[20];
    memmove( text, cur_path, 19 );
    text[19] = '\0';
    if(strlen(cur_path)>19)
      {
        for(int tp=18, cp=strlen(cur_path)-1;cp>6;cp--,tp--)
          if(cur_path[cp]!='\\')
            text[tp] = cur_path[cp];
          else
            break;
        memmove( text+tp-3, "...\\", 4 );
      }

    ODstatic4 = new AStatic(text, 19);
    Spoint.x = 200;
    Spoint.y = 30;
    ODstatic4->Create( Spoint, open_wnd, 2);

    ODstatic5 = new AStatic("驱动器:",strlen("驱动器:"));
    Spoint.x = 200;
    Spoint.y = 210;
    ODstatic5->Create( Spoint, open_wnd, 2);

    UC_DefineActive( open_wnd, TYPE_INPUTLINE, 1 );
    UC_WindowEnable( open_wnd );                          // 显示定义好的窗口
  }

// 应用主体
void classOpenDlg::EnterApp( void )
  {
    while(1)
      {
        UC_WindowsCentral();

        Message = UC_GetMessage();
        if (Message==ID_OK)
          {
            if(OpenFileDialog)
              {
                if(FinalProcess())
                  return;
                else
                  UC_DialogWarning(NULL, NULL,
                    "%s\n不能够找到该文件\n\n请检查给出文件名及其路径的正确性.",
                    a_szFileTitle);
              }
            else
              {
                if(FinalProcess())
                  {
                    if(ID_YES==UC_DialogYesNo(NULL, NULL, NULL,
                                              "%s\n该文件已存在,是否要覆盖?",
                                              a_szFileTitle))
                      return;
                  }
                else
                  return;
              }
          }
        if (Message==ID_CANCEL)
         return;
      }
  }

LPSTR classOpenDlg::GetPathName() const  // return full path name
  {
    return ((LPSTR)a_szFileName);
  }

LPSTR classOpenDlg::GetFileName() const  // return only filename
  {
    return ((LPSTR)a_file);
  }

LPSTR classOpenDlg::GetFileExt() const   // return only ext
  {
    return ((LPSTR)a_ext);
  }

LPSTR classOpenDlg::GetFileTitle() const // return file title
  {
    return ((LPSTR)a_szFileTitle);
  }

void classOpenDlg::OnOK()
  {
    UC_SetMessage(ID_OK);
  }

void classOpenDlg::OnCancel()
  {
    UC_SetMessage(ID_CANCEL);
  }

