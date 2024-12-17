`bind()` - bind a name to a socket （此处 name 指主机地址信息：<IP, port, etc.>）

```c
#include <sys/types.h>
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *addr, int addrlen);
```

使用 `socket()` 创建的套接字 `sockfd` 最初是未命名的，只能通过其地址簇来识别。
`bind()` 函数将 `addr` 指向的本地套接字地址（sockaddr）分配给 `sockfd` 。

参数
`sockfd` - 指定要绑定的套接字的文件描述符
`addr` - 指向包含要绑定到套接字的地址的 `sockaddr` 结构。地址的长度和格式取决于套接字的地址簇。
`addrlen` - 指定 `addr` 参数指向的 `sockaddr` 结构的长度

![[通用结构#^856579]]

自动选取可用端口以及本机 IP 地址
```c
addr.sin_port = htons(0);  // choose an unused port at random
addr.sin_addr.s_addr = htonl(INADDR_ANY);  // use my IP address
```


NOTE
1. `sockfd` 指向的套接字可能需要进程有适当的权限才能使用 `bind()` 函数。
2. 应用程序可以使用 `getsockname()` [[getsockname]] 函数检索指定的 `socket` 的 name 。
3. 所有低于 1024 (0-1023) 的端口都是保留的（除非是超级用户），可使用（1024-65535）中未被使用的端口
4. 有时，杀掉服务器后再次运行程序可能显示 `Address already in use` （`bind()` 失败），原因是占用了端口的套接字仍然挂在内核中，需要等待它被清除，或向代码添加如下代码以允许其重用端口。
```c
// 设置 socket 重用端口属性
int yes = 1;

if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    exit(1);
}
```
5. 某些情况`bind()` 并不是必要的。如，`connect()` 到远程机器，不关心本地的端口是什么，可以简单地调用 `connect()` 。（它会检查套接字是否未绑定，并在必要的情况下，绑定到未使用的本地端口）

---

示例：
```c
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define ADDR "10.12.110.57"

int main() {
    int sockfd;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // error checking omitted

    addr.sin_family = AF_INET;    // 主机序
    addr.sin_port = htons(PORT);  // short, 网络序
    addr.sin_addr.s_addr = inet_addr(ADDR);
    memset(&(addr.sin_zero), '\0', 8);  // zero the rest of the struct

    // don't forget your error checking for bind()
    bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
    
}
```