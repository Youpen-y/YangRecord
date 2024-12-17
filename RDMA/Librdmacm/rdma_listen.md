`rdma_listen` - 监听传入的连接请求

```c
#include <rdma/rdma_cma.h>

int rdma_listen(struct rdma_cm_id *id, int backlog);
```

参数
`id` - RDMA 标识符
`backlog` - backlog of incoming connection requests

描述
启动侦听传入连接请求或 datagram service lookup。监听将被限制在本地绑定到源地址。

返回值
成功时返回 0 ，失败时返回 -1 （并设置 `errno`）

NOTES
在调用此函数前，用户必须通过调用 `rdma_bind_addr()` 将 `rdma_cm_id` 绑定到本地地址。如果 `rdma_cm_id` 绑定到特定 IP 地址，则侦听将仅限于该地址和关联的 RDMA 设备。如果 `rdma_cm_id` 仅绑定到 RDMA 端口号。则侦听将在所有 RDMA 设备上进行。