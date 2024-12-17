1. 基于目录 与 基于锁的分布式内存
2. JIAJIA支持的共享内存大于单机内存，既保持  无地址转换  远程页获取，又保持固定大小的 cache 是如何做到的？（每一页如何找到相应的cache，似乎是cache 中保存了相应页的地址）
3. 容错性，当一台机器崩溃时，如何保证系统正确性？（日志系统 ，考虑非易失性内存）
4. RDMA 支持分散/聚合条目（Scatter/Gather entries），即支持读取多个内存缓冲区然后作为一个流发出去或者接收一个流然后写入到多个内存缓冲区里去。也即是说，不用像JIAJIA之前做的将写向量优化，将修改的内容打包到消息中，发到远端；
5. JIAJIA 原生支持UDP，这和基于消息的RDMA类似。（所有数据包的组装都在RDMA硬件上完成）
6. 统计信息精确到纳秒级别（使用 `timespec` 取代 `timeval` ）
``` c
struct timespec {  // declared in <time.h>
    time_t tv_secs;   // seconds
    long tv_nsecs;    // nanoseconds
};

// note: 大多数系统需要程序链接 librt 库才能使用以下函数
int clock_getres(clockid_t clk_id, struct timespec *res);
int clock_gettime(clockid_t clk_id, struct timespec *tp);
int clock_settime(clockid_t clk_id, const struct timespec *tp);
```

7. JIAJIA3.0 目标：系统动态均衡（`jia_balance(flags)`）
    1. 当需要利用远端计算能力时，计算模式（computing mode）
    2. 当需要利用远端内存时，内存模式（memory mode）
    3. 混合模式（即利用远端计算能力，又利用远端存储能力）
    4. 使用一个线程定时统计本机信息并广播
内存模式针对的问题：
配置文件解析库
- `libconfig` - 解析结构化配置文件
- `libyaml` - 解析 YAML 格式的配置文件

8. 传递数据压缩与加密
9. 动态锁管理，
10. 变信号驱动 I/O 为事件驱动 I/O 

每一片共享内存维护一个等待队列{id1, id2, ...}，调度线程不断从等待队列中调度出机器并授予权限。


MPI（消息传递接口）
基于 RDMA 的单侧通信操作


一个高度可定制的系统，可以通过配置文件进行配置。

类似 home 迁移，实现访问权限迁移。针对某块共享内存，如果其写权限处于被占用的状态，则无法再赋予写权限（写时复制）；而对于读权限，可用性高则允许授予之前版本的共享内存，一致性高则等待直至释放写权限。



基准测试：
NAS Parallel Benchmarks


导致 段违例（SIGSEGV）信号的原因多种》

grant 消息与 ack 是否有重复作用？grant用于其他目的

应用场景：随机化算法、MapReduce 框架

提供细粒度的数据共享

为了实现对不同网络协议实现的支持，也考虑到目前传统以太网仍是主流的现状，我们并不打算用完全替代。相反，而是提供用户可选择的

`mmap()` 建立的映射将替换指定地址范围的任何先前映射。flag `MAP_SHARED` 和 `MAP_PRIVATE` 指定映射类型，并且必须指定其中之一。`MAP_SHARED` 指定写入修改映射对象。无需对对象进行进一步操作即可进行修改。而 `MAP_PRIVATE` 指定对映射区域的初始写入将创建该页的副本，之后所有写入都应用该副本。

映射的类型在 `fork()` 中保留，建立映射后，`mmap` 调用中使用的文件描述符不需要保持打开状态。如果它被关闭，映射将保留，直到映射被 `munmap()` 撤销或被新映射替换。如果通过调用 `truncate()` 来截断映射文件，则访问不再存在的文件区域会导致 `SIGBUS` 信号。

```c
int fd;
caddr_t result;
if ((fd = open("/dev/zero", O_RDWR)) == -1)
    return ((caddr_t)-1);
result = mmap(0, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
(void) close(fd);
```

其他内存控制功能：
`int mlock(caddr_t addr, size_t len);` 导致指定地址范围内的页被锁定在物理内存中。对锁定页的引用（本进程或其他进程）不会导致需要 IO 操作的 page fault 。该操作会占用物理资源并可能中断正常的系统操作，因此 `mlock()` 的使用仅限于超级用户。系统仅允许将于配置相关的页面限制锁定在内存中，如果超过此限制，对 `mlock` 的调用将失败。

`int munlock(caddr_t addr, size_t len);` 释放物理页上的锁。如果对单个映射的地址范围进行多个 `mlock()` 调用，则单个 `munlock()` 调用将释放这些锁。但，如果对同一页的不同映射进行了 `mlock`，则在所有映射上的锁都被释放之前，该页不会解锁。当删除映射时，锁会被释放，无论是通过 `mmap` 操作替换还是用 `munmap` 删除。
锁在与 `MAP_PRIVATE` 映射关联的 `copy-on-write` 事件上的页面之间传输，因此包含 `MAP_PRIVATE` 映射的地址范围上的锁将与 copy-on-write 重定向一起透明地保留。

`int mlockall(int flags);` 和 `int munlockall(void);` 与 `mlock()` 和 `munlock()` 类似，但它们对整个地址空间进行操作。`mlockall()` 在地址空间中的所有页上设置锁，而 `munlockall()` 则删除地址空间中所有页上的所有锁，无论是由 `mlock()` 还是 `mlockall()` 建立的。

`int msync(caddr_t addr, size_t len, int flags);` 导致指定范围内的所有已修改页刷新到这些地址映射的对象。它类似于文件的 `fsync()` 。


Spark（）




