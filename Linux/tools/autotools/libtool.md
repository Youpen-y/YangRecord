### 引言
问题：
- 不同的系统有其不同格式的共享库。
如（以 hello 为例）：

| 文件名形式            | 操作系统                        |
| ---------------- | --------------------------- |
| `libhello.so`    | Linux、Unix（System V ABI）    |
| `libhello.dll`   | Windows                     |
| `libhello.sl`    | HP-UX（Hewlett-Packard Unix） |
| `libhello.dylib` | macOS（Darwin）               |

- 构建共享库时，编译器和链接器所需的参数也各不相同

| 编译/链接参数       | 平台              | 说明                                  |
| ------------- | --------------- | ----------------------------------- |
| `-fPIC`       | Linux, Unix     | 生成位置无关代码（Position Independent Code） |
| `-shared`     | Linux, Unix     | 表示要生成共享库（.so）                       |
| `-KPIC`, `-G` | Solaris (SunOS) | Sun Studio 编译器的共享库构建参数              |
| `-bM:SRE`     | AIX (IBM)       | IBM AIX 系统上构建共享库的链接器参数              |
不仅**构建**共享库需要系统特定参数，**链接（linking）到共享库**时也可能需要用 `-rpath`、`-l`、`-L` 等特殊选项，并且行为会依赖系统和编译器。

- **并不是所有系统都支持共享库**。有些嵌入式或老式平台只支持静态链接 `.a` 文件。你的构建系统必须考虑这种情况。

开发者很难手动处理这些差异。这正是 `libtool` 存在的意义：封装这些平台的差异，让你用统一的方式构建共享库，自动选择正确的编译参数。

---
`Libtool` 的解决方案：
1. 新的库格式 `.la` （libtool archive），可抽象所有其他库格式。（不过 libtool archives 的想法现在已经被认为过时了）
2. 编译器和链接器的包装脚本，使用真实库将涉及 `libhello.la` 的操作转换为当前系统的正确操作。
3. 在 Makefile.am 中，只需创建并链接到 `*.la` 文件即可。这些操作会得到适当的转换。

`Libtool` 使用：
Libtool 在配置阶段需要使用一些专用的 `.m4` 宏来检测系统对共享库的支持情况。如：是否支持 `-fPIC`； 共享库后缀名 `.so` ，`.dll`，`.dylib` 等；链接器参数和运行时路径处理。这些 `.m4` 宏文件存放在项目的 `m4/` 目录下。
- 在 `configure.ac` 中调用 `LT_INIT`，（可以附加参数来启用静态库或共享库）
```m4
# LT_PREREQ([2.4])
LT_INIT([shared static])
```
- 在 `Makefile.am` 中使用 `_LTLIBRARIES` 声明 Libtool archives 
```make
lib_LTLIBRARIES = libfoo.la
libfoo_la_SOURCES = foo.c foo.h
```
- 使用 `_LDADD` 链接到本地 Libtool archives（程序链接自构建的 `.la` 库）
``` make
# 承接上一个 Makefile.am

bin_PROGRAMS = myapp
myapp_SOURCES = main.c
myapp_LDADD = ../lib/libfoo.la
```

运行结果：
```
.
├── config.h
├── config.log
├── config.status
├── lib
│   ├── libhello.la
│   ├── Makefile
│   ├── say.lo
│   └── say.o
├── libtool
├── Makefile
├── src
│   ├── hello
│   ├── main.o
│   └── Makefile
├── stamp-h1
└── test
    ├── bin
    │   └── hello
    └── lib
        ├── libhello.a
        ├── libhello.la
        ├── libhello.so -> libhello.so.0.0.0
        ├── libhello.so.0 -> libhello.so.0.0.0
        └── libhello.so.0.0.0
```
默认情况下，静态和动态库都被构建。这种默认行为可以通过传递给 `LT_INIT(options...)` 的选项改变：
- `disable-shared` 默认不构建动态库
- `disable-static` 默认不构建静态库
当然，也可以通过使用 `configure` 的选项覆盖这些设置。
- `--enable-shared`    构建动态库
- `--disable-shared` 不构建动态库
- `--enable-static`   构建静态库
- `--disable-static` 不构建静态库
总是至少构建一种风格，有些系统没有留下任何选择。

---
#### `Libtool` 对共享库版本管理（versioning）的机制和原理

为了 **兼容性** 和 **共存性**：
- 一个系统上可能有多个版本的同一个库（如 `libhello.so.1`, `libhello.so.2`）
- 不同的程序依赖不同版本的库接口
- 如果接口（比如函数名、参数、返回值）变了，不加管理可能导致崩溃或不可预测行为

版本库允许几个版本共存，它确保程序使用实现其所需接口的库。
接口 = 公开变量和函数 + I/O + 使用的文件格式/通信协议/ IO 格式

`Libtool` 的版本号 ≠ 发布版本号
比如你发布的是 `libhello 1.0.0`，这只是你作为开发者标记的版本，但 `Libtool` 会用三个数字表示接口版本：
```bash
libhello.la 中指定：
    -version-info CURRENT:REVISION:AGE
```
`Libtool` 会将它们转换成一个 `.so` 文件名，比如：
```bash
libhello.so.3.1.0
```

| 字段名        | 含义                                                       |
| ---------- | -------------------------------------------------------- |
| `CURRENT`  | 当前实现的接口版本号                                               |
| `REVISION` | 同一个接口版本的“修订号”——表示内部改动，不影响接口                              |
| `AGE`      | 表示支持哪些旧版本的接口，比如 `AGE=2` 表示兼容 `CURRENT-2` 到 `CURRENT` 的接口 |
这些数字通过 `-version-info` 指定：
```make
# lib/Makefile.am
lib_LTLIBRARIES = libhello.la
libhello_la_SOURCES = say.c say.h
libhello_la_LDFLAGS = -version-info CURRENT:REVISION:AGE
# 示例：
# libhello_la_LDFLAGS = -version-info 0:0:0
```

版本变化：

| 变化                       | 修改版本                     |
| ------------------------ | ------------------------ |
| (bug fixes) 不改变接口        | CURRENT: REVISION+1: AGE |
| (new functions) 增强了接口    | CURRENT+1 : 0 : AGE+1    |
| (removed functions) 接口改变 | CURRENT+1 : 0 : 0        |


