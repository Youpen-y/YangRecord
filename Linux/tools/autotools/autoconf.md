### 概览
`autoconf` 是一个生成 shell 脚本的工具，这些脚本可以用来自动配置软件源代码包以适应多种 Posix-like 系统。
本质上 `autoconf` 是一个编译器，将 `configure.ac` 脚本编译为 shell 脚本，然后该脚本可在任何没有 `autoconf` 的平台上运行。`configure.ac` 使用 `M4sh` 语言，其中包含各种 M4 宏。在运行 `autoconf` 时会使用 `M4` 预处理器运行 `configure.ac` 脚本，所有宏都被扩展（包含相应的 shell 代码），生成纯 shell 代码。

工作方式
1. 创建一个 `configure.ac` 脚本，在其中定义各种设置，如版本名称、版本、要使用的编译器以及输出文件的位置。
2. 使用 `autoconf` 运行它以创建 `configure` 脚本（`autoconf` 作用是从系统收集系统以填充使用 `automake` 创建的 `Makefile.in` 模板） 

`Autoconf` 生成的配置脚本在运行时不需要用户手动干预；通常甚至不需要指定系统类型的参数。相反，他们单独测试其所使用的软件包可能需要的每个功能是否存在。

`Autoconf` 使得用户使用更轻松。主要目标不是为软件包维护者自动生成 `configure` 。

`Autoconf` 脚本是使用 `shell` 和 `M4`（一种比 C 预处理器功能强大得多的宏处理语言）组合编写的，使用 `m4` 宏。需要 GNU M4 版本 1.4.8 或更高版本才能生成脚本。


---
#### 一些基础
- `Autoconf` 等价于 `M4` with more machinery，并且更多预定义的宏。
- `Autoconf` M4 宏格式多为：`A[CST]_??????` 。也可能有一些以 `_` 开头的私有版本。
- 配置脚本名称通常为 `configure.ac` 。
- `Autoconf`使用 `[` 和 `]` 引用参数（而 `M4` 采用引号）。其中无法转义，如果想插入 `[` 和 `]` 可采用如下方案 
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

#### 初始化和基本设置

1. `AC_INIT(package, version, [bug-report-adddress], [tarname], [url])`
初始化 `autoconf` 脚本，定义包名、版本号和 bug 报告地址。
示例：
```m4
AC_INIT([MyProject], [1.0], [bug@example.com])
```

2. `AC_PREREQ(version)`
需要的 Autoconf 的最小版本。
```bash
AC_PREREQ([2.65])
```

3. `AC_CONFIG_SRCDIR([file])`
指定用于检查源代码的文件，确保用户运行配置脚本的位置正确
```m4
AC_CONFIG_SRCDIR([src/main.c])
```

4. `AC_CONFIG_AUX_DIR(dir)`
辅助脚本如 `install-sh` 和 `depcomp` 等存放目录
```m4
AC_CONFIG_AUX_DIR([build-aux])
```

5. `AC_CONFIG_FILES(files..., [cmds], [init-cmds])`
指定需要生成的文件（通常是 Makefile），files 中包含 `AC_SUBST` 生成的定义。`cmds` 和 `init-cmds` 很少使用
```m4
# AC_CONFIG_FILES([Makefile sub/Makefile script.sh:script.in])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
```
`Automake` 会为有 `FILE.am` 的 `FILE` 创建模板 `FILE.in`，但也允许处理`non-Makefiles` （非 Makefile）。

下面是一个模板示例（`script.in`），其中 `@XYZ@` 是 `AC_SUBST([XYZ])` 定义的占位符。
```shell
#!/bin/sh

SED='@SED@'
TAR='@TAR@'
d=$1; shift; mkdir "$d"
for f; do
    "$SED" 's/#.*//' "$f" > "$d/$f"
done
"$TAR" cd "$d.tar" "$d"
```
相应的 `script.sh`，`config.status` 完成替换
```sh
#!/bin/sh

SED='/usr/xpg4/bin/sed'
TAR='/usr/bin/tar'
d=$1; shift; mkdir "$d"
for f; do
    "$SED" 's/#.*//' "$f" > "$d/$f"
done
"$TAR" cf "$d.tar" "$d"
```

6. `AC_CONFIG_HEADERS(headers...)`
通常，C/C++ 代码需要知道 `configure` 脚本所做测试的结果。一个模板 C 头文件将自动由 `autoheader` 产生，通常命名为 `config.h.in` 。但最终头文件由 `configure` 产生，通常命名为 `config.h` 。
为所有的 `HEADER.in` 创建头文件。仅应使用一个头文件除非你知道自己在做什么（`autoheader` 仅为第一个 `HEADER` 创建 `HEADER.in`）。`HEADERS` 中包含 `AC_DEFINE` 生成的定义。
```m4
AC_CONFIG_HEADERS([config.h])   # 从 config.h.in 中创建 config.h；
```

7. `AC_CONFIG_SUBDIRS(dirs...)`
在构建系统中包含并递归配置子项目（子目录）。当你在一个项目中嵌套使用其他独立的 `autotools` 项目（如子库、插件等），使用该宏。
- 它将告诉 `configure` 脚本：进入这些子目录中也执行各自的 `configure` 脚本。
- 子目录中通常有独立的 `configure.ac`，`Makefile.am` 等，自己构成一个小项目。
- 常用于：多模块项目、第三方源码包嵌入

#### 编译器和工具检测
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

5. `AC_CHECK_PROGS(var, progs, [var-if-not-found])`
定义 `var` 为第一个发现的 `PROGS` ，否则为 `var-if-not-found`
```m4
AC_CHECK_PROGS([TAR], [tar gtar], [:])
if test "$TAR" = :; then
    AC_MSG_ERROR([This package needs tar.])
fi
```

6. `AC_CHECK_DECLS`
检查声明

7. `AC_CHECK_MEMBERS`
检查结构成员

8. `AC_CHECK_TYPES`
检查类型：`AC_CHECK_TYPES(int8_t)`
#### 系统特性检测
1. `AC_CANONICAL_HOST`
检测宿主系统的架构、操作系统等信息

2. `AC_CANONICAL_TARGET`
检测目标系统的信息（主要用于交叉编译）

3. `AC_CHECK_LIB(library, function, [act-if-found], [act-if-not])`
检查某个库 `library` 是否存在及其函数 `function` 是否可用。如果库存在且函数可用执行 `act-if-found`，否则 `act-if-not`。
```m4
AC_CHECK_LIB([m], [sqrt], [], [AC_MSG_ERROR([libm not found])])
```
如果 `act-if-found` 未设置并且该库被找到，`AC_CHECK_LIB` 将会执行 `LIBS="-lLIBRARY $LIBS` 并 `#define HAVE_LIBLIBRARY` 。
> `Automake` 使用 `$LIBS` 链接所有东西。

4. `AC_CHECK_HEADER(header, [action-if-found], [action-if-not-found], [includes])` 或 `AC_HEADER_*` 宏
检查头文件 `header` 是否存在，如果发现了头文件，`#define HAVE_HEADER_H 1` 
```m4
AC_CHECK_HEADER([stdio.h], [], [AC_MSG_ERROR([stdio.h not found])])
AC_CHECK_HEADERS([sys/param.h unistd.h])
AC_CHECK_HEADERS([wchar.h])
```
可以在源文件中利用 `HAVE_SYS_PARAM_H`、`HAVE_UNISTD_H` 和 `HAVE_WCHAR_H`。
```c
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
```

示例2：
```configure.ac
[...]
AC_CHECK_HEADERS([spawn.h], 
    [echo "Header spawn.h was found"; has_spawn=yes], 
    [echo "Header spawn.h was not found"]) 
echo ${has_spawn} 
[...]
```
执行 `./configure`
```bash
$ ./configure 
[...] 
checking for spawn.h... yes 
Header spawn.h was found 
yes 
[...]
```

`AC_CHECK_HEADERS([...], [...])`：依次检查多个头文件是否存在；如果其中任意一个头文件存在，执行第二个参数中的 shell 代码

5. `AC_CHECK_FUNC(function, [action-if-found], [action-if-not-found])`
检查函数是否存在
```m4
AC_CHECK_FUNC([printf], [], [AC_MSG_ERROR([printf not found])])
```
检查多个函数：`AC_CHECK_FUNCS(functions..., [action-if-found], [action-if-not-found])`
- 在 `configure.ac` 中产生 `ac_cv_func_<function>` 变量
- 在配置头文件（`config.h`）中定义 `HAVE_<FUNCTION>`
一些 `AC_FUNC_*` 宏：`AC_FUNC_FORK`, `AC_FUNC_GETLOADAVG`, `AC_FUNC_MALLOC` 等

6. `AC_SEARCH_LIBS(function, search-libs, [action-if-found], [action-if-not-found], [other-libraries])`
通过链接一个调用 `function` 的简单程序查找定义 `function` 的库；首先尝试无库，然后一个接一个的使用 `search-libs` 中不同的库。
如果找到库，则 `-llibrary` 会附加到 `LIBS` 变量前面，因此程序将链接到它。`action-if-found` 被执行。如果没找到，执行 `action-if-not-found`。
`other-libraries` 允许传递链接测试成功可能需要的额外的 `-l<foo>` 参数。
> 查找库更好的解决方案是 `pkg-config`

#### 配置选项
7. `AC_ARG_ENABLE(option, help-string, [action-if-given], [action-if-not-given]`
添加 `--enable-option` 配置选项
- `option`：参数名
- `help-string`：当运行 `./configure --help` 时用于描述参数的字符串。直接传递给宏可能会导致文本无法对齐或正确填充，通常使用 `AS_HELP_STRING` 参数来创建字符串
- `action-if-given`：用户指定 `--enable-foo` 传递参数时使用的 `M4sh` 代码；如果有参数值，可以通过 `$enableval`（`--with` 对应 `$withval`）引用。
- `action-if-not-given`：没有指定任何参数时执行的 `M4sh` 代码，允许为变量设置默认值
```m4
AC_ARG_ENABLE([debug], [--enable-debug Enable debug mode], [CFLAGS="$CFLAG -g"], [])
```

2. `AC_ARG_WITH(option, help-string, [action-if-given], [action-if-not-given])`
添加 `--with-option` 配置选项
```m4
AC_ARG_WITH([libfoo],
	[--with-libfoo=PATH Specify libfoo path],
	[LIBFOO_PATH=$withval], [LIBFOO_PATH=/usr/lib])

# 提供 --with-libfoo 时，LIBFOO_PATH 被赋值，否则采用默认值 /usr/lib
```



#### 输出信息
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

3. `AC_MSG_NOTICE([message])`
发送消息给用户

4. `AC_MSG_ERROR(error-description, [exit-status = $?/1])`
如果出现错误，停止配置并显示错误消息
```m4
AC_MSG_ERROR([gcc is required])
```

4. `AC_MSG_WARN([message])`
打印信息，但不停止配置

#### C/C++ 特性
1. `AC_C_INLINE`
检查 C 编译器是否支持 `inline`

2. `AC_C_CONST`
检查编译器是否支持 `const`

3. `AC_PROG_CC_STDC`
检查 C 编译器是否支持 ANSI C 标准

#### 条件处理
1. `AC_DEFINE(variable, value, description)`
在配置生成的头文件（通常为`config.h`）中定义宏
```m4
AC_DEFINE([ENABLE_DEBUG], [1], [Enable debug mode])
```
将在 `config.h` 中生成
```c
/* Enable debug mode */
#define ENABLE_DEBUG 1
```

2. `AC_DEFINE_UNQUOTED(variable, value, description)`
定义未加引号的宏
```m4
AC_DEFINE_UNQUOTED(MY_MACRO, 100, [An example macro])
```

3. `AC_SUBST(variable, value)`
替换模板文件中变量。
```m4
PREFIX=/usr/local
AC_SUBST([PREFIX])
```
下述三种表示是等价的：
```
// 1
AC_SUBST([FOO], [foo])
// 2
FOO=foo
AC_SUBST([FOO])
// 3
AC_SUBST([FOO])
FOO=foo
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
选项：
- `foreign`：告诉 `automake` 不需要所有 GNU Coding Style 文件，如 NEWS，README，AUTHORS 等
- `dist-bzip2, dist-xz`：告诉 `automake` 在执行 `make dist` 时产生那种 tarball 格式
- `subdir-objects`：告诉 `automake` 对象被放置在与源文件的子目录相对应的构建目录的子目录中
- `version`：期望的最小 `automake` 版本（如 `1.14.1`）

#### 测试
虽然 `autoconf` 提供了许多预先编写的测试来识别头文件、符号和库的存在，但这显然无法覆盖软件开发人员在识别软件构建环境可能遇到的所有情况。
`autoconf` 还提供了编写自定义测试例程的接口，分为两类："build tests" 和 "run  tests"。取决于测试如何进行。
##### 构建测试（build tests）
检查是否给定的源代码被正确预处理、编译或链接。是高级宏（如 `AC_CHECK_HEADERS`，`AC_CHECK_FUNCS`）的底层实现机制，也可手动使用来做更灵活或自定义的检测。
- 预处理测试：`AC_PREPROC_IFELSE(input, [action-if-true], [action-if-false])`
- 编译测试：`AC_COMPILE_IFELSE(input, [action-if-true], [action-if-false])
- 链接测试：`AC_LINK_IFELSE(input, [action-if-true], [action-if-fasle])`
`input` 使用 `AC_LANG_SOURCE` 或 `AC_LANG_PROGRAM` 格式化。
```m4
# 只验证 `#include <stdio.h>` 在预处理阶段是否可用，不编译、不链接
AC_PREPROC_IFELSE(
	[AC_LANG_SOURCE([[#include <stdio.h>]])],
	[AC_MSG_RESULT([stdio.h is available for preprocessing])],
	[AC_MSG_ERROR([Cannot preprocess stdio.h])]
)

# 检查是否能编译特定语法或声明，常用于检查关键字、语法、类型等
AC_COMPILE_IFELSE(
	[AC_LANG_SOURCE([[
		int main(void) {
			int a = 1;
			return 0;
		}
	]])],
	[AC_MSG_RESULT([Code compiled successfully])],
	[AC_MSG_ERROR([Compilation failed])]
)

# 检查能否链接一个函数（如 dlopen）
AC_LINK_IFELSE(
	[AC_LANG_SOURCE([[
		#include <dlfcn.h>
		int main() {
			dlopen(0, 0);
			return 0;
		}
	]])],
	[AC_MSG_RESULT([dlopen is linkable])],
	[AC_MSG_ERROR([dlopen linkage failed])]
)
```

|宏名|操作|作用|
|---|---|---|
|`AC_PREPROC_IFELSE`|只做预处理|检查头文件是否能被找到和正确展开|
|`AC_COMPILE_IFELSE`|预处理 + 编译|检查代码能否通过语法检查（是否能被编译）|
|`AC_LINK_IFELSE`|预处理 + 编译 + 链接|检查代码是否能成功链接（是否能构建一个完整的程序）|
`AC_PREPROC_IFELSE`不推荐使用，当直接调用时，预处理器缺少编译器前端设置的一些定义，一些功能可能在两者之间表现不一致，“构建测试”对于交叉编译时安全的，只要系统中存在适当的编译器和链接器。

`AC_LANG_SOURCE`：直接嵌入源码片段
```m4
AC_LANG_SOURCE([code])
```
- 参数 `code` 是完整的测试源代码字符串（必须包含 `main()` 函数，能独立编译/链接）

`AC_LANG_PROGRAM`：自动构建 `main()`，适合函数调用测试
```m4
AC_LANG_PROGRAM([prologue], [body])
```
- `prologue`：通常是 `#include` 指令或全局声明
- `body`：`main()` 函数的内容
- 自动生成结构：
```c
[prologue]
int main() {
	[body]
}
```


##### 运行时测试
使用宏 `AC_RUN_IFELSE`：用于编译、链接并运行一段测试程序，从而判断系统功能或特性的存在或行为。结构如下：
```m4
AC_RUN_IFELSE(
	[program],     # 编译并运行的测试程序
	[action-if-true],
	[action-if-false],
	[action-if-cross-compiling]    # 当处于交叉编译环境下，无法运行测试程序，执行的第三个分支
)
```
示例：
```m4
AC_RUN_IFELSE(
	[AC_LANG_PROGRAM([#include <stdio.h>],
	                 [return sizeof(void *) == 8 ? 0 : 1;])],
	[AC_MSG_RESULT([64-bit platform])],
	[AC_MSG_RESULT([Not a 64-bit platform])],
	[AC_MSG_WARN([Cannot run test program: cross-compiling])]
)
```

注意：`AC_RUN_IFELSE` 依赖本机能运行测试程序，所以适合本地构建，但不适合交叉编译（如目标是 `ARM/Linux`，而用的是 `x86/Linux`）。

#### 缓存
`autoconf` 做的每个测试通过与缓存变量相关。
- 允许通过传递具有预定义值的缓存文件来加快配置步骤。
- 如果测试结果由于某种原因不正确，则允许覆盖测试结果

1. `AC_CACHE_VAL(cache-id, commands-to-set-it)`
如果 `cache-id` 未设置，运行 `commands`，`commands` 设置 `cache-id` 变量且无副作用。

2. `AC_CACHE_CHECK(message, cache-id, commands)`
包装 `AC_CACHE_VAL` 以打印 `message`


#### 选项
1. 如何控制是否使用外部软件（或依赖）？
通常通过 `--with-<package>=<arg>` / `--without-<package>` 选项。如下示例：
```bash
./configure --with-openssl=/usr/local
./configure --without-gtk
```

这是如何实现的？—— By `AC_ARG_WITH` 宏
```m4
AC_ARG_WITH([<package>],
    [help-string],
    [action-if-given],
    [action-if-not-given])
```
参数说明：

| 参数                    | 说明                                            |
| --------------------- | --------------------------------------------- |
| `<package>`           | 包名，例如 `openssl`，对应 `--with-openssl`           |
| `help-string`         | 用于 `./configure --help` 显示的说明文字               |
| `action-if-given`     | 如果用户指定了 `--with-xxx` 或 `--without-xxx`，则执行此代码 |
| `action-if-not-given` | 用户未指定任何 `--with` 选项时执行的默认逻辑                   |
在 `action-if-given` 内部可使用 `$withval` 变量，在外部可使用 `$with_<package>`（值为：`yes/no/<path>`） 。


2. 包选项
当包提供可选特征时，可以通过 `--enable-<feature>` 和 `--disable-<feature>` 选项来控制特征。
这通过 `AC_ARG_ENABLE` 宏实现
```m4
AC_ARG_ENABLE(feature, help-string,
            [action-if-given], [action-if-not-given])
```
参数与 `AC_ARG_WITH` 类似，在 `action-if-given` 内部，可使用 `$enableval` 变量，外部可使用 `$enable_<feature>` 。

3. 格式化 `help-string`
`AS_HELP_STRING` 宏帮助格式化 `./configure --help` 输出中的说明文本，让所有选项整齐对其，提升可读性。
```m4
AS_HELP_STRING(left-hand-side, right-hand-side,
               [indent-column = 26],
               [wrap-column = 79])
```

| 参数                  | 含义                        |
| ------------------- | ------------------------- |
| `left-hand-side`    | 配置选项，比如 `--with-foo=PATH` |
| `right-hand-side`   | 对这个选项的说明                  |
| `indent-column`（可选） | 说明文本从第几列开始（默认是第 26 列）     |
| `wrap-column`（可选）   | 超出多少列后换行（默认 79）           |

示例：与 `AC_ARG_WITH` 结合使用
```m4
AC_ARG_WITH([foo],
    [AS_HELP_STRING([--with-foo=PATH], [Use foo installed in PATH])],
    [foo_path=$withval],
    [foo_path=/usr/local])
```
这样，`./configure --help` 中就会显示：
```ini
    --with-foo=PATH     Use foo installed in PATH
```

- `AC_ARG_VAR(VARIABLE, DESCRIPTION)`
声明可以被设置的环境变量，在 `./configure --help` 时的环境变量部分输出。
`VARIABLE`：希望用户可以设置的变量名（如 `CC`，`CFLAGS`，`PKG_CONFIG`，...）
`DESCRIPTION`：该变量的说明，会出现在 `./configure --help` 的输出中


- `AC_PATH_PROG(VARIABLE, PROGRAM, [VALUE-IF-NOT-FOUND], [PATH]`
在系统 PATH 中查找 `PROGRAM` 程序
参数：

| 参数                   | 说明                      |
| -------------------- | ----------------------- |
| `VARIABLE`           | 要设置的 shell 变量名          |
| `PROGRAM`            | 要查找的程序名称（可执行文件）         |
| `VALUE-IF-NOT-FOUND` | 如果找不到程序，使用的默认值（可选）      |
| `PATH`               | 用于搜索的目录列表，默认为系统的 `PATH` |
示例：
```m4
AC_PATH_PROG([PKG_CONFIG], [pkg-config], [no])
```
含义：
- 在系统的 `PATH` 中查找名为 `pkg-config` 的程序
	- 找到了，设置变量 `PKG_CONFIG` 为其绝对路径
	- 没找到，设置为 `no`

实际用例：假设构建项目时依赖自定义工具 `foo-tool`:
```m4
AC_ARG_VAR([FOO_TOOL], [path to foo-tool])
AC_PATH_PROG([FOO_TOOL], [foo-tool], [/usr/local/bin/foo-tool])
```
```bash
$ ./configure FOO_TOOL=/opt/bin/foo-tool
# 如果没有设置，`AC_PATH_PROG` 会尝试在 `PATH` 中查找 foo-tool
# 找不到就使用 /usr/local/bin/foo-tool 作为默认值
```

#### 其他
- 切换语言环境 - `AC_LANG_PUSH(language)` 与 `AC_LANG_POP()`
```m4
AC_LANG_PUSH(language)
... # 在这个语言环境下执行的检测代码
AC_LANG_POP()
```
通常用于临时切换语言做某些测试，然后恢复回原来的语言。
`language`：`C`, `C++`, `Fortran`, `Fortran 77`, `Objective C`, `Objective C++` 等

- `AC_CONFIG_MACRO_DIR`：声明附加宏文件的存放位置和查找位置。
```m4
AC_CONFIG_MACRO_DIR([m4])
```

- `m4_include`：用于在 `configure.ac` 文件中包含其他的 M4 宏文件内容。类似于 C 语言中 `#include` 指令，简单地复制文件的内容。
示例：
假设你有一个名为 `my_macros.m4` 的文件放在 `m4/` 目录中，内容如下：
```m4
AC_DEFUN([MY_CHECK_FOO], [
	AC_CHECK_HEADERS([foo.h])
	AC_CHECK_LIB([foo], [foo_init])
])
```
可以在 `configure.ac` 中这样用：
```m4
AC_INIT([myapp], [1.0])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])

m4_include([m4/my_macros.m4])

MY_CHECK_FOO

AC_OUTPUT
```

- `AC_PROG_LN_S`：查找软链接命令
检测系统是否支持 `ln -s` （创建符号链接）。如果不支持，会退回使用 `cp` 复制文件的方式。定义一个变量 ￼￼LN_S
```m4
AC_PROG_LN_S
```
结果：将定义一个变量 `LN_S` ，值为：
- 系统支持符号链接：`ln -s`
- 系统不支持 `ln -s`，为 `ln` 或 `cp -p`
示例：
```make
install:
    $(LN_S) $(srcdir)/config.h $(DESTDIR)/config.h
```

- `AC_PROG_MKDIR_P`：查找 `mkdir -p` 的可移植实现
检测可用的创建多级目录的命令，一般为 `mkdir -p` 。
使用方式：
```m4
AC_PROG_MKDIR_P
```
结果：将定义一个变量 `MKDIR_P` ，值为：
- `/bin/mkdir -p`
- `mkdir -p`
- 或 fallback 脚本路径（如果系统不直接支持）
示例：
```make
install:
    $(MKDIR_P) $(DESTDIR)$(bindir)
```

- `AC_USE_SYSTEM_EXTENSIONS`：启用常见系统扩展（如 GNU, BSD 等）以获得更多标准库特性和 POSIX 以外的功能。
在一些平台上，很多功能（如 `strdup()`，`getline()`）默认是不可见的。必须定义宏如：
- `_GNU_SOURCE`（GNU 系统扩展）
- `_DEFAULT_SOURCE`（glibc 新版本）
- `_BSD_SOURCE`（已废弃，兼容 BSD）
这个宏会自动添加这些定义：
```c
#define _GNU_SOURCE 1
#define _DEFAULT_SOURCE 1
```

- `AC_SYS_LARGEFILE`：启用对大文件（> 2GB）的支持
某些平台默认使用 32 位 `off_t` ，无法处理大于 2GB 的文件。该宏会自动定义：
```c
#define _FILE_OFFSET_BITS 64
#define _LARGE_FILES 1
```
使系统头文件和库函数切换到 64 位版本，如：
`fopen()` -> `fopen64()`
`stat()` -> `stat64()`

- `AM_MAINTAINER_MODE([option])`：控制维护者模式（maintainer mode）的宏。
控制是否自动执行某些只对开发者或维护者有用的规则，如：
- 自动更新 `configure`、`Makefile.in`、`aclocal.m4` 等生成文件；
- 自动重新运行 `autoconf`、`automake`、`aclocal`、`autoheader`；
- 依赖 `.in` 文件的规则。
选项：
- `enable`，默认启用维护者规则（用户可通过 `--disable-maintainer-mode` 禁用）
- `disable`（默认），禁用维护者规则

----



