`pkg-config` - 返回已安装库的元信息（meta information）

作为编译应用程序和库的辅助工具，可帮助在命令行中插入正确的编译器选项，以便应用程序可以使用 `gcc -o test test.c $(shell pkg-config --libs --cflags glib-2.0)` 等参数，而无需硬编码 `glib` （或其他库）的路径。它与语言无关，因此可以用来定义文档工具的位置。

主要用于检索系统中已安装库的信息，通信用于编译和链接一个或多个库。
功能：
- 查找库的安装路径 —— 帮助找到库的头文件路径和库文件路径
- 提供编译和链接选项 —— 自动生成包含编译器选项（`-I`）和链接器选项（`-L/-l`）的命令行参数
- 管理库的依赖关系 —— 处理库之间的依赖，如使用 A 库时，可能会自动包含它依赖的 B 库。

工作原理
- `.pc` 文件。每个库都会安装一个或多个 `.pc` 文件（package configuration 文件），该文件存储了库的元数据。
- 通常，`.pc` 文件位于以下目录之一
	- `/usr/lib/pkgconfig/`
	- `/usr/share/pkgconfig/`
	- `/usr/local/lib/pkgconfig/`
	- 通过 `PKG_CONFIG_PATH` 指定的自定义目录
- `pkg-config` 通过读取以 `.pc` 为后缀的配置文件获取关于已安装库的信息。
一个`example.pc`示例
```ini
prefix=/usr/local
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include

Name: Example
Description: An example library
Version: 1.0.0
Cflags: -I${includedir}
Libs: -L${libdir} -lexample
```
关键字段：
- `Cflags`：编译时需要的头文件路径选项
- `Libs`：链接时需要的库路径和库文件选项

使用方法
- 获取头文件路径（编译选项）
```bash
$ pkg-config --cflags example.pc
-I/usr/local/include
```

- 获取库路径和链接选项
```bash
$ pkg-config --libs example
-L/usr/local/lib -lexample
```

- 检查库是否安装
```bash
$ pkg-config --exists zlib && echo "Installed" || echo "Not installed"
Installed
```

- 其他信息
```bash
$ pkg-config --modversion libcurl # 查看库版本
8.5.0
$ pkg-config --print-errors # 显示错误信息
```

两个使用场景：
1. 在 Makefile 中使用
```Makefile
CC = gcc
CFLAGS = -Wall $(shell pkg-config --cflags example)
LDFLAGS = $(shell pkg-config --libs example)

all: program

program: main.c
	$(CC) $(CFLAGS) main.c -o program $(LDFLAGS)
```

2. 在命令行中直接使用
```bash
gcc main.c -o program $(pkg-config --cflags --libs example)
```

设置环境变量
- `PKG_CONFIG_PATH`：如果 `.pc` 文件不存在默认路径，可以通过设置 `PKG_CONFIG_PATH` 来指定路径。
```bash
export PKG_CONFIG_PATH=/custom/path/lib/pkgconfig:$PKG_CONFIG_PATH
```
- `PKG_CONFIG_LIBDIR`：替换默认的 `.pc` 搜索路径。

---
在 `autoconf` 中，可以使用与 `pkg-config` 相关的宏来检测库依赖和获取其编译及链接信息。这些宏简化了在 `configure.ac` 中管理外部库的过程。
1. `PKG_CHECK_MODULES`
作用：检测指定的库是否可用，并获取其编译和链接选项。
语法：
```m4
PKG_CHECK_MODULES(variable-prefix, modules, [ACTION-IF-FOUND], [ACTION-IF-NOT-FOUND])
```
`variable-prefix`：生成的变量的前缀，用于存储结果（如 `FOO_CFLAGS` 和 `FOO_LIBS`）。
`modules`：需要检测的库（`pkg-config`模块名），多个库用空格分隔。
`[ACTION-IF-FOUND]`：（可选），找到库时执行的命令。
`[ACTION-IF-NOT-FOUND]`：（可选），未找到库时执行的命令。

示例：
```m4
PKG_CHECK_MODULES([FOO], [glib-2.0 >= 2.56 gtk+-3.0], [], [
  AC_MSG_ERROR([glib-2.0 and gtk+-3.0 are required.])
])
```
如果库找到：
- `FOO_CFLAGS` 将包含编译选项（如 `-I`路径）
- `FOO_LIBS` 将包含链接选项（如 `-L`路径）
如果库未找到：
- 执行 `AC_MSG_ERROR` 并终止配置过程

2. `PKG_PROG_PKG_CONFIG`
作用：检测 `pkg-config` 工具是否可用，并设置内部宏变量。
语法：
```m4
PKG_PROG_PKG_CONFIG([MINIMUM-VERSION])
```
- `MINIMUM-VERSION`（可选）：指定最低版本的 `pkg-config`

环境变量在 `configure.ac` 的处理
1. `PKG_CONFIG`
作用：定义 `pkg-config` 工具的路径
用法：在 `configure.ac` 中可以直接使用
```m4
AC_ARG_VAR([PKG_CONFIG], [path to pkg-config binary])
```

2. `PKG_CONFIG_PATH`
作用：指定 `.pc` 文件的搜索路径
用法：在 `configure.ac` 中通过 `AC_SUBST` 将 `PKG_CONFIG_PATH` 替换为实际路径
```m4
AC_SUBST([PKG_CONFIG_PATH], [/custom/path/lib/pkgconfig])
```