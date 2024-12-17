`rdma_resolve_addr` - 解析 目的 和可选 源地址

```c
#include <rdma/rdma_cma.h>

int rdma_resolve_addr (struct rdma_cm_id *id, struct sockaddr *src_addr, struct sockaddr *dst_addr, int timeout_ms);
```

参数
`id` -  RDMA 标识符
`src_addr` - 源地址
`dst_addr`  -  目标地址
`timeout_ms`  -  等待解析完成的时间

描述
将目标地址和可选源地址从 IP 地址解析为 RDMA 地址。如果成功，指定的 `rdma_cm_id` 将绑定到本地设备。

返回值
成功时返回 0 ；失败时返回 -1 ，并设置 `errno` 来指示失败原因。

NOTE
此调用用于将给定目标 IP 地址映射到可用的 RDMA 地址。IP 到 RDMA 地址的映射是使用本地路由表或通过 ARP 完成的。如果给出了源地址，则 `rdma_cm_id` 绑定到该地址，就像调用 `rdma_bind_addr` 一样。如果未给出源地址，并且 `rdma_cm_id` 尚未绑定到设备，则 `rdma_cm_id` 将根据本地路由表绑定到源地址。此调用后，`rdma_cm_id` 将绑定到 RDMA 设备。此调用通常是在调用 `rdam_resolve_route` 和 `rdma_connect` 之前从连接的主动端（active side, 客户端）进行的。

IB SPECIFIC
此调用将目标 IP 地址和源 IP 地址（如果给定）映射到 GID。为了执行映射，IPoIB 必须在本地和远程节点上运行。