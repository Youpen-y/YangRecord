`rdma_ack_cm_event` - 释放一个通信事件

```c
#include <rdma/rdma_cma.h>

int rdma_ack_cm_event(struct rdma_cm_event *event);
```

参数
`event` - 待释放的事件

描述
所有 `rdma_get_cm_event` 分配的事件都必须被释放，成功的 gets 和 acks 之间应该是一一对应的。此调用释放 `event` 结构及其引用的任何内存。

返回值
成功时返回 0 ，失败时返回 -1 （设置 `errno`）




