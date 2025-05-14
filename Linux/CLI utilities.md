CLI（Command-Line Interface）utilities，命令行界面实用程序

三阶段流程：
![[CLI-utilities 流程.png]]

- 设置阶段：标志、选项、本地化等的初始化
- 解析阶段：读取输入以设置执行参数
- 执行阶段：处理输入，为系统调用准备参数
（期间检查约束条件导致不同执行结果，`EXIT_SUCCESS`, `EXIT_FAILURE`, `EXIT_CANCELED`...，执行失败后提供反馈）


命令行参数可以是段选项或长选项。
1. 短选项
	- 以 `-` 为前缀
	- 通常是单个字符，如 `-a`, `-b`，也可以组合使用，如 `-ab`（等同于 `-a -b`）
	- 在程序中，短选项通常用字符串（`string`）来表示。如，在 `getopt()` 函数中，可以定义 `"ab"` 字符串来表示 `-a` 和 `-b`。
	- 短选项使用“如果选项无参数，仅使用字母；单冒号(`:`)表示强制参数，双冒号(`::`)表示可选参数。如 `kill` 的短选项字符串为 `Lln:s:t`，这表示 `L`、`l`、`t` 不带参数，但 `n` 和 `s` 需要参数。
2. 长选项
	- 以 `--` 为前缀
	- 在程序中，长选项通常使用结构体（`struct`）来定义，以便指定选项的名称、是否需要参数等信息。如，在 `getopt_long()` 函数中，常用 `struct option` 结构体来存储这些选项。
示例：
```c
#include <stdio.h>
#include <getopt.h> // getopt_long

int main(int argc, char *argv[]) {
	const char *short_options = "hv";  // 定义短选项 "-h" 和 "-v"

	struct option long_options[] = {
		{"help", no_argument, NULL, 'h'}, // 定义长选项 "--help"
		{"version", no_argument, NULL, 'v'}, //定义长选项 "--version"
		{0, 0, 0, 0} // 结束标志
	};

	int opt;
	while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
		switch (opt) {
			case 'h':
				printf("Help option selected\n");
				break;
			case 'v':
				printf("Version option selected\n");
				break;
			default:
				printf("Unknown option\n");
		}
	}

	return 0;
}
```

`struct option` 用于描述长选项名称。参数 `longopts` 是该类型的结构体数组，每个长选项对应一个结构体，数组以全零元素结尾。
```c
struct option {
	const char *name; // 长选项名称
	int has_arg; // 该字段表示选项是否接受参数；三个合法值（no_argument(0), required_argument(1), optional_argument(2)）
	int *flag;
	int val;
};
```

---
#### 实用程序初始化
所有实用程序在 `main()` 开头附近都有一个简短的初始化过程：
```
initialize_main(&argc, &argv);
set_program_name(argv[0]);
setlocale(LC_ALL, "");
bindtextdomain(PACKAGE, LOCALEDIR);
textdomain(PACKAGE);

atexit(close_stdout);
```

#### 使用 `Getopt` 进行解析
- `getopt_long()` 函数返回下一个选项，并在所有实用程序中使用
```c
int getopt_long(int argc, char *argv[],
				const char *optstring,
				const struct option *longopts,
				int *longindex);
```
#### 使用 `fts` 遍历文件系统
类 Unix 系统通常支持 `fts` 库，`fts` 提供了一种简洁且高效的方式来遍历目录树，而不需要手动使用 `opendir()` 和 `readdir()` 递归处理子目录。它会自动处理符号链接、循环引用等问题，使得遍历文件系统更安全、更方便。
关键概念：
1. `FTS` 结构体（File Tree System，文件树结构），文件系统遍历会话，维护遍历状态，管理目录树的遍历。
- 由 `fts_open()` 创建，用于遍历目录树
```c
FTS *fts_open(char * const *path_argv, int options, int (*compar)(const FTSENT **, const FTSENT **));

int fts_close(FTS *ftsp);
```
- 通过 `fts_read()` 获取目录树中的文件/目录
- 通过 `fts_set()` 和 `fts_close()` 控制遍历过程

2. `FTSENT` 结构体（表目录树中的一个节点，即文件/目录节点）
-   `fts_read()` 返回 `FTSENT *`，表示当前遍历到的文件/目录信息。
```c
typedef struct _ftsent {
    struct _ftsent *fts_cycle;      /* 循环检测指针（检测符号链接循环） */
    struct _ftsent *fts_parent;     /* 指向父目录的指针 */
    struct _ftsent *fts_link;       /* 指向同一目录下下一个文件的指针 */
    DIR *fts_dirp;                  /* 指向 `DIR*`，表示目录指针（用于存储目录内容） */

    long fts_number;                /* 用户自定义的数字值（可以存储额外数据） */
    void *fts_pointer;              /* 用户自定义的指针值（用于存储额外数据） */

    char *fts_accpath;              /* 访问文件的路径（相对路径或绝对路径） */
    char *fts_path;                 /* 根路径（等于 `fts_fts->fts_path`，即 `fts_open()` 指定的路径） */

    int fts_errno;                  /* 读取文件或目录时的错误码 */
    int fts_symfd;                  /* 处理符号链接时的文件描述符 */
    size_t fts_pathlen;             /* `fts_path` 的长度 */

    FTS *fts_fts;                   /* 指向 `FTS` 结构体，代表遍历上下文 */

    ptrdiff_t fts_level;            /* 目录层级（-1 表示 root 父级，0 表示 root，1 及以上为子目录） */
    size_t fts_namelen;             /* `fts_name` 的长度 */

    /* fts_info: 指示文件类型 */
    unsigned short int fts_info;    

    /* fts_flags: 内部使用的标志位 */
    unsigned short int fts_flags;   

    /* fts_instr: 由 `fts_set()` 设定的指令 */
    unsigned short int fts_instr;   

    struct stat fts_statp[1];       /* `stat()` 调用的文件信息 */
    char fts_name[__FLEXIBLE_ARRAY_MEMBER]; /* 文件名 */
} FTSENT;
```
`FTSENT->fts_info` 字段表示当前节点的类型：
- `FTS_D`：目录
- `FTS_F`：普通文件
- `FTS_SL`：符号链接
- `FTS_SLNONE`：无效符号链接
- `FTS_DNR`：目录无法读取（权限问题）
- `FTS_NS`：文件无法 `stat()` （权限问题）
- `FTS_DC`：目录循环（符号链接导致）

#### 系统调用包装器和辅助程序
`coreutils` 经常通过 `libc` 提供的包装器和辅助程序来调用系统调用。
`libc` 提供许多文本写入函数，如`fwrite()` 和 `write()`。`coreutils` 引入了非标准函数，如 `full_write()` 。除非发生硬故障，否则 `full_write()` 函数会不断重试写入。它依靠 `safe_write()` 在中断期间重试 `write()` 系统调用。其他与写入相关的帮助程序仅在单个实用程序中使用。例如 _dd_ 中的 `iwrite()` 、 _split_ 中的 `cwrite()` 。

#### 常用函数
所有实用程序至少使用三个函数：`main()`、`usage()` 和 `_()`。
`usage()` 函数显示该实用程序的帮助，包括输入参数列表、它们的含义和正确的语法。
`_()` 函数实际上在 `system.h` 中定义的宏，它将简单字符串绑定到 GNU `gettext.h` 中的本机语言支持功能。如果它是一个要显示给用户的字符串，则它可能被此函数包装。

#### 常用代码行
```c
#include "system.h"
```
该头文件定义了系统相关的宏、变量和有用的非标准函数。它提供了必要的“转换”，以允许 coreutils 在尽可能多的体系结构上构建。许多 C 标准和 POSIX 标头都包含在此标头中，例如： _unistd.h_、 _limits.h_ 、 _ctypes.h_ 、 _time.h_ 、 _string.h_ 、 _errno.h_ 、 _stdbool.h_ 、 _stdlib.h_ 、 _fcntl.h_ 、 _inttypes.h_ 和 _locale.h_ 。

```c
#define PROGRAM_NAME "program_name"
```
定义程序的正式名称。

```c
#define AUTHORS proper_name ("YuPeng-y")
```
定义实用程序的作者。

```c
emit_try_help()
```
输出失败后打印帮助建议。包括在线文档的链接。这将出现在 `usage()` 的开头

```c
emit_ancillary_info (PROGRAM_NAME)
```
在特定命令的输出后打印常见的额外帮助信息。包括在线文档的链接。这出现在 `usage()` 的末尾

```c
exit (status)
```
系统调用以给定状态结束执行。这出现在 `usage()` 的末尾

```c
initialize_main(&argc, &argv)
```
一个特殊的初始化函数，用于处理 VMS（VAX/VMS 操作系统）的通配符（wildcard）展开。在大多数操作系统（如 Linux、macOS、Windows）上，通常被定义为空操作，即不会做任何实际的事情。
> 在 VMS 上，shell **不会**自动展开通配符（例如 `*` 或 `?`），所以 `initialize_main()` 负责**将通配符文件名展开**，并更新 `argc` 和 `argv`。

```c
set_program_name(argv[0])
```
使用第一个输入参数作为基本程序名称，丢弃 `argv[0]` 的路径部分。

```c
setlocale(LC_ALL, "")
```
在执行期间设置国际化选项。由 `<locale.h>` 中的 `libc` 提供。

```c
#include <libintl.h>
textdomain(PACKAGE)
```
设置当前程序的文本域，以启用国际化。作用是指定当前程序要使用的翻译域（text domain），即 `.mo` 语言翻译文件所属的标识符。
`PACKAGE` 是一个字符串，通常是程序的名称；`.mo` 文件通常放在：
```c
/usr/share/locale/<语言>/LC_MESSAGES/<PACKAGE>.mo

// 德语（de）
/usr/share/locale/de/LC_MESSAGES/myprogram.mo
```


```c
bindtextdomain(PACKAGE, LOCALEDIR)
```
使用免费软件 `gettext.h` 设置国际化功能的目录

```c
atexit(close_stdout)
```
注册 `close_stdout` 函数，用于程序结束时调用。除了关闭之外，还会刷新缓冲区。

```c
IF_LINT(something);
```
如果使用 `linter`，则通过将代码包含在括号内来抑制 GCC 警告，通常是 NOP

#### C 习语
- `!!`：两次逻辑非操作，将一个值强制转换为布尔类型（0/1）。
- `do { ... } while(0)`：这个非循环通常包含一个多语句宏，以确保预处理器替换后正确标记。