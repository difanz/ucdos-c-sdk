/**  *************  +/
      UC AStatic
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

/********** ClassStatic Implementation *********/

#include <string.h>
#include <stdio.h>
#include <dos.h>
#include "static.h"

AStatic::AStatic( LPCSTR Text, int length )
  {
    if(Text!=NULL)
      {
        StaticText = new char[_fstrlen(Text)+1];
        _fmemmove( StaticText, Text, _fstrlen(Text)+1 );
      }
    else
      {
        StaticText = new char[1];
        *StaticText = '\0';
      }
    TextLen = length;
  }

AStatic::~AStatic()
  {
    delete[] StaticText;
  }

BOOL AStatic::Create( const POINT point, WINDOWS* pParentWnd, UINT nID)
  {
    static_pParentWnd = pParentWnd;
    static_ID = nID;
    TextLocal.x = point.x;
    TextLocal.y = point.y;

    static_pWnd = UC_DefineLabel( static_pParentWnd,
                                  TextLocal.x, TextLocal.y,
                                  0,
                                  NULL,
                                  StaticText,
                                  NULL,
                                  0);
    return TRUE;
  }

int AStatic::GetText(LPSTR lpszBuffer) const
  {
    _fmemmove( lpszBuffer, StaticText, _fstrlen(StaticText)+1 );
    return _fstrlen(StaticText);
  }

int AStatic::SetText(LPSTR lpszBuffer)
  {
    delete[] StaticText;
    StaticText = new char[_fstrlen(lpszBuffer)+1];
    _fmemmove( StaticText, lpszBuffer, _fstrlen(lpszBuffer)+1 );
    UpdateShow();
    return _fstrlen(StaticText);

  }

void AStatic::UpdateShow( void )
  {
    setfillstyle( SOLID_FILL,WHITE);
    UC_WindowBar( static_pParentWnd, TextLocal.x, TextLocal.y,
                  TextLen * 8, 16);
    TextLen = _fstrlen( StaticText);
    RECT Srect = { TextLocal.x, TextLocal.y,
                   TextLocal.x + TextLen *8,
                   TextLocal.y + 16 };
    static_pWnd->text = StaticText;
    UC_InvalidataRect( static_pParentWnd, &Srect );
  }
