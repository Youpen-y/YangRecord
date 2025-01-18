`rdma_dereg_mr` - 注销已注册的内存区域

语法
```c
#include <rdma/rdma_verbs.h>
int rdma_dereg_mr(struct ibv_mr *mr);
```

参数
`mr` - 对已注册内存缓冲区的引用。


描述
取消注册已注册用于 RDMA 或消息操作的内存缓冲区。在销毁 `rdma_cm_id` 之前，用户应为与 `rdma_cm_id` 关联的所有已注册内存调用 `rdma_dereg_mr`。

返回值
成功时返回 0 ；错误时返回 -1 并设置 `errno` 来指示失败原因。

Note:
所有使用 `rdma_cm_id` 注册的内存都与与该 id 关联的保护域相关联。用户必须在保护域被破坏之前取消注册所有已注册的内存。