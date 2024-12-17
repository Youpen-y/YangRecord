`getservent`，`getservbyname`，`getservbyport`，`setservent`，`endservent` - get service entry

头文件
```c
#include <netdb.h>
```

```c
struct servent *getservent(void);
```

```c
struct servent *getservbyname(const char *name, const char *proto);
struct servent *getservbyport(int port, const char *proto);
```

```c
void setservent(int stayopen);
void endservent(void);
```

描述
在 `<netdb.h>` 中定义的 `servent` 结构：
```c
struct servent {
    char  *s_name;    /* 官方 service name */
    char **s_aliases; /* 别名列表（以 NULL 结尾） */
    int    s_port;    /* 端口号（按网络字节顺序） */
    char  *s_proto;   /* 使用的协议 */
};
```

`getservent()` 函数从 services 数据库中读取下一个条目，并返回一个 `servent` 结构，其中包含该条目中断开的字段。如有必要，将打开与数据库的连接。

`getservbyname()` 函数为数据库中匹配协议 `proto` 与服务 `name` 的条目返回一个 `servent` 结构。如果 `proto` 为 NULL，则将匹配任何协议。如有必要，将打开与数据库的连接。

`getservbyport()` 函数为数据库中匹配协议 `proto` 与端口 `port` 的条目返回一个 `servent` 结构。如果 `proto` 为 NULL，则将匹配任何协议。如有必要，将打开与数据库的连接。

`setservent()` 函数打开与数据库的连接，并将下一个条目设置为第一个条目。如果 `stayopen` 为非零，则在调用其中上述一个 `getserv*()` 函数之间不会关闭与数据库的连接。

`endservent()` 函数关闭与数据库的连接。

返回值
`getservent()`，`getservbyname()`，`getservbyport()` 函数返回指向静态分配的 `servent` 结构的指针，如果失败或到达文件末尾，则返回 NULL 。