/**  *************  +/
      UC DirList
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

/********** ClassFileList Implementation *********/

#include <string.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>
#include "dirlist.h"

ADirList::ADirList(void (*CB_scroll)(SCROLLBAR *sbar, float prevcure),
                        void(*CB_sele)(void), AObject* parentclass)
  {
    CB_dirlist_scroll  = CB_scroll;
    CB_dirlist_sele    = CB_sele;
    lpDirNameArray     = NULL;
    lpCurDirNameArray  = NULL;
    DirCount           = 0;
    pParentClass        = parentclass;
    lpdirlistNull[0]   = new char[2];
    *(lpdirlistNull[0])= ' ';
    *(lpdirlistNull[0]+1)= '\0';
  }

ADirList::~ADirList()
  {
    delete lpdirlistNull[0];
    ResetContent();
  }

void ADirList::ResetContent()
  {
    ASTRING * lpDirNameTmp1;
    ASTRING * lpDirNameTmp2;

    lpDirNameTmp1 = lpDirNameTmp2= lpDirNameArray;

    while (lpDirNameTmp1!=NULL)
      {
        lpDirNameTmp1 = lpDirNameTmp1->lpNextStr;
        delete lpDirNameTmp2->lpStrItem;
        delete lpDirNameTmp2;
        lpDirNameTmp2 = lpDirNameTmp1;
      }
    lpDirNameArray     = NULL;
    lpCurDirNameArray  = NULL;
    DirCount           = 0;
  }

BOOL ADirList::Create( const LISTBOXRECT& rect, WINDOWS* pParentWnd, UINT nID)
  {
    dirlist_pParentWnd = pParentWnd;
    dirlist_ID = nID;
    ListHeight  = rect.height;
    dirlist_wnd = UC_DefineListBox( dirlist_pParentWnd,
                                     rect.left,rect.top,
                                     rect.width,
                                     rect.height,
                                     DirCount,
                                     CB_dirlist_scroll,
                                     CB_dirlist_sele,
                                     1 );
    return TRUE;
  }

int ADirList::AddString(LPCSTR lpszItem)
  {
    char * lpszDirName;

    try
      {
        lpszDirName = new char[_fstrlen(lpszItem)+1];
        _fmemmove( lpszDirName, lpszItem, _fstrlen(lpszItem)+1 );

        if(lpDirNameArray==NULL)
          {
            lpDirNameArray = new ASTRING;
            lpCurDirNameArray = lpDirNameArray;
          }
        else
          {
            lpCurDirNameArray->lpNextStr = new ASTRING;
            lpCurDirNameArray = lpCurDirNameArray->lpNextStr;
          }
      }
    catch (xalloc)
      {
        putchar('\007');
        nomem("No enough Memory !");
        return LB_ERRSPACE;
      }

    DirCount++;
    lpCurDirNameArray->lpStrItem = lpszDirName;
    lpCurDirNameArray->ItemNum   = DirCount;
    lpCurDirNameArray->lpNextStr = NULL;
    return DirCount;
  }

int ADirList::Dir(UINT attr, LPCSTR lpszWildCard)
  {
    struct find_t  list_dir;
    int wReturn;
    char pathname[MAXPATH];

    _fmemmove( pathname, lpszWildCard, _fstrlen(lpszWildCard)+1 );

    ResetContent();
    dirlist_wnd->sbar->cure = 0;
    UC_UpdateScrollbar( dirlist_pParentWnd, dirlist_wnd->sbar );

    do
      {
        DOS_CHECKDISK=0;
        DOS_CHECKRETERR=-1;
        _dos_findfirst( lpszWildCard, attr, &list_dir );
      }
    while(DOS_CHECKRETERR!=-1);

    if ((!_dos_findfirst( lpszWildCard, attr, &list_dir ))
          &&(list_dir.attrib == _A_SUBDIR)&&strcmp(list_dir.name,"."))
      if((wReturn=AddString((LPSTR)list_dir.name))<0)
        return wReturn;

    while(_dos_findnext( &list_dir ) == 0)
      if(list_dir.attrib == _A_SUBDIR)
        if((wReturn=AddString((LPSTR)list_dir.name))<0)
          return wReturn;

   dirlist_wnd->sbar->max=DirCount;
   UC_DisplayScrollbar( dirlist_pParentWnd, dirlist_wnd->sbar );
   UpdateShow(0);
   return wReturn;
  }

void ADirList::DirSelect()
  {
    ASTRING * lpDirNameTmp = lpDirNameArray;

    if(lpDirNameArray==NULL)
      return;

    while ((lpDirNameTmp!=NULL)&&((lpDirNameTmp->ItemNum-1)!=CurSelect))
      lpDirNameTmp = lpDirNameTmp->lpNextStr;

    _fmemmove( SeleDirName, lpDirNameTmp->lpStrItem,
               _fstrlen(lpDirNameTmp->lpStrItem)+1 );

    pParentClass->OnChange( TYPE_LISTBOX, dirlist_ID);
  }

void ADirList::GetDirName(LPSTR lpszBuffer) const
  {
    _fmemmove( lpszBuffer, SeleDirName, _fstrlen(SeleDirName)+1 );
  }

void ADirList::UpdateShow( int nSelect )
  {
    ASTRING * lpDirNameTmp = lpDirNameArray;
    int CurTop = (int)UC_GetBoxTopCount(dirlist_wnd);

    if(lpDirNameTmp==NULL)
      {
        dirlist_wnd->sbar->max = 1;
        UC_UpdateListBox( dirlist_pParentWnd, dirlist_wnd,
                          lpdirlistNull, 1 );
        return;
      }

    CurSelect = nSelect;
    while ((lpDirNameTmp!=NULL)&&((lpDirNameTmp->ItemNum-1)!=CurTop))
      lpDirNameTmp = lpDirNameTmp->lpNextStr;

    for(int us1=0;us1<ListHeight;us1++)
      {
        if(lpDirNameTmp==NULL)
          break;
        lpdirlist[us1] =  lpDirNameTmp->lpStrItem;
        lpDirNameTmp = lpDirNameTmp->lpNextStr;
      }

    UC_UpdateListBox( dirlist_pParentWnd, dirlist_wnd,
                      lpdirlist, us1 );
  }
