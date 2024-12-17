`ibv_post_recv` - 将工作请求（WRs）列表发布到接收队列

```c
#include <infiniband/verbs.h>

int ibv_post_recv(struct ibv_qp *qp, struct ibv_recv_wr *wr, struct ibv_recv_wr **bad_wr);
```

描述
`ibv_post_recv()` 将以 `wr` 开头的工作请求链表发布到队列对（QP）的接收队列。它会在第一次失败时停止处理此列表中的 WR（在发布请求时可以立即检测到），并通过 `bad_wr` 返回此失败的 `WR` 。

返回值
`ibv_post_recv()` 成功时返回 0 ，失败时返回 `errno` 值（可指示失败原因）。

Notes
只有在 WR 请求完全执行并且从相应的完成队列（CQ）中检索到 Work Completion 后，WR 使用的缓冲区才能安全地重用。

如果 QP `qp` 与共享队列关联，则必须使用函数 `ibv_post_srq_recv()` 而不是 `ibv_post_recv()` ，因为 QP 自己的接收队列将不会被使用。

如果 WR 被发布到 UD QP，则传入消息（incoming message）的全局路由标头（GRH）将被放置在 scatter list 的前 40 个字节中。如果传入消息中不存在 GRH，则第一个字节将不确定。这意味着在所有情况下，传入消息的实际数据将从 scatter list 中缓冲区偏移 40 字节开始。

```c
struct ibv_recv_wr {

    uint64_t        wr_id;            /* 用户定义的 WR ID */

    struct ibv_recv_wr     *next;     /* 指向链表中下一个 WR 的指针，如果是最后一个WR，该值为 NULL */

    struct ibv_sge         *sg_list;  /* 指针指向 s/g array */

    int         num_sge;              /* s/g array 的大小 */

};
```

```c
struct ibv_sge {

    uint64_t        addr;

    uint32_t        length;

    uint32_t        lkey;

};
```