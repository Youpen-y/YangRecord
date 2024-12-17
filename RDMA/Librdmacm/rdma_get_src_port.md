`rdma_get_src_port` - 返回绑定 `rdma_cm_id` 的本地端口号

```c
#include <rdma/rdma_cma.h>

uint16_t rdma_get_src_port(struct rdma_cm_id *id);
```

参数
`id` - RDMA 标识符

描述
返回已绑定到本地地址的 `rdma_cm_id` 的本地端口号

返回值
返回与本地端点关联的 16-bit 端口标识符，如果 `rdma_cm_id` 未绑定端口，则返回值为 0 。
