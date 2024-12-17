#socket-programming 
`getsockname` - get socket name

```c
#include <sys/socket.h>

int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```


描述
`getsockname()` 返回套接字 `sockfd` 绑定到的当前地址至 `addr` 指向的缓冲区中。应初始化参数 `addrlen` 以指示 `addr` 指向的空间大小。返回时，它包含套接字地址的实际大小。

如果提供的缓冲区太小，返回的地址将被截断；这种情况下， `addrlen` 将返回一个大于提供给本次调用的值。

返回值
成功时返回 0 ；出错时返回 -1 并设置 `errno`

错误
EBADF - 参数 `sockfd` 不是有效的文件描述符
EFAULT - `addr` 参数指向的内存不在进程地址空间的有效部分
EINVAL - `addrlen` 无效（如，为负数）
ENOBUFS - 系统中没有足够的资源来执行该操作
ENOSOCK - 文件描述符 `sockfd` 不指向套接字

参见[[网络编程之绑定到任意端口并显示]]