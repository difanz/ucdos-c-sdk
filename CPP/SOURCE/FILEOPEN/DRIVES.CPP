/**  *************  +/
      UC Drives
**  **************  **
      I Believe
      1996-8-11
/+  **************  */

/********** ClassFilter Implementation *********/

#include <string.h>
#include <stdio.h>
#include <dos.h>
#include <dir.h>
#include "drives.h"

#define CF 1  /* Carry flag */

typedef struct DeviceParams {
                   BYTE  dpSpecialFunction;
                   BYTE  dpDeviceType;
                   WORD  dpDeviceAtributes;
                   WORD  dpCylinders;
                   BYTE  dpMediaType;
                   char  BPB[31];
                 } DP;

ADrives::ADrives(void (*CB_scroll)(SCROLLBAR *sbar, float prevcure),
                        void(*CB_sele)(void), AObject* parentclass)
  {
    char drivetem[3] = {"X:"};

    CB_drives_scroll        = CB_scroll;
    CB_drives_sele          = CB_sele;
    lpDrivesNameArray       = NULL;
    lpCurDrivesNameArray    = NULL;
    DrivesCount             = 0;
    pParentClass            = parentclass;
    InitDrives();

    strcpy(SeleDrives, drivetem);
  }

ADrives::~ADrives()
  {
    ResetContent();
  }

void ADrives::ResetContent()
  {
    ASTRING * lpDrivesNameTmp1;
    ASTRING * lpDrivesNameTmp2;

    lpDrivesNameTmp1 = lpDrivesNameTmp2= lpDrivesNameArray;
    while (lpDrivesNameTmp1!=NULL)
      {
        lpDrivesNameTmp1 = lpDrivesNameTmp1->lpNextStr;
        delete lpDrivesNameTmp2->lpStrItem;
        delete lpDrivesNameTmp2;
        lpDrivesNameTmp2 = lpDrivesNameTmp1;
      }

    lpDrivesNameArray      = NULL;
    lpCurDrivesNameArray   = NULL;
    DrivesCount            = 0;
  }

BOOL ADrives::Create( const LISTBOXRECT& rect, WINDOWS* pParentWnd,
                      int drive, UINT nID)
  {
    ASTRING * lpDrivesNameTmp = lpDrivesNameArray;

    drives_pParentWnd = pParentWnd;
    drives_ID = nID;
    int CurDrive = drive - 1;
    ListHeight  = rect.height;

    SeleDrives[0] = 'A' + CurDrive;

    while ((lpDrivesNameTmp!=NULL)
           &&((lpDrivesNameTmp->lpStrItem[2]-'A')!=CurDrive))
      lpDrivesNameTmp = lpDrivesNameTmp->lpNextStr;
    _fmemmove( SeleDrivesName, lpDrivesNameTmp->lpStrItem,
               _fstrlen(lpDrivesNameTmp->lpStrItem)+1 );

    drives_wnd = UC_DefinePopBox( drives_pParentWnd,
                                  rect.left,rect.top,
                                  rect.width,
                                  rect.height,
                                  DrivesCount,
                                  SeleDrivesName,
                                  CB_drives_scroll,
                                  CB_drives_sele );

    drives_wnd->box->sbar->cure = lpDrivesNameTmp->ItemNum-1;
    CurSelect = lpDrivesNameTmp->ItemNum-1;
    return TRUE;
  }

int ADrives::AddString(LPCSTR lpszItem)
  {
    char * lpszDrivesName;

    try
      {
        lpszDrivesName = new char[_fstrlen(lpszItem)+1];
        _fmemmove( lpszDrivesName, lpszItem, _fstrlen(lpszItem)+1 );

        if(lpDrivesNameArray==NULL)
          {
            lpDrivesNameArray = new ASTRING;
            lpCurDrivesNameArray = lpDrivesNameArray;
          }
        else
          {
            lpCurDrivesNameArray->lpNextStr = new ASTRING;
            lpCurDrivesNameArray = lpCurDrivesNameArray->lpNextStr;
          }
      }
    catch (xalloc)
      {
        putchar('\007');
        nomem("No enough Memory !");
        return LB_ERRSPACE;
      }

    DrivesCount++;
    lpCurDrivesNameArray->lpStrItem = lpszDrivesName;
    lpCurDrivesNameArray->ItemNum   = DrivesCount;
    lpCurDrivesNameArray->lpNextStr = NULL;
    return DrivesCount;
  }

int ADrives::InitDrives( void )
  {
    int  wReturn = 0;
    struct REGPACK reg;
    char drivesstr[40];
    DP dp;

    ResetContent();

    for(unsigned i=1;i<27;i++)
      {
        reg.r_bx = i;
        reg.r_ax = 0x4409;
        intr(0x21, &reg);
        if (reg.r_flags & CF)
          DrivesState[i]=-1;
        else
          {
            sprintf( drivesstr, "\xaf\xe7%c:", 'A'+i-1 );
            DrivesState[i]=0;

            dp.dpSpecialFunction = 0;
            reg.r_ax = 0x440D;
            reg.r_bx = i;
            reg.r_cx = 0x0860;
            reg.r_ds = FP_SEG((void far *)&dp);
            reg.r_dx = FP_OFF((void far *)&dp);
            intr(0x21, &reg);
            if (reg.r_flags & CF)
              ;
            else
              {
                switch (dp.dpDeviceType)
                  {
                    case 0 :
                    case 1 :
                    case 2 :
                    case 3 :
                    case 4 :
                    case 7 :
                    case 9 :
                             reg.r_ax = 0x440E;
                             reg.r_bx = i;
                             intr(0x21, &reg);
                             if (reg.r_flags & CF)
                               ;
                             else
                               if(reg.r_ax&0x00FF)
                                {
                                  DrivesState[i]= -1;
                                  DrivesState[(reg.r_ax&0x00FF)]= 0;
                                }
                             break;
                    case 5 :
                             {
                               struct ffblk ffblk;
                               int done;
                               char drive[9] = {"A:\\*.*"};

                               drive[0] = 'A'+i-1;
                               done = findfirst(drive,&ffblk,FA_LABEL);
                               if(!done)
                                 {
                                   char volname[256];
                                   char *pvolname = volname;

                                   if(strlen(ffblk.ff_name)<8)
                                    strcpy( volname, ffblk.ff_name);
                                   else
                                    for(int i=0;i<strlen(ffblk.ff_name)+1;i++)
                                      if(ffblk.ff_name[i]!='.')
                                       *pvolname++ = ffblk.ff_name[i];
                                   strcat(drivesstr, volname);
                                 }
                             }
                    case 8 :
                    default:
                          break;
                  }
              }

            if(DrivesState[i]!=-1)
              if((wReturn=AddString((LPSTR)drivesstr))<0)
                return wReturn;
          }
      }
   return wReturn;
  }

BOOL ADrives::DetectDisk( int drive )
  {
    if(DrivesState[drive+1]!=-1)
      return TRUE;
    else
      return FALSE;
  }

int ADrives::SetDisk( int drive )
  {
    if(DrivesState[drive+1]!=-1)
      {
        ASTRING * lpDrivesNameTmp = lpDrivesNameArray;

        while ((lpDrivesNameTmp!=NULL)
               &&((lpDrivesNameTmp->lpStrItem[2]-'A')!=drive))
          lpDrivesNameTmp = lpDrivesNameTmp->lpNextStr;
        CurSelect = lpDrivesNameTmp->ItemNum-1;
        drives_wnd->box->sbar->cure = lpDrivesNameTmp->ItemNum-1;

        DrivesSelect();
        return DrivesCount;
      }
    return -1;
  }

void ADrives::DrivesSelect()
  {
    ASTRING * lpDrivesNameTmp = lpDrivesNameArray;

    while ((lpDrivesNameTmp!=NULL)
           &&((lpDrivesNameTmp->ItemNum-1)!=CurSelect))
      lpDrivesNameTmp = lpDrivesNameTmp->lpNextStr;

    SeleDrives[0] = *(lpDrivesNameTmp->lpStrItem+2);

    UC_UpdateInputLine(drives_pParentWnd,
                       drives_wnd,
                       lpDrivesNameTmp->lpStrItem);

    pParentClass->OnChange( TYPE_COMBOBOX, drives_ID);
  }

void ADrives::GetDrivesName(LPSTR lpszBuffer) const
  {
    _fmemmove( lpszBuffer, SeleDrives, _fstrlen(SeleDrives)+1 );
  }

void ADrives::UpdateShow( int nSelect )
  {
    ASTRING * lpDrivesNameTmp = lpDrivesNameArray;
    int CurTop = (int)UC_GetBoxTopCount(drives_wnd->box);

    CurSelect = nSelect;
    while ((lpDrivesNameTmp!=NULL)&&((lpDrivesNameTmp->ItemNum-1)!=CurTop))
      lpDrivesNameTmp = lpDrivesNameTmp->lpNextStr;

    for(int us1=0;us1<ListHeight;us1++)
      {
        if(lpDrivesNameTmp==NULL)
          break;
        lpdriveslist[us1] =  lpDrivesNameTmp->lpStrItem;
        lpDrivesNameTmp = lpDrivesNameTmp->lpNextStr;
      }
    drives_wnd->box->sbar->max=DrivesCount;
    UC_UpdateListBox( drives_pParentWnd, drives_wnd->box,
                      lpdriveslist, us1 );
  }
