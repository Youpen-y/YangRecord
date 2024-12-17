#socket-programming 
```
socket - 创建一个通信端点
```

头文件
``` c
#include <sys/types.h>
#include <sys/socket.h>
```

函数原型
``` c
int socket(int domain, int type, int protocol);
```

描述
`socket()` 创建一个通信端点，并返回指向该端点的文件描述符（fd）。成功调用返回的文件描述符将是不为进程打开的最小（lowest-numbered）文件描述符。\

`domain` 指定通信领域；选择用于通信的协议簇。这些协议簇定义于 `<sys/socket.h>` 中。\

常见（`man 7 address_families` 查看更多）
- AF_UNIX / AF_LOCAL 本地通信
- AF_INET       IPv4 网络协议
- AF_INET6      IPv6 网络协议

`type` 指定通信语义（什么类型的套接字）。

- SOCK_STREAM   提供有序、可靠、双向基于连接的字节流，支持带外数据（out-of-band data）传输
- SOCK_DGRAM    支持数据报（无连接、不可靠固定大小消息）
- SOCK_SEQPACKET    提供有序、可靠、双向基于连接的数据传输路径用于固定最大长度的数据报
- SOCK_RAM      提供原始（raw）网络协议访问
- SOCK_RDM
- SOCK_PACKET
`PF_INET` 与 `AF_INET` 实际上具有相同的值（`#define AF_INET PF_INET`）

`protocol` 指定该 `socket` 使用的特定协议。通常只有一个协议存在来支持特定的套接字 `type` 在给定的协议族中，在这种情况下，可以将 `protocol` 指定为 0 （即根据 `type` 选择正确的协议）。
然而，可能存在许多协议，在这种情况下，必须以这种方式指定特定的协 议。要使用的协议号是特定于通信所发生在的"communication domain"; 看 `protocol(5)`。关于如何将协议名称字符串映射到协议编号，请参见 `getprotoent(3)`。