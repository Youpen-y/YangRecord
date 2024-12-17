`connect` - 连接一个套接字

```c
#include <sys/types.h>
#include <sys/socket.h>

int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
```

参数
`sockfd` - 本地创建的用于与远端通信的套接字描述符
`serv_addr` - 包含目的IP 地址和端口的 `struct sockaddr`
`addrlen` - `sockaddr` 结构的长度

如果 `sockfd` 尚未绑定到本地地址，则 `connect()` 应将其绑定到一个未使用的本地地址（除非套接字地址的地址簇是 `AF_UNIX`）。

如果 `sockfd` 不是连接模式（UDP），则 `connect()` 将设置套接字的 peer address，并且不会建立连接。
- 对于 `SOCK_DGRAM` 套接字，对端地址指定了后续 `send()` 函数数据报发送到何处，并限制后续 `recv()` 函数的远端发送方。如果 `serv_addr` 是协议的空地址，则应重置套接字的对端地址。

如果 `sockfd` 是连接模式，则 `connect()` 将尝试建立与 `serv_addr` 参数指定地址的连接。
- 套接字的文件描述符未设置 `O_NONBLOCK` 且无法立即建立连接，则 `connect()` 将阻塞最多一个未指定的超时间隔，直到建立连接。如果在建立连接之前超时间隔到期，`connect()` 将失败并且连接尝试将被中止。如果在被阻塞等待建立连接时 `connect()` 被信号中断，`connect()` 将失败并将 `errno` 设置为 EINTR，但不应中止连接请求，而应异步建立连接。
- 套接字的文件描述符设置为 `O_NONBLOCK` 且无法立即建立连接 ，则 `connect()` 将失败并将 `errno` 设置为 EINPROGRESS，但不应中止连接请求，而应异步建立连接。在建立连接之前，对同一套接字的 `connect()` 后续调用将失败并 `errno` 设置为 EALREADY 。

当异步建立连接时， `select()` 和 `poll()` 应指示套接字的文件描述符已准备好写入。

正在使用的套接字可能需要进程具有适当的权限才能使用 `connect()` 函数


```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in dest_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // 设置服务器地址
    memset(&serv_addr, 0, sizeof(serv_addr));
    dest_addr.sin_family = AF_INET;  // IPv4 protocol
    dest_addr.sin_addr.s_addr = inet_addr(IP);
    dest_addr.sin_port = htons(PORT); // port
    
    
    // 连接到服务器
    if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("Connection Failed");
        exit(1);
    }

    printf("Successfully connected to the server.\n");

    // 发送数据到服务器
    char *message = "Hello, Server!";
    if (send(sockfd, message, strlen(message), 0) < 0) {
        perror("Send failed");
        exit(1);
    }

    printf("Hello message sent\n");

    // 关闭套接字
    close(sockfd);
    return 0;
}
```