`rdma_migrate_id` - 迁移 `rdma_cm_id` 至一个新的事件通道。

```c
int rdma_migrate_id(struct rdma_cm_id *id, struct rdma_event_channel *channel);
```

NOTES
该函数将 `通信标识符` 迁移到指定的事件通道，并将与 `rdma_cm_id` 关联的任何待处理事件移动到新通道。在通道之间迁移时，用户不应轮询 `rdma_cm_id` 的当前事件通道上的事件或对 `rdma_cm_id` 调用其他函数。如果当前事件通道上存在任何未确认的事件时，此调用将被阻止。

如果通道参数为 NULL ，则指定的 `rdma_cm_id` 将被置于同步操作模式。对该 `id` 的所有调用都将阻塞，直到操作完成。