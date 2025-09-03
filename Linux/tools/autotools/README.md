Autotools = `autoconf` + `automake` + `libtool` + ... 的公共名称，是一种快速简单的源代码管理和打包工具，方便用户编译和安装软件。
- 通常被软件开发者（尤其是 C/C++）用来为不同的 Unix-like（POSIX-like）系统创建/发布“自动构建”的源代码。
`pkg-config` 系统是使用库的简单方法，但并不是所有库都使用它。
`aclocal` 将自动把 `m4/` 目录下的 `.m4` 宏包括进来。
```bash
aclocal -I m4
```
`libtool` 构建可移植 C/C++ 库的关键组件，处理共享和静态库的复杂性，在 `configure.ac` 中添加 `LT_INIT`。

可分为两类：
1. GNU Autoconf
- `autoconf`：根据 `configure.ac` 创建 `configure`
- `autoheader`：根据 `configure.ac` 创建 `config.h.in`
- `autoreconf`：以正确的顺序运行所有工具
- `autoscan`：扫描源文件以解决常见的可移植性问题，以及 `configure.ac` 中缺少的相关宏。
- `autoupdate`：更新 `configure.ac` 中的废弃的宏
- `ifnames`：从所有 `#if/#ifdef/...` 指定中搜集标识符
- `autom4te`：`Autoconf` 的心脏。它驱动 M4 并实现上述大多数工具使用的功能。不仅对创建 `configure` 文件有用
2. GNU Automake
- `automake`：从 `Makefile.am`s 和 `configure.ac` 中创建 `Makefile.in`s。
- `aclocal`：扫描 `configure.ac` 用于使用第三方宏，并在 `aclocal.m4` 中收集定义。


```
如果使用 pkg-config ？
1. 在 configure.ac 中添加 `PKG_CHECK_MODULES([DEPS], [list-of-libs])`   # list-of-libs 是使用空格分隔的库名列表；库名可以添加 `>= version_number`；
2. 在 Makefile.am 中添加变量 DEPS_CFLAGS & DEPS_LIBS
AM_CFLAGS = $(DEPS_CFLAGS)
AM_LIBS = $(DEPS_LIBS)
```

如今的惯例：使用 `m4` 子目录存放 `m4` 文件。How to do that?
```
1. 创建 m4 子目录
mkdir m4

2. 修改 configure.ac ，告诉 autoconf 使用它
AC_CONFIG_MACRO_DIR([m4])

3. 修改 Makefile.am
ACLOCAL_AMFLAGS = -I m4 --install
```

项目示例：
```css
your-project/
|--- configure.ac
|--- Makefile.am
|--- m4/            <-- 放自定义或第三方 .m4 宏文件
|    └── ax_check_openssl.m4
|--- src/
|    └── main.c
└── ...
```


---
最终用户的视角（`./configure` 产生 Makefile ；调用 make ，make install 安装）
```bash
./configure --(probes)-> | Build System |
 |                           Makefile
make
 |
make install
```

图示：
![[autoconf-image.png]]
开发者编写 `configure.ac` 和 `Makefile.am` ，这两个文件通过 `autoconf` 和 `automake` 生成 `configure` 和 `makefile.in` —— 这一过程可通过调用 `autoreconf` 实现。

简化的 `configure` 流程：（模板文件=>实际文件）
![[简化 configure 流程.png]]
详细流程：
![[configure 详细流程.png]]
- `config.status` 实际处理模板
- `configure -C` 将缓存结果至 `config.cache` 以加速重新配置
示例1：（`configure.ac`）
```configure.ac
AC_INIT([hello], [0.01])    # program name: hello, version: 0.01
AC_OUTPUT   # required final line(outputs results)
```
通过 `autoreconf -i` 命令生成 `configure` 文件。
```bash
$ autoreconf -i     # -i means "create/install any support files needed"
# 生成 autom4te.cache, configure
```

--- 
面向 `autoconf` 的 `configure.ac` 文件由特定的语言编写，具有以下特征：
- 实际上是由 `m4`宏处理器处理的Bourne shell script ，但通常使用预先创建的定义
- `#` 是注释符
- 核心风格规则：
	- 使用 `[]` 括住参数 （最好总是这样，但整数不需要）
	- 空格很重要
		- 参数前可以有空格（即`AC_INIT( [])`）
		- 但 `(` 前不能有空格（即 `AC_INIT ([])`：错误）
		- `,` 和 `)` 前不能有空格（即 `AC_INIT([] ,[])`：错误，`AC_INIT([], [] )`：错误）

示例2：（`configure.ac`）
```
AC_INIT([hello], [0.01])
AC_PREREQ([2.68])               # autoconf 版本需大于 2.68
AC_CONFIG_SRCDIR([hello.c])     # 安全性：文件必须存在
AC_CONFIG_HEADER([config.h])    # Make config header
AC_CONFIG_AUX_DIR([build-aux])  # 辅助文件目录
AM_INIT_AUTOMAKE([1.11 -Wall -Werror])  # 初始化 automake
AC_CONFIG_FILES([Makefile])     # "configure" 创建 Makefile
AC_PROG_CC                      # 查找并探寻 C 编译器
AC_PROG_CXX                     # 查找并探寻 C++ 编译器
AC_PROG_LEX                     # 查找 flex/lex
AC_PROG_YACC                    # 查找 bison/yacc
# ... 其他检查
AC_OUTPUT
```

---
`Makefile.am` 是 `makefile` ，但将赋值给特殊变量（命令模式）：`{WHERE}_{PRIMARY}`，`AM_...`。
```
{WHERE}_{PRIMARY} = targets...
AM_{...} = ...
{TARGET}_{SPECIFICS} = files...
```
含义：
- `{WHERE}_{PRIMARY} = targets...`：创建一类特定的构建目标(PRIMARY)，并指定这些目标应当安装到哪个目录(WHERE)。

- `AM_{...} = ...` ：这些变量定义 `automake` 范围的值。
```make
AM_CPPFLAGS：默认的 C 预处理器标志
AM_CFLAGS：默认的 C 编译器标志
AM_CXXFLAGS：默认的 C++ 编译器标志
```

- `{TARGET}_{SPECIFICS} = files...`：设置特定于目标的信息（如果有的话，覆盖默认的）。
```make
{TARGET}_SOURCES    # 目标的源文件
{TARGET}_LDADD      # 程序的额外的对象
{TARGET}_CPPFLAGS   # 指定目标的 C 预处理标志
# hello_CPPFLAGS = -DDEBUG
```

`{WHERE}`：一个以 `dir` 结尾的 makefile 变量，指定安装目录。
常见的几个 `{WHERE}` 值如下：
- `bin`：最终程序将安装到`$(bindir)`，也就是执行文件目录，通常是`/usr/local/bin` 或 `/usr/bin` ，默认为 `$(prefix)/bin`。
- `lib`：将目标安装到 `$(libdir)`，通常是库文件目录，如 `/usr/local/lib`，默认为 `$(prefix)/lib`
- `noinst`：表示不安装这些目标，仅用于构建过程中的测试或中间文件。
- `check`：表示这些目标是用于 `make check` 测试阶段的，不会被安装。
`{PRIMARY}` 是文件的类型，常见的 `{PRIMARY}` 如下：
- `PROGRAMS`：可执行程序
- `LIBRARIES`：静态或共享库
- `LTLIBRARIES`：使用 `libtool` 管理的库
- `HEADER`：头文件
- `SCRIPTS`：脚本文件
- `DATA`：普通数据文件
示例：
![[makefile.am example.png]]
示例：
```make
bin_PROGRAMS = mytool
```
含义：构建一个可执行文件 `mytool` ，并安装到 `$(bindir)` 目录。

```make
lib_LTLIBRARIES = libfoo.la
```
含义：构建一个使用 `libtool` 的共享库 `libfoo.la` ，并安装到 `$(libdir)` 。

```make
noinst_PROGRAMS = testprog
```
含义：构建一个不被安装的程序 `testprog` ，可能用于测试或中间构建。

```make
check_PROGRAMS = test_suite
```
含义：构建一个专门用于测试阶段 `make check` 的程序 `test_suite` 。