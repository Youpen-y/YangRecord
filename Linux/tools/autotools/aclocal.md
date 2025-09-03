`aclocal` - 通过扫描 `configure.ac`（或旧的 `configure.in`） 生成 `aclocal.m4`

`aclocal` 搜索范围：
- 使用 `-I` 选项指定的目录
- 三方包可能用于安装它们宏的系统范围目录（`/usr/share/aclocal`）
- `automake` 的私有宏目录

---
在包中管理自定义的宏，步骤：
- 创建  `m4/` 子目录，用于存放 `.m4` 宏文件
- 自定义的宏如 `AX_FUNC_MKDIR` 应该保存在 `m4/` 下的 `.m4` 文件中
- 在项目根目录下的 `Makefile.am` 中添加 `ACLOCAL_AMFLAGS = -I m4` ：告诉 `aclocal` 去包含 `m4/` 目录中的自定义宏文件
- 在 `configure.ac` 中使用定义的宏
`autoreconf -i` 或 `make` 自动触发重新生成 configure 文件时，都会用到 `ACLOCAL_AMFLAGS`，确保包含你的自定义宏。
如果你的某个 `.m4` 文件中定义的宏在 `configure.ac` 中被使用，它会被 `make dist` 自动打包；如果没被使用，就会被忽略。

---

`autoconf` 仅知道其提供的宏，如（`m4_*`，`AS_*`，`AH_*`，`AC_*`，`AT_*`）；不知道三方工具提供的宏（如，`automake` 的 `AM_*` 宏）。
> `automake` 包含许多可以在您的包中使用的 `autoconf` 宏。这些宏必须在 `aclocal.m4` 中定义，否则不会被 `autoconf` 看到。（即，除了 `configure.ac`，`autoconf` 还读取 `aclocal.m4`）

`aclocal` 提供了一种获取 `automake` 提供的宏的便捷方法，而无需到处搜索。同时，`aclocal` 机制允许其他包提供自己的宏，可以使用它来维护自定义的宏。

启动时，`aclocal` 会扫描可以找到的所有 `.m4` 文件以寻找宏定义；然后它扫描 `configure.ac`，将定义的宏以及需要的宏放入 `aclocal.m4`。

---
`autoconf` 宏是构建 `configure` 脚本的核心，它们的作用是：自动检测编译环境的特性，如函数是否存在、库是否可用、类型是否定义等。
`autoconf` 已经提供了很多标准宏，如：
- `AC_CHECK_HEADER([stdio.h])`
- `AC_CHECK_FUNCS([malloc strdup])`
- `AC_PROG_CC`
但有时需要为自己项目的特殊需求编写自定义宏（放在 `m4/` 目录）。
两种可编写类型宏：
类型1：高层（高抽象）宏
特点：
- 只是把多个已有宏**组合封装成一个新名字**，用于简化或复用配置逻辑
- 不直接写 shell 代码
- 不执行实际测试，只调用已有的宏
- 常用于清晰结构或减少重复代码
如：
```m4
AC_DEFUN([MY_CHECK_STANDARD_DEPS], [
	AC_CHECK_HEADER([stdio.h])
	AC_CHECK_FUNCS([malloc free])
])
```
然后可以在 `configure.ac` 中这样用：
```m4
MY_CHECK_STANDARD_DEPS
```

类型2：低层（基础）宏
特点：
- 真正执行新的 configure 检测逻辑
- 编写实际的 shell 代码、编译测试片段、检查结果
- 需要用 `AC_CACHE_CHECK` 或 `AC_CACHE_VAL` 来缓存结果（避免每次重复检测）
- 结果要以 `AC_DEFINE` 或 `AC_SUBST` 等方式输出
示例：
```
AC_DEFUN([MY_CHECK_BIG_ENDIAN], [
  AC_CACHE_CHECK([whether system is big endian], [my_cv_big_endian],
    [my_cv_big_endian=no
     AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
       [[
         unsigned int i = 1;
       ]], [[
         return (*((char*)&i) == 0);
       ]]
     )], [my_cv_big_endian=yes])
    ])
  
  if test "$my_cv_big_endian" = yes; then
    AC_DEFINE([IS_BIG_ENDIAN], [1], [Define if the system is big endian])
  fi
])
```
该宏就是真正在检测系统字节序，是真正做检测的“底层宏”。
注意：低层宏要注意性能和可重复性，必须使用 `AC_CACHE_CHECK` 来缓存结果，否则每次运行 `configure` 都会重新编译测试代码，很慢。

定义宏：`AC_DEFUN(MACRO-NAME, MACRO-BODY)`  —— 定义 `MACRO-NAME` 为 `MACRO-BODY`
需注意避免可能冲突的名称。宏命名空间：
`m4_`：原始 M4 宏，以及 `M4sugar` 宏。
`AS_`：M4sh 宏（宏化 shell 结构）
`AH_`：Autoheader 宏
`AC_`：Autoconf 宏（在上述层之上）
- `AC_CHECK_`：普通检查
- `AC_FUNC_`：特定的函数检查
- `AC_HEADER_`：特定的头文件检查
- `AC_PROG_`：特定的程序检查
- ...
`AM_`：`automake` 宏
`AT_`：`autotest` 宏

以 `mkdir()` 为例：
POSIX 系统上的 `mkdir()` 需要两个参数；而 Mingw32 的 `mkdir()` 需要仅一个参数，同样 Win32 上 的 `_mkdir()` 也接受一个参数。
```
#if HAVE_MKDIR
# if MKDIR_ONE_ARG
#  define mkdir(a,b) mkdir(a)
# endif
#else
# if HAVE__MKDIR
#  define mkdir(a,b) _mkdir(a)
# else
#  error "Don't know how to create a directory."
# endif
#endif
```
可以写一个 `autoconf` 宏来定义这些宏：`HAVE_MKDIR, MKDIR_ONE_ARG, HAVE__MKDIR`
示例：高级宏 `AX_FUNC_MKDIR`
```
AC_DEFUN([AX_FUNC_MKDIR],
[AC_CHECK_FUNCS([mkdir _mkdir]) # 定义 HAVE_MKDIR 和 HAVE__MKDIR
AC_CHECK_HEADERS([io.h]) # 如果 io.h 存在，定义 HAVE_IO_H
AX_FUNC_MKDIR_ONE_ARG # 定义 MKDIR_ONE_ARG..
])
```
- `AX_` 表明扩展宏的命名空间，`FUNC_` 使用 `autoconf` 分类宏的相同常规。

示例：
```m4
# _AX_FUNC_MKDIR_ONE_ARG(IF-ONE-ARG, IF-TWO-ARGS)
# -----------------------------------------------
# Execute IF-TWO-ARGS if mkdir() accepts two arguments;
# execute IF-ONE-ARG otherwise.   （这里是记录 autoconf 宏的传统方式）
AC_DEFUN([_AX_FUNC_MKDIR_ONE_ARG],
[AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/stat.h>
#if HAVE_UNISTD_H
# include <unistd.h>
#endif
#if HAVE_IO_H
# include <io.h>
#endif
]], [[mkdir (".", 0700);]])], [$2], [$1])]
)
```
其中 `AC_COMPILE_IFELSE` 将创建一个小程序并尝试编译它。它将依据是否编译成功执行其中一个 `_AX_FUNC_MKDIR_ONE_ARG` 的参数。
使用 `_AX` 前缀用于不打算直接使用的 helper macros 。

---
#### 写一个低级宏
低级宏需要
- 打印*checking whether ...* 消息
- 做真正的检查
- 缓存检查结果
`configure -C` 在 `config.cache` 中缓存结果以加速重建。
上述需要通过 `AC_CACHE_CHECK` 宏实现：（语法）
```m4
AC_DEFUN(MACRO-NAME,
[AC_CACHE_CHECK(WHETHER-MESSAGE,
                CACHE-VARIABLE,     # CACHE-VARIABLE 匹配 *_cv_*
                CODE-TO-SET-CACHE-VARIABLE) # 包含检查，当缓存被使用时将跳过
CODE-USING-CACHE-VARIABLE       # 总是执行，这里使用 `AC_SUBST` 和 `AC_DEFINE`
])
```

低级宏示例：（`AX_FUNC_MKDIR_ONE_ARG`）
```m4
AC_DEFUN([AX_FUNC_MKDIR_ONE_ARG],
[AC_CACHE_CHECK([whether mkdir takes one argument],
                [ax_cv_mkdir_one_arg],
[_AX_FUNC_MKDIR_ONE_ARG([ax_cv_mkdir_one_arg=yes],
                        [ax_cv_mkdir_one_arg=no])])                 
if test x"$ax_cv_mkdir_one_arg" = xyes; then
    AC_DEFINE([MKDIR_ONE_ARG], 1,
              [Define if mkdir takes only one argument.])
fi])    # AX_FUNC_MKDIR_ONE_ARG
```
- `AC_CACHE_CHECK`
	- 打印 *checking whether mkdir...*
	- 做检查（除非已经做过了）
	- 在 `ax_cv_mkdir_one_arg` 中缓存结果

将配置 actions （if语句）保留在 `AC_CHECK_CHECK` 之外，无论检查是运行还是缓存，都必须执行它们。

#### 编写 `autoconf` 宏的建议
1. Test for features, not for systems. 检测“系统提供了什么功能，而不是硬编码是那个系统”。
```m4
# 不要这样做
if test "$host_os" = "mingw32"; then
    AC_DEFINE([NEED_SPECIAL_MKDIR], 1, [Special mkdir for Windows])
fi

# 而应该这样
AC_CHECK_FUNCS([mkdir])
AC_CHECK_DECLS([mkdir], [], [], [[#include <sys/stat.h>]])
```

2. Avoid writing tests that are conditional on previous tests. 测试（`AC_CHECK_...`）应该是无条件的，避免依赖前一个测试的结果来决定是否进行下一个测试。
```m4
# 不要这样做
if test "$have_mkdir" = yes; then
    AC_CHECK_HEADERS([sys/stat.h])
fi

# 而应该这样，所有测试都会出现在 config.log，方便调试
AC_CHECK_HEADERS([sys/stat.h])
if test "$ac_cv_header_sys_stat_h" = yes; then
    ...
fi
```

3. Do not reinvent the wheel. 
`autoconf` 自带了大量经过验证的宏（如 `AC_CHECK_FUNCS`, `AC_HEADER_STDC`, `AC_FUNC_MALLOC`, `AC_PROG_CC` 等等），应该尽量使用这些已有宏，而不是自己重新写相同功能的 shell 测试。

4. Remember to `[quote]`
在编写 `.m4` 宏时，一定要正确使用方括号 `[]` 来引用参数和代码块，避免过早展开或参数错误。方括号会让宏参数延迟展开。
```m4
# 错误示例
AC_DEFINE(VERSION, "1.0")

# 正确写法
AC_DEFINE([VERSION], ["1.0"], [Define the version])
```

5. Read the Portable Shell section of the Autoconf manual, before writing shell code.
Autoconf 里的 shell 代码必须写得“兼容各种古老或怪异的 Unix shell”，比如：
- 不使用 Bash 特性（如 `[[ ]]`, `${var,,}`）
- 使用 `test` 而不是 `[[]]`
- 不假设 `/bin/sh` 是 Bash
- 避免 `function` 关键字
- 用 `AS_IF` 而不是原始 `if`，更安全
所以在写 shell 检测逻辑时，请先看 Autoconf 文档中 Portable Shell 一节。

5. Test your macros on different systems.
	1. 在 `config.log` 中查看测试的详细过程、shell 命令、编译错误等
	2. 如果没有 BSD 系统、老版本系统或其他结构设备，可以通过 Google for "free shell account" 远程 SSH 登陆测试。