MPI (Message Passing Interface)，是面向消息传递库的开发人员和用户的规范。
MPI 处理消息传递并行编程模型：通过对每个进程的协作操作，数据从一个进程的地址空间移动到另一个进程的地址空间。
如今，MPI-5.0 标准正在指定中。
### 历史
最初，MPI 为分布式内存架构设计。
![[单核分布式.png]]
随着架构趋势的变化，共享内存 SMP 通过网络组合起来，创建了混合分布式内存/共享内存系统。
MPI 实现者调整了他们的库，以无缝处理这两种类型的底层内存架构。而且还调整/开发了处理不同互连和协议的方法。
![[多核分布式.png]]
如今，MPI 几乎可以在任何硬件平台上运行：
- 分布式内存
- 共享内存
- 混合
但编程模型仍然是一个分布式内存模型，而不管机器的底层物理架构如何。

所有并行性都是显式的：程序员负责正确识别并行性并使用 MPI 结构实现并行算法。

### 使用原因
- 标准化
- 可移植性
- 性能
- 丰富的功能（MPI-4中定义了430个函数，大多数 MPI 程序都可以使用 十几个或更少的例程编写）
- 可用性

### 多种实现
尽管 MPI 编程接口已标准化，但实际的库实现会有所不同。
考虑因素：
- 支持哪个版本的 MPI 标准？
- 是否支持特定 MPI 版本中的所有功能？
- 是否添加了任何新功能？
- 支持哪些网络接口？
- MPI 应用程序是如何编译的？
- MPI 作业是如何启动的？
- Runtime environment variable controls ?

| MPI 库            | Where                                  | 编译器                  |
| ---------------- | -------------------------------------- | -------------------- |
| MVAPICH          | Linux 集群                               | GNU, Intel, Clang    |
| Open MPI         | Linux 集群                               | GNU, Intel, Clang    |
| Intel MPI        | Linux 集群                               | Intel, GNU           |
| IBM Spectrum MPI | Coral Early Access and Sierra clusters | IBM, GNU, PGI, Clang |


| 实现       | 语言      | 脚本名称                  | 底层编译器              |
| -------- | ------- | --------------------- | ------------------ |
| Open MPI | C       | mpicc                 | 用于加载的编译器包的 C 编译器   |
|          | C++     | mpicc/mpic++/mpicxx   | 用于加载的编译器包的 C++ 编译器 |
|          | Fortran | mpif77/mpif90/mpifort | Fortran 编译器        |

MPI 库的线程支持级别各不相同
- `MPI_THREAD_SINGLE` - Level 0，仅执行一个线程
- `MPI_THREAD_FUNNELED` - Level 1，进程可以是多线程的，但只有主线程会进行 MPI 调用 - 所有 MPI 调用都汇集到主线程
- `MPI_THREAD_SERIALIZED` - Level 2，进程可以是多线程的，多个线程可以进行 MPI 调用，但一次只能进行一个调用。即，由于所有 MPI 调用都是序列化的，因此不会同时从两个不同的线程进行调用。
- `MPI_THREAD_MULTIPLE` - Level3，多个线程可以无限制地调用 MPI 。

---

通过 MPI 程序结构
![[MPI 程序结构.png]]

头文件：
进行 MPI库 调用的所有程序都需要

| C include file     | Fortran include file |
| ------------------ | -------------------- |
| `#include "mpi.h"` | `#include 'mpif.h'`  |
函数调用格式：（C 区分大小写；Fortran 不区分）
```c
rc = MPI_Xxxxx(parameter, ...)

// e.g
rc = MPI_Bsend(&buf, count, type, dest, tag, comm)
```

MPI 使用 `communicators` 和 `groups` 的对象来定义哪些进程集合可以相互通信。

### 环境管理例程
`MPI_Init`
`MPI_Comm_size`
`MPI_Comm_rank`
`MPI_Abort`
`MPI_Get_processor_name`
`MPI_Get_version`
`MPI_Initialized`
`MPI_Wtime`
`MPI_Wtick`
`MPI_Finalize`

```c
// required MPI include file
#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    int  numtasks, rank, len, rc;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    // initialize MPI
    MPI_Init(&argc,&argv);
    
    // get number of tasks
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    // get my rank
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    
    // this one is obvious
    MPI_Get_processor_name(hostname, &len);
    printf ("Number of tasks= %d My rank= %d Running on %s\n", numtasks,rank,hostname);
    
    // do some work with message passing
    ...
    // done with MPI
    MPI_Finalize();
}
```

### 示例：$\pi$ 的计算
近似求 $\pi$ 的 `Monte Carlo` 算法（串行）：
```
npoints = 10000
circle_count = 0

do j = 1, npoints
    generate 2 random numbers between 0 and 1
    xcoordinate = random1
    ycoordinate = random2
    if (xcoordinate, ycoordinate) inside circle
    then circle_count = circle_count + 1
end do

PI = 4.0*circle_count/npoints
```
并行方案：
- 将循环迭代（loop iterations） 分解为可由不同任务同时执行的块
- 每个任务都会多次执行其循环部分
- 每个任务都可以执行其工作，而无需其他任务提供的任何信息（没有数据依赖关系）
- 主任务（Master task） 使用 send/receive 点对点操作接收来自其他任务的结果
```
npoints = 10000
circle_count = 0

p = number of tasks
num = npoints/p

do j = 1,num
  generate 2 random numbers between 0 and 1
  xcoordinate = random1
  ycoordinate = random2
  if (xcoordinate, ycoordinate) inside circle
  then circle_count = circle_count + 1
end do

if I am MASTER
  receive from WORKERS their circle_counts
  compute PI (use MASTER and WORKER calculations)
else if I am WORKER
  send to MASTER circle_count
endif
```
关键：在可用的 task 之间划分工作，这些 tasks 通过点对点消息传递调用来传达数据。
![[PI 任务划分.png]]

Point-to-Point 操作的类型
MPI 点对点操作通常涉及两个不同的 MPI 任务之间的消息传递，并且只涉及两个不同的 MPI 任务。一个任务正在执行发送（send）操作，另一个任务正在执行匹配的接收（recv）操作。

用于不同目的的类型的发送和接收例程
- Synchronous send （同步发送）
- Blocking send / blocking receive （阻塞发送/阻塞接收）
- Non-Blocking send / Non-Blocking receive
- Buffered send （缓冲发送）
- Combined send / receive （组合发送/接收）
- "Ready" send

Order and Fairness
Order:
MPI 保证消息不会相互重叠。
如果发送方连续向同一目标发送两条消息（Message 1 and Message 2），并且两条消息都与同一接收匹配，则接收操作将在消息 2 之前接收消息 1 。
如果接收方连续发布两个接收（Receive 1 and Receive 2），并且两个接收都在查找相同的消息，则 Receive 1 将在 Receive 2 之前接收消息。
如果有多个线程参与通信操作，则顺序规则不适用。
Fairness:
MPI 不保证公平性，由程序员来防止“Operation Starvation”

#### 点对点通信函数

| 函数                   | 原型                                                   |
| -------------------- | ---------------------------------------------------- |
| Blocking sends       | MPI_Send(buffer, count, type, dest, tag, comm)       |
| Non-blocking sends   | MPI_Isend(buffer,count,type,dest,tag,comm,request)   |
| Blocking receive     | MPI_Recv(buffer,count,type,source,tag,comm,status)   |
| Non-blocking receive | MPI_Irecv(buffer,count,type,source,tag,comm,request) |

其中
- `buffer` - 引用要发送或接收的数据的地址空间。
- `count` - 要发送的特定类型的数据元素的数量。
- `type` - 数据类型
- `dest` - 目的进程
- `source` - 源进程
- `tag` - 用于唯一标识消息
- `communicator` - 指示通信上下文或源字段或目标字段有效的进程集。除非程序员显式创建新的 communicator，否则通常使用预定义的 communicator MPI_COMM_WORLD。
- `status` - 对于接收操作，指示消息的来源和消息的标签。在 C 中，此参数是指向预定义结构 `MPI_Status` 的指针（如，`stat.MPI_SOURCE` `stat.MPI_TAG`）

#### Collective Communication Routines
集体通信函数
![[Collective Communication.png]]
类型：
- 同步 - 进程等待，直到组的所有成员都到达同步点
- 数据移动 - 广播（broadcast）、Scatter/Gather、all to all
- Collective Computation (reductions) - 组的一个成员从其他成员收集数据，并对该数据执行运算（min, max, add, multiply, etc）等
Scope（范围）：
集体通信例程必须包含 `communicator` 范围内的所有进程。默认情况下，所有进程都是 `communicator MPI_COMM_WORLD` 中的成员。其他 `communicator` 可由程序员定义。
如果 `communicator` 中的一个任务不参与，可能发生意外行为，包括程序失败。
因此，程序员有责任确保 `communicator` 中的所有进程都参与任何集体操作。

进程子集内的集体操作是通过以下方法完成的：首先将子集划分为新组，然后将新组附加到新的 `communicators` 中。
使用 MPI-3 时，Collective 操作可以是阻塞的，也可以是非阻塞的。
函数接口
`MPI_Barrier` - 同步操作。在组中创建 `barrier` 同步。每个任务在到达 `MPI_Barrier` 调用时都会阻塞，直到组中所有任务都到达相同的 `MPI_Barrier` 调用。然后，所有任务可以自由进行。
```c
MPI_Barrier(comm)
```
`MPI_Bcast` - 数据移动操作。将消息从等级为 `root` 的进程广播（发送）到组（**group**, 细分为组）中的所有其他进程。
```c
MPI_Bcast(&buffer, count, datatype, root, comm)
```
`MPI_Scatter` - 数据移动操作。将不同消息从单个源 task 发送到组中的每个 task 。
```c
MPI_Scatter(&sendbuf, sendcnt, sendtype, &recvbuf, recvcnt, recvtype, root, comm)
```
![[MPI_Scatter.png]]

`MPI_Gather` - 数据移动操作。将组中每个任务的不同消息收集到单个目标任务。
```c
MPI_Gather(&sendbuf, sendcnt, sendtype, &recvbuf, recvcount, recvtype, root, comm)
```

`MPI_Allgather` - 数据移动操作。将数据连接到组中的所有任务。实际上，组中的每个任务都在组内执行一对多的广播操作。
![[MPI_Allgather.png]]
`MPI_Reduce` - Collective Computation Operation
对组中的所有任务应用 Reduce 操作，将结果放在一个任务中。
预定义的 MPI Reduction 操作如下：
`MPI_MAX`, `MPI_MIN`, `MPI_SUM`, `MPI_PROD`, `MPI_LAND`, `MPI_BAND`, `MPI_LOR`, `MPI_BOR`, `MPI_LXOR`, `MPI_BXOR`, `MPI_MAXLOC`, `MPI_MINLOC`

`MPI_Allreduce` - 应用 Reduction 操作并将结果放置在组中的所有任务中。这相当于 `MPI_Reduce` 后跟 `MPI_Bcast`
![[MPI_Allreduce.png]]

`MPI_Reduce_scatter` - 首先，对组中所有 task 的向量进行元素规约（element-wise reduction） 。接下来，结果向量被拆分为不相交的段并分布在各个任务中。这相当于 `MPI_Reduce` 后跟 `MPI_Scatter` 操作。
![[MPI_Reduce_scatter.png]]

`MPI_Alltoall` - 组中的每个 task 执行一个分散操作，按索引顺序向组中的所有任务发送不同的消息。
![[MPI_Alltoall.png]]

`MPI_Scan` - 对 任务组中的 Reduction 操作执行 Scan 操作（偏序 Reductions）。
![[MPI_Scan.png]]

#### Group 与 Communicator 管理例程
组是一组有序的进程。组中的每个进程都与唯一的整数 rank \[0, N-1\]相关联，其中 N 是组中的进程数。在 MPI 中，组在系统内存中表示为对象。程序员只能通过 `handle` 访问它。组始终与 `communicator` 对象相关联。
`Communicator` 包含一组可以相互通信的进程。所有 MPI 消息都必须指定一个 `communicator` 。
![[MPI_COMM_WORLD.png]]
编程注意事项与限制：Groups/Communicators 是动态的，它们可以在程序执行期间创建和销毁。
进程可能位于多个 Groups / Communicator 中，它们在每个 Groups / Communicator 中具有唯一的等级。

