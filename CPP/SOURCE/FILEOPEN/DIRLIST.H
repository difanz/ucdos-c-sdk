/**  *************  +/
      UC DirList
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

#if __BCPLUSPLUS__< 0x0340
#error Use Borland C++ 4.5 or later releases mode to compile FileOpen Module!
#endif

#ifndef SDK_DIRLIST_CLASS
#define SDK_DIRLIST_CLASS

#include <stdlib.h>
#include <except.h>
#include "aocp_def.h"
#include "object.h"
#include "sdk.h"


class ADirList:AObject
  {
    private:
      AObject * pParentClass;
      LISTBOX * dirlist_wnd;
      WINDOWS * dirlist_pParentWnd;
      SCROLLBAR * dirlist_sbar;
      void (*CB_dirlist_scroll)(SCROLLBAR *sbar, float prevcure);
      void (*CB_dirlist_sele)(void);
      ASTRING * lpDirNameArray;
      ASTRING * lpCurDirNameArray;
      LPSTR  lpdirlist[10];
      LPSTR  lpdirlistNull[1];
      char   SeleDirName[MAXFILE+MAXEXT+1];
      UINT dirlist_ID;
      int  ListHeight;
      int  CurSelect;
      int  DirCount;

      void DirSelect();
      void UpdateShow( int nSelect );

    public:
      ADirList(void (*CB_scroll)(SCROLLBAR *sbar, float prevcure),
                void(*CB_sele)(void), AObject* parentclass);
      ~ADirList();
      virtual void OnChange( UINT Type, UINT IDcom) { };

      BOOL Create( const LISTBOXRECT& rect, WINDOWS* pParentWnd, UINT nID);

      int AddString(LPCSTR lpszItem);
      void ResetContent();

      void GetDirName(LPSTR lpszBuffer) const;
      int Dir(UINT attr, LPCSTR lpszWildCard);

      friend void CB_dirlist_scroll(SCROLLBAR *sbar, float prevcure);
      friend void CB_dirlist_sele(void);
  };

#endif


