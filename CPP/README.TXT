UCDOS SDK C++ 高级编程例子
--------------------------

    在此 <CPP> 目录中包含 UCDOS SDK for C/C++ 基于面向对象的 C++ 编程实例，
可供使用 C++ 编写程序的高级程序员在使用 SDK 编写应用时参考。
    我们在这里使用的 C++ 编译器是 BC 4.5，在 Windows 环境下进行编译工作。
所以请您准备 BC 4.5 版本。


<CPP> 的下级目录说明
━━━━━━━━━━
    <INCLUDE>             包含 'FileOpen' 和 '计算器模块' 的头文件.
    <LIB>                 包含 'FileOpen' 和 '计算器模块' 的库文件.
    <EXAMPLES>
             <FILEOPEN>   包含 'FileOpen' 应用实例.
             <CALC>       包含 '计算器模块' 应用实例.
    <SOURCE>
             <FILEOPEN>   包含 'FileOpen' 源代码(供修改参考).

描述
━━

   计算器模块
   ----------
           "计算器模块" 是一个计算器窗口的应用, 提供标准计算器的功能.
       可在应用程序中调用, 进行数据计算, 还可通过成员函数置入数据和获
       取数据. '计算器模块' 的应用详见 '计算器模块' 应用实例.

   FileOpen
   --------
           "FileOpen" 是一个用 C++ 编写的打开和存储文件对话框. 功能和
       用法同 Visual C++ MFC 库中的 CFileDialog 类基本相同. "FileOpen"
       的应用详见 'FileOpen' 应用实例. 如有兴趣可参阅 'FileOpen' 源代
       码.

                                               Author: I Believe
                                               09. 29.1996
