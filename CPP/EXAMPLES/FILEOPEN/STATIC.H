/**  *************  +/
      UC AStatic
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

#if __BCPLUSPLUS__< 0x0340
#error Use Borland C++ 4.5 or later releases mode to compile FileOpen Module!
#endif

#ifndef SDK_ASTATIC_CLASS
#define SDK_ASTATIC_CLASS

#include <stdlib.h>
#include <except.h>
#include "aocp_def.h"
#include "object.h"
#include "sdk.h"

class AStatic:AObject
  {
    private:
      WINDOWS * static_pParentWnd;
      UINT    static_ID;
      LABEL * static_pWnd;
      char * StaticText;
      POINT  TextLocal;
      int    TextLen;

    public:
      AStatic( LPCSTR Text, int length );
      ~AStatic();
      virtual void OnChange( UINT Type, UINT IDcom) { };

      BOOL Create( const POINT point, WINDOWS* pParentWnd, UINT nID);

      void UpdateShow( void );

      int GetText(LPSTR lpszBuffer) const;
      int SetText(LPSTR lpszBuffer) ;
  };

#endif


