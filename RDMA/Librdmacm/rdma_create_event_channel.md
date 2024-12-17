`rdma_create_event_channel`  -  打开用于报告通信事件的通道

```c
#include <rdma/rdma_cma.h>

struct rdma_event_channel *rdma_create_event_channel(void);
```

描述
异步事件通过（Event Channels）事件通道报告给用户。

NOTE
事件通道用于 direct `rdma_cm_id` 上的所有事件。对于许多客户端来说，单个事件通道可能就足够了，但是，当管理大量连接或 `cm_id` 时，用户可能会发现将不同 `cm_id` 的事件定向到不同通道进行处理很有用。

所有创建的事件通道必须通过调用 `rdma_destroy_event_channel` 来销毁。用户应调用 `rdma_get_cm_event` 来检索事件通道上的事件。

每个事件通道都映射到一个文件描述符。可以像任何其他 `fd` 一样使用和操作以改变它的行为。用户可以将 `fd` 设为非阻塞、轮询或 select the fd 等。

```c
struct rdma_event_channel {
    int         fd;
};
```