`rdma_disconnect` - 该函数断开连接

```c
#include <rdma/rdma_cma.h>

int rdma_disconnect(struct rdma_cm_id *id);
```

参数
`id` - RDMA 标识符

描述
断开连接并将任何关联的 QP 转换为错误状态，这会将所有已发布的工作请求（work requests）刷新到完成队列。该函数应该由连接的客户端和服务器端（both）调用。成功断开连接后，连接两端都会生成 `RDMA_CM_EVENT_DISCONNECTED` 事件。

返回值
成功时返回 0 ，错误时返回 -1 （设置 `errno` 指示失败原因）。

