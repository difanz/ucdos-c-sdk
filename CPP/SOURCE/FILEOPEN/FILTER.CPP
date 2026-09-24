/**  *************  +/
      UC Filter
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

/********** ClassFilter Implementation *********/

#include <string.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>
#include "filter.h"

AFilter::AFilter(void (*CB_scroll)(SCROLLBAR *sbar, float prevcure),
                        void(*CB_sele)(void), char * strFilter,
                        AObject* parentclass)
  {
    CB_filter_scroll        = CB_scroll;
    CB_filter_sele          = CB_sele;
    pszFilter               = strFilter;
    lpFilterArray           = NULL;
    lpCurFilterArray        = NULL;
    lpFilterNameArray       = NULL;
    lpCurFilterNameArray    = NULL;
    FilterCount             = 0;
    pParentClass            = parentclass;
    lpfilterlistNull[0]     = new char[2];
    *(lpfilterlistNull[0])  = ' ';
    *(lpfilterlistNull[0]+1)= '\0';
    InitFilter();
    _fmemmove( SeleFileName, lpFilterArray->lpStrItem,
               _fstrlen(lpFilterArray->lpStrItem)+1 );
    _fmemmove( SeleFilterName, lpFilterNameArray->lpStrItem,
               _fstrlen(lpFilterNameArray->lpStrItem)+1 );

  }

AFilter::~AFilter()
  {
    delete lpfilterlistNull[0];
    ResetContent();
  }

void AFilter::ResetContent()
  {
    ASTRING * lpFilterNameTmp1;
    ASTRING * lpFilterNameTmp2;

    lpFilterNameTmp1 = lpFilterNameTmp2= lpFilterNameArray;
    while (lpFilterNameTmp1!=NULL)
      {
        lpFilterNameTmp1 = lpFilterNameTmp1->lpNextStr;
        delete lpFilterNameTmp2->lpStrItem;
        delete lpFilterNameTmp2;
        lpFilterNameTmp2 = lpFilterNameTmp1;
      }

    lpFilterNameTmp1 = lpFilterNameTmp2= lpFilterArray;
    while (lpFilterNameTmp1!=NULL)
      {
        lpFilterNameTmp1 = lpFilterNameTmp1->lpNextStr;
        delete lpFilterNameTmp2->lpStrItem;
        delete lpFilterNameTmp2;
        lpFilterNameTmp2 = lpFilterNameTmp1;
      }

    lpFilterArray          = NULL;
    lpCurFilterArray       = NULL;
    lpFilterNameArray      = NULL;
    lpCurFilterNameArray   = NULL;
    FilterCount            = 0;
  }

BOOL AFilter::Create( const LISTBOXRECT& rect, WINDOWS* pParentWnd, UINT nID)
  {
    filter_pParentWnd = pParentWnd;
    filter_ID = nID;
    ListHeight  = rect.height;

    filter_wnd = UC_DefinePopBox( filter_pParentWnd,
                                  rect.left,rect.top,
                                  rect.width,
                                  rect.height,
                                  FilterCount,
                                  SeleFilterName,
                                  CB_filter_scroll,
                                  CB_filter_sele );

    pParentClass->OnChange( TYPE_COMBOBOX, filter_ID);
    return TRUE;
  }

int AFilter::AddString(LPCSTR lpszItem)
  {
    char * lpszFilterName;

    try
      {
        lpszFilterName = new char[_fstrlen(lpszItem)+1];
        _fmemmove( lpszFilterName, lpszItem, _fstrlen(lpszItem)+1 );

        if(lpFilterNameArray==NULL)
          {
            lpFilterNameArray = new ASTRING;
            lpCurFilterNameArray = lpFilterNameArray;
          }
        else
          {
            lpCurFilterNameArray->lpNextStr = new ASTRING;
            lpCurFilterNameArray = lpCurFilterNameArray->lpNextStr;
          }
      }
    catch (xalloc)
      {
        putchar('\007');
        nomem("No enough Memory !");
        return LB_ERRSPACE;
      }

    FilterCount++;
    lpCurFilterNameArray->lpStrItem = lpszFilterName;
    lpCurFilterNameArray->ItemNum   = FilterCount;
    lpCurFilterNameArray->lpNextStr = NULL;
    return FilterCount;
  }

int AFilter::InitFilterItem(LPCSTR lpszItem)
  {
    char * lpszFilter;

    try
      {
        lpszFilter = new char[_fstrlen(lpszItem)+1];
        _fmemmove( lpszFilter, lpszItem, _fstrlen(lpszItem)+1 );

        if(lpFilterArray==NULL)
          {
            lpFilterArray = new ASTRING;
            lpCurFilterArray = lpFilterArray;
          }
        else
          {
            lpCurFilterArray->lpNextStr = new ASTRING;
            lpCurFilterArray = lpCurFilterArray->lpNextStr;
          }
      }
    catch (xalloc)
      {
        putchar('\007');
        nomem("No enough Memory !");
        return LB_ERRSPACE;
      }

    lpCurFilterArray->lpStrItem = lpszFilter;
    lpCurFilterArray->ItemNum   = FilterCount;
    lpCurFilterArray->lpNextStr = NULL;
    return FilterCount;
  }

int AFilter::InitFilter( void )
  {
    int  wReturn = 0;
    int  if1 = 0;
    BOOL ChangeFlag = FALSE;
    char filterstr[40];

    ResetContent();

    while(*pszFilter!='|')
      {
        while(*pszFilter!='|')
          filterstr[if1++] = *pszFilter++;

        filterstr[if1] = '\0';
        if1 = 0;
        pszFilter++;

        ChangeFlag = !ChangeFlag;
        if(ChangeFlag)
          {
            if((wReturn=AddString((LPSTR)filterstr))<0)
              return wReturn;
          }
        else
          {
            if((wReturn=InitFilterItem((LPSTR)filterstr))<0)
              return wReturn;
          }
      }

   return wReturn;
  }

void AFilter::FilterSelect()
  {
    ASTRING * lpFilterNameTmp = lpFilterNameArray;
    ASTRING * lpFilterTmp     = lpFilterArray;

    if(lpFilterNameArray==NULL)
      return;

    while ((lpFilterNameTmp!=NULL)&&((lpFilterNameTmp->ItemNum-1)!=CurSelect))
      {
        lpFilterNameTmp = lpFilterNameTmp->lpNextStr;
        lpFilterTmp = lpFilterTmp->lpNextStr;
      }

    _fmemmove( SeleFileName, lpFilterTmp->lpStrItem,
               _fstrlen(lpFilterTmp->lpStrItem)+1 );

    UC_UpdateInputLine(filter_pParentWnd,
                       filter_wnd,
                       lpFilterNameTmp->lpStrItem);

    pParentClass->OnChange( TYPE_COMBOBOX, filter_ID);
  }

void AFilter::GetFileName(LPSTR lpszBuffer) const
  {
    _fmemmove( lpszBuffer, SeleFileName, _fstrlen(SeleFileName)+1 );
  }

void AFilter::UpdateShow( int nSelect )
  {
    ASTRING * lpFilterNameTmp = lpFilterNameArray;
    int CurTop = (int)UC_GetBoxTopCount(filter_wnd->box);

    if(lpFilterNameTmp==NULL)
      {
        filter_wnd->box->sbar->max = 1;
        UC_UpdateListBox( filter_pParentWnd, filter_wnd->box,
                          lpfilterlistNull, 1 );
        return;
      }

    CurSelect = nSelect;
    while ((lpFilterNameTmp!=NULL)&&((lpFilterNameTmp->ItemNum-1)!=CurTop))
      lpFilterNameTmp = lpFilterNameTmp->lpNextStr;

    for(int us1=0;us1<ListHeight;us1++)
      {
        if(lpFilterNameTmp==NULL)
          break;
        lpfilterlist[us1] =  lpFilterNameTmp->lpStrItem;
        lpFilterNameTmp = lpFilterNameTmp->lpNextStr;
      }
    filter_wnd->box->sbar->max=FilterCount;
    UC_UpdateListBox( filter_pParentWnd, filter_wnd->box,
                      lpfilterlist, us1 );
  }
