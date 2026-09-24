// 图标文件读取与匹配函数

#include <alloc.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include "sdk.h"

// 读入图标内存映象
// 返回分配内存的图标指针
// 返回 NULL 表示失败
// filename= 图标文件路径名
void *UC_ReadIconFile(char *filename)
{
   int handle;
   int len;
   void *buf;
   WORD width, depth, bits;

   if ((handle = open(filename, O_RDONLY | O_BINARY)) == -1)
      return NULL;
   len = filelength(handle);
   if ((buf = malloc(len)) == NULL) {
      close(handle);
      return NULL;
   }
   if ((len = read(handle, buf, len)) == -1) {
      close(handle);
      MyFREE(buf);
      return NULL;
   }
   close(handle);
   if (!UC_GetIconInfo(buf, 0, &width, &depth, &bits)) {
      MyFREE(buf);
      return NULL;
   }
   return buf;
}

// 在一个 ICO 文件中按要求找出合适的一个
// 返回 0 表示没有合适的. 建议如果需要的话使用第一个
// 参数: cpIconAddr   图标指针
//       wWidth       所要求的图标宽
//       wDepth       所要求的图标高度
//       wBits        所要求的图标颜色数
WORD UC_GetIconApposite(void *cpIconAddr,
                        WORD wWidth, WORD wDepth, WORD wBits)
{
   WORD i;
   WORD width, depth, bits;

   for (i = 1; UC_GetIconInfo(cpIconAddr, i, &width, &depth, &bits); i++) {
      if (width == wWidth && depth == wDepth && bits == wBits)
         return i;
   }
   return 0;
}
