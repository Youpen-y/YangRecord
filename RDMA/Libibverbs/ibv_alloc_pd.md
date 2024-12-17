`ibv_alloc_pd`, `ibv_dealloc_pd` - allocate or deallocate a protection domain (PDs)

语法
```c
#include <infiniband/verbs.h>

struct ibv_pd *ibv_alloc_pd(struct ibv_context *context);

int ibv_dealloc_pd(struct ibv_pd *pd);
```

描述
`ibv_alloc_pd()` 为 RDMA 设备上下文  `context` 分配 PD 。
`ibv_dealloc_pd()` deallocates  保护域 PD 。

