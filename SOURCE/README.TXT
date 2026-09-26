SDK.LIB 核心库源码说明
===================================================================

一、模块列表
    1.  AGIDRV.C    显示驱动加载器与 BGI 接口 (含 XMSMM.C)
    2.  AGI_BMP.C   BMP/ICO 图像解码与渲染引擎 (含 STRETCH.C)
    3.  AGI_WIN.C   多窗口系统主单元 (含 WINBASE, WINDRAW, WINEVT1/2, WINTEXT, TIMER)
    4.  DIALOG.C    通用系统对话框
    5.  FILEOPEN.C  文件选择对话框
    6.  HARDERR.C   DOS 硬件严重错误处理 (INT 24h)
    7.  HELP.C      超文本联机帮助系统引擎 (USHLP)
    8.  ICON.C      Windows 16色图标加载与绘制
    9.  WINSIZE.C   窗口排版与平铺/层叠计算
    10. GDI.ASM     图形底层驱动分发
    11. STA.ASM     显示卡芯片检测
    12. STRELINE.ASM 扫描线缩放加速
    13. TEXTOUT.ASM 英文字符快速输出
    14. VGAFADE.ASM 调色板淡入淡出动画
    15. ICO*.ASM    内置系统图标数据 (APP, ASKM, ERROR, EXCL, EXIT, STOP)

二、编译说明
    在 DOS 下运行 BUILD.BAT 或执行 make -f MAKEFILE。
    使用 Borland C++ 3.1 和 Turbo Assembler 3.1 编译。
    编译产物输出至 ..\SDK.LIB。
