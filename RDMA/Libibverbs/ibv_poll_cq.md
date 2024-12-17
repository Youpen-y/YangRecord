`ibv_poll_cq` - 轮询完成队列（CQ）

```c
#include <infiniband/verbs.h>

int ibv_poll_cq(struct ibv_cq *cq, int num_entries, struct ibv_wc *wc);
```

描述
`ibv_poll_cq()` 轮询 CQ `cq` 的工作完成情况，并返回前 `num_entries` 个（或 如果 CQ 包含的 work completion 数目少于此数量，则返回所有可用的 completions）到数组 `wc` 中。 



返回值
成功时，返回一个非负值，指示找到的 completions 。失败时，返回负值。

笔记
每个轮询的 completion 都会从 CQ 中删除，并且无法返回给 CQ。
用户应以防止发生 CQ 溢出的速度消耗 Work Completions 。如果 CQ 溢出，将触发异步事件  `IBV_EVENT_CQ_ERR` ，并且 CQ 无法使用。