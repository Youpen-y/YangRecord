`ibv_create_cq` - 创建完成队列 （CQ）

```c
#include <infiniband/verbs.h>

struct ibv_cq *ibv_create_cq(struct ibv_context *context, int cqe,
                             void *cq_context,
                             struct ibv_comp_channel *channel,
                             int comp_vector);
```

描述
`ibv_create_cq()` 为 RDMA 设备上下文创建完成队列（CQ）。

当发送或接收队列中的一个未完成的工作请求被完成了，一个 Work Completion 将添加到该工作队列的 CQ 中。此工作完成表明未完成的工作请求已完成（不再被视为未完成）并提供其详细信息（status, direction, opcode ）

单个 CQ 可以共享用于在多个 QP 之间发送、接收队列共享。Work Completion 保存指定 QP 编号及其来源的队列（Send or Receive） 的信息。

用户可以定义 CQ 的最小大小，实际创建的大小可以等于或大于该值。

参数
- `context` - 从 `ibv_open_device()` 返回的 RDMA 设备上下文
- `cqe` - 请求的CQ 的最小容量，\[1.. dev_cap.max_cqe]
- `cq_context` - （可选）用户定义的值将在 `cq->cq_context` 中可用。当使用 `ibv_get_cq_event()` 等待完成事件通知时，将返回该值。
- `channel` - （可选）完成事件通道，用于指示新的工作完成已添加到此 CQ 。NULL 表示将不使用完成事件通道。
- `comp_vector` - 将用于发送完成事件信号的 MSI0-X 完成向量。如果这些中断的 IRQ 关联性掩码已配置为将每个 MSI-X 中断分散到由不同的内核处理，则可以使用此参数将完成工作负载分散到多个内核上。值可以是 \[0.. context->num_comp_vectors]