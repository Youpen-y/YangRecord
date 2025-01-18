`rdma_create_qp` - 分配一个 QP

```c
#include <rdma/rdma_cma.h>

int rdma_create_qp(struct rdma_cm_id *id, struct ibv_pd *pd, struct ibv_qp_init_attr *qp_init_attr);
```

参数
`id` - RDMA 标识符
`pd` - 可选的该 QP 的保护域
`qp_init_attr` - 初始 QP 属性

描述
分配与指定 `rdma_cm_id` 关联的 QP 并将其转换为发送和接收。

返回值
成功时返回 0，错误时返回 -1。如果发生错误，将设置 `errno` 来指示失败原因

NOTES
在调用此函数之前，`rdma_cm_id` 必须绑定到本地 RDMA 设备，并且保护域必须针对同一设备。分配给 `rdma_cm_id` 的 QPs 由 `librdmacm` 根据它们的状态自动转换。分配后， QP 将准备好处理 posting of receives 。如果 QP 未连接，它将准备好 post sends。
**如果未给出保护域，即 pd 参数为 NULL - 则将使用默认保护域创建 `rdma_cm_id` 。每个RDMA设备分配一个默认保护域。**

初始 QP 属性由 `qp_init_attr` 参数指定。`ibv_qp_init_attr` 中的 `send_cq` 和 `recv_cp` 字段是可选的。 如果未指定发送或接收完成队列，则 `rdma_cm` 将为 QP 分配 `CQ` 以及相应的完成通道。`rdma_cm` 创建的完成通道和 CQ 数据通过 `rdma_cm_id` 结构暴露给用户。

创建的 QP 的实际 capabilities 和 属性将通过 `qp_init_attr` 参数返回给用户。`rdma_cm_id` 只能与单个 QP 相关联。