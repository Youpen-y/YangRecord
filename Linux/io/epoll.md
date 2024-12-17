### 函数接口
`epoll` 是一个用于 **scalable** I/O 事件通知机制的 Linux 内核系统调用，在 Linux 内核 2.6 版本中首次引入。
功能是：监视多个文件描述符（fd），以查看其中任何一个上是否可以进行 I/O 。它旨在取代旧的 POSIX `select(2)` 和 `poll(2)` 系统调用，以在某些应用程序中（需要监视的文件描述符的数量很大）实现更好的性能。（`select` 和 `poll` 复杂度 O(n)，`epoll` 是O(1)）。
`epoll` 类似于 FreeBSD 的 `kqueue` ，由一组用户空间函数组成。

`epoll` 使用红黑树数据结构来跟踪当前正在监视的所有文件描述符。

#### API

头文件 `<sys/epoll.h>`

```c
int epoll_create(int size); // 从 Linux 2.6.27 和 glibc 2.9.4 已被弃用
/* 从 Linux 2.6.8，size 参数被忽略，必须大于零 */
int epoll_create1(int flags);

// return value: 成功 非负文件描述符；失败 -1 并设置 errno
/* 
Errors
- EINVAL (size 不是正值，或 flags 中指定的值无效)
- EMFILE 遇到 /proc/sys/fd/epoll/max_user_instances 对每用户 epoll 实例数量的限制
- ENFILE 已达到打开文件总数的系统限制
- ENOMEM 内存不足，无法创建内核对象
```
`epoll_create()` 创建一个 `epoll instance` 并返回一个引用该实例的文件描述符。该文件描述符用于所有后续对 `epoll` 接口调用。 当不再需要时，应使用 `close()` 关闭 。

> 使用文件描述符作为对 `epoll instance` 的引用的原因是：这使得 `epoll instance` 也是可轮询的。允许更高级的使用，如使用 `epoll`, `select`, `poll` 监控 `epoll instance`。

`epoll_create()` 返回的文件描述符。当所有引用 `epoll instance` 的文件描述符都已关闭时，内核会销毁该实例并释放相关资源以供重用。
最近的 `epoll_create1()` 扩展了 `epoll_create()` 的功能。
{
    如果 flags == 0 ，那么 `epoll_create1()` 除 了删除过时的 `size` 参数外，与 `epoll_create()` 相同。
    flags == EPOLL_CLOEXEC，在新文件描述符上设置 `close-on-exec(FD_CLOEXEC)` 标志。
    参阅 `open(2)` 中 `O_CLOEXEC` 标志的描述。
}

---
Notes
在最初的 `epoll_create()` 实现中， `size` 参数通知内核调用者期望添加到 `epoll instance` 的文件描述符的数量。内核使用此信息作为在描述时间的内部数据结构中最初分配的空间量的提示。（如有必要，如果调用者的使用量超出了 `size` 中给出的提示，内核将分配更多空间。）
如今，内核动态调整所需数据结构的大小，而不需要提示，但 `size` 仍必须大于 0 ，以确保新的 `epoll` 应用程序在旧内核上运行时的向后兼容性。


---

```c
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
```
`epoll_ctl()` 用来注册感兴趣的特定文件描述符。该系统调用对文件描述符 `epfd` 引用的 `epoll instance` 执行控制操作 ，它请求对目标文件描述符 `fd` 执行操作 `op` 。
在当前 `epoll instance` 上注册的文件描述符集合有时称为epoll` set 。

操作 `op` 可为
- `EPOLL_CTL_ADD` 在文件描述符 `epfd` 引用的 `epoll instance` 上注册目标文件描述符 `fd`，并将事件 `event` 与连接到 `fd` 的内部文件关联起来
- `EPOLL_CTL_DEL` 从 `epfd` 引用的 `epoll instance` 中删除（注销）目标文件描述符 `fd` 。`event` 被忽略且可以为 NULL 。
- `EPOLL_CTL_MOD` 更改与目标文件描述符 `fd` 关联的事件 `event`
```c
/* Valid opcodes ( "op" parameter ) to issue to epoll_ctl().  */

#define EPOLL_CTL_ADD 1 /* Add a file descriptor to the interface.  */
#define EPOLL_CTL_DEL 2 /* Remove a file descriptor from the interface.  */
#define EPOLL_CTL_MOD 3 /* Change file descriptor epoll_event structure.  */

typedef union epoll_data {
    void    *ptr;
    int      fd;
    uint32_t u32;
    uint64_t u64;
} epoll_data_t;

struct epoll_event {
    uint32_t      events;   /* Epoll events */
    epoll_data_t  data;     /* User data variable */
}
```

`events` 成员是使用以下可用事件类型组成的位集
```c
enum EPOLL_EVENTS {

    EPOLLIN = 0x001,         // 关联的文件可用于 read 操作
#define EPOLLIN EPOLLIN

    EPOLLPRI = 0x002,        // 有紧急数据可用于 read 操作
#define EPOLLPRI EPOLLPRI

    EPOLLOUT = 0x004,        // 关联的文件可用于 write 操作
#define EPOLLOUT EPOLLOUT

    EPOLLRDNORM = 0x040,
#define EPOLLRDNORM EPOLLRDNORM

    EPOLLRDBAND = 0x080,
#define EPOLLRDBAND EPOLLRDBAND

    EPOLLWRNORM = 0x100,
#define EPOLLWRNORM EPOLLWRNORM

    EPOLLWRBAND = 0x200,
#define EPOLLWRBAND EPOLLWRBAND

    EPOLLMSG = 0x400,
#define EPOLLMSG EPOLLMSG

    EPOLLERR = 0x008,       // 关联的文件描述符上发生错误情况。epoll_wait 会一直等待这个事件；没有必要在 events 中设置
#define EPOLLERR EPOLLERR

    EPOLLHUP = 0x010,       // 挂起（Hang up） 发生在关联的文件描述符上，epoll_wait 会一直等待这个事件；没有必要在 events 中设置
#define EPOLLHUP EPOLLHUP

    EPOLLRDHUP = 0x2000,
#define EPOLLRDHUP EPOLLRDHUP

    EPOLLEXCLUSIVE = 1u << 28,
#define EPOLLEXCLUSIVE EPOLLEXCLUSIVE

    EPOLLWAKEUP = 1u << 29,
#define EPOLLWAKEUP EPOLLWAKEUP

    EPOLLONESHOT = 1u << 30,  // 设置关联文件描述符的一次性行为。
#define EPOLLONESHOT EPOLLONESHOT

    EPOLLET = 1u << 31       // 设置关联文件描述符的 Edge Triggered behavior （边缘触发行为）。epoll 的默认行为是 Level Triggered （水平触发）
#define EPOLLET EPOLLET
};
```

返回值
成功时返回 0 ；失败时返回 -1

---

```c
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
int epoll_pwait(int epfd, struct epoll_event *events, int maxevents, int timeout, const sigset_t *sigmask);
// 与 epoll_wait 相比，其最后一个参与允许将线程的信号掩码暂时且原子地由参数替换
```
`epoll_wait()` 等待文件描述符 `epfd` 指向的 `epoll instance` 上注册的 I/O 事件，如果当前没有事件可用，则阻塞调用线程。
`event` 指向的内存区域将包含调用者可用的事件。`epoll_wait()` 最多返回 `maxevents` （`maxevents` 必须大于零）。

`timeout` 参数指定 `epoll_wait()` 将阻塞的最小毫秒数。（此间隔将向上舍入到系统时钟粒度，内核调度延迟意味着阻塞间隔可能小幅超出。）
- 指定 `timeout` 为 -1 会导致 `epoll_wait()` 无限期阻塞。
- 指定 `timeout` 为 0 会导致 `epoll_wait()` 立即返回，即使没有可用的事件。

每个返回的结构中的 `data` 将包含用户使用 `epoll_ctl` （EPOLL_CTL_ADD, EPOLL_CTL_MOD）设置的相同数据，而 `events` 成员将包含返回的 event bit field。

`epoll_wait()` 和 `epoll_pwait()` 之间的关系类似于 `select` 和 `pselect` 之间的关系：与 `pselect` 一样， `epoll_pwait()` 允许应用程序安全地等待，直到文件描述符准备好或直到捕获到一个信号。

返回值
成功时，`epoll_wait()` 返回为请求的 I/O 做好准备的文件描述符的数量，如果在请求的 `timeout` 毫秒内没有文件描述符准备好，则返回 0 。当发生错误时，`epoll_wait()` 返回 -1 ，并适当设置 `errno` 。

---
### 属性
#### 触发
level-triggered（水平触发） and edge-triggered（边沿触发）

水平触发意味着如果 `fd` 处于就绪状态，则始终为真。
边沿触发意味着仅在 `fd` 状态变化时，才为真。
（比如，程序读取缓冲区为 100 字节，而文件描述符有 200 字节的数据。假设调用 `read` 一次，然后再次调用 `epoll_wait`。还有 100 字节的数据可供读取。
在水平触发模式下，内核会注意到这一点并通知进程应该再次调用 `read` 。
而在边沿触发模型下，内核将立即进入睡眠状态，如果另一方正在等待响应（如，发送的数据是某种 RPC），那么双方将“死锁”，因为服务器将等待客户端发送更多数据，但客户端将等待服务器发送响应。）

要使用边缘触发轮询，必须将文件描述符置于非阻塞模式

当进程调用 `epoll_wait` 函数时， `epoll instance` 首先扫描 level-triggered 的文件描述符，看是否有文件描述符可以返回；如果所有文件描述符都注册了 level-triggered events，那么它只比 `poll` 或 `select` 快一点。如果 edge-triggered events 位于 `epoll instance` 的就绪列表中，则返回该事件。所以内核根本不需要扫描。（O(1) 的原因）

与早期的 O(n) 算法{select / poll}不同，`epoll` 是一种 O(1) 算法 —— 这意味着它可以随着监视的文件描述符的增加而扩展。
`select` 在监视的文件描述符列表中使用线性搜索，而 `epoll` 在内核文件结构中使用回调。

---
#### 示例
将一个文件描述符 `fd` 加入 `epoll instance` 
```c
void addfd(int epollfd, int fd, bool one_shot, int trigger_mode) {
    epoll_event event;
    event.data.fd = fd;

    if (1 == trigger_mode) {
        event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    } else {
        event.events = EPOLLIN | EPOLLRDHUP;
    }

    if (one_shot) {
        event.events |= EPOLLONESHOT;  // 单次触发，收到此事件后需重新添加
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}
```

从 `epoll instance` 中删除一个描述符
```c
void removefd(int epollfd, int fd) {
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}
```

事件循环
```c
void event_loop() {
    while (1) {
        int number = epoll_wait(epoll_fd, events, MAX_EVENT_NUMBER, -1);
        // 阻塞等待事件的发生，返回事件的数目，并将触发的事件写入 events
        if (number < 0 && errno != EINTR) { // EINTR: 系统调用中断
            fprintf(stderr, "epoll failure");
            break;
        }

        for (int i = 0; i < number; ++i) {
            int sockfd = events[i].data.fd;

            if (sockfd == m_listenfd) { // 如果事件来自监听端
                // 处理逻辑
            }
            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                // 处理逻辑
            }
            else if (events[i].events & EPOLLIN) // 可读事件
            {
                // 处理逻辑
            }
            else if (events[i].events & EPOLLOUT) // 可写事件
            {
                // 处理逻辑
            }
        }
    }
}
```


#### 图形化
- `epoll_create/epoll_create1`
![[epoll_create 创建 epoll instance.png]]

- `epoll_ctl`
![[epoll_ctl 注册感兴趣的fd.png]]


- `epoll_wait`
![[epoll_wait 等待就绪 fd.png]]
![[内核提供就绪 fd 给进程.png]]


---

![[epoll 结构.png]]
`epoll` 内核实现细节（可参见）
https://idndx.com/the-implementation-of-epoll-1/
https://idndx.com/the-implementation-of-epoll-2/
https://idndx.com/the-implementation-of-epoll-3/
https://idndx.com/the-implementation-of-epoll-4/