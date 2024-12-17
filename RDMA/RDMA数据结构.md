### RDMA CM ID
```c
struct rdma_cm_id {
    struct ibv_context  *verbs;   // RDMA 设备的上下文
    struct rdma_event_channel *channel; //通信事件通道
    void            *context;     // 用于自定义上下文，可以存储应用程序特定的数据
    struct ibv_qp       *qp;      // 队列对指针，用于发送和接收数据的队列
    struct rdma_route    route;   // 路由信息，包含源地址和目标地址等网络路由信息
    enum rdma_port_space     ps;
    uint8_t          port_num;    // RDMA 设备端口号
    struct rdma_cm_event    *event; // 当前事件指针，存储最近的通信管理事件
    struct ibv_comp_channel *send_cq_channel; // 发送完成事件通道，用于通知发送完成事件
    struct ibv_cq       *send_cq;  // 发送完成队列，存储发送操作的完成状态
    struct ibv_comp_channel *recv_cq_channel; // 接收完成事件通道，通知接收完成事件
    struct ibv_cq       *recv_cq;  // 
    struct ibv_srq      *srq;      // 共享接收队列
    struct ibv_pd       *pd;       // 保护域，用于内存注册和访问控制
    enum ibv_qp_type    qp_type;   // 队列对的类型
};
```

`rdma_cm_id` 在概念上等同于用于 RDMA 通信的套接字。不同之处在于，RDMA 通信需要在通信发生之前显式绑定到指定的 RDMA 设备，并且大多数操作本质上是异步的。
示例：
```c
struct rdma_cm_id *id;
struct rdma_event_channel *ec;

// 创建事件通道
ec = rdma_create_event_channel();

// 创建通信标识
rdma_create_id(ec, &id, NULL, RDMA_PS_TCP);

// 设置保护域
id->pd = ibv_alloc_pd(id->verbs);

// 创建完成队列
id->send_cq = ibv_create_cq(id->verbs, cq_size, NULL, id->send_cq_channel, 0);
id->recv_cq = ibv_create_cq(id->verbs, cq_size, NULL, id->recv_cq_channel, 0);

// 创建队列对
struct ibv_qp_init_attr qp_attr = {
    .send_cq = id->send_cq;
    .recv_cq = id->recv_cq;
    .qp_type = IBV_QPT_RC
};
rdma_create_qp(id, id->pd, &qp_attr);
```

---
### RDMA EVENT CHANNEL
```c
struct rdma_event_channel {
    int         fd;
};
```

---
### RDMA CM EVENT
```c
struct rdma_cm_event {
    struct rdma_cm_id   *id; // 与事件相关的连接标识符
    struct rdma_cm_id   *listen_id; // 监听连接的标识符
    enum rdma_cm_event_type  event; // 事件类型
    int          status;     // 事件状态
    union {
        struct rdma_conn_param conn;
        struct rdma_ud_param   ud;
    } param;
};

enum rdma_cm_event_type {
    RDMA_CM_EVENT_ADDR_RESOLVED,      // 地址解析完成
    RDMA_CM_EVENT_ADDR_ERROR,         // 地址解析错误
    RDMA_CM_EVENT_ROUTE_RESOLVED,     // 路由解析完成
    RDMA_CM_EVENT_ROUTE_ERROR,        // 路由解析错误
    RDMA_CM_EVENT_CONNECT_REQUEST,    // 收到连接请求
    RDMA_CM_EVENT_CONNECT_RESPONSE,   // 收到连接响应
    RDMA_CM_EVENT_CONNECT_ERROR,      // 连接错误
    RDMA_CM_EVENT_UNREACHABLE,        // 目标不可达
    RDMA_CM_EVENT_REJECTED,           // 连接被拒绝
    RDMA_CM_EVENT_ESTABLISHED,        // 连接建立
    RDMA_CM_EVENT_DISCONNECTED,       // 连接断开
    RDMA_CM_EVENT_DEVICE_REMOVAL,     // 设备移除
    RDMA_CM_EVENT_MULTICAST_JOIN,     // 加入多播组
    RDMA_CM_EVENT_MULTICAST_ERROR,    // 多播错误
    RDMA_CM_EVENT_ADDR_CHANGE,        // 地址变更
    RDMA_CM_EVENT_TIMEWAIT_EXIT       // 退出时间等待状态
};
```

---
### Scatter Gather
使用 `ibv_sge` 收集/分发数据3
``` c
struct ibv_sge {
    uint64_t addr;
    uint32_t length;
    uint32_t lkey;
}
```
`addr` - 缓冲区的地址
`length` - 缓冲区的长度
`lkey` - Local Memory Key（本地密匙），用于保护内存区域
Note：使用 `ibv_reg_mr()` 注册与特定保护域关联的内存区域（MR），每次成功注册会产生一个 MR，该 MR具有唯一 `lkey` 和 `rkey` 值。调用进程的虚拟空间中的每个内存地址都可以被注册：
- Local memory （either variable or array）局部内存（变量或数组）
- Global memory（either vairable or array）全局内存（变量或数组）
- Dynamically allocated memory（using malloc() or mmap()）动态分配的内存（malloc/mmap）
- Shared memory（共享内存）
- Addresses from the text segment （文本段中的地址）
注册的内存缓冲区 不必是页对齐的。


### Device Context
```c
struct ibv_context {
    struct ibv_device      *device; // 代表了一个 IB 设备，该指针由用户通过 ibv_get_device_list() 函数获得，在创建上下文时传递给 ibv_open_device() 函数
    struct ibv_context_ops  ops; // 包含一组函数指针，指向 Verbs API 的操作函数，这些函数提供对设备进行操作的接口，如发送和接收数据、注册内存等。ibv_context_ops 结构体允许用户自定义操作函数，以便在特定的硬件或操作环境中进行优化。
    int         cmd_fd; // 用于与 IB 设备的命令接口进行通信的文件描述符，通过该描述符，用户可以发送命令给设备，如创建或销毁资源。
    int         async_fd; // 用于接收异步事件通知，当设备发生异步事件（如完成队列中的操作时），该文件描述符可用来通知用户
    int         num_comp_vectors; // 完成队列向量的数量（每个向量可以关联一个或多个 CQ）
    pthread_mutex_t     mutex; // 互斥锁，用于同步对上下文的访问。在多线程环境中，当多个线程同时访问同一个上下文时，该互斥锁可以确保线程安全
    void               *abi_compat; // 可用于存储兼容性信息，以确保不同库版本之间的 的 ABI 兼容
};
```
`ibv_context`代表与 IB 设备相关的上下文。通常用于创建和管理与设备相关的资源，如 内存注册、队列对和事件等。

---
### RDMA CM EVENT
```c
struct rdma_cm_event {
    struct rdma_cm_id   *id;  // 代表一个 RDMA 连接的端点
    struct rdma_cm_id   *listen_id; // 监听连接请求的端点，当一个连接请求被接收时，这个字段指向监听该请求的端点
    enum rdma_cm_event_type  event; // 表示发生的事件类型。
    int          status; // 表示事件的状态。（某些事件成功与否）
    union {
        struct rdma_conn_param conn; // 当事件类型是连接相关的（如连接请求或连接建立），该结构体会被用来传递连接参数，如私有数据、QoS 信息等
        struct rdma_ud_param   ud; //事件类型是无连接的 RDMA 操作相关（如 UD 服务），该结构体会被用来传递无连接的参数，如远程内存访问的地址、键值等
    } param;
};

enum rdma_cm_event_type {
    RDMA_CM_EVENT_ADDR_RESOLVED,
    RDMA_CM_EVENT_ADDR_ERROR,
    RDMA_CM_EVENT_ROUTE_RESOLVED,
    RDMA_CM_EVENT_ROUTE_ERROR,
    RDMA_CM_EVENT_CONNECT_REQUEST,
    RDMA_CM_EVENT_CONNECT_RESPONSE,
    RDMA_CM_EVENT_CONNECT_ERROR,
    RDMA_CM_EVENT_UNREACHABLE,
    RDMA_CM_EVENT_REJECTED,
    RDMA_CM_EVENT_ESTABLISHED,
    RDMA_CM_EVENT_DISCONNECTED,
    RDMA_CM_EVENT_DEVICE_REMOVAL,
    RDMA_CM_EVENT_MULTICAST_JOIN,
    RDMA_CM_EVENT_MULTICAST_ERROR,
    RDMA_CM_EVENT_ADDR_CHANGE,
    RDMA_CM_EVENT_TIMEWAIT_EXIT
};
```
`rdma_cm_event` 结构体是 RDMA CM API 中用来表示 RDMA 连接管理事件的容器。当 RDMA 连接的状态发生变化时，如连接请求、连接建立或连接断开，该结构体将被填充并传递给用户。

---
### RDMA ADDRINFO
```c
struct rdma_addrinfo {
	int			ai_flags;         // 地址信息标志
	int			ai_family;        // 地址簇 (AF_INET, AF_INET6, AF_IB)
	int			ai_qp_type;       // 队列对类型 (IBV_QPT_RC, IBV_QPT_UC, IBV_QPT_UD)
	int			ai_port_space;
	socklen_t		ai_src_len;
	socklen_t		ai_dst_len;
	struct sockaddr		*ai_src_addr;
	struct sockaddr		*ai_dst_addr;
	char			*ai_src_canonname;
	char			*ai_dst_canonname;
	size_t			ai_route_len;
	void			*ai_route;
	size_t			ai_connect_len;
	void			*ai_connect;
	struct rdma_addrinfo	*ai_next; // 链表中的下一个地址信息结构
};

/* ai_flags */
#define RAI_PASSIVE 0x00000001 // 被动（服务器）模式 
#define RAI_NUMERICHOST 0x00000002 // 主机名是数字形式 
#define RAI_NOROUTE 0x00000004 // 不执行路由解析 
#define RAI_FAMILY 0x00000008 // 指定地址族



```