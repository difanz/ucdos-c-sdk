// XMSMM.C - XMS 内存管理程序
// 供 UCDOS SDK 内部使用

static WORD total_xms_kb = 0;
static WORD used_xms_kb = 0;
static WORD global_xms_handle = 0;

void UC_MoveXMS(void far *p)
{
   asm {
      push es
      push ds
      push ds
      pop es
      mov si, word ptr p
      mov ax, word ptr p+2
      mov ds, ax
      mov ah, 0bh
      call far ptr es:[xms_entry]
      pop ds
      pop es
   }
}

void UC_CheckXMS(void)
{
   int i;
   char status;

   for (i = 0; i < 1000; i++) {
      xms_table[i].offset_k = 0;
      xms_table[i].size_k = 0;
   }
   xms_entry = NULL;

   asm {
      mov ax, 4300h
      int 2fh
      mov status, al
   }
   if (status != (char)0x80)
      return;

   asm {
      push es
      mov ax, 4310h
      int 2fh
      mov ax, es
      pop es
      mov word ptr [xms_entry+2], ax
      mov word ptr [xms_entry], bx
      mov ah, 9
      mov dx, total_xms_kb
      or dx, dx
      jnz alloc_xms
      mov dx, 0ffffh
      mov total_xms_kb, dx
   }
alloc_xms:
   asm {
      call far ptr [xms_entry]
      mov status, al
      mov global_xms_handle, dx
   }
   if (status == 0) {
      asm {
         mov ah, 8
         call far ptr [xms_entry]
      }
      if (_AX == 0) {
         xms_entry = NULL;
         global_xms_handle = 0;
         total_xms_kb = 0;
      } else {
         total_xms_kb = _AX;
         asm {
            mov dx, ax
            mov ah, 9
            call far ptr [xms_entry]
            mov global_xms_handle, dx
         }
      }
   }
}

void UC_GetFreeXMS(WORD *totalk, WORD *freek)
{
   *totalk = total_xms_kb;
   *freek = total_xms_kb - used_xms_kb;
}

int UC_AllocXMS(WORD size, WORD *handle)
{
   int i;

   if (xms_entry == NULL || (total_xms_kb - used_xms_kb) < size)
      return 0;

   for (i = 0; i < 1000; i++) {
      if (xms_table[i].size_k == 0) {
         xms_table[i].size_k = size;
         xms_table[i].offset_k = used_xms_kb;
         used_xms_kb += size;
         *handle = i + 1;
         return 1;
      }
   }
   return 0;
}

DWORD get1024(WORD k)
{
   return (DWORD)k << 10;
}

void UC_FreeXMS(WORD handle)
{
   int i;
   WORD off_k, sz_k;
   struct {
      DWORD Length;
      WORD SourceHandle;
      DWORD SourceOffset;
      WORD DestHandle;
      DWORD DestOffset;
   } move;

   if (xms_entry == NULL || !UC_VerifyXMSHandle(handle))
      return;

   handle--;
   if ((move.Length = get1024(used_xms_kb - xms_table[handle].offset_k - xms_table[handle].size_k)) != 0) {
      move.SourceHandle = global_xms_handle;
      move.SourceOffset = get1024(xms_table[handle].offset_k + xms_table[handle].size_k);
      move.DestHandle = global_xms_handle;
      move.DestOffset = get1024(xms_table[handle].offset_k);
      UC_MoveXMS(&move);
   }

   used_xms_kb -= xms_table[handle].size_k;
   off_k = xms_table[handle].offset_k;
   sz_k = xms_table[handle].size_k;
   xms_table[handle].size_k = 0;

   for (i = 0; i < 1000; i++) {
      if (xms_table[i].size_k != 0 && xms_table[i].offset_k > off_k) {
         xms_table[i].offset_k -= sz_k;
      }
   }
}

int UC_VerifyXMSHandle(WORD handle)
{
   if (xms_entry != NULL && handle != 0 && handle <= 1000 && xms_table[handle - 1].size_k != 0)
      return 1;
   return 0;
}

void UC_InitToXMS(WORD handle, void *buf, DWORD offset)
{
   xms_move.SourceHandle = 0;
   memmove(&xms_move.SourceOffset, &buf, 4);
   xms_move.DestHandle = global_xms_handle;
   xms_move.DestOffset = get1024(xms_table[handle - 1].offset_k) + offset;
}

void UC_InitFromXMS(WORD handle, void *buf, DWORD offset)
{
   xms_move.DestHandle = 0;
   memmove(&xms_move.DestOffset, &buf, 4);
   xms_move.SourceHandle = global_xms_handle;
   xms_move.SourceOffset = get1024(xms_table[handle - 1].offset_k) + offset;
}

void UC_MoveToXMS(WORD count)
{
   if (count & 1)
      count++;
   xms_move.Length = count;
   UC_MoveXMS(&xms_move);
   xms_move.DestOffset += xms_move.Length;
}

void UC_MoveFromXMS(WORD count)
{
   void far *dst;
   char byte_buf[2];
   void far *BYTE_PTR;

   if (count == 0)
      return;

   if (count == 1) {
      dst = *(void far **)&xms_move.DestOffset;
      xms_move.Length = 2;
      memmove(&xms_move.DestOffset, &byte_buf, 4);
      UC_MoveXMS(&xms_move);
      asm {
         mov word ptr BYTE_PTR+2, ss
         lea ax, byte_buf
         mov word ptr BYTE_PTR, ax
         push es
         push ds
         mov ax, word ptr dst+2
         mov bx, word ptr dst
         mov cx, word ptr BYTE_PTR+2
         mov dx, word ptr BYTE_PTR
         mov es, ax
         mov di, bx
         mov ds, cx
         mov si, dx
         mov al, [si]
         mov es:[di], al
         pop ds
         pop es
      }
      *(void far **)&xms_move.DestOffset = dst;
      xms_move.SourceOffset += 2;
   } else {
      asm {
         test word ptr count, 1
         jz short XMS_EVEN_COUNT
      }
      xms_move.Length = count - 1;
      UC_MoveXMS(&xms_move);
      xms_move.SourceOffset += xms_move.Length - 1;
      dst = *(void far **)&xms_move.DestOffset;
      *(DWORD *)&xms_move.DestOffset += xms_move.Length - 1;
      xms_move.Length = 2;
      UC_MoveXMS(&xms_move);
      xms_move.SourceOffset += 3;
      *(void far **)&xms_move.DestOffset = dst;
      asm jmp short XMS_READ_DONE
XMS_EVEN_COUNT:
      xms_move.Length = count;
      UC_MoveXMS(&xms_move);
      xms_move.SourceOffset += xms_move.Length;
XMS_READ_DONE:
      ;
   }
}
