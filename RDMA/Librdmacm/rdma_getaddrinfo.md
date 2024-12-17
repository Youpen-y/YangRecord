`rdma_getaddrinfo` - 提供与传输无关的地址转换。

```c
#include <rdma/rdma_cma.h>

int rdma_getaddrinfo(const char *node, const char *service, const struct rdma_addrinfo *hints, struct rdma_addrinfo **res);
```

参数
`node` - 可选的，要解析的点分十进制 IPv4 或 IPv6 十六进制地址
`service` - 服务名称或地址的端口号
`hints` - 对 `rdma_addrinfo` 结构的引用，其中包含有关调用方支持的服务类型的提示
`res` - 指向包含响应信息的 `rdma_addrinfo` 结构链表的指针

描述
解析目标节点和服务地址并返回建立通信所需的信息。提供与 `getaddrinfo` 等效的 RDMA 功能。

返回值
成功时返回 0 ，出错时返回 -1 （设置 `errno` 来指示失败原因）

`EAI_ADDRFAMILY` - 指定的网络主机在请求的地址簇中没有任何网络地址
`EAI_AGAIN` - The name server 返回临时失败提示。稍后再试
`EAI_FAIL` - The name server 返回永久性失败提示
`EAI_MEMORY` - Out of memory
`EAI_NODATA` - 指定的网络主机存在，但未定义任何网络地址
`EAI_NONAME` - `node` 或 `service` 未知；或 `node` 和 `service` 为空
`EAI_SERVICE` - 所请求的服务对于所请求的 QP 类型不可用，可以通过另一种 QP 类型获得
`EAI_QPTYPE` - 请求的套接字类型不支持。如 `hints.ai_qptype` 和 `hints.ai_port_space` 不一致（如，分别为 `IBV_QPT_UD` 和 `RDMA_PS_TCP`），则可能会发生这种情况。
`EAI_SYSTEM` - 其他系统错误，具体查看 `errno` 。`gai_strerror()` 函数可以将这些错误代码转换为人类可读的字符串，适合错误报告

NOTES
必须提供 `node` , `service` 或 `hints` 。如果提供了 `hints` ，操作将由 `hints.ai_flags` 控制。如果指定了 `RAI_PASSIVE` ，则调用将解析地址信息以在连接的被动端使用（passive side, server）。如果提供了 `node` ，`rdma_getaddrinfo` 将尝试解析给定 `node` 的RDMA地址、路由和连接数据。`hints` 参数（如果提供）可用于控制结果输出，如下所示。如果未给出 `node` ，`rdma_getaddrinfo` 将尝试根据 `hints.ai_src_addr` ，`hints.ai_dst_addr` 或 `hints.ai_route` 解析 RDMA 寻址信息。

**rdma_addrinfo**
```c
struct rdma_addrinfo {
    int         ai_flags;
    int         ai_family;
    int         ai_qp_type;
    int         ai_port_space;
    socklen_t       ai_src_len;
    socklen_t       ai_dst_len;
    struct sockaddr     *ai_src_addr;
    struct sockaddr     *ai_dst_addr;  
    char            *ai_src_canonname;
    char            *ai_dst_canonname;
    size_t          ai_route_len;
    void            *ai_route;
    size_t          ai_connect_len;
    void            *ai_connect;
    struct rdma_addrinfo    *ai_next;
};
```

参数
`ai_flags` - 控制操作的提示标志。支持的标志有：
- `RAI_PASSIVE` - 指示结果将用于连接的被动/侦听端
- `RAI_NUMERICHOST` - 如果指定，则节点参数（如果提供）必须是 numerical network address 。该标志阻止任何冗长的地址解析
- `RAI_NOROUTE` - 如果设置，此标志将抑制任何冗长的路由解析
- `RAI_FAMILY` - 如果设置，则 `ai_family` 设置应用作解释节点参数的输入提示
`ai_family` - 源地址和目标地址协议簇。`AF_INET`, `AF_INET6` 和 `AF_IB`
`ai_qp_type` - 指示用于通信的 RDMA QP 的类型。支持 `IBV_QPT_UD` （不可靠数据报） 和 `IBV_QPT_RC` （可靠连接）
`ai_port_space` - RDMA port space in use. 支持 `RDMA_PS_UDP`，`RDMA_PS_TCP` 和 `RDMA_PS_IB`
`ai_src_len` - `ai_src_addr` 引用的源地址的长度。如果无法为给定目标发现适当的源地址，则该值为 0 。
`ai_dst_len` - `ai_dst_addr` 引用的目标地址的长度。如果 `RAI_PASSIVE` 标志被指定为 `hints` 的一部分，则该值为 0 
`ai_src_addr` - 如果提供，则为本地 RDMA 设备的地址
`ai_dst_addr` - 如果提供，则为目标 RDMA 设备的地址
`ai_src_canonname` - The canonical for the source
`ai_dst_canonname` - The canonical for the destination
`ai_route_len` - `ai_route` 引用的路由信息缓冲区的大小。如果底层传输不需要路由数据，或无法解析任何数据，则该值为 0 。
`ai_route` - RDMA 传输的路由信息，需要路由数据作为连接建立的一部分。路由数据的格式取决于底层传输。如果使用 Infiniband 传输，如果路由数据可用，`ai_route` 将在输出上引用 `struct ibv_path_data` 数组。可以通过在 `rdma_getaddrinfo` 的输入上设置所需的路由数据字段来限制路由路径。对于 Infiniband，`hints.ai_route` 可以在输入上引用 `struct ibv_path_record` 或 `struct ibv_path_data` 的数组。
`ai_connect_len` - `ai_connect` 引用的连接信息的大小。如果底层传输不需要额外的连接信息，则该值为 0 
`ai_connect` - 作为连接建立过程的一部分交换的数据。如果提供，`ai_connect` 数据必须作为私有数据传输，其后跟随任何用户提供的私有数据。
`ai_next` - 指向列表中下一个 `rdma_addrinfo` 结构的指针。如果不再存在结构，则将为 NULL 。

示例：
```c
void get_rdma_addr(const char *host, const char *port) {
    struct rdma_addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));

    // 设置提示信息
    hints.ai_family = AF_UNSPEC; // IPv4 或 IPv6
    hints.ai_qp_type = IBV_QPT_RC;
    hints.ai_port_space = RDMA_PS_TCP;

    // 获取地址信息
    int ret = rdma_getaddrinfo(host, port, &hints, &res);
    if (ret) { 
        fprintf(stderr, "rdma_getaddrinfo error: %s\n", gai_strerror(ret)); 
        return; 
    }

    // 遍历结果 
    struct rdma_addrinfo *cur = res; 
    while (cur) { 
        // 使用地址信息 
        // 例如：创建 RDMA 连接 
        create_rdma_connection(cur); 
        cur = cur->ai_next; 
    } 
    
    // 释放地址信息 
    rdma_freeaddrinfo(res); 
}
```





