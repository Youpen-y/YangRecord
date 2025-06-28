`fopen, fdopen, freopen` —— 流打开函数

语法：
```c
#include <stdio.h>

FILE *fopen(const char *path, const char *mode);
FILE *fdopen(int fd, const char *mode);
FILE *freopen(const char *path, const char *mode, FILE *stream);
```

描述：
`fopen()` 函数打开 `path` 指向的字符串为名称的文件，并将流与其关联。
参数 `mode` ：
- `r`：以只读方式打开文件，文件必须已经存在。
- `w`：以仅写入的方式打开文件。如果文件已存在，则删除其当前内容。如果文件不存在，则创建该文件。
- `r+`：打开文件进行读写。如果文件已存在。文件内容初始不变，但文件位置设置为文件的开头。
- `w+`：打开文件进行读写。如果文件已存在，则删除其当前内容。如果文件不存在，则创建该文件。
- `a`：打开文件仅用于追加操作。追加操作与写入操作相同，不同之处在于数据仅写入文件当前末尾。如果文件不存在，则创建该文件。
- `a+`：打开文件进行追加和读取。如果文件存在，则其追加之前的内容保持不变。如果文件不存在，则创建文件。读取的初始文件位置位于文件开头，而追加的文件位置位于文件末尾。
上述字符串后附加字符 `x` ，表示如果文件已存在，将导致 `fopen` 失败，而不是打开该文件。保证不会破坏尝试打开的任何文件。


`FILE` ：`/usr/include/x86_64-linux-gnu/bits/types/FILE.h`
```C
#ifndef __FILE_defined
#define __FILE_defined 1

struct _IO_FILE;

/* The opaque type of streams. This is the definition used elsewhere. */
typedef struct _IO_FILE FILE;

#endif
```

`_IO_FILE` 结构体：`/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h`
```c
/* The tag name of this struct is _IO_FILE to preserve historic
   C++ mangled names for functions taking FILE* arguments.
   That name should not be used in new code. */
struct _IO_FILE
{
	int _flags;    /* 高位是魔数（_IO_MAGIC），其余位是各种标志（如是否到达文件、是否出错等） */

    /* C++ streambuf 协议相关指针 */
    char *_IO_read_ptr; /* 当前读取指针 */
    char *_IO_read_end; /* 读取区域的结束位置 */
    char *_IO_read_base;    /* 追加+读取区域起始位置 */
    char *_IO_write_base;   /* 写入区域的起始位置 */
    char *_IO_write_ptr;    /* 当前写入指针 */
    char *_IO_write_end;    /* 写入区域的结束位置 */
    char *_IO_buf_base; /* 缓冲区域的起始位置 */
    char *_IO_buf_end;  /* 缓冲区域的结束位置 */

    /* 支持回退和撤销的字段 */
    char *_IO_save_base; /* 指向 non-current 写入区域起始的指针 */
    char *_IO_backup_base; /* 指向备份区域（backup area）第一个有效字符的指针 */
    char *_IO_save_end; /* 指向 non-current 写入区域末尾的指针 */

    struct _IO_marker *_markers; /* 用于标记流中的特定位置，支持如 fgetpos/fsetpos 之类的操作 */
    struct _IO_FILE *_chain; /* 用于将多个文件流对象串联成链表，便于管理所有打开的流 */
    int _fileno;    /* 文件描述符 */
    int _flags2:24; /* 额外的标志位（24位）*/
    /* 当 malloc 分配缓冲区失败时的备用缓冲区 */
    char _short_backupbuf[1];
    __off_t _old_offset; /* 文件偏移量（老版本的，可能不再使用）*/

	unsigned short _cur_column; /* 当前列号（用于格式化输出）*/
	signed char _vtable_offset; /* 虚表偏移（C++相关）*/
	char _shortbuf[1]; /* 备用短缓冲区 */

	_IO_lock_t *_lock; /* 多线程环境下的流锁，保证线程安全 */
#ifdef _IO_USE_OLD_IO_FILE
};
```

`fopen()` 库函数与 `open()`系统调用的对应

| `fopen()`模式 | `open()`标志                        |
| ----------- | --------------------------------- |
| `r`         | `O_RDONLY`                        |
| `w`         | `O_WRONLY`\|`O_CREAT`\|`O_TRUNC`  |
| `a`         | `O_WRONLY`\|`O_CREAT`\|`O_APPEND` |
| `r+`        | `O_RDWR`                          |
| `w+`        | `O_RDWR`\|`O_CREAT`\|`O_TRUNC`    |
| `a+`        | `O_RDWR`\|`O_CREAT`\|`O_APPEND`   |

---
`fdopen()` 用于将一个已有的文件描述符（通常来自 `open()`，`socket()`，`pipe()` 或 `dup()` 等系统调用）转换为一个标准 I/O 流（`FILE *`），以便使用 `fread()`、`fprintf()`、`fgets()` 等标准库函数来进行读写。
- `mode` - 字符串，类似于 `fopen()` 的模式，如 `r, w, a, r+`等

>[!NOTE]
>- 不会创建新的文件描述符，而是“封装”原有的 `fd`。
>- `fdopen()` 不会改变文件的读/写指针位置或权限。
>- 不能 `fclose()` 一个 `FILE *` 并再尝试用原 `fd` ；关闭 `FILE *`会自动关闭 `fd` 


#### 流和文件描述符
##### 文件描述符
文件描述符是操作系统内核维护的一个非负整数，用于标识进程打开的文件、管道、套接字等资源。它是系统调用层面的抽象，直接与内核交互。

特点：
- 每个进程都有自己的文件描述符表
- 标准文件描述符：0（`stdin`）、1（`stdout`）、2（`stderr`）
- 通过系统调用如 `open()`，`read()`，`write()`，`close()` 操作
- 提供底层、直接的文件访问方式

使用示例：
```c
int fd = open("file.txt", O_RDWR);
if (fd != -1) {
	write(fd, "Hello", 5);
	close(fd);
}
```

##### 流
流是 C 标准库提供的高级抽象，封装了文件描述符并提供缓冲、格式化等附加功能。它是库函数层面的抽象，建立在文件描述符之上。
特点：
- 提供缓冲机制，提供 I/O效率
- 支持格式化输入输出（`printf`，`scanf` 等）
- 标准流：`stdin`，`stdout`，`stderr`
- 通过库函数如 `fopen()`，`fread()`，`fwrite()`，`fclose()` 操作
- 跨平台兼容性好
使用示例：
```c
FILE *fp = fopen("file.txt", "w");
if (fp != NULL) {
	fprintf(fp, "Hello, World\n");
	fclose(fp);
}
```

##### 相互关系
```txt
流 --- fileno() ---> 文件描述符
文件描述符 --- fdopen() ---> 流
```





