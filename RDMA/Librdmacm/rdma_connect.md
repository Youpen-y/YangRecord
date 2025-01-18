`rdma_connect`  -  发起主动的连接请求

```c
#include <rdma/rdma_cma.h>

int rdma_connect(struct rdma_cm_id *id, struct rdma_conn_param *conn_param);
```

`id` -  RDMA 标识符
`conn_param`  -  连接参数

```c
struct rdma_conn_param { 
	const void *private_data; // 私有数据 
	uint8_t private_data_len; // 私有数据长度 
	uint8_t responder_resources; // 响应者资源数量 
	uint8_t initiator_depth; // 发起者深度 
	uint8_t flow_control; // 流控制 
	uint8_t retry_count; // 重试次数 
	uint8_t rnr_retry_count; // RNR重试次数 
	uint8_t srq; // 是否使用SRQ 
	uint8_t qp_num; // QP号 
};
```

`private_data` - 指向用户自定义数据的指针
`private_data_len` - 自定义数据的长度，最大通常为 56 字节
- 这些数据会在连接建立过程中传递给对端
- 常用于交换初始化信息,如QP配置参数等

`responder_resources` - 指定接收端可以同时处理的未完成的 RDMA Read 请求数量
 - 影响接收端为 RDMA Read 分配的资源
 - 设置过小可能限制性能，设置过大会浪费资源
 - 一般根据预期的并发RDMA Read操作数来设置

`initiator_depth` - 指定发起端可以发出的未完成的RDMA Read请求数量
- 必须小于或等于对端的 `responder_resources`
- 控制发起端能够同时进行的RDMA Read操作数量
- 影响内存使用和性能表现

`flow_control` - 控制QP(队列对)的流控机制
- 0 表示禁用流控,非0 启用
- 启用流控可以防止接收队列溢出

`retry_count` - 指定在报告错误之前重试发送请求的次数
- 用于处理瞬时网络故障
- 通常设置为7-8比较合理

`rnr_retry_count` - RNR(Receiver Not Ready)重试次数
- 当接收端暂时无法处理请求时的重试次数
- 可以设置为0(不重试)到7(最大重试)
- 建议设置为7以提高可靠性

`srq` - 是否使用共享接收队列(Shared Receive Queue)
- 0表示不使用,非0使用
- SRQ可以在多个QP之间共享接收资源

`qp_num` -  指定要使用的QP号
- 通常设为0让系统自动分配

描述
对于 `RDMA_PS_TCP` 类型的 `rdma_cm_id` ，此调用发起到远程目标（remote destination） 的连接请求。对于 `RDMA_PS_UDP` 类型的 `rdma_cm_id` ，它启动对提供数据报服务的远端 QP 的查找。

返回值
成功时返回 0 ；失败时返回 -1 ，并设置 `errno` 来指示失败原因。

Note
在调用此例程之前，用户必须通过调用 `rdma_resolve_route` 或 `rdma_create_ep` 解析到目标地址的路由。