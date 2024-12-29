英伟达 ConnectX InfiniBand 智能网卡最新产品 ConnectX-8 同时支持网络协议 InfiniBand 和 Ethernet，最大带宽 可达800Gb/s 。
https://nvdam.widen.net/s/pxsjzhgw6j/connectx-datasheet-connectx-8-supernic-3231505


充分利用 SMP 特性


软件分布式共享内存表现不佳的主要原因是节点之间：带宽低、延迟高。

SDSM 允许用户对集群进行编程，将集群视为一个大型的NUMA（非统一内存访问）机器。

–针对不同数据中心可定制化的sDSM


Cache 一致性都是为实现某种存储一致性模型而设计的。
在释放一致性中，一个处理机对共享变量写的新值，其他处理机只有等到该处理机释放锁后才能看到；
而在顺序一致性中，一个处理机写的值会立刻传播给所有处理机。
一致性协议：
- 如何传播新值：写使无效与写更新
- 怎样产生新值：单写协议与多写协议
- 何时传播新值：及时传播和延迟传播
- 新值传播到何处：侦听协议与目录协议


很多情况下，顺序并不重要（即执行顺序不影响最终结果，对于 `non-mutating` 操作更是如此，非更改操作是指不改变系统状态或正在处理的数据的操作），基于消息的方法可以在这些情况下提供更高的性能。
—— [Review on "Is It Time to Replace TCP in Data Centers?"](https://blog.ipspace.net/2023/01/data-center-tcp-replacement/#:~:text=While%20the%20order%20of%20execution%20might%20not%20matter%20for%20read%2Donly%20transactions%20with%20no%20side%20effects%2C%20it%E2%80%99s%20crucial%20if%20the%20messages%20cause%20a%20state%20change%20in%20the%20recipient%20application.) 

在大量对等点之间跨有损网络可靠地广播消息是一个难题。虽然原子广播算法（`atomic broadcast`）解决了集中式服务器的单点故障问题，但这些算法引入了队头阻塞问题。双峰多播算法是一种使用 `gossip protocol` 的随机算法，通过允许无序接收某些消息避免了队头阻塞。—— [HOL Blocking wiki](https://en.wikipedia.org/wiki/Head-of-line_blocking#:~:text=Reliably%20broadcasting%20messages,%5B6%5D)

应用程序堆栈问题：在测量 gRPC 时，未卸载情况下，小消息往返的软件开销约为 60 微妙，而其中 TCP 堆栈和网络所花费的时间仅 30 微妙。

内核网络栈，对于大多数负载可以很好地工作，但是对延迟特别敏感的工作负载会受到影响。今天，内核每秒只能转发 1M 到 2M 的数据包，而一些旁路替代方案的速率接近每秒 15M 的数据包。



示例：
代码会导致 ABA 问题吗
```c
while (1) {
    current_value = atomic_load(&msg_queue->free_count);
    if (current_value > 0) {
        if (atomic_compare_exchange_weak(&msg_queue->free_count,
                                         &current_value,
                                         current_value - 1)) {
            break;
        }
    }
}
```

ABA 问题发生情况：
线程 A 从共享变量读取值 var
其他线程修改该变量，然后将其恢复为 var
当线程 A 执行 CAS 操作时，由于值仍然是 var ，即使系统状态在之间发生了变化，它仍然会成功

考虑JIAJIA实现中的情况（实际上是 ABCA）
1. Thread A (main) 读取 `msg_queue->free_count = 1`，即 `current_value = 1`
2. Thread B (server) 将 `msg_queue->free_count` 从 1 递减至 0 ，break 出循环，原子修改共享变量 tail 
3. Thread C (client) 将 `msg_queue->free_count` 从 0  递增至 1
4. Thread A `compare_and_swap` 将 `msg_queue->free_count` 从 1 递减至 0 ，break 出循环，原子修改共享变量 tail 

这里发生的 ABCA （ABA变形）并不重要，因为 `tail` 充当了消除歧义的因素，在 BC 中修改的 tail 将会被 A 感知。



`jia_falloc()` 实现



平均内存访问时间：
$$
T = m \times T_m + T_h + E
$$
其中， m 是未命中率，$T_m$ 是发生未命中时进行主存访问的时间（或者，对于多级缓存，下一级缓存的平均内存访问时间），$T_h$ 是延迟（即引用缓存的时间，命中/未命中应该相同），$E$ 是次要影响（如，多处理器系统中的排队影响）

缓存有两个主要指标：延迟和命中率。许多次要因素也会影响缓存性能。
缓存的命中率描述了找到所搜索项目的频率。
- 更高效的替换策略会跟踪更多使用信息，以提高给定缓存大小的命中率。缓存的延迟描述了请求所需项目后，缓存在命中时可以返回该项目的时间。
- 更快的替换策略通常跟踪较少的使用信息，或者使用直接映射缓存（directed-mapped cache），不跟踪任何信息，以减少更新信息所需的时间。
- 每个替换策略都是命中率和延迟之间的折中。
命中率测量通常在基准应用程序上执行，会因应用程序而异。如，视频和音频流应用程序的命中率通常接近于零。


---
矩阵相乘（4096X4096）
```c
for i in xrange(4096):
	for j in xrange(4096):
		for k in xrange(4096):
			C[i][j] += A[i][k] * B[k][j]
```

![[Pasted image 20241209142217.png]]

每个版本都代表了原始Python代码的连续改进。“Running time”是版本的运行时间。“GFLOPS”是版本每秒执行的数十亿次64位浮点运算。“Absolute speedup”是相对于Python的时间，而“Relative speedup”，我们用一个额外的精度数字表示，是相对于前一行的时间。“峰值分数”是相对于计算机峰值835 GFLOPS的GFLOPS。如需详细信息，请参阅方法。
—— Charles E. Leiserson _et al._ There’s plenty of room at the Top: What will drive computer performance after Moore’s law?._Science_**368**,eaam9744(2020).DOI:[10.1126/science.aam9744](https://doi.org/10.1126/science.aam9744)


---
![[Pasted image 20241212114601.png]]
InfiniBand Software Stack https://www.nminoru.jp/~nminoru/network/infiniband/iba-software-stack.html


---
[SECM: Securely and efficiently connections setup using RDMA-CM - ScienceDirect](https://www.sciencedirect.com/science/article/pii/S1389128624003736)

---

`RDMA` 采用异步队列（`Asynchronous queues`）:
- 数据传输路径由向发送/接受队列中提交工作请求（ `work requests`） 和从完成队列中搜集完成事件驱动。
> Data path is driven by submitting work requests to send and receive queues and collecting completions from completion queues.
- 允许计算和通信的高效重叠。
- 允许通过 `per-CPU queues` 或 `per-thread queues` 支持高效的并行应用。

---
论文中比较
网络往返延迟：UDP，IPoIB，UD SEND/RECV

---
`RDMA` Doorbell Batching 
每个硬件厂商都会编写自己的底层驱动（如，在 `rdma-core/providers` 下，`mlx4` 实现的
`mlx4_post_send`, `rxe`实现的 `rxe_post_send` ）
[mlx4_post_send](https://github.com/linux-rdma/rdma-core/blob/c8517c5df900556735273a1b18e33a025b42870f/providers/mlx4/qp.c#L213)

MMIO 相关文件
- `rdma-core/util/mmio.h`
- `rdma-core/util/mmio.c`

Doorbell 批处理主要影响通知 `NIC` 有 `work requests` 处理的效率，并不会直接减少网络往返次数。
Doorbell Ring Operation
敲门铃操作——通知 `NIC` 工作队列中有新的工作请求。此操作通过将内存映射 I/O （MMIO）写入 NIC 上的特定寄存器来完成的。
- 由于 MMIO 写入，每个 Doorbell Ring Operation 都会产生延迟，这是一个相对昂贵的操作（绕过 CPU 缓存并需要与硬件交互）。
- 减少门铃响铃操作的数量可以最大限度地减少与这些 MMIO 写入相关的开销，从而提高性能。

Doorbell Batching （门铃批处理——最小化 MMIO 写入开销，侧重于本地通知效率）
通过链接多个工作请求（`work requests`）并在单个批次中发布它们，来减少敲门铃的次数。不必为每个工作请求敲门铃，而是为整个批次敲一次门铃。

示例：
```c
struct ibv_send_wr wr_list[10];

for(int i = 0; i < 10; ++i) {
	wr_list[i] = create_wr(sge, ...);	// 创建 WR
}

// 链接 WRs 以便门铃批处理
for (size_t i = 0; i < 9; ++i) {
	wr_list[i].next = &wr_list[i+1];
}

// 发布一批 WRs
int ret = ibv_post_send(qp, &wr_list[0], &bad_wr);
if (ret != 0) {
	exit(-1);
}

// 轮询完成
struct ibv_wc wc;
int num_completions = 0;
while (num_completions < wr_list.size()) {
	int nc = ibv_poll_cq(cq, 1, &wc);
	if (nc < 0) {
		exit(-1);
	}
	if (nc > 0) {
		if (wc.status != IBV_WC_SUCCESS) {
			exit(-1);
		}
		++num_completions;
	}
}
```

---
论文中绘制 `InfiniBand` QP 的状态机。

在 `InfiniBand` 中，处于 `INIT` 状态的 `QPs` 能发布 `RRs` （Receive Requests）但不能处理。

---
`Verbs` 中有两种函数：慢路径函数和快路径函数。
- 慢路径函数——与资源（如上下文、保护域和内存区域）的创建和配置相关的函数。由于涉及内核，因此会产生上下文切换的昂贵开销。如 `ibv_open_device()`, `ibv_alloc_pd`，`ibv_reg_mr` _基本上在初始化时完成_
- 快路径函数——处理操作的启动和完成。由于绕过内核，比慢速路径函数快得多。通信的关键路径主要由快速路径函数组成，偶尔也包含慢速路径函数。



---
[Which Queue Pair type to use?](https://www.rdmamojo.com/2013/06/01/which-queue-pair-type-to-use/)
RDMA UD 模式
在 RDMA 的 UD 模式中，每个消息必须适合单个请求数据包，而数据包的大小受限于网卡的 MTU （最大传输单元）。如果要传输的数据大于 MTU，需要在应用层或其他协议层自行处理分片和重组。

一个 QP 可以以不可靠的方式以单播（一对一）或多播（一对多）方式向任何其他 UD QP 发送和接收消息。不能保证另一端会收到消息：损坏或不按顺序的数据包将被静默丢弃。对数据包的排序没有任何保证。

将（一个数据包）消息发送到 `Fabric` （网络结构）后，请求方认为消息操作完成。
响应方在收到完整的消息并将数据写入其（本地）内存后，将消息操作视为完成。

解决方案：
- 将 `msg` 大小限制为 $MSG_{size} < MTU$ ，无非是多下发几次消息。
- 对大消息进行分片重组。

---
预取的设计与优化

优化前
机器想要访问非本地页，将触发两次段违例
- 第一次段违例负责向远端发送 GETP(addr，read) 消息，远端返回 addr 对应的页，授予读权限
- 第二次段违例负责向远端发送 GETP(addr, write) 消息，远端返回 addr 对应的页，授予写权限

利用局部性原理（预先打包机器随后的几页）实现预取
依赖变量：
`prefetch` - 优化开关
`prefetch_pages` - 希望预取的页数
`max_checking_pages` - 向后检查的页数

触发条件
`prefetch=on && prefetch_pages > 0 && max_checking_pages > 0` 

远端机器收到 `GETP(addr, read)`，将最多向后检查 `max_checking_pages`，至多打包 `prefetch_pages` 页返回。

很多优化是一种权衡，优化效果依赖于具体应用使权衡的天平更偏向于那边。

