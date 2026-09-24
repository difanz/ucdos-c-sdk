/**  *************  +/
      UC OpenDlg
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

#include "opendlg.h"
#include <stdio.h>

extern classOpenDlg* ibOpenDlg;

void IB_RedrawWin( WINDOWS * /*wnd*/)
  {
    ibOpenDlg->SysUpdate = TRUE;
    ibOpenDlg->UpdateShow();
  }

void IB_ResizeWin( WINDOWS * /*wnd*/ )
  {
  }

void IB_CloseWin( void )
  {
    ibOpenDlg->OnCancel();
  }

void CB_Open_OK( void )
  {
    ibOpenDlg->OnOK();
  }

void CB_Open_Cancel( void )
  {
    ibOpenDlg->OnCancel();
  }

/* ********  filelist CallBack *********/

void CB_filelist_scroll( SCROLLBAR *sbar, float /*prevcure*/ )
  {
    ibOpenDlg->ODfilelist->UpdateShow( (int)(sbar->cure) );
  }

void CB_filelist_sele( void )
  {
    ibOpenDlg->ODfilelist->FileSelect();
  }

/* ********  fileedit CallBack *********/

void CB_fileedit_sele( void )
  {
    ibOpenDlg->ODfileedit->FileNameSelect();
  }

/* ********  filter CallBack *********/

void CB_filter_scroll( SCROLLBAR *sbar, float /*prevcure*/ )
  {
    ibOpenDlg->ODfilter->UpdateShow( (int)(sbar->cure) );
  }

void CB_filter_sele( void )
  {
    ibOpenDlg->ODfilter->FilterSelect();
  }

/* ********  drives CallBack *********/

void CB_drives_scroll(SCROLLBAR *sbar, float /*prevcure*/)
  {
    ibOpenDlg->ODdrives->UpdateShow( (int)(sbar->cure) );
  }

void CB_drives_sele(void)
  {
    ibOpenDlg->ODdrives->DrivesSelect();
  }

/* ********  dirlist CallBack *********/

void CB_dirlist_scroll(SCROLLBAR *sbar, float /*prevcure*/)
  {
    ibOpenDlg->ODdirlist->UpdateShow( (int)(sbar->cure) );
  }

void CB_dirlist_sele(void)
  {
    ibOpenDlg->ODdirlist->DirSelect();
  }

