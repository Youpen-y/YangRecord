`rdma_create_ep` - 分配通信标识符（`rdma_cm_id`）和可选的 QP

```c
#include <rdma/rdma_cma.h>

int rdma_create_ep(struct rdma_cm_id **id, struct rdma_addrinfo *res, struct ibv_pd *pd, struct ibv_qp_init_attr *qp_init_attr);
```

参数
`id` - 将返回分配的通信标识符的引用
`res` - 从 `rdma_getaddrinfo` 返回的 `rdma_cm_id` 关联的地址信息
`pd` - 如果 QP 与 `rdma_cm_id` 关联，则为可选保护域
`qp_init_attr` - 可选的初始 QP 属性

描述
创建用于跟踪通信信息的标识符

返回值
成功时返回 0 ，失败时返回 -1 （并设置 `errno` ）

NOTES
使用 `rdma_getaddrinfo()` 解析地址信息后，用户可以使用此调用根据结果分配 `rdma_cm_id` 。
如果 `rdma_cm_id` 将用于连接的活动端（client），这意味着 `res->ai_flag` 没有设置 `RAI_PASSIVE` ，并且 `qp_init_attr` 不为 NULL ，`rdma_create_ep` 将自动在 `rdma_cm_id` 上创建 QP 。QP 将与指定的保护域（如果提供 pd）相关联，或者与默认保护域相关联。用户应参照 `rdma_create_qp` 了解有关 `pd` 和 `qp_init_attr` 参数使用的详细信息。调用 `rdma_create_ep()` 后，返回的 `rdma_cm_id` 可以通过调用 `rdma_connect()` 进行连接。主动方（client）调用 `rdma_resolve_addr()` 和 `rdma_resolve_route()` 不是必须的。

如果 `rdma_cm_id` 将用在连接的被动端（server，通过设置 `res->ai_flag` 为 RAI_PASSIVE 来指示），则此调用将保存提供的 pd 和 qp_init_attr 参数 。当通过调用 `rdma_get_request()` 检索新的连接请求时，与新连接关联的 `rdma_cm_id` 将自动使用 pd 和 `qp_init_attr` 参数与 QP 关联。在调用 `rdma_create_ep()` 之后，可以通过立即调用 `rdma_listen()` 将返回的 `rdma_cm_id` 置于监听状态。被动方调用 `rdma_bind_addr()` 不是必须的。之后可以通过调用 `rdma_get_request()` 检索连接请求。

新创建的 `rdma_cm_id` 将被设置为使用同步操作。希望异步操作的用户必须使用 `rdma_migrate_id()` 将 `rdma_cm_id` 迁移到用户创建的事件通道。

用户必须通过调用 `rdma_destroy_ep()` 来释放创建的 `rdma_cm_id`。














