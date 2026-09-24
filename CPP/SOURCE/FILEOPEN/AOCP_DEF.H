

#ifndef __AOCP_DEF__
#define __AOCP_DEF__

//#define NDEBUG
#define DEBUG

#include <assert.h>
#include "sdk.h"

#define LPSTR  char far*
#define LPCSTR const char far*
#define UINT   unsigned int
#define LB_OKAY       0
#define LB_ERR        (-1)
#define LB_ERRSPACE      (-2)
#define TYPE_COMBOBOX  100


//typedef unsigned long DWORD;

typedef struct _AString {
                  char * lpStrItem;
                  int  ItemNum;
                  struct _AString * lpNextStr;
                }  ASTRING;

typedef struct _ListBoxRect {
                      int  left;
                      int  top;
                      int  width;
                      int  height;
                   } LISTBOXRECT;

typedef struct _POINT {
                   int x;
                   int y;
                 }  POINT;

typedef struct tagOPENFILENAME { /* ofn */
                      DWORD     lStructSize;
//                      HWND      hwndOwner;
//                      HINSTANCE hInstance;
                      LPCSTR    lpstrFilter;
                      LPSTR     lpstrCustomFilter;
                      DWORD     nMaxCustFilter;
                      DWORD     nFilterIndex;
                      LPSTR     lpstrFile;
                      DWORD     nMaxFile;
                      LPSTR     lpstrFileTitle;
                      DWORD     nMaxFileTitle;
                      LPCSTR    lpstrInitialDir;
                      LPCSTR    lpstrTitle;
                      DWORD     Flags;
                      UINT      nFileOffset;
                      UINT      nFileExtension;
                      LPCSTR    lpstrDefExt;
//                      LPARAM    lCustData;
//                      UINT      (CALLBACK* lpfnHook) (HWND, UINT, WPARAM, LPARAM);

                      LPCSTR    lpTemplateName;
                  } OPENFILENAME;


#endif
