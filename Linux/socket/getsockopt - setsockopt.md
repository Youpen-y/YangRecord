`getsockopt`, `setsockopt` - 获取，设置套接字选项

允许程序员查询和修改套接字的各种属性和行为。

```c
#include <sys/socket.h>

int getsockopt(int sockfd, int level, int optname,
              void *optval, socklen_t *optlen);
```
描述
`getsockopt()` 函数操纵与套接字 `sockfd` 关联的选项。 

选项
`sockfd` - 套接字描述符
`level` - 协议层级（如，`SOL_SOCKTE`(检索套接字级别的选项), `IPPROTO_TCP`(选项由 TCP 解释)）
`optname` - 选项名称
- `SO_KEEPALIVE` - 连接是否定期传输消息保持活动状态（如果协议支持）
- `SO_OOBINLINE` - 套接字是否将接收到的带外数据（out-of-band data, 标记为紧急的数据）保留为内联。该选项应存为一个 int 值。
- `SO_SNDBUF` - 发送缓冲区的大小，（int）
- `SO_RCVBUF` - 接收缓冲区的大小，（int）
- `SO_TYPE` - 套接字类型，（int）
`optval` - 指向存储选项值的缓冲区
`optlen` - 指向表示 `optval` 缓冲区大小的变量的指针

---

```c

int setsockopt(int sockfd, int level, int optname,
              const void *optval, socklen_t optlen);
```

参数：
- `sockfd`: 套接字描述符
- `level`: 协议层级
- `optname`: 选项名称
- `optval`: 指向包含新选项值的缓冲区
- `optlen`: `optval` 缓冲区的大小


返回值
成功时，标准选项返回 0 ；出错时返回 -1 ，并设置 `errno` 来指示错误。

示例1 - 设置和获取 TCP keep-alive 选型
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 设置 TCP keep-alive
    int keepalive = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_KEEPLIVE, &keepalive, sizeof(keepalive)) < 0) {
        perror("setsockopt");
        return -1;
    }

    // 获取 TCP keep-alive 设置
    int optval;
    socklen_t optlen = sizeof(optval);
    if (getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
        perror("getsockopt");
        return -1;
    }
    printf("TCP keep-Alive is %s\n", optval ? "ON" : "OFF");

    return 0;
}
```

示例2 - 设置和获取接收缓冲区大小
```c
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 设置接收缓冲区大小
    int rcvbufsize = 32768; // 32k
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &rcvbufsize, sizeof(rcvbufsize)) < 0) {
        perror("setsockopt");
        return -1;
    }

    // 获取实际得接收缓冲区大小
    int actual_rcvbuf;
    socklen_t optlen = sizeof(actual_rcvbuf);
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &actual_rcvbuf, &optlen) < 0) {
        perror("getsockopt");
        return -1;
    }
    printf("Actual receive buffer size: %d\n", actual_rcvbuf);

    return 0;
}
```

示例3 - 设置和获取超时选项
```c
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 设置接收超时
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt");
        return -1;
    }

    // 获取接收超时设置
    struct timeval get_timeout;
    socklen_t optlen = sizeof(get_timeout);
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &get_timeout, &optlen) < 0) {
        perror("getsockopt");
        return -1;
    }
    printf("Receive timeout: %ld seconds and %ld microseconds\n", get_timeout.tv_sec, get_timeout.tv_usec);

    return 0;
}
```

示例4 - 设置和获取地址重用选项
```c
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 设置地址重用
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        return -1;
    }

    // 获取地址重用设置
    int get_reuse;
    socklen_t optlen = sizeof(get_reuse);
    if (getsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &get_reuse, &optlen) < 0) {
        perror("getsockopt");
        return -1; 
    } 
    printf("Address reuse is %s\n", get_reuse ? "enabled" : "disabled"); 
    return 0; 
}
```