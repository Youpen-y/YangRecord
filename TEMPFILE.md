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
