/**  *************  +/
      UC FileList
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

/********** ClassFileList Implementation *********/

#include <string.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>
#include "filelist.h"

AFileList::AFileList(void (*CB_scroll)(SCROLLBAR *sbar, float prevcure),
                        void(*CB_sele)(void), AObject* parentclass)
  {
    CB_filelist_scroll  = CB_scroll;
    CB_filelist_sele    = CB_sele;
    lpFileNameArray     = NULL;
    lpCurFileNameArray  = NULL;
    FileCount           = 0;
    pParentClass        = parentclass;
    lpfilelistNull[0]   = new char[2];
    *(lpfilelistNull[0])= ' ';
    *(lpfilelistNull[0]+1)= '\0';
  }

AFileList::~AFileList()
  {
    delete lpfilelistNull[0];
    ResetContent();
  }

void AFileList::ResetContent()
  {
    ASTRING * lpFileNameTmp1;
    ASTRING * lpFileNameTmp2;

    lpFileNameTmp1 = lpFileNameTmp2= lpFileNameArray;

    while (lpFileNameTmp1!=NULL)
      {
        lpFileNameTmp1 = lpFileNameTmp1->lpNextStr;
        delete lpFileNameTmp2->lpStrItem;
        delete lpFileNameTmp2;
        lpFileNameTmp2 = lpFileNameTmp1;
      }
    lpFileNameArray     = NULL;
    lpCurFileNameArray  = NULL;
    FileCount           = 0;
  }

BOOL AFileList::Create( const LISTBOXRECT& rect, WINDOWS* pParentWnd, UINT nID)
  {
    filelist_pParentWnd = pParentWnd;
    filelist_ID = nID;
    ListHeight  = rect.height;
    filelist_wnd = UC_DefineListBox( filelist_pParentWnd,
                                     rect.left,rect.top,
                                     rect.width,
                                     rect.height,
                                     FileCount,
                                     CB_filelist_scroll,
                                     CB_filelist_sele,
                                     1 );
    return TRUE;
  }

int AFileList::AddString(LPCSTR lpszItem)
  {
    char * lpszFileName;

    try
      {
        lpszFileName = new char[_fstrlen(lpszItem)+1];
        _fmemmove( lpszFileName, lpszItem, _fstrlen(lpszItem)+1 );

        if(lpFileNameArray==NULL)
          {
            lpFileNameArray = new ASTRING;
            lpCurFileNameArray = lpFileNameArray;
          }
        else
          {
            lpCurFileNameArray->lpNextStr = new ASTRING;
            lpCurFileNameArray = lpCurFileNameArray->lpNextStr;
          }
      }
    catch (xalloc)
      {
        putchar('\007');
        nomem("No enough Memory !");
        return LB_ERRSPACE;
      }

    FileCount++;
    lpCurFileNameArray->lpStrItem = lpszFileName;
    lpCurFileNameArray->ItemNum   = FileCount;
    lpCurFileNameArray->lpNextStr = NULL;
    return FileCount;
  }

int AFileList::Dir(UINT attr, LPCSTR lpszWildCard)
  {
    struct find_t  list_file;
    int wReturn;
    char pathname[MAXPATH];

    _fmemmove( pathname, lpszWildCard, _fstrlen(lpszWildCard)+1 );

    ResetContent();
    filelist_wnd->sbar->cure = 0;
    UC_UpdateScrollbar( filelist_pParentWnd, filelist_wnd->sbar );

    do
      {
        DOS_CHECKDISK=0;
        DOS_CHECKRETERR=-1;
        _dos_findfirst( lpszWildCard, attr, &list_file );
      }
    while(DOS_CHECKRETERR!=-1);

    if (!_dos_findfirst( lpszWildCard, attr, &list_file ))
      if((wReturn=AddString((LPSTR)list_file.name))<0)
        return wReturn;

    while( _dos_findnext( &list_file ) == 0 )
      if((wReturn=AddString((LPSTR)list_file.name))<0)
        return wReturn;

   filelist_wnd->sbar->max=FileCount;
   UC_DisplayScrollbar( filelist_pParentWnd, filelist_wnd->sbar );

   UpdateShow(0);
   return wReturn;
  }

void AFileList::FileSelect()
  {
    ASTRING * lpFileNameTmp = lpFileNameArray;

    if(lpFileNameArray==NULL)
      return;

    while ((lpFileNameTmp!=NULL)&&((lpFileNameTmp->ItemNum-1)!=CurSelect))
      lpFileNameTmp = lpFileNameTmp->lpNextStr;

    _fmemmove( SeleFileName, lpFileNameTmp->lpStrItem,
               _fstrlen(lpFileNameTmp->lpStrItem)+1 );
    pParentClass->OnChange( TYPE_LISTBOX, filelist_ID);
  }

void AFileList::GetFileName(LPSTR lpszBuffer) const
  {
    _fmemmove( lpszBuffer, SeleFileName, _fstrlen(SeleFileName)+1 );
  }

void AFileList::UpdateShow( int nSelect )
  {
    ASTRING * lpFileNameTmp = lpFileNameArray;
    int CurTop = (int)UC_GetBoxTopCount(filelist_wnd);

    if(lpFileNameTmp==NULL)
      {
        filelist_wnd->sbar->max = 1;
        UC_UpdateListBox( filelist_pParentWnd, filelist_wnd,
                          lpfilelistNull, 1 );
        return;
      }

    CurSelect = nSelect;
    while ((lpFileNameTmp!=NULL)&&((lpFileNameTmp->ItemNum-1)!=CurTop))
      lpFileNameTmp = lpFileNameTmp->lpNextStr;

    for(int us1=0;us1<ListHeight;us1++)
      {
        if(lpFileNameTmp==NULL)
          break;
        lpfilelist[us1] =  lpFileNameTmp->lpStrItem;
        lpFileNameTmp = lpFileNameTmp->lpNextStr;
      }

    UC_UpdateListBox( filelist_pParentWnd, filelist_wnd,
                      lpfilelist, us1 );
  }
