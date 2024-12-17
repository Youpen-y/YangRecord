`ibv_req_notify_cq` - request completion notification on a completion queue (CQ)
请求完成队列（CQ）上的完成通知

```c
#include <infiniband/verbs.h>

int ibv_req_notify_cq(struct ibv_cq *cq, int solicited_only);
```

描述
    `ibv_req_notify_cq()` 请求完成队列 `cq` 上的完成通知。将新的 CQ 条目（CQE）添加到 `cq` 后，完成事件将添加到与 CQ 关联的完成通道中。
    如果参数 `solicited_only` 为零，则为任何新 CQE 生成完成事件。
    如果参数 `solicited_only` 不为零，则仅为新的 CQE（特定的） 生成事件，该 CQE 是“Solicited”（已请求）。
    A CQE is solicited if it is a receive completion for a message with the Solicited Event header bit set（设置了特定位）, or if the status is not successful（状态不成功）.  All other successful receive completions, or any successful send completion is unsolicited.

返回值
成功时返回 0 ，失败时返回 `errno` 值（指示失败原因）

NOTES
The request for notification is "one shot". Only one completion event will be generated for each call to `ibv_req_notify_cq()`.
请求通知是“一次性的”，每次调用 `ibv_req_notify_cq()` 仅一个完成事件将会产生。