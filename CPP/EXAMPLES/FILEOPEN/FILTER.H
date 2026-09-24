/**  *************  +/
      UC Filter
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

#if __BCPLUSPLUS__< 0x0340
#error Use Borland C++ 4.5 or later releases mode to compile FileOpen Module!
#endif

#ifndef SDK_FILTER_CLASS
#define SDK_FILTER_CLASS

#include <stdlib.h>
#include <except.h>
#include "aocp_def.h"
#include "object.h"
#include "sdk.h"


class AFilter:AObject
  {
    private:
      AObject * pParentClass;
      INPUTLINE * filter_wnd;
      WINDOWS * filter_pParentWnd;
      SCROLLBAR * filter_sbar;
      void (*CB_filter_scroll)(SCROLLBAR *sbar, float prevcure);
      void (*CB_filter_sele)(void);
      ASTRING * lpFilterArray;
      ASTRING * lpCurFilterArray;
      ASTRING * lpFilterNameArray;
      ASTRING * lpCurFilterNameArray;
      LPSTR  lpfilterlist[5];
      LPSTR  lpfilterlistNull[1];
      char   SeleFileName[MAXFILE+MAXEXT+1];
      char   SeleFilterName[40];
      char * pszFilter;
      UINT filter_ID;
      int  ListHeight;
      int  CurSelect;
      int  FilterCount;

      void FilterSelect();
      void UpdateShow( int nSelect );

    public:
      AFilter(void (*CB_scroll)(SCROLLBAR *sbar, float prevcure),
                void(*CB_sele)(void), char * strFilter, AObject* parentclass);
      ~AFilter();
      virtual void OnChange( UINT Type, UINT IDcom) { };

      BOOL Create( const LISTBOXRECT& rect, WINDOWS* pParentWnd, UINT nID);

      int GetCount() const;
      int GetTopIndex() const;
      int SetTopIndex(int nIndex);

      int GetCurSel() const;
      int SetCurSel(int nSelect);

      int GetText(int nIndex, LPSTR lpszBuffer) const;
      int GetTextLen(int nIndex) const;

      int AddString(LPCSTR lpszItem);
      int InitFilterItem(LPCSTR lpszItem);
      void ResetContent();

      void GetFileName(LPSTR lpszBuffer) const;
      int InitFilter( void );

      friend void CB_filter_scroll(SCROLLBAR *sbar, float prevcure);
      friend void CB_filter_sele(void);
  };

#endif


