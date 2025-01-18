`rdma_reg_msgs` - 注册数据缓冲区以发送或接收消息

语法：
```c
#include <rdma/rdma_verbs.h>

struct ibv_mr *rdma_reg_msgs(struct rdma_cm_id *id, void *addr, size_t lenght);
```

参数
`id` - 对将使用消息缓冲区的通信标识符的引用
`addr` - 要注册的内存缓冲区的地址
`length` - 要注册的内存的总长度

描述
注册用于发送和接收消息或用于 RDMA 操作的内存缓冲区数组。使用 `rdma_reg_msgs` 注册的内存缓冲区可以使用 `rdma_post_send` 或 `rdma_post_recv` 发布到 `rdma_cm_id` ，或者指定为 `RDMA` 读取操作的目标或 RDMA 写入请求的源。

返回值
如果成功，则返回对已注册内存区域的引用，如果出错则返回 NULL 。
如果出错，将设置 `errno` 来指示失败原因。


注意：
`rdma_reg_msgs` 用于注册数据缓冲区数组，该数组将用于在与 `rdma_cm_id` 关联的队列对上发送和/或接收消息。内存缓冲区注册到与标识符关联的保护域中。数据缓冲区数组的起始位置通过 `addr` 参数指定，数组的总大小由 `length` 给出。

所有数据缓冲区应在作为工作请求发布之前注册。用户必须通过调用 `rdma_dereg_mr` 取消注册所有已注册的内存。




