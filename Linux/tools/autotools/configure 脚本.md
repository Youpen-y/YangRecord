```bash
./configure --help
```

使用：
```bash
./configure [OPTION]... [VAR=VALUE]...
```
为了赋值环境变量（如，`CC, CFLAGS...`），使用 `VAR=VALUE`。

选项：
- `-h/--help`：展示 help 并退出
- `--help=short`：展示特定于该包的选项
- `--help=recursive`：展示所有包含的包的简短 help 
- `-V/--version`：展示版本信息并退出
- `-q/--quiet, --silent`：不打印 `checking ...` 消息
- `--cache-file=FILE`：将测试结果缓存在 `FILE` 
- `-C/--config-cache`：`--cache-file=config.cache` 的别名
- `-n/--no-create`：不创建输出文件
- `--srcdir=DIR`：在 `DIR` 中寻找源代码

安装目录：
`--prefix=PREFIX`：在 `PREFIX` （`/usr/local`）中安装架构无关的文件
`--exec-prefix=EPREFIX`：在 `EPREFIX` 中安装架构相关（architecture-dependent）文件

默认，`make install` 将在 `/usr/local/bin`，`/usr/local/lib` 等中安装所有文件。可以使用 `--prefix` 指定其他安装前缀，如 `--prefix=$HOME/temp` 将安装文件至 `$HOME/temp` 目录下，其结构如下：
```bash
.
├── bin     // 存放执行文件
│   ├── attr
│   ├── getfattr
│   └── setfattr
├── etc     // 配置文件
│   └── xattr.conf
├── include // 头文件
│   └── attr
│       ├── attributes.h
│       ├── error_context.h
│       ├── libattr.h
│       └── xattr.h
├── lib     // 静态/动态库
│   ├── libattr.a
│   ├── libattr.la
│   ├── libattr.so -> libattr.so.1.1.2502
│   ├── libattr.so.1 -> libattr.so.1.1.2502
│   ├── libattr.so.1.1.2502
│   └── pkgconfig
│       └── libattr.pc
└── share
    ├── doc
    │   └── attr
    │       ├── CHANGES
    │       ├── COPYING
    │       └── COPYING.LGPL
    ├── locale
    │   ├── cs
    │   │   └── LC_MESSAGES
    │   │       └── attr.mo
    │   ├── nl
    │   │   └── LC_MESSAGES
    │   │       └── attr.mo
    │   ├── ...
    │   └── sv
    │       └── LC_MESSAGES
    │           └── attr.mo
    └── man     // 手册页
        ├── man1
        │   ├── attr.1
        │   ├── getfattr.1
        │   └── setfattr.1
        └── man3
            ├── attr_get.3
            ├── attr_getf.3 -> attr_get.3
            ├── attr_list.3
            ├── attr_listf.3 -> attr_list.3
            ├── attr_multi.3
            ├── attr_multif.3 -> attr_multi.3
            ├── attr_remove.3
            ├── attr_removef.3 -> attr_remove.3
            ├── attr_set.3
            └── attr_setf.3 -> attr_set.3
```

精细调优安装目录：
- `--bindir=DIR`：用户程序 `EPREFIX/bin`
- `--sbindir=DIR`：系统管理程序 `EPREFIX/sbin`
- `--libexecdir=DIR`：program executables `EPREFIX/libexec`
- `--sysconfdir=DIR`：只读单机数据 `PREFIX/etc`
- `--sharedstatedir=DIR`：可修改的架构无关数据 `PREFIX/com`
- `--localstatedir=DIR`：可修改的单机数据 `PREFIX/var`
- `--runstatedir=DIR`：可修改的每-process 数据 `LOCALSTATEDIR/run`
- `--libdir=DIR`：object code libraries `EPREFIX/lib`
- `--includedir=DIR`：C 头文件 `PREFIX/include`
- `--oldincludedir=DIR`：non-gcc C 头文件 `/usr/include`
- `--datarootdir=DIR`：只读架构无关数据根目录 `PREFIX/share`。
- `--datadir=DIR`：只读架构无关数据 `DATAROOTDIR`
- `--infodir=DIR`：info 文档 `DATAROOTDIR/info`
- `--localedir=DIR`：locale-dependent 数据 `DATAROOTDIR/locale`
- `--mandir=DIR`：手册文档 `DATAROOTDIR/man`
- `--docdir=DIR`：文档根目录 `DATAROOTDIR/doc/attr`
- `--htmldir=DIR`：html 文档 `DOCDIR`
- `--dvidir=DIR`：dvi 文档 `DOCDIR`
- `--pdfdir=DIR`：pdf 文档 `DOCDIR`
- `--psdir=DIR`：ps 文档 `DOCDIR`

程序名（应对已有同名程序的情况）：
- `--program-prefix=PREFIX`：在安装后的程序名前添加一个前缀
```bash
./configure --program-prefix=g
# 如果原本程序名 `make` ，安装后会变成 gmake
```
- `--program-suffix=SUFFIX`：在安装后的程序名后添加一个后缀
```bash
./configure --program-suffix=-1.0
# 如果原本是 gcc ，那么安装后就变成 gcc-1.0
```
- `--program-transform-name=PROGRAM`：使用 `sed PROGRAM` 语法对程序名进行正则替换
```bash
./configure --program-transform-name='s/^/gnu-/'
# 用 sed 命令把每个程序名前加上 gnu- ，如 make -> gnu-make, ld -> gnu-ld
```

平台架构相关：
- `--build=BUILD`：当前使用的系统，即编译器正在运行的系统（编译系统），通常自动检测。
- `--host=HOST`：要运行程序的系统，即目标系统，默认和 `--build` 一样
如果在本地机器上编译，不做交叉编译，只需要运行 `./configure` ，将会自动猜测 `build` 和 `host`。
而如果在系统 A 上编译，但生成的是给系统 B 用的程序，此时需要用 `--host` 指定目标系统。如 `./configure --host=arm-linux-gnueabihf` —— 在当前 x86 系统上编译出一个运行在 ARM 系统上的程序。

可选特征：
- `--enable-dependency-tracking`：启用依赖追踪（默认启用）。开发时增量构建和跟踪依赖。
- `--disable-dependency-tracking`：关闭依赖跟踪，提升构建速度（不会生成 `.d` 文件），常用于临时构建或一次性打包。
Autotools 会自动检测你用的编译器支持哪种依赖格式，常见的类型有：

| 名称           | 编译器/环境     | 特点              |
| ------------ | ---------- | --------------- |
| `none`       | 不支持依赖追踪    | 无依赖追踪           |
| `gcc`        | 老版本 GCC    | 用 `-MM` 等方式生成依赖 |
| `gcc3`       | GCC 3+     | 最快、原生支持 `-MMD`  |
| `makedepend` | X11 工具集的方式 | 较旧              |
| `sgi`        | SGI 编译器    | 仅在特定平台使用        |



一些常用的环境变量

| 环境变量                  | 含义                                                               |
| --------------------- | ---------------------------------------------------------------- |
| `CC`                  | C 编译器                                                            |
| `C_FLAGS`             | C 编译器标志                                                          |
| `LDFLAGS`             | 链接器标志，如 `-L<lib dir>`， 如果在非标准目录 `<lib dir>` 下有库                  |
| `LIBS`                | 传给链接器的库，如 `-l<library>`                                          |
| `CPPFLAGS`            | C/C++ 预处理器标志，如 `-I<include dir>` ，如果在非标准目录 `<include dir>` 下有头文件 |
| `LF_SYS_LIBRARY_PATH` | 用户定义的运行时库搜索路径                                                    |
| `CPP`                 | C 预处理器                                                           |

`Autoconf`（自动配置工具）支持嵌套子项目（sub-packages）的机制。即：
> 一个主项目的源码中可以包含其他使用 `Autoconf` 构建的子项目（如第三方库、组件等），这些子项目也各自带有自己的 `configure` 脚本，主项目的构建系统可以递归地管理它们。

对于包安装者而言，`configure` 选项将被递归传递给 `sub-packages`。`configure --help=recursive` 显示所有 `sub-packages` 的帮助手册。
对于维护者而言，每个子项目有其独立的 `configure.ac` 和 `Makefile.am`，维护起来方便。





