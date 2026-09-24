// 窗口平铺与层叠算法

#include <alloc.h>
#include <stdlib.h>
#include <math.h>
#include "sdk.h"

int UC_GetWindowsCount(void)
{
   int count = 0;
   WINDOWS *wnd;

   for (wnd = wintable_head; wnd != NULL; wnd = wnd->next) {
      if (!(wnd->mode & WS_SINGLELINE) &&
          wnd->minmax != SW_SHOWMIN &&
          !(wnd->mode & (WS_NOMOVE | WS_NOSIZE)) &&
          wnd->mode != 0)
         count++;
   }
   return count;
}

void UC_CascadeWindows(int left, int top, int right, int bottom)
{
   WINDOWS *wnd, *head;
   int x, y;
   int w, h;
   register int x1, y1;

   if (left >= right || top >= bottom)
      return;

   head = wintable_head;
   wnd = wintable_head;
   if (!UC_WindowVerify(wnd))
      return;

   x = (x1 = left);
   y = (y1 = top);

   w = UC_GetWindowsCount();
   if (!w)
      return;

   w = (w - 1) * (wnd->syschar_size + 6);
   w = (right - left) - w;
   h = (bottom - top) * 2 / 3;

   UC_MouseHide();
   do {
      UC_WindowEnable(wnd);
      if (wnd->minmax == SW_SHOWMAX)
         UC_MaxWindow(wnd);

      if (!(wnd->mode & WS_SINGLELINE) &&
          wnd->minmax == SW_SHOWNOR &&
          !(wnd->mode & (WS_NOMOVE | WS_NOSIZE)) &&
          wnd->mode != 0) {
         UC_WindowResize(x1, y1, x1 + w, y1 + h);
         x1 = x + wnd->syschar_size + 6;
         y1 = y + wnd->syschar_size + 6;
         if (x1 + wnd->width - 1 > right || y1 + wnd->height - 1 > bottom) {
            x1 = left;
            y1 = top;
         }
         x = x1;
         y = y1;
      }
      wnd = wintable_head;
   } while (wnd != head);
   UC_MouseShow();
}

void UC_HTileWindows(int left, int top, int right, int bottom)
{
   WINDOWS *wnd, *head;
   int win_w, win_h1, win_h2;
   int c, r;
   int rem;
   int cur;
   register int count;
   register int adj;

   if (left >= right || top >= bottom)
      return;

   head = wintable_head;
   wnd = wintable_head;
   if (!UC_WindowVerify(wnd))
      return;

   count = UC_GetWindowsCount();
   if (!count)
      return;

   c = r = sqrt(count);
   rem = count - r * c;
   if (rem > c) {
      adj = 2;
      rem -= c;
   } else {
      adj = 1;
   }

   win_w = (right - left + 1) / c;
   win_h1 = (bottom - top + 1) / (r + adj - 1);
   win_h2 = (bottom - top + 1) / (r + adj);

   UC_MouseHide();
   cur = 1;
   do {
      UC_WindowEnable(wnd);
      if (wnd->minmax == SW_SHOWMAX)
         UC_MaxWindow(wnd);

      if (!(wnd->mode & WS_SINGLELINE) &&
          wnd->minmax == SW_SHOWNOR &&
          !(wnd->mode & (WS_NOMOVE | WS_NOSIZE)) &&
          wnd->mode != 0) {
         int x, y, h;
         if (cur > rem * (r + adj)) {
            x = left + (count - cur) / (r + adj - 1) * win_w;
            y = top + (count - cur) % (r + adj - 1) * win_h1;
            h = win_h1;
         } else {
            x = left + ((count - cur) + c - rem) / (r + adj) * win_w;
            y = top + ((count - cur) + c - rem) % (r + adj) * win_h2;
            h = win_h2;
         }
         UC_WindowResize(x, y, x + win_w - 1, y + h - 1);
         cur++;
         if (cur > count)
            break;
      }
      wnd = wintable_head;
   } while (wnd != head);
   UC_MouseShow();
}

void UC_VTileWindows(int left, int top, int right, int bottom)
{
   WINDOWS *wnd, *head;
   int win_w, win_h1, win_h2;
   int count;
   int r;
   int adj;
   int cur;
   register int c;
   register int rem;

   if (left >= right || top >= bottom)
      return;

   head = wintable_head;
   wnd = wintable_head;
   if (!UC_WindowVerify(wnd))
      return;

   count = UC_GetWindowsCount();
   if (!count)
      return;

   c = r = sqrt(count);
   rem = count - c * c;
   adj = 1;
   if (c * 2 == rem) {
      rem = 0;
      c += 2;
   } else if (rem > c) {
      rem -= c;
      c++;
   } else if (rem == c) {
      rem = 0;
      c++;
   }

   win_w = (right - left + 1) / c;
   win_h1 = (bottom - top + 1) / (r + adj - 1);
   win_h2 = (bottom - top + 1) / (r + adj);

   UC_MouseHide();
   cur = 1;
   do {
      UC_WindowEnable(wnd);
      if (wnd->minmax == SW_SHOWMAX)
         UC_MaxWindow(wnd);

      if (!(wnd->mode & WS_SINGLELINE) &&
          wnd->minmax == SW_SHOWNOR &&
          !(wnd->mode & (WS_NOMOVE | WS_NOSIZE)) &&
          wnd->mode != 0) {
         int x, y, h;
         if (cur > rem * (r + adj)) {
            x = left + (count - cur) / (r + adj - 1) * win_w;
            y = top + (count - cur) % (r + adj - 1) * win_h1;
            h = win_h1;
         } else {
            x = left + ((count - cur) + c - rem) / (r + adj) * win_w;
            y = top + ((count - cur) + c - rem) % (r + adj) * win_h2;
            h = win_h2;
         }
         UC_WindowResize(x, y, x + win_w - 1, y + h - 1);
         cur++;
         if (cur > count)
            break;
      }
      wnd = wintable_head;
   } while (wnd != head);
   UC_MouseShow();
}
