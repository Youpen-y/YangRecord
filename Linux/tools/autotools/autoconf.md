`autoconf` 是一个生成 shell 脚本的工具，这些脚本可以用来自动配置软件源代码包以适应多种 Posix-like 系统。
本质上 `autoconf` 是一个编译器，将 `configure.ac` 脚本编译为 shell 脚本，然后该脚本可在任何没有 `autoconf` 的平台上运行。`configure.ac` 使用 `M4sh` 语言，其中包含各种 M4 宏。在运行 `autoconf` 时会使用 `M4` 预处理器运行 `configure.ac` 脚本，所有宏都被扩展（包含相应的 shell 代码），生成纯 shell 代码。

工作方式
1. 创建一个 `configure.ac` 脚本，在其中定义各种设置，如版本名称、版本、要使用的编译器以及输出文件的位置。
2. 使用 `autoconf` 运行它以创建 `configure` 脚本（`autoconf` 作用是从系统收集系统以填充使用 `automake` 创建的 `Makefile.in` 模板） 

`Autoconf` 生成的配置脚本在运行时不需要用户手动干预；通常甚至不需要指定系统类型的参数。相反，他们单独测试其所使用的软件包可能需要的每个功能是否存在。

`Autoconf` 使得用户使用更轻松。主要目标不是为软件包维护者自动生成 `configure` 。

`Autoconf` 脚本是使用 `shell` 和 `M4`（一种比 C 预处理器功能强大得多的宏处理语言）组合编写的，使用 `m4` 宏。需要 GNU M4 版本 1.4.8 或更高版本才能生成脚本。


---
一些基础：
- `Autoconf` M4 宏格式多为：`A[CST]_??????` 。也可能有一些以 `_` 开头的私有版本。
- 配置脚本名称通常为 `configure.ac` 。
- `M4`使用 `[` 和 `]` 引用参数。其中无法转义，如果想插入 `[` 和 `]` 可采用如下方案 
	- 使用 Quadrigraph ，`@<:@` 代表 `[` ；`@>:@` 代表 `]` 。
	- 更改引用：`changequote(<<,>>)` 将引用更改为 `<<` 和 `>>` 。
	- 注意：M4 会将 `[[]]` 转为 `[]` 。尽可能避免使用 `[` 和 `]`。

示例：
现在有一个 `Makfile` 如下，使用自动化构建改造它
```makefile
CXX=g++
LD=g++
CXXFLAGS=-Wall -Wextra

program: program.o
		$(LD) -o $@ $^

.PHONY: clean
clean:
		rm -f program *.o
```

首先，将 `Makefile` 重命名为 `Makefile.in` 并编辑它，如下
```makefile
CXX=@CXX@
LD=@CXX@
CXXFLAGS=@CXXFLAGS@

program: program.o
	$(LD) -o $@ $^

.PHONY: clean
clean:
	rm -f program *.o
```
其中使用占位符 `@foo@` （配置脚本将替换）取代硬连接的编译器名称和标志等。
编写相应的配置脚本：
```
AC_INIT([program], [1.0])

dnl Switch to a C++ compiler, and check if it works.
AC_LANG(C++)
AC_PROG_CXX

dnl Process Makefile.in to create Makefile
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
```
其中 `dnl` 是 `autoconf` 的注释。
- 使用 `autoconf` 命令生成 `configure` 脚本
- 执行 `./configure && make` 输出如下
```
checking for g++... g++
checking whether the C++ compiler works... yes
checking for C++ compiler default output file name... a.out
checking for suffix of executables... 
checking whether we are cross compiling... no
checking for suffix of object files... o
checking whether the compiler supports GNU C++... yes
checking whether g++ accepts -g... yes
checking for g++ option to enable C++11 features... none needed
configure: creating ./config.status
config.status: creating Makefile
make: 'program' is up to date.

```
- 可以使用 `./configure CXXFLAGS="-Wall -Wextra" && make` 添加编译器标志，或者 `./configure CXX=g++-5 && make` 改变编译器

---
`autoconf` 中的大多数测试都与编译的代码有关。为此，`autoconf` 生成一个小程序，然后通过编译器和可能的链接器运行该文件，并检查生成的错误代码。如：
```c
AC_CHECK_LIB(m, cos)
```
将生成一个如下程序
```c
char cos();
int main()
{
	cos();
	return 0;
}
```
然后尝试编译并使用 `libm` 链接它。
`AC_CHECK_HEADERS` 做类似的事情，但只运行编译器，而不运行链接器。

---
有时，依赖项是可选的（有无某个库都行）。两种常见的处理方法：
1. 条件编译（设置宏）
标准方案：使用 `config.h` （尽管使用 `-D` 可行，但会使得程序依赖于参数）
```c
a=0
AC_CHECK_HEADERS(zlib.h, [], [a=1])
AC_SEARCH_LIBS(deflate, z, [], [a=1])

if test $a == 0
then
		AC_DEFINE(HAVE_LIBZ)
fi

AC_CONFIG_HEADERS(config.h)
```
`AC_DEFINE` 将导致 `#define` 写入 `config.h`中。`AC_CONFIG_HEADERS` 必须位于 `AC_OUTPUT` 之前，否则它将被忽略。`config.h.in` 是 include 文件的模板，但替换是在 `#undef` 行上完成的，必须包含：
```c
#undef HAVE_LIBZ
```
如果 `HAVE_LIBZ` 由 `configure` 导出，则该行将被注释掉。然后，`C++` 代码可以通过 `#include <config.h>` 使用宏。

2. 在不同源文件之间选择
如：首先更改 `Makefile.in` 以便可以使用从 `configure` 获得参数
```
objs=@objs@

program: program.o $(objs)
	$(LD) -o $@ $^ $(LDFLAGS) $(LIBS)

```
更改 `configure.ac` ，以便使用一个或林一个目标文件填充 `@objs@` 。
```c
dnl List of object files as determined by autoconf
object_files=""

a=0
AC_CHECK_HEADERS(zlib.h, [], [a=1])
AC_SEARCH_LIBS(deflate, z, [], [a=1])

if test $a == 0
then
	object_files="$object_files hello_libz.o"
else
	object_files="$object_files hello_no_libz.o"
fi

dnl Make AC_OUTPUT substitute the contents of $object_files for @objs@
AC_SUBST(objs, ["$object_files"])
```
现在 `configure` 将根据测试结果选择两个源文件之一进行编译和链接。

---
`autoconf` 常见宏

初始化和基本设置

1. `AC_INIT(package, version, bug-report)`
初始化 `autoconf` 脚本，定义包名、版本号和 bug 报告地址。
示例：
```m4
AC_INIT([MyProject], [1.0], [bug@example.com])
```

2. `AC_CONFIG_SRCDIR([file])`
指定用于检查源代码的文件，确保用户运行配置脚本的位置正确
```m4
AC_CONFIG_SRCDIR([main.c])
```

3. `AC_CONFIG_FILES([files])`
指定需要生成的文件（通常是 Makefile）
```m4
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
```

编译器和工具检测
1. `AC_PROG_CC`
检测 C 编译器（如 `gcc` 或 `clang`）
```m4
AC_PROG_CC
```

2. `AC_PROG_CXX`
检测 C++ 编译器
```m4
AC_PROG_CXX
```

3. `AC_PROG_LIBTOOL`
启用 GNU Libtool，用于生成共享库
```m4
AC_PROG_LIBTOOL
```

4. `AC_CHECK_TOOL(tool, program)`
检测特定工具（如 `ar` 或 `ld`）
```m4
AC_CHECK_TOOL([AR], [ar])
```

系统特性检测
1. `AC_CANONICAL_HOST`
检测宿主系统的架构、操作系统等信息

2. `AC_CANONICAL_TARGET`
检测目标系统的信息（主要用于交叉编译）

3. `AC_CHECK_LIB(library, function)`
检查某个库是否存在及其功能是否可用
```m4
AC_CHECK_LIB([m], [sqrt], [], [AC_MSG_ERROR([libm not found])])
```

4. `AC_CHECK_HEADER(header)`
检查头文件是否存在
```m4
AC_CHECK_HEADER([stdio.h], [], [AC_MSG_ERROR([stdio.h not found])])
```

5. `AC_CHECK_FUNC(function)`
检查函数是否存在
```m4
AC_CHECK_FUNC([printf], [], [AC_MSG_ERROR([printf not found])])
```

配置选项
1. `AC_ARG_ENABLE(option, help-string, [action-if-given], [action-if-not-given]`
添加 `--enable-option` 配置选项
```m4
AC_ARG_ENABLE([debug], [--enable-debug Enable debug mode], [CFLAGS="$CFLAG -g"], [])
```

2. `AC_ARG_WITH(option, help-string, [action-if-given], [action-if-not-given])`
添加 `--with-option` 配置选项
```m4
AC_ARG_WITH([libfoo],
	[--with-libfoo=PATH Specify libfoo path],
	[LIBFOO_PATH=$withval], [LIBFOO_PATH=/usr/lib])
```

输出信息
1. `AC_MSG_CHECKING([message])`
输出 "checking ..." 的消息（用于检查进度）
```m4
AC_MSG_CHECKING([for gcc])
```

2. `AC_MSG_RESULT([message])`
输出检查结果
```m4
AC_MSG_RESULT([found])
```

3. `AC_MSG_ERROR([message])`
如果出现错误，停止配置并显示错误消息
```m4
AC_MSG_ERROR([gcc is required])
```

C/C++ 特性
1. `AC_C_INLINE`
检查 C 编译器是否支持 `inline`

2. `AC_C_CONST`
检查编译器是否支持 `const`

3. `AC_PROG_CC_STDC`
检查 C 编译器是否支持 ANSI C 标准

条件处理
1. `AC_DEFINE(variable, value, description)`
在配置生成的 `config.h`中定义宏
```m4
AC_DEFINE([ENABLE_DEBUG], [1], [Enable debug mode])
```

2. `AC_DEFINE_UNQUOTED(variable, value, description)`
定义未加引号的宏
```m4
AC_DEFINE_UNQUOTED(MY_MACRO, 100, [An example macro])
```

3. `AC_SUBST(variable)`
替换模板文件中变量
```m4
PREFIX=/usr/local
AC_SUBST([PREFIX])
```

常见扩展宏
1. `PKG_CHECK_MODULES([module],[modules])`
使用 `pkg-config` 检测模块（需要 `pkg.m4`）
```m4
PKG_CHECK_MODULES([GTK], [gtk+-3.0])
```

2. `AM_INIT_AUTOMAKE([options])`
初始化 `automake`
```m4
AM_INIT_AUTOMAKE([foreign])
```

