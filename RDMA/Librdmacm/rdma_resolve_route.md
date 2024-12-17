解析建立连接所需的路由信息

```c
#include <rdma/rdma_cma.h>

int rdma_resolve_route(struct rdma_cm_id *id, int timeout_ms);
```

参数
`id` - RDMA 标识符
`timeout_ms` - 等待解析完成的时间

描述
解析到目标地址的RDMA路由以建立连接。目标地址必须已通过调用 `rdma_resolve_addr` 解析

返回
成功时返回 0 ，错误时返回 -1 。发生错误，设置 `errno` 来指示失败原因。

NOTES
该函数在 `rdma_resolve_addr` 之后、`rdma_connect 之前调用。

INFINIBAND SPECIFIC
该调用获取连接使用的路径记录。