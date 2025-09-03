`automake` - 帮助创建可移植的和 GNU-标准符合的 `Makefiles` ，通过简单的 `Makefile.am` s 创建 `Makefile.in`s。

`Makefile.am` 和 `Makefiles` 遵循大致相同的语法，但是它们通常仅包含可变定义。
- `automake` 可以从这些定义中创建构建规则。
- 可以在 `Makefile.am` 中添加额外的 `Makefile` 规则，`automake` 将在输出中保留它们。

要使用 `automake` ，需要在 `configure.ac` 中声明 `automake` 。
```m4
AM_INIT_AUTOMAKE([OPTIONS...])  # 检查 automake 产生 Makefile 需要的工具

# 示例：
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
```
选项：
- `version`：正确构建当前项目需要的最小 `automake` 版本。如 `1.12`
- `gnu, cygnus(1.13后删除), gnits(不存在), foreign`：选择要使用的“风格”，将指定当前项目的一些更具体的选项和警告级别。
	- 默认设置（`gnu`）：要求严格，必须包含 GNU 项目推荐的标准文件，如 `NEWS`、`COPYING`、`AUTHORS`、`ChangeLog`、`README`。否则 `automake`失败
	- `foreign`：要求宽松，适合非 GNU 风格项目
- `silent-rules`：允许在构建时使用 `Linux` 内核风格的 “silent”输出。从 `automake` 1.13 开始，此选项是默认隐含的，并且完全是无操作的。
```bash
$ ../configure --enable-silent-rules 或在 configure.ac 中添加 AM_SILENT_RULES([yes])
$ make 
make[1]: Entering directory '/home/yongy/Projects/ubuntu-package-src/example/project1/build'
  CC       src/main.o
  CC       src/helper.o
  CCLD     myapp
make[1]: Leaving directory '/home/yongy/Projects/ubuntu-package-src/example/project1/build'
```
- `subdir-objects`：编译相关子目录中的源文件。
```css
# 默认情况下
src/main.c --> main.o （在顶层目录）

# 使用 subdir-objects 后
src/main.c --> src/main.o （目标文件在源目录对应的位置）
```
- `dist-gzip, dist-bzip2, dist-xz, dist-zip,...`：指定由 `make dist` 生成的源代码分发的存档格式。
- `-Wgnu, -Wportability, ..., -Werror`：在 `automake` 中启用（或禁用）警告类别。
	- `-Wall`：打开所有警告
	- `-Werror`：将警告报告为错误

另外，`AC_CONFIG_FILES(FILES...)` 将为其中有 `FILE.am` 的文件创建 `FILE.in` 。
```m4
AC_CONFIG_FILES([Makefile sub/Makefile])
# 准备 Makefile.am 和 sub/Makefile.am
```

### 全局 `automake` 变量
`automake` 提供了一些预定义的变量，可以在 `Makefile.am` 中设置。
- `AM_CPPFLAGS`：默认预处理器标志（如 `-Iinclude/`），用于 `gcc -E`
- `AM_CFLAGS`：默认编译器标志（如 `-Wall -O2`），用于 `gcc -c`
- `AM_LDFLAGS`：默认链接器标志（如 `-static`），用于 `gcc -o`
- `LDADD`：`configure` 未检测到的但应该链接的库

不要设置 `CPPFLAGS`，`CFLAGS` 和 `LDFLAGS`，以便用户可以在环境中传递它们。如：
```bash
./configure CFLAGS="-O0 -g" LDFLAGS="-static"
```

每目标变量
- `<product>_SOURCES`：源文件列表
- `<product>_LDADD`：链接时添加的库（`.la, -a, -so`）
- `<product>_CPPFLAGS`：预处理器参数（如 `-Iinclude`），优先级高于 `AM_CPPFLAGS`
- `<product>_CFLAGS`：编译器参数（如 `-O0 -Wall`），优先级高于 `AM_CFLAGS`
- `<product>_LDFLAGS`：链接器参数（如 `-static`），优先级高于 `AM_LDFLAGS`

路径变量

| 变量名            | 含义                 | 举例（假设在 `build/src/` 目录下） |
| -------------- | ------------------ | ------------------------ |
| `top_srcdir`   | 到源代码树根目录的相对路径      | `../../`                 |
| `srcdir`       | 从当前构建目录到对应源目录的相对路径 | `../../src`              |
| `top_builddir` | 到构建树根目录的相对路径       | `../..`                  |
| `builddir`     | 当前构建目录（构建时所在目录）    | `.`                      |

### `Makefile.am`  声明目标的惯例
```Makefile.am
where_PRIMARY = targets ...
```
`targets` 应该构建为
- `_PROGRAMS`
- `_LIBRARIES`
- `_LTLIBRARIES` 
- `_HEADERS`
- `_SCRIPTS`
- `_DATA`
- `_MANS`, `_TEXINFOS`：文档

`targets` 应该安装在
- `bin_`：`$(bindir)`
- `lib_`：`$(libdir)`
- `custom_`：`$(customdir)`，自定义 `customdir`
- `noinst_`：不安装
- `check_`：由 `make check` 构建
可选：
- `dist_`：分发目标（如果不是默认的）
- `nodist_`：不分发目标

示例1：
```make
bin_PROGRAMS = myapp
lib_LTLIBRARIES = libstuff.la
```
- `make` 会构建 `myapp` 和 `libstuff.la`
- `make install` 会把它们安装到：
	- `$(bindir)/myapp` （即 `/usr/local/bin/myapp`）
	- `$(libdir)/libstuff.la` （即 `/usr/local/lib/libstuff.la`）

示例2：
```Makefile.am
bin_PROGRAMS = foo run-me   # 两个程序 foo 和 run-me
foo_SOURCES = foo.c foo.h print.c print.h # 指定程序 foo 的源文件
run_me_SOURCES = run.c run.h print.c # 指定程序 run-me 的源文件
```
结果：
1. 程序 `foo` 和 `run-me` 将安装在 `$(bindir)`
2. 每个程序 `PROGRAM` 的源需通过 `PROGRAM_SOURCES` 指定；程序名中的 Non-alphanumeric 字符被映射为 `_` 。
3. `automake` 自动从这些文件中构建并链接对象文件；头文件不会被编译。我们列出它们，仅为了它们同样被分发（`automake` 不会分发其不知道的文件）。
4. 可以使用相同的源用于两个程序。
5. 编译器和链接器是根据扩展名推断出的。

#### 静态库的构建与使用
在 `configure.ac` 中添加 `AC_PROG_RANLIB` 。
注意：库的名称格式为 `lib*.a` 。
示例：
```make
lib_LIBRARIES = libfoo.a libbar.a   # 这些库将被安装在 $(libdir)
libfoo_a_SOURCES = foo.c privfoo.h  # 私有头不会被安装，像普通源文件一样
libbar_a_SOURCES = bar.c privbar.h
include_HEADERS = foo.h bar.h   # 公共头将被安装在 $(includedir)
```

#### 目录布局
需要每个目录一个 `Makefile` （即一个 `Makefile.am`）。
它们必须都要在 `configure.ac` 中声明。
如：
```configure.ac
AC_CONFIG_FILES([Makefile lib/Makefile src/Makefile src/dira/Makefile src/dirb/Makefile])
```
`make` 在最顶层执行；
`Makefile.am`s 应该使用 `SUBDIRS` 变量修复递归目录的顺序；当前目录隐式地在子目录构建完后构建，也可以通过 `.` 指定顺序。
如：
```Makefile.am
# Makefile.am，顶层目录指定下一层目录
SUBDIR = lib src

## src/Makefile.am
SUBDIR = dira . dirb   # 先构建 dira，接着当前目录，然后 dirb
```

#### VPATH 构建
VPATH builds（又叫 `out-of-source builds`）是一种构建方式，其中
- 源代码保存在一个目录（比如 `src/`）
- 构建输出（对象文件、可执行文件）保存在另一个目录（比如 `build/`）r

因此，源文件不一定在当前目录。
两个树：
- `build tree` —— 构建树（`Makefile` 和对象文件在构建树）
- `source tree` —— 源树（`Makefile.am`，`Makefile.in` 和源文件在源树）
- 如果在当前目录使用 `./configure` 配置，两个树在同一目录下。 

在每个 `Makefile` 中，`config.status` 将定义 `$(srcdir)`：匹配源目录的路径。当在 `automake` 变量中引用**源文件**或**目标**时，你不需要担心 `source` 和 `build` ，因为 `make` 会检查两个目录。
当为工具指定标志（`flags`）时或写自定义命令时，可能需要使用 `$(srcdir)` 。例如，为了告诉编译器包含源自 `dir/` 中的头文件，应该写 `-I$(srcdir)/dir` ，而非 `-Idir` （`-Idir` 将从 build tree 中获取头文件）。

#### Convenience Libraries（方便库）
如何组织和构建一个“方便库”（不需要安装，仅供内部其他模块链接使用），并在其他程序中使用它？
项目结构示例：
```
project/
├── lib/
│   ├── xalloc.c
│   ├── xalloc.h
│   └── Makefile.am
├── src/
│   ├── foo.c
│   ├── foo.h
│   ├── print.c
│   ├── print.h
│   ├── run.c
│   ├── run.h
│   └── Makefile.am
```

`lib/Makefile.am`：
```make
noinst_LIBRARIES = libcompat.a
libcompat_a_SOURCES = xalloc.c xalloc.h 
```
- `noinst_LIBRARIES`：这个静态库不会被安装，只用于本项目内部构建。
- `libcompat.a`：要构建的静态库名称（传统 Unix 风格命名）
- `libcompat_a_SOURCES` 是它的源文件列表：`xalloc.c` 和头文件 `xalloc.h` 

`src/Makefile.am`：
```make
LDADD = ../lib/libcompat.a
AM_CPPFLAGS = -I$(srcdir)/../lib
bin_PROGRAMS = foo run-me
foo_SOURCES = foo.c foo.h print.c print.h
run_me_SOURCES = run.c run.h print.c
```
- `LDADD`：是链接器附加参数，在构建所有 `bin_PROGRAMS` 时都会加上它
- `AM_CPPFLAGS`：是传给 C/C++ 预处理器的额外编译选项，`-I$(srcdir)/../lib` 表示添加头文件搜索路径，指向 `lib/` 目录。
- 也可以使用 per-target 变量：这些变量将应用于单个程序。如
```make
run_me_LDADD = ../lib/libcompat.a
run_me_CPPFLAGS = -I$(srcdir)/../lib $(EFENCELIB)
```

#### Per-Target Flags（每目标标志）
假定 `foo` 是一个程序或库：
- `foo_CFLAGS`：额外的 C 编译器标志
- `foo_CPPFLAGS`：额外的预处理器标志（`-I` 和 `-D`）
- `foo_LDADD`：额外的链接器对象，`-l` 和 `-L`（如果 `foo` 是一个程序）
- `foo_LIBADD`：额外的链接器对象，`-l` 和 `-L`（如果 `foo` 是一个库）
- `foo_LDFLAGS`：额外的链接器标志
`foo_XXXFLAGS` 的默认值是 `$(AM_XXXFLAGS)` 。
在 `Automake/Autoconf` 中链接你自己项目里的库时，，应该使用"普通文件名"，不要用 `-l` 或 `-L` 这样的链接器选项（这两个常用于链接外部系统或已安装的库）。示例如下：
```make
# 在 Makefile.am 中链接自己项目内部的静态库或便利库时，应该直接使用文件路径
LDADD = ../lib/libcompat.a  # ✅ 正确做法

# 不要写成：
LDADD = -L../lib -lcompat     # ❌ 不推荐

# 针对系统路径（/usr/lib, /usr/local/lib）下的库，通常使用 -l 或 -L
LDADD = -lz -lm -lpthread   # 链接 zlib ，数学库，pthread 线程库
```

可以结合 `AC_CHECK_LIB` 判断是否存在库，以及添加链接器选项。
```
# 检查库 efence 是否存在，如果存在，变量 EFENCELIB 赋值为 -lefence
AC_CHECK_LIB([efence], [malloc], [EFENCELIB=-lefence])
AC_SUBST([EFENCELIB])   # 替换模板中的 EFENCELIB 变量
```

#### `make dist` 和 `make distcheck` 行为
—— 创建发布用的源代码包（tarball），如`myproject-1.0.tar.gz`
- `make dist`：会创建一个 `.tar.gz` 源码包，包含项目的所有源代码、构建脚本、文档等。
- `make distcheck`：
	1. 创建源码包（如上）
	2. 解压并构建它
	3. 执行 `make install`, `make uninstall`
	4. 确保构建干净无误，没有遗漏文件
源码包中会包含哪些内容？
- 所有 `*_SOURCES` 变量声明的源代码（如下述的 `mytool, main.c, util.c`）
```make
bin_PROGRAMS = mytool
mytool_SOURCES = main.c util.c
```
- 所有 `*_HEADERS` 变量声明的头文件也会被打包（下述的 `util.h`）
```make
mytool_HEADERS = util.h
```
- 所有通过 `dist_..._SCRIPTS` 声明的脚本文件也会被包含（下述`myscript.sh`）
```make
dist_bin_SCRIPTS = myscript.sh
```
- 所有通过 `dist_..._DATA` 声明的数据文件（下述 `default.cfg`）
```make
dist_pkgdata_DATA = default.cfg
```
- 标准文档文件。（`README, NEWS, ChangeLog, AUTHORS, COPYING, INSTALL`）这些不是强制，但若存在就会自动进包。可通过 `automake --help` 查看文件列表。
- 在 `EXTRA_DIST` 中列举的文件或目录
```make
SUBDIRS = lib src   # 构建顺序为 lib/，src/ 和当前目录。
EXTRA_DIST = HACKING    # 执行 make dist 时，会额外打包 HACKING 文件
```

#### 条件构建和无条件分发
Conditional builds and Unconditional distribution
```make
# Conditional Programs
bin_PROGRAMS = foo
if WANT_BAR
    bin_PROGRAMS += bar
endif
foo_SOURCES = foo.c
bar_SOURCES = bar.c
if WANT_BAR
    foo_SOURCES += bar.c
endif
```
如果 `WANT_BAR` 为真，则构建程序 `bar`，且 `bar.o` 被链接到 `foo` 。
但无关 `WANT_BAR` 真或假，`foo.c` 和 `bar.c` 都会被分发。
这易于移植，`config.status` 将注释掉必须禁用的 `Makefile.in` 规则。条件是必须在 `configure.ac` 中声明和定义 `WANT_BAR` 。

如何声明？
`AM_CONDITIONAL(NAME, CONDITION)` 宏：根据某个条件，定义一个 `Makefile.am` 中可用的“条件变量”`NAME` 。
- `NAME`：变量名（用以引用条件分支的字符串），有效的块名称以字母开头，且仅包含字母、数字和下划线符号。按照不成文的惯例，通常全部大写。
- `CONDITION`：是一个 shell 命令（布尔表达式），它的退出码为 0 时表示“真”（enable `NAME`）。
示例：
```configure.ac
AC_CHECK_HEADER([bar.h], [use_bar=yes])
AM_CONDITIONAL([WANT_BAR], [test "$use_bar" = yes])
```
```Makefile.am
if WANT_BAR
...
else
...
endif
```

仅当 `bar.h` 在系统上时启用 `WANT_BAR` 。

注意：`automake` 条件变量的使用
不能在变量定义中使用：（下例不会奏效）
```
bin_PROGRAMS = \
    baz \
if WANT_BAR
    bar \
endif
    foobar
```
但可以用作中间变量，或用在 `+=` 赋值
```
if WANT_BAR
WANT_PROGS = bar
endif

bin_PROGRAMS = \
    baz \
    $(WANT_PROGS) \
    foobar
# 或者
bin_PROGRAMS = \
    baz \
    foobar 

if DEBUG
bin_PROGRAMS += baz
endif
```

#### `Makefile.am` 和 `Makefile.in` 的关系
```sh
Makefile.am --(automake)--> Makefile.in --(configure)--> Makefile
```
- `Makefile.am` 的内容几乎会原样拷贝到 `Makefile.in` 中。
- `automake` 会在 `Makefile.in` 中添加额外的规则和变量，用来实现 `Makefile.am` 中使用的特殊变量的语义。
如在 `Makefile.am` 中写的高级抽象：
```make
bin_PROGRAMS = mytool
mytool_SOURCES = main.c util.c
```
`automake` 会自动在 `Makefile.in` 中展开成：
- 如何编译 `main.c`
- 如何链接 `mytool`
- 安装规则（`make install`）
- 清理规则（`make clean`）
- 依赖规则

此外，`automake` 会做一些小的转换，以便可移植地（跨平台）处理 `Makefile.am` 中的某些结构，如：
1. 条件语句
```make
if USE_SSL
    mytool_SOURCES += ssl.c
endif
```
`automake` 会将这些 `if` 语句转换成标准 `make` 能理解的形式 —— 因为不同平台的 `make` 实现不一定支持 GNU-style 条件。

2. `+=` 运算
```make
mytool_SOURCES = main.c
mytool_SOURCES += util.c
```
不是所有系统都支持 `+=` ，所以 `automake` 会将它转换为：
```make
mytool_SOURCES = main.c util.c
```

#### `Automake` 可扩展性和注意事项
1. 可以在 `Makefile.am` 中自定义 make 规则，不仅限于 `automake` 的那些变量（如 `bin_PROGRAMS`，`*_SOURCES` 等）。
如：
```make
style-check:
    @echo "Checking code style..."
    clang-format --dry-run --Werror *.c
```
之后，用于可以运行 `make style-check` 。
通常，可以添加一些维护用的目标，如：
- `make style-check`：检查代码风格
- `make generate-docs`：生成文档
- `make update-faq`：生成 FAQ
- `make list`，`make test` 等
还可以定义一些非常规文件的构建逻辑，如：
```make
FAQ: src/faq.txt tools/faqgen.py
    python3 tools/faqgen.py src/faq.txt > FAQ
```

2. 可以在 `Makefile.am` 中定义自用变量（对于 `automake` 无意义），可以在自己的规则中使用它们。
```make
MYFLAGS = -DDEBUG -DVERSION=3

debug:
    $(CC) $(MYFLAGS) -o debug main.c
```

3. 警告：自定义的变量或规则如果和 `automake` 预定义的重名，会覆盖 `automake` 的行为，可能引发错误或不兼容。如果确实要扩展标准规则，建议使用：
```make
clean-local:
    rm -f *.tmp
```
可以使用 `automake` 的 `--warnings=all` 或 `-Wall` 选项输出命名冲突或覆盖 `automake` 变量的警告。

#### 建议
1. 使用 `-Wall -Werror` ，编译时使用 GCC/Clang 的警告和错误开关
```make
AM_CFLAGS = -Wall -Werror
```

2. Keep Your Setup Simple（KYSS）
3. Do not lie to `automake`
所谓“骗 automake”，指的是：
- 声明了某些文件/变量，但实际上并没有
	- 如写了 `foo_SOURCES = main.c` ，但 `main.c` 实际上并不存在或不会被生成
- 绕过 `automake` 的机制
	- 比如直接修改 `Makefile.in` （而不是通过 `.am` 和 `configure.ac`）

2. 如果 `make` 重建配置文件失败，手动运行 `autoreconf --install` 。
	1. 如果不奏效，使用 `autoreconf --install --force`。
	2. 如果还不奏效，使用`make -k maintainer-clean; autoreconf --install --force` （仅在必要时做）

---
一个项目通常被组织为多个目录。`automake` 为此提供两种处理方式：
1. `recursive make`：将对子目录分别调用 `make`，每个子目录有其单独的 `Makefile.am`
2. `non-recursive make`：仅有一个 `Makefile.am`，构建 everything 。
`recursive make` 过去是主流，但它有明显的缺点。
- 并行构建的性能较差
- Recursive Make Considered Harmful -- 递归 make 导致依赖关系不可见、调试困难、性能差等问题
现代项目越来越多地使用 `non-recursive make`，如果 `Makefile.am` 变得太大，可以用 `include` 拆分它。
```make
include src/Makefile-fragments.am
include lib/Makefile-fragments.am
```

---
`Recursive make`
在顶层 `Makefile.am` 中使用 `SUBDIRS` 变量指定包含 `Makefile.am` 的子目录。
```configure.ac
AC_CONFIG_FILES([Makefile src/Makefile])
```
```Makefile.am
SUBDIRS = src
```
```src/Makefile.am
bin_PROGRAMS = hello
hello_SOURCES = main.c
```
---
`Non-recursive make`
`AM_INIT_AUTOMAKE` 宏接受一个 `subdir-objects` 参数。
如果指定 `subdir-objects` 后，`Makefile.am` 就可以引用其他目录中的源码文件。
```configure.ac
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_FILES([Makefile])
```
```Makefile.am
bin_PROGRAMS = hello
hello_SOURCES = \
        src/main.c \
        src/hello.c
```
注意：当指定 `_SOURCES` 时，尝试使用变量保存相同的前缀是一个常见的错误，因为 `automake` 不会扩展 `_SOURCES` 中的变量定义，尽管这个问题可能会在未来版本中得到解决，但考虑到最大程度的兼容性，不建议使用。

---

| 规则                            | 调用                              |
| ----------------------------- | ------------------------------- |
| No `AM_SILENT_RULES`          | 默认展示完整编译命令，但是使用 `make V=0` 可以精简 |
| With `AM_SILENT_RULES([yes])` | 默认静默输出，但可以使用 `make V=1` 显示更多    |

