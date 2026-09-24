# UCDOS SDK for C/C++ 2.0 (1996)

UCDOS SDK for C/C++ 2.0 是北京希望电脑公司于 1996 年随 UCDOS 发布的图形界面与应用程序开发工具包（主创作者：**简晶** 等）。该工具包提供了基于 DOS 的多窗口图形系统（UCVision）、图形驱动接口（GDI/AGIDRV）、超文本帮助系统（USHLP）及 C++ 面向对象支持。

当年发行的软盘中仅包含了预编译库 `SDK.LIB` 与部分演示代码。本仓库整理复原了 `SDK.LIB` 核心库的全部 C 语言与汇编源码，并按原版目录结构归档了完整的开发环境与官方示例。

## 授权与致谢 (Permission & Accreditation)

本项目发布已获得原作者 **简晶** 先生的许可。

特此向简晶先生及原北京希望电脑公司团队致谢，感谢他们当年在 DOS 中文图形环境与中文信息处理领域所做出的杰出贡献与开创性工作。

## 目录结构 (Directory Layout)

```
sdk-src/
├── APP/        官方应用实例（文本编辑、围棋、生命游戏、动画播放等）
├── BMP/        配套图形与背景素材
├── CPP/        C++ 视窗扩展库与示例（计算器、文件选择器）
├── EXAMPLES/   官方基础控件与窗口编程示例
├── ICO/        图标资源与查看工具
├── INCLUDE/    C/C++ API 接口头文件（SDK.H 等）
├── LIB/        编译生成的静态链接库（SDK.LIB）
├── SOURCE/     SDK.LIB 核心库源码与 Makefile
├── tools/      现代自动化构建脚本（基于 DOSBox-X）
├── BUILD.BAT   根目录一键构建脚本
├── CLEAN.BAT   清理临时文件
├── MAKEFILE    根目录构建文件
└── README      官方说明书（DOS 下配合 README.COM 阅读）
```

## 核心库架构 (`SOURCE/`)

核心静态库 `SDK.LIB` 主要由以下部分构成：

### 1. 窗口与事件系统 (UCVision)
- **`AGI_WIN.C`**：视窗核心模块，包含：
  - `WINBASE.C`：窗口管理、视口裁减与控件结构管理。
  - `WINDRAW.C`：窗口外观渲染、阴影边框及常用控件（按钮、单选、复选、列表、滚动条）绘制。
  - `WINEVT1.C`：键盘与鼠标底层事件分发、点击命中测试。
  - `WINEVT2.C`：单行文本输入编辑、双字节字符处理、菜单与弹出式菜单系统。
  - `WINTEXT.C`：文本绘制与排版、鼠标指针形状控制、光标（Caret）闪烁。
  - `TIMER.C`：8253 定时器中断调度（INT 8 / INT 1Ch）。
- **`WINSIZE.C`**：窗口平铺与层叠排版计算。
- **`DIALOG.C`**：标准系统对话框（提示框、确认框、输入框）。
- **`FILEOPEN.C`**：标准文件打开对话框。

### 2. 图形驱动与底层加速 (GDI & Display Driver)
- **`GDI.ASM`**：底层图形函数分发、画线与填充、INT 48h 字符加速与字库调用接口。
- **`AGIDRV.C`**：图形驱动装载与 BGI 接口模拟。
- **`XMSMM.C`**：XMS 内存管理支持。
- **`STA.ASM`**：显卡硬件识别与检测（支持 ET4000、S3、Cirrus、Trident、Oak 等芯片及 VESA）。
- **`TEXTOUT.ASM`**：西文字符图形模式快速输出。
- **`VGAFADE.ASM`**：调色板平滑淡入淡出。
- **`STRELINE.ASM`**：扫描线缩放加速。

### 3. 图像与资源处理
- **`AGI_BMP.C`** & **`STRETCH.C`**：BMP/ICO 图像解码、缩放与 DDB 格式转换。
- **`ICON.C`**：Windows 图标格式加载。
- **`ICO*.ASM`**：系统内置 16 色标准图标数据。

### 4. 系统服务
- **`HELP.C`**：USHLP 超文本联机帮助引擎。
- **`HARDERR.C`**：DOS 硬件关键错误处理（INT 24h）。

## 构建说明 (Building)

工程源代码采用 GBK/CP936 编码与 DOS CRLF 换行符。

### 1. 在 DOS / DOSBox 中构建

推荐环境：Borland C++ 3.1、Turbo Assembler 3.1 (TASM)、Turbo Librarian 3.02 (TLIB)。

在根目录运行：
```bat
BUILD.BAT
```
或进入 `SOURCE\` 目录执行：
```bat
make -f MAKEFILE
```
编译完成后将自动打包生成 `LIB\SDK.LIB` 与根目录 `SDK.LIB`。

运行 `CLEAN.BAT` 可清除构建产生的中间文件。

### 2. 编译示例

进入 `EXAMPLES\` 目录，运行：
```bat
BUILD.BAT HELLO.C
BUILD.BAT WIN\WIN1.C
BUILD.BAT WID\BUTTON.C
```
即可编译并链接对应的示例可执行文件。

### 3. 跨平台自动化构建 (Linux)

在 Linux 环境下，可以通过 `tools/build.py` 驱动 DOSBox-X 进行无头构建。需提供 DOSBox-X 路径与 Borland C++ 3.1 目录（支持命令行参数或环境变量）：

```bash
# 方式一：通过命令行参数指定
python3 tools/build.py --all --dosbox /path/to/dosbox-x --bc31 /path/to/bc31

# 方式二：通过环境变量指定
export DOSBOX_BIN=/path/to/dosbox-x
export BC31_DIR=/path/to/bc31
python3 tools/build.py --all

# 重新打包 LIB
python3 tools/build.py --lib
```
