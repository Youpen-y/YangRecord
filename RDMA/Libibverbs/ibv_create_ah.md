`ibv_create_ah()` 创建与保护域关联的地址句柄（Address Handle）
`ibv_destroy_ah()` 销毁 AH
语法
```c
#include <infiniband/verbs.h>
struct ibv_ah *ibv_create_ah(struct ibv_pd *pd,
									  struct ibv_ah_attr *attr);

int ibv_destroy_ah(struct ibv_ah *ah);
// 成功返回 0 ，否则返回 errno 指示失败原因
```


当发送请求（Send Request）将发送到不可靠数据包 QP 时，稍后将使用此 AH 。

`struct ibv_ah_attr` 描述了新创建的 AH 所请求的属性。
```c
struct ibv_ah_attr {
	struct ibv_global_route_grh grh;	// 当数据包发送到另一个子网或多播组时，非常有用。
	uint16_t		dlid;		// 如果目的地位于同一子网中，则为该子网将数据包传送到的端口的 LID 。如果目的地在另一个子网上，则为路由器的 LID 。
	uint8_t		sl;		// 4bits，要使用的服务级别
	uint8_t		src_path_bits;	// 使用的源路径位。当在端口中使用 LMC 时（即每个端口覆盖一系列 LID），非常有用。数据包与端口的基本 LID 一起发送，并与源路径位的值进行按位或运算。值 0 表示使用端口的 base LID 。
	uint8_t		static_rate; // 限制发送到子网的数据包速率的值。
	uint8_t		is_global;	// 如果该值包含非零的任何值，则该 AH 中存在 GRH 信息，即 grh 字段有效。
	uint8_t		port_num;	// 发送数据包的本地物理端口。
};
```

- `grh` - 全局路由头（GRH）的属性，
```c
struct ibv_global_route {
	union ibv_gid	dgid;	// 标识数据包的目的端口
	uint32_t			flow_label;	// 如果该值设置为非零值，会向具有多个出站路径的交换机和路由器发出提示，这些数据包序列必须按顺序传递，这些数据包保持在同一路径上，以便它们不会被重新排序。
	uint8_t			sgid_index;	// 端口 GID 表中的索引，用于识别数据包的发起者
	uint8_t			hop_limit;	// 数据包被丢弃前允许经过的跳数（路由器数量）。每个路由器在收到数据包时将该值减 1 ，当该值达到 0 时，丢弃
	uint8_t			traffic_class;	// 使用此值，数据包的发起者指定路由器处理它们所需的传送优先级
};
```