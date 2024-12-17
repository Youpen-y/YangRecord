`rdma_notify` - 通知 `librdmacm` 异步事件

```c
#include <rdma/rdma_cma.h>

int rdma_notify(struct rdma_cm_id *id, enum ibv_event_type event);
```

参数
`id` - RDMA 标识符
`event` - 异步事件

描述
用于通知 `librdmacm` 与 `rdma_cm_id` 关联的 QP 上发生的异步事件

返回值
成功时返回 0 ，失败时返回 -1 （并设置 `errno`）
如果 `errno` 设置为 `EISCONN` （传输端点已连接），则表明底层通信管理器在处理对 `rdma_notify` 的调用之前建立了连接。在这种情况下，可安全地忽略该错误。

NOTES
QP 上发生的异步事件通过用户的设备事件处理程序（device event handler）报告。
本函数用于通知 `librdmacm` 通信事件。在大多数情况下，不需要使用该函数，但如果连接建立是在带外完成的（如，通过Infiniband完成的），则可以在尚未被视为已连接的 QP 上接收数据。在这种情况下，该函数强制连接进入已建立状态（established state），以处理连接永远不会自行形成的罕见请开给你。调用该函数可确保将 `RDMA_CM_EVENT_ESTABLISHED` 事件传递到应用程序。应向 CM 报告的事件有 `IB_EVENT_COMM_EST`