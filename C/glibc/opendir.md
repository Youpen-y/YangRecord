`opendir`, `fdopendir` - 打开一个目录

```c
#include <sys/types.h>
#include <dirent.h>

DIR *opendir(const char *name);
DIR *fdopendir(int fd);
```
`DIR` - 代表一个目录流（directory stream）
```c
#include <dirent.h>
/* This is the data type of directory stream objects. The actual structure is opaque to users. */
typedef struct __dirstream DIR;
```

在`unix/sysv/linux/dirstream.h` 中发现
```c
struct __dirstream
{
    int fd;  /* 文件描述符 */
    __libc_lock_define (, lock)  /* 该结构互斥锁 */
    size_t allocation;  /* 为该块分配的空间 */
    size_t size;   /* 该块中的总共有效数据 */
    size_t offset; /* 该块中的当前索引 */

    off_t filepos; /* 要读的下一个条目的位置 */
    int errcode;   /* Delayed error code */

        /* Directory block.  We must make sure that this block starts
       at an address that is aligned adequately enough to store
       dirent entries.  Using the alignment of "void *" is not
       sufficient because dirents on 32-bit platforms can require
       64-bit alignment.  We use "long double" here to be consistent
       with what malloc uses.  */
    char data[0]  __attribute__ ((aligned (__alignof__(long double))));
}
```
`opendir()` 函数打开一个与目录 `name` 对应的目录流，并返回指向该目录流的指针。该流位于目录中的第一个目录。
`fdopendir()` 函数类似于 `opendir()` ，但返回打开文件描述符 `fd` 引用的目录的目录流。在成功调用 `fdopendir()` 后，`fd` 将由实现内部使用，否则应用程序不应使用它。

返回值
`opendir()` 和 `fdopendir()` 函数返回指向目录流的指针。出错时，返回 NULL，并设置相应的 `errno` 。

Notes
目录流的底层文件描述符可以使用 `dirfd` 获得。


```c
int dirfd(DIR *dirstream);
```
函数 `dirfd` 返回与目录流 `dirstream` 关联的文件描述符。该描述符可以一直使用，直到使用 `closedir` 关闭目录为止。如果目录流实现不使用文件描述符，则返回 -1 。