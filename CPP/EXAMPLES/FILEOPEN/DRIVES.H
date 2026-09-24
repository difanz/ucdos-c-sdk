/**  *************  +/
      UC Drives
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

#if __BCPLUSPLUS__< 0x0340
#error Use Borland C++ 4.5 or later releases mode to compile FileOpen Module!
#endif

#ifndef SDK_DRIVES_CLASS
#define SDK_DRIVES_CLASS

#include <stdlib.h>
#include <except.h>
#include "aocp_def.h"
#include "object.h"
#include "sdk.h"


class ADrives:AObject
  {
    private:
      AObject * pParentClass;
      INPUTLINE * drives_wnd;
      WINDOWS * drives_pParentWnd;
      void (*CB_drives_scroll)(SCROLLBAR *sbar, float prevcure);
      void (*CB_drives_sele)(void);
      ASTRING * lpDrivesNameArray;
      ASTRING * lpCurDrivesNameArray;
      LPSTR  lpdriveslist[5];
      char   SeleDrives[MAXDRIVE];
      char   SeleDrivesName[40];
      int    DrivesState[27];
      UINT   drives_ID;
      int    ListHeight;
      int    CurSelect;
      int    DrivesCount;

      void DrivesSelect();
      void UpdateShow( int nSelect );

    public:
      ADrives(void (*CB_scroll)(SCROLLBAR *sbar, float prevcure),
                void(*CB_sele)(void), AObject* parentclass);
      ~ADrives();
      virtual void OnChange( UINT Type, UINT IDcom) { };

      BOOL Create( const LISTBOXRECT& rect, WINDOWS* pParentWnd,
                   int drive, UINT nID);

      int AddString(LPCSTR lpszItem);
      void ResetContent();

      BOOL DetectDisk( int drive );
      int SetDisk( int drive );
      void GetDrivesName(LPSTR lpszBuffer) const;
      int InitDrives( void );

      friend void CB_drives_scroll(SCROLLBAR *sbar, float prevcure);
      friend void CB_drives_sele(void);
  };

#endif


