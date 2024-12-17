`rdma_connect`  -  发起主动的连接请求

```c
#include <rdma/rdma_cma.h>

int rdma_connect(struct rdma_cm_id *id, struct rdma_conn_param *conn_param);
```

`id` -  RDMA 标识符
`conn_param`  -  连接参数

描述
对于 `RDMA_PS_TCP` 类型的 `rdma_cm_id` ，此调用发起到远程目标（remote destination） 的连接请求。对于 `RDMA_PS_UDP` 类型的 `rdma_cm_id` ，它启动对提供数据报服务的远端 QP 的查找。

返回值
成功时返回 0 ；失败时返回 -1 ，并设置 `errno` 来指示失败原因。

Note
在调用此例程之前，用户必须通过调用 `rdma_resolve_route` 或 `rdma_create_ep` 解析到目标地址的路由。