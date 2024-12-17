`rdma_bind_addr` - 将 RDMA 标识符绑定到源地址

```c
#include <rdma/rdma_cma.h>

int rdma_bind_addr(struct rdma_cm_id *id, struct sockaddr *addr);
```

`id` - RDMA 标识符
`addr` - 本地地址信息。允许使用 Wildcard 。

描述
将源地址与 `rdma_cm_id` 相关联。地址可以是通配符。如果绑定到特定的本地地址， `rdma_cm_id` 也将绑定到本地 RDMA 设备。

返回值
成功时返回 0 ， 失败时返回 -1 。如果发生错误，设置 `errno` 来指示失败原因。

Notes
通常，在调用 `rdma_listen()` 绑定到特定端口号之前调用此例程，但也可能在调用 `rdma_resolve_addr` 绑定到特定地址之前，在连接的 active 端调用。

如果用于绑定到端口 0，`rdma_cm` 将选择一个可用端口，可以使用 `rdma_get_src_port` 检索该端口。

示例：
```c
// 服务器端监听示例
void server_listen() {
    struct rdma_event_channel *ec;
    struct rdma_cm_id *listener;
    struct sockaddr_in addr;

    ec = rdma_create_event_channel();
    rdma_create_id(ec, &listener, NULL, RDMA_PS_TCP);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;

    rdma_bind_addr(listener, (struct sockaddr *)&addr);
    rdma_listen(listener, 10);

    while (1) {
        handle_cm_event(ec);
    }
}

// 客户端连接示例
void client_connect() {
    struct rdma_event_channel *ec;
    struct rdma_cm_id *id;
    struct sockaddr_in addr;

    ec = rdma_create_event_channel();
    rdma_create_id(ec, &id, NULL, RDMA_PS_TCP);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    inet_pton(AF_INET, "192.168.1.1", &addr.sin_addr);

    rdma_resolve_addr(id, NULL, (struct sockaddr *)&addr, 2000);

    while (1) {
        handle_cm_event(ec);
    }
}
```