`rdma_get_cm_event` - 检索下一个 pending（待处置） 的通信事件。

```c
#include <rdma/rdma_cma.h>

int rdma_get_cm_event(struct rdma_event_channel *channel, struct rdma_cm_event **event);
```

参数
`channel` - 用于检查事件的事件通道
`event` - 分配有关下一个通信事件的信息

描述
检索通信事件。如果没有待处理的事件，默认情况下，调用将阻塞，直到收到事件。

返回值
成功时返回 0 ， 失败时返回 -1 （设置 `errno` ）

NOTES
可以通过修改与给定通道关联的文件描述符来更改此函数的默认同步行为。所有报告的事件都必须通过调用 `rdma_ack_cm_event` 进行确认。`rdma_cm_id` 的销毁将被阻止，直到相关事件得到确认。

EVENT DATA
通信事件详细信息在 `rdma_cm_event` 结构中返回。该结构由 `rdma_cm` 分配并由 `rdma_ack_cm_event`  函数释放。

```c
struct rdma_cm_event {

    struct rdma_cm_id   *id;

    struct rdma_cm_id   *listen_id;

    enum rdma_cm_event_type  event;

    int          status;

    union {

        struct rdma_conn_param conn;

        struct rdma_ud_param   ud;

    } param;

};
```

`id` - 与事件关联的 `rdma_cm` 标识符。如果事件类型是 `RDMA_CM_EVENT_CONNECT_REQUEST`，则它引用该通信的新 ID。

`listen_id` - 对于 `RDMA_CM_EVENT_CONNECT_REQUEST` 事件类型，这引用相应的监听请求标识符。

`event` - 指定发生的通信事件的类型。
```c
enum rdma_cm_event_type {
    RDMA_CM_EVENT_ADDR_RESOLVED,    /* 地址解析成功完成 */
    RDMA_CM_EVENT_ADDR_ERROR,       /* 地址解析失败 */
    RDMA_CM_EVENT_ROUTE_RESOLVED,   /* 路由解析成功完成 */
    RDMA_CM_EVENT_ROUTE_ERROR,      /* 路由解析失败 */
    RDMA_CM_EVENT_CONNECT_REQUEST,  /* 在 passive side 方生成，通知用户有新的连接请求 */
    RDMA_CM_EVENT_CONNECT_RESPONSE, /* 在 active side 生成，通知用户已成功响应连接请求。仅在没有与其关联的QP的 rdma_cm_id 上生成  */
    RDMA_CM_EVENT_CONNECT_ERROR,    /* 尝试建立连接时发生错误，（主动/被动侧生成） */
    RDMA_CM_EVENT_UNREACHABLE,      /* active side 生成，通知用户远程服务器无法访问或无法响应连接请求。如果该事件是响应 InfiniBand 的 UD QP解析请求而生成的，则事件状态字段将包含 errno 或 IB CM SIDR REP 消息中携带的状态结果 */
    RDMA_CM_EVENT_REJECTED,         /* 连接请求或响应被远程端点拒绝。event status字段将包含传输特定的拒绝原因。在 InfiniBand 下，这是 IB CM REJ 消息中携带的拒绝原因 */
    RDMA_CM_EVENT_ESTABLISHED,      /* 表明已与远程端点建立连接 */
    RDMA_CM_EVENT_DISCONNECTED,     /* 连接已断开 */
    RDMA_CM_EVENT_DEVICE_REMOVAL,   /* 与 rdma_cm_id 关联的本地 RDMA 设备已被删除。收到此事件后，用户必须销毁相关的 rdma_cm_id */
    RDMA_CM_EVENT_MULTICAST_JOIN,   /* 多播加入操作（rdma_join_multicast）成功完成*/
    RDMA_CM_EVENT_MULTICAST_ERROR,  /* 加入多播组时发生错误，或者如果该组已加入，则在现有组上发生错误。指定的多播组不再可访问，如果需要，应重新加入 */
    RDMA_CM_EVENT_ADDR_CHANGE,      /* 通过地址解析与该 ID 关联的网络设备更改了其 HW 地址，如，在绑定故障转移之后。对于希望用于其 RDMA 会话的链接与网络堆栈保持一致的应用程序，此事件可以作为提示 */
    RDMA_CM_EVENT_TIMEWAIT_EXIT     /* 与连接关联的 QP 已退出其 timewait 状态，现在可以重新使用。 QP 断开连接后，它会保持在 timewait 状态，以允许正在传输的数据包退出网络。timewait 状态完成后， rdma_cm 将报告此事件 */
};
```

`status` - 返回与事件关联的任何异步错误信息。如果操作成功，则 `status` 为 0 ，否则状态值非零，并且设置为 `-errno` 或传输特定值。

`param` - 根据事件类型提供其他详细信息。用户应根据与事件关联的 `rdma_cm_id` 的 `rdma_port_space` 选择 `conn` 或 `ud` 子字段。

```c
struct rdma_conn_param {

    const void *private_data;     /* 引用用户控制的缓冲区。缓冲区的内容被复制并作为通信请求的一部分透明地传递到远程端。如果不需要 private_data ，则可以为 NULL */

    uint8_t private_data_len;     /* 指定用户控制的数据缓冲区的大小。请注意，传输到远程端的实际数据量取决于传输方式，并且可能大于请求的数据量。*/

    uint8_t responder_resources;  /* 本地端将从远程端接受的未完成的 RDMA read 和 atomic 操作的最大数量。该值必须小于或等于本地 RDMA 设备属性 max_qp_rd_atom 和远程 RDMA 设备属性 max_qp_init_rd_atom。远程端点可以在接受连接时调整该值。 */

    uint8_t initiator_depth;      /* 本地端所拥有的对远程端的 未完成的 RDMA read 和 atomic 操作的最大数量。仅适用于 RDMA_PS_TCP 。该值必须小于或等于本地 RDMA 设备属性 max_qp_init_rd_atom 和远程 RDMA 设备属性 max_qp_rd_atom。远程端点可以在接受连接时调整该值。*/

    uint8_t flow_control;         /* 指定硬件流控制是否可用。该值与远程对等方交换，不用于配置 QP。仅适用于 RDMA_PS_TCP。*/

    uint8_t retry_count;          /* 发生错误时应在连接上重试数据传输操作的最大次数。此设置控制发生 retry send, RDMA, 和 atomic 操作的次数，仅适用于 RDMA_PS_TCP。
    ignored when accepting */

    uint8_t rnr_retry_count;     /* 来自远端 peer 的 send 操作在收到 Receiver not Ready(RNR) 错误后应在连接上重试的最大次数。当 send request 在缓冲区准备好接收传入数据之前到达，会生成 RNR 错误。仅适用于 RDMA_PS_TCP */

    /* Fields below ignored if a QP is created on the rdma_cm_id. */

    uint8_t srq;                 /* 指定与连接关联的 QP 是否正在使用共享接收队列（SRQ）。如果已在 rdma_cm_id 上创建 QP ，则库会忽略此字段。仅适用于 RDMA_PS_TCP */

    uint32_t qp_num;             /* 指定与连接关联的 QP number。如果已在 rdma_cm_id 上创建 QP，则库会忽略此字段。仅适用于 RDMA_PS_TCP */

};
```

```c
struct rdma_ud_param {

    const void *private_data;

    uint8_t private_data_len;

    struct ibv_ah_attr ah_attr;

    uint32_t qp_num;

    uint32_t qkey;

};
```