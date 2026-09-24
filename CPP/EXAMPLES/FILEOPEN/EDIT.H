/**  *************  +/
      UC AStatic
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

#if __BCPLUSPLUS__< 0x0340
#error Use Borland C++ 4.5 or later releases mode to compile FileOpen Module!
#endif

#ifndef SDK_AEDIT_CLASS
#define SDK_AEDIT_CLASS

#include <stdlib.h>
#include <dir.h>
#include "aocp_def.h"
#include "object.h"
#include "sdk.h"

class AEdit:AObject
  {
    private:
      AObject * pParentClass;
      WINDOWS * edit_pParentWnd;
      INPUTLINE * edit_pWnd;
      char   EditText[MAXPATH+2];
      void (*CB_editbox_sele)(void);
      POINT  TextLocal;
      UINT EditBox_ID;

    public:
      AEdit( void(*CB_sele)(void), LPCSTR Text, AObject * parentclass );
      ~AEdit();
      virtual void OnChange( UINT Type, UINT IDcom) { };

      BOOL Create( const POINT point, WINDOWS* pParentWnd, UINT nID);

      void UpdateShow( void );

      int GetText(LPSTR lpszBuffer) const;
      int SetText(LPSTR lpszBuffer) ;
      void FileNameSelect( void );

      friend void CB_editbox_sele(void);
  };

#endif


