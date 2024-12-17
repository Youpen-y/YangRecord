`ibv_reg_mr`
`ibv_dereg_mr`
`ibv_reg_mr_iova`
`ibv_reg_dmabuf_mr`
注册或取消注册一个 内存区域（memory region, MR）

---
```c
#include <infiniband/verbs.h>

struct ibv_mr *ibv_reg_mr(struct ibv_pd *pd, void *addr, size_t length, int access);

int ibv_dereg_mr(struct ibv_mr *mr);
```

`ibv_reg_mr()` 注册一个与保护域 `pd` 相关联的内存区域（memory region, MR）。该内存区域的起始地址是 `addr` ，大小是 `length` 。
参数 `access` 描述了想要的内存保护属性；要么是 0 或是以下多个 flags 的按位或：
注意：MR始终启用 本地读 （local read）权限
当设置了 `IBV_ACCESS_REMOTE_WRITE` 或 `IBV_ACCESS_REMOTE_ATOMIC` 时，也必须设置 `IBV_ACCESS_LOCAL_WRITE` 。
- `IBV_ACCESS_LOCAL_WRITE`：使能本地写权限
- `IBV_ACCESS_REMOTE_WRITE`：使能远程写权限
- `IBV_ACCESS_REMOTE_READ`：使能远程读权限
- `IBV_ACCESS_REMOTE_ATOMIC`：使能远程原子操作权限（如果支持的话）
- `IBV_ACCESS_MW_BIND`：启用内存窗口绑定
- `IBV_ACCESS_FLUSH_GLOBAL`：启用全局可见放置类型的远程刷新操作（如果支持的话）
- `IBV_ACCESS_FLUSH_PERSISTENT`：启用具有持久性放置类型的远程刷新操作（如果支持的话）
> placement type （放置类型）在IB网络和RDMA技术中，指用于指定数据在内存中分配和放置方式的属性之一。它可以影响数据在内存中的位置、可见性和持久性等方面。
> 常见种类：
> - 局部放置类型（local placement type）：（本地计算和存储任务，无需跨节点访问和共享数据）
>     - 表示数据仅在本地结点的内存中分配和存储，不涉及远程节点
>     - 适用于本地数据处理和存储，不需要远程访问的情况
> - 全局可见放置类型（global visibility placement type）：（多节点之间共享和同步数据的分布式应用）
>     - 表示数据在内存中分配时可在多个节点之间共享和访问
>     - 通常用于需要在多个节点之间共享和访问共享内存状态的分布式应用场景，可以通过远程操作实现数据同步和共享
> - 持久性放置类型（Persistence Placement Type）：（程序重启后继续访问数据的场景）
>     - 表示数据在内存中的存储是持久性的，即数据在程序结束后仍然保留
>     - 用于需要在程序重启后继续访问数据的场景，可以确保数据的持久性存储
- `IBV_ACCESS_ZERO_BASED`：使用从MR开始的字节偏移量来访问这个MR，  而不是指针地址
- `IBV_ACCESS_ON_DEMAND`：创建一个按需 paging MR
- `IBV_ACCESS_HUGETLB`：对该 MR 保证使用 Huge Pages，仅适用于显示模式下的`IBV_ACCESS_ON_DEMAND` 

为了创建一个隐式 ODP MR，应该设置`IBV_ACCESS_ON_DEMAND`，`addr` 为 0 且 `length` 为 `SIZE_MAX`

如果设置了 `IBV_ACCESS_HUGETLB`，应用意识到对于此 MR，所有的页是大页，必须保证它不会破坏大页