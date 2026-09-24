/**  *************  +/
      UC AStatic
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

/********** ClassEdit Implementation *********/

#include <string.h>
#include <stdio.h>
#include <dos.h>
#include "edit.h"

AEdit::AEdit( void(*CB_sele)(void), LPCSTR Text, AObject * parentclass )
  {
    pParentClass = parentclass;
    CB_editbox_sele = CB_sele;
    if(Text!=NULL)
      _fmemmove( EditText, Text, _fstrlen(Text)+1 );
    else
      EditText[0] = '\0';
  }

AEdit::~AEdit()
  {
  }

BOOL AEdit::Create( const POINT point, WINDOWS* pParentWnd, UINT nID)
  {
    edit_pParentWnd = pParentWnd;
    EditBox_ID = nID;
    TextLocal.x = point.x;
    TextLocal.y = point.y;

    edit_pWnd = UC_DefineInputLine( edit_pParentWnd,
                                    TextLocal.x, TextLocal.y,
                                    18,
                                    EditText,
                                    MAXPATH,
                                    CB_editbox_sele);

    return TRUE;
  }

int AEdit::GetText(LPSTR lpszBuffer) const
  {
    _fmemmove( lpszBuffer, EditText, _fstrlen(EditText)+1 );
    return _fstrlen(EditText);
  }

int AEdit::SetText(LPSTR lpszBuffer)
  {
    _fmemmove( EditText, lpszBuffer, _fstrlen(lpszBuffer)+1 );
    UpdateShow();
    return _fstrlen(EditText);
  }

void AEdit::FileNameSelect( void )
  {
    pParentClass->OnChange( TYPE_INPUTLINE, EditBox_ID);
  }

void AEdit::UpdateShow( void )
  {
    UC_UpdateInputLine(edit_pParentWnd,
                       edit_pWnd,
                       EditText);
  }
