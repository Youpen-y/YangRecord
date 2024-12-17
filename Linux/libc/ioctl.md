`ioctl` - 控制设备

概要
```c
#include <sys/ioctl.h>

int ioctl(int fd, unsigned long op, ...);  /* glibc, BSD */
int ioctl(int fd, int op, ...); /* musl, other UNIX */
```

描述
`ioctl()` 系统调用操作 special files 的底层设备参数。尤其是，字符特殊文件（character special files） 的许多操作特性可以用 `ioctl()` 操作来控制。

参数 
`fd` 必须是打开的文件描述符。
`op` 是与设备相关的操作代码。
第三个参数是指向内存的无类型指针（传统上是 `char *argp`）。

`ioctl()` 的 `op` 参数中编码了该参数是 `in` 参数还是 `out` 参数，以及参数 `argp` 的大小（以字节为单位）。用于指定 `op` 的宏和定义位于文件 `<sys/ioctl.h>` 中。

返回值
通常成功时返回 0 ，还有一些 `ioctl()` 操作使用返回值作为输出参数。失败时返回 -1 ，并设置 `errno` 来指示错误。



