`rdma_accept` - 调用以接收一个连接请求

```c
#include <rdma/rdma_cmd.h>

int rdma_accept(struct rdma_cm_id *id, struct rdma_conn_param *conn_param);
```

参数
`id` - 与请求关联的连接标识符
`conn_param` - 建立连接所需的信息。有关详细信息，请参阅连接属性 `struct rdma_conn_param`。

描述
在侦听端 （listening side） 调用该函数以接收一个连接请求或数据报服务查找请求。

返回值
成功时返回 0 ，错误时返回 -1 。如果发生错误，设置 `errno` 指示失败的原因。

注意
与套接字 `accept()` 例程不同，`rdma_accept()` 不会在侦听 `rdma_cm_id` （listening `rdma_cm_id`）上调用。相反，在调用 `rdma_listen()` 之后，用户等待一个 `RDMA_CM_EVENT_CONNECT_REQUEST` 事件发生。该事件为用户提供一个新创建的 `rdma_cm_id` ，类似于一个新的套接字，但该 `rdma_cm_id` 绑定到特定的 RDMA 设备。在这个新的 `rdma_cm_id` 上调用 `rdma_accept()`。

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