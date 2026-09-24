围棋程序工程文件内容

go.c                    ; 主程序文件
goico.obj               ; About 窗口显示的图标
man1.obj                ; 黑子图标
man2.obj                ; 白子图标
cursor1.obj             ; 手形光标1
cursou2.obj             ; 手形光标2
sdk.lib                 ; SDK 开发库

    以上的 .OBJ 文件均通过 BC 中提供的 BGIOBJ 程序用相应 .ICO 文件转成。

注: 由于是例子程序的缘故，这里提供的围棋程序并非一个完善的应用程序, 其
    中仅实现了主要的围棋双人对下基本功能，很多细节有待感兴趣者进一步挖
    掘。
