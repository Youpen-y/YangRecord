#socket-programming 
```
inet_aton, inet_addr, inet_network, inet_ntoa, inet_makeaddr, inet_lnaof, inet_netof - 互联网地址操作函数
```

头文件
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
```

函数原型
``` c
// in_addr 结构， 头文件 #include <netinet/in.h>
typedef uint32_t in_addr_t;

struct in_addr {
    in_addr_t s_addr;
};
```

####  `inet_aton()`
``` c
int inet_aton(const char *cp, struct in_addr *inp);
```

`inet_aton()` 将 Internet 主机地址 `cp` 从 IPv4 numbers-and-dots 表示法转换为二进制形式（以网络字节顺序）并将其存储在 `inp` 指向的结构中。如果地址有效，`inet_aton()` 返回非零值，否则返回零。\

`cp` 中提供的地址可以采用以下形式：

```
a.b.c.d     四个数字部分中的每一个都指定地址的一个字节；字节从左到右的顺序分配以产生二进制地址。
a           值 a 被解释为 32 位值，直接存储在二进制地址中，无需任何字节重新排列。
在上述形式中，点分地址的组成部分可以用十进制、八进制（以0开头）、十六进制（以0x开头）指定。
```

---
#### `inet_ntoa()`
``` c
char *inet_ntoa(struct in_addr in);
```
`inet_ntoa()` 函数将以网络字节顺序给出的 Internet 主机地址转换为 IPv4 点分十进制表示法的字符串。该字符串在静态分配的缓冲区中返回，后续调用将覆盖该缓冲区。

#### `inet_addr()`
``` c
in_addr_t inet_addr(const char *cp);
```
`inet_addr()` 函数将 Internet 主机地址 `cp` 从 IPv4 数字和点表示法转换为网络字节顺序的二进制数据。如果输入无效，返回 `INADDR_NONE` (通常为-1)。使用此函数是有问题的，因为 `-1` 是有效地址(`255.255.255.255`)。避免使用它，而应使用 `inet_aton()`, `inet_pton()`, `getaddrinfo()`，它们提供了一种更简洁的方式来指示错误返回。

#### `inet_network()`
``` c
in_addr_t inet_network(const char *cp);
```
`inet_network()` 函数将 `cp` (采用 IPv4 数字和点表示法的字符串)转换为适合用作 Internet 网络地址数字（主机字节顺序）。成功后，返回转换后的地址。如果输入无效，则返回 -1 。

#### `inet_makeaddr()`
``` c
struct in_addr inet_makeaddr(int net, int host);
```
`inet_makeaddr()` 函数是 `inet_netof()` 和 `inet_lnaof()` 的逆函数。它以网络字节顺序返回 Internet 主机地址，该地址是通过将网络号 `net` 与本地地址 `host`（两者均按主机字节顺序）组合而创建。

#### `inet_lnaof, inet_netof()`
``` c
in_addr_t inet_lnaof(struct in_addr in);
in_addr_t inet_netof(struct in_addr in);
```
`inet_lnaof()` 函数返回 Internet 地址的本地网络地址部分，返回值采用主机字节顺序。
`inet_netof()` 函数返回 Internet 地址的网络号部分。返回值采用主机字节顺序。

Note:
`i386` 主机字节顺序是小端序（Least Significant Byte first）
Internet 网络字节顺序是大端序（Most Significant Byte first）