`ibv_get_cq_event` , `ibv_ack_cq_events` - 获取并确认完成队列（CQ）事件

```c
#include <infiniband/verbs.h>

int ibv_get_cq_event(struct ibv_comp_channel *channel, struct ibv_cq **cq, void **cq_context);
void ibv_ack_cq_events(struct ibv_cq *cq, unsigned int nevents);
```

描述
`ibv_get_cq_event()` 等待完成事件通道 `channel` 中下一个完成事件。使用获取事件的 CQ 填充参数 `cq` ，使用 CQ 的上下文填充参数 `cq_context` 。

返回值
`ibv_get_cq_event()` 成功时返回 0 ，失败时返回 -1 .

NOTES
`ibv_get_cq_event()` 返回的所有完成事件都必须使用 `ibv_ack_cq_events()` 进行确认。为了避免竞争，销毁 CQ 将等待所有完成事件得到确认；这确保 `gets` 和 `acks` 之间的一一对应关系。

在数据路径中调用 `ibv_ack_cq_events()` 可能相对昂贵，因为它必须使用 mutex 。因此，最好通过记录需要确认的事件数量并在一次调用 `ibv_ack_cq_events()` 中确认多个完成事件来分摊此成本。

示例
以下代码示例演示了处理完成事件的一种可能方法。
步骤：
Stage I : 准备
1. 创建一个 CQ
2. 请求新（第一个）完成事件的通知
Stage II : Completion Handling Routine
3. 等待完成事件并确认（ack）它
4. 请求下一个完成事件的通知
5. 清空 CQ

```c
cq = ibv_create_cq(ctx, 1, ev_ctx, channel, 0);
if (!cq) {
    fprintf(stderr, "Failed to create CQ\n");
    return 1;
}

/* 在任何 completion 被创建之前，请求通知 */
if (ibv_req_notify_cq(cq, 0)) {
    fprintf(stderr, "Couldn't request CQ notification\n");
    return 1;
}

...

/* 等待完成事件 */
if (ibv_get_cq_event(channel, &ev_cq, &ev_ctx)) {
    fprintf(stderr, "Failed to get cq_event\n");
    return 1;
}
/* Ack the event */
ibv_ack_cq_events(ev_cq, 1);
/* 请求对下一个完成事件的通知 */
if (ibv_req_notify_cq(ev_cq, 0)) {
    fprintf(stderr, "Couldn't request CQ notification\n");
    return 1;
}

/* Empty the CQ: poll all of the completions from the CQ (如果存在的话) */
do {
    ne = ibv_poll_cq(cq, 1, &wc);
    if (ne < 0) {
        fprintf(stderr, "Failed to poll completions from the CQ\n");
        return 1;
    }
    /* 在CQ中可能存在额外未完成的事件 */
    if (ne == 0)
        continue;
    if (wc.status != IBV_WC_SUCCESS) {
        fprintf(stderr, "Completion with status 0x%x was found\n", wc.status);
        return 1;
    }
} while (ne);
```

以下代码示例演示在非阻塞模式下处理完成事件的一种可能方法
步骤：
1. 将完成事件通道（completion event channel）设置为非阻塞
2. 轮询通道直到有完成事件
3. 获取完成事件并确认它
```c
/* 改变完成通道的阻塞模式 */
flags = fcntl(channel->fd, F_GETFL);
rc = fcntl(channel->fd, F_SETFL, flags | O_NONBLOCK);
if (rc < 0) {
    fprintf(stderr, "Failed to change file descriptor of completion event channel\n");
    return 1;
}

/*
 * poll the channel until it has an event and sleep ms_timeout
 * milliseconds between any iteration
 */
my_pollfd.fd      = channel->fd;
my_pollfd.events  = POLLIN;
my_pollfd.revents = 0;
do {
	rc = poll(&my_pollfd, 1, ms_timeout);
} while (rc == 0);
if (rc < 0) {
	fprintf(stderr, "poll failed\n");
	return 1;
}
ev_cq = cq;
/* Wait for the completion event */
if (ibv_get_cq_event(channel, &ev_cq, &ev_ctx)) {
        fprintf(stderr, "Failed to get cq_event\n");
        return 1;
}
/* Ack the event */
ibv_ack_cq_events(ev_cq, 1);
```