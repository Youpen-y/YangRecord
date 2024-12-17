#### 主机和网络字节序转换
h - host order
n - net order
l  - long (4 bytes)
s - short (2 bytes)
``` c
htonl, htons, ntohl, ntohs - convert values between host and network byte order
```

#### 头文件
``` c
#include <arpa/inet.h>
```

#### 函数原型及描述
``` c
uint32_t htonl(uint32_t hostlong);
```
`htonl` 函数将无符号整数 `hostlong` 从主机序转化为网络序

---

``` c
uint16_t htons(uint16_t hostshort);
```
`htons` 函数将无符号短整数 `hostlong` 从主机序转化为网络序

---

``` c
uint32_t ntohl(uint32_t netlong);
```
`ntohl` 函数将无符号整数 `netlong` 从网络序转化为主机序

---

``` c
uint16_t ntohs(uint16_t netshort);
```
`ntohs` 函数将无符号短整数 `netshort` 从网络序转化为主机序


在 i386 上，主机序是 Least Significant Byte first（小端序）；
而网络序通常是 Most Significant Byte first（大端序）。