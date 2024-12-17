分配一个通信标识符

```c
#include <rdma/rdma_cma.h>

int rdma_create_id (struct rdma_event_channel* channel, struct rdma_cm_id **id, void *context, enum rdma_port_space ps);
```

参数
- `channel` -  与分配的`rdma_cm_id` 关联的 events 将会报告到此通信通道上。可能为 NULL（?同步/异步）
- `id` - 分配的通信标识符
- `context` - 用于指定的与 `rdma_cm_id` 关联的 context
- `ps` - RDMA port space

返回值
成功时返回 0 ，错误时返回 -1。若发生错误，设置 `errno` 以指示失败原因

注意：
`rdma_cm_id` 在概念上等同于用于 RDMA 通信的套接字。不同之处在于，RDMA 通信需要在通信发生之前显式绑定到指定的 RDMA 设备，并且大多数操作本质上是异步的。 `rdma_cm_id` 上的异步通信事件通过关联的 事件通道(`rdma_event_channel`) 报告。如果通道参数为NULL，则`rdma_cm_id`将被置于同步操作中。同步操作时，导致事件的调用将阻塞，直到操作完成。该事件将通过 `rdma_cm_id` 结构返回给用户，并且直到进行另一个 rdma_cm 调用才可以访问。
用户必须通过调用 `rdma_destroy_id` 释放` rdma_cm_id`。

Port Space
下述不同 port spaces 提供的服务的详细信息
- `RDMA_PS_IPOIB`：
- `RDMA_PS_TCP` ：提供可靠的、面向连接的 QP 通信。与 TCP 不同，RDMA port space 提供基于消息而非流的通信。
- `RDMA_PS_UDP`：提供不可靠、无连接的 QP 通信。支持数据报和多播通信。
- `RDMA_PS_IB`：提供任何 IB 服务（UD, UC, RC, XRC等）

```c
struct rdma_cm_id {
    struct ibv_context  *verbs;
    struct rdma_event_channel *channel;
    void            *context;
    struct ibv_qp       *qp;
    struct rdma_route    route;
    enum rdma_port_space     ps;
    uint8_t          port_num;
    struct rdma_cm_event    *event;
    struct ibv_comp_channel *send_cq_channel;
    struct ibv_cq       *send_cq;
    struct ibv_comp_channel *recv_cq_channel;
    struct ibv_cq       *recv_cq;
    struct ibv_srq      *srq;
    struct ibv_pd       *pd;
    enum ibv_qp_type    qp_type;
};
```