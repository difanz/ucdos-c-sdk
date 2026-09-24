/**  *************  +/
      UC FileList
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

#if __BCPLUSPLUS__< 0x0340
#error Use Borland C++ 4.5 or later releases mode to compile FileOpen Module!
#endif

#ifndef SDK_FILELIST_CLASS
#define SDK_FILELIST_CLASS

#include <stdlib.h>
#include <except.h>
#include "aocp_def.h"
#include "object.h"
#include "sdk.h"


class AFileList:AObject
  {
    private:
      AObject * pParentClass;
      LISTBOX * filelist_wnd;
      WINDOWS * filelist_pParentWnd;
      SCROLLBAR * filelist_sbar;
      void (*CB_filelist_scroll)(SCROLLBAR *sbar, float prevcure);
      void (*CB_filelist_sele)(void);
      ASTRING * lpFileNameArray;
      ASTRING * lpCurFileNameArray;
      LPSTR  lpfilelist[10];
      LPSTR  lpfilelistNull[1];
      char   SeleFileName[MAXFILE+MAXEXT+1];
      UINT filelist_ID;
      int  ListHeight;
      int  CurSelect;
      int  FileCount;

      void FileSelect();
      void UpdateShow( int nSelect );

    public:
      AFileList(void (*CB_scroll)(SCROLLBAR *sbar, float prevcure),
                void(*CB_sele)(void), AObject* parentclass);
      ~AFileList();
      virtual void OnChange( UINT Type, UINT IDcom) { };

      BOOL Create( const LISTBOXRECT& rect, WINDOWS* pParentWnd, UINT nID);

      int AddString(LPCSTR lpszItem);
      void ResetContent();

      void GetFileName(LPSTR lpszBuffer) const;
      int Dir(UINT attr, LPCSTR lpszWildCard);

      friend void CB_filelist_scroll(SCROLLBAR *sbar, float prevcure);
      friend void CB_filelist_sele(void);
  };

#endif


