`listen` - 监听套接字上的连接

```c
#include <sys/socket.h>

int listen(int sockfd, int backlog);

// backlog 参数的值取决于 期望在两次调用 accept() 之间看到的连接数量（而非限制了连接总量）
// 如果程序调用 accept() 的速度很快，low backlog 通常不是问题
```


`listen()` 将 `sockfd` 引用的套接字标记为 passive socket （被动套接字），即，作为将用于使用 `accept()` 接受传入连接请求的套接字。

参数
`sockfd` 参数是一个类型为 SOCK_STREAM 或 SOCK_SEQPACKET 的套接字。
`backlog` 参数定义 `sockfd` 的挂起连接队列（the queue of pending connections）可以增长的最大长度。如果连接请求在队列已满时到达，则客户端可能会收到带有 `ECONNREFUSED` 指示的错误，或者如果底层协议支持重传，则可能会忽略该请求，以便稍后重新尝试连接成功。
``
返回值
成功时，返回 0 ；失败时返回 -1 ，并设置 `errno` 来指示错误。


错误
EADDRINUSE - 另一个套接字已在侦听同一端口
EBADF - 参数 `sockfd` 不是有效的文件描述符
ENOTSOCK - 文件描述符 `sockfd` 不引用套接字
EOPNOTSUPP - 套接字不是支持 `listen()` 操作的类型

NOTES
为了接受连接，需要执行如下步骤：
1. 使用 `socket()` 创建套接字
2. 使用 `bind()` 将套接字绑定到本地地址，以便其他套接字可以通过 `connect` 连接到它
3. 使用 `listen()` 指定接受传入连接的意愿和传入连接的队列限制
4. 使用 `accept()` 接受连接

Linux 2.2 中 TCP 套接字上的 `backlog` 参数的行为发生了变化。现在它指定等待接受的完全建立的套接字的队列长度（the queue length for completely established sockets waiting to be accepted），而不是 incomplete 的连接请求的数量。
The maximum length of the queue for incomplete sockets can be set using /proc/sys/net/ipv4/tcp_max_syn_backlog. 

如果`backlog` 参数大于 `/proc/sys/net/core/somaxconn` 中的值，则它会默默地截断为该值。从Linux 5.4开始，该文件中的默认值是4096；在早期内核中，默认值为 128。

