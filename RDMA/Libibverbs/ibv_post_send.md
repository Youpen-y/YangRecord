将 work requests, WR 发布到发送队列

```c
#include <infiniband/verbs.h>

int ibv_post_send(struct ibv_qp *qp, struct ibv_send_wr *wr, struct ibv_send_wr **bad_wr);
```

描述
`ibv_post_send()` 将以 `wr` 开头的工作请求链表发布到队列对 `qp` 的发送队列。它会在第一次失败时停止处理此列表中的 `WR` （在发布请求时可以立即检测到），并通过 `bad_wr` 返回此失败的 `WR`。

``` c
struct ibv_send_wr {
        uint64_t                wr_id;                  /* User defined WR ID */
        struct ibv_send_wr     *next;                   /* Pointer to next WR in list, NULL if last WR */
        struct ibv_sge         *sg_list;                /* Pointer to the s/g array */
        int                     num_sge;                /* Size of the s/g array */
        enum ibv_wr_opcode      opcode;                 /* Operation type */
        int                     send_flags;             /* Flags of the WR properties */
        uint32_t                imm_data;               /* Immediate data (in network byte order) */
        union {
                struct {
                        uint64_t        remote_addr;    /* Start address of remote memory buffer */
                        uint32_t        rkey;           /* Key of the remote Memory Region */
                } rdma;
                struct {
                        uint64_t        remote_addr;    /* Start address of remote memory buffer */
                        uint64_t        compare_add;    /* Compare operand */
                        uint64_t        swap;           /* Swap operand */
                        uint32_t        rkey;           /* Key of the remote Memory Region */
                } atomic;
                struct {
                        struct ibv_ah  *ah;             /* Address handle (AH) for the remote node address */
                        uint32_t        remote_qpn;     /* QP number of the destination QP */
                        uint32_t        remote_qkey;    /* Q_Key number of the destination QP */
                } ud;
        } wr;
};
```


scatter/gather list
``` c
struct ibv_sge {
        uint64_t                addr;      /* Start address of the local memory buffer */
        uint32_t                length;    /* Length of the buffer */
        uint32_t                lkey;      /* Key of the local Memory Region */
};
```

每个 QP 传输服务支持一组特定的操作码，如下表所示：
```c
OPCODE                      | IBV_QPT_UD | IBV_QPT_UC | IBV_QPT_RC
----------------------------+------------+------------+-----------
IBV_WR_SEND                 |     X      |     X      |     X
IBV_WR_SEND_WITH_IMM        |     X      |     X      |     X
IBV_WR_RDMA_WRITE           |            |     X      |     X
IBV_WR_RDMA_WRITE_WITH_IMM  |            |     X      |     X
IBV_WR_RDMA_READ            |            |            |     X
IBV_WR_ATOMIC_CMP_AND_SWP   |            |            |     X
IBV_WR_ATOMIC_FETCH_AND_ADD |            |            |     X
```

属性 `send_flags` 描述WR的属性，可以是 0 或 以下一个或多个标志的按位或：
`IBV_SEND_FENCE` - 设置 fence 指示器。仅对传输服务类型 `IBV_QPT_RC` 的QP有效
`IBV_SEND_SIGNALED` - 设置完成通知指示器。仅当使用 `sq_sig_all=0` 创建 QP 时相关
`IBV_SEND_SOLICITED` - 设置请求事件（solicited event）指示符。仅对 send and RDMA Write with immediate 有效
`IBV_SEND_INLINE` - 将给定收集列表中的数据（data in given gather list）作为一个 send WQE 中的内联数据发送。仅对 Send and RDMA Write 有效。不会检查 `L_Key` 

返回值
`ibv_post_send()` 成功时返回 0 ，失败时返回 `errno` 值（指示失败原因）

Note
在完全执行请求并且从相应的完成队列（CQ）中检索到 Work Completion 之前，用户不应更改或销毁与 WR 关联的 AH，以避免意外行为。
只有在 WR 请求完全执行并且从相应的完成队列（CQ）中检索到 Work Completion 后，WR使用的缓冲区才能安全地重用。但是，如果设置了 `IBV_SEND_INLINE` 标志，则可以在调用返回后立即重用缓冲区。