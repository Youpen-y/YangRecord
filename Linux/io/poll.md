`poll`, `ppoll` 等待文件描述符上的某个事件

``` c
#include <poll.h>

int poll(struct pollfd *fds, nfds_t nfds, int timeout);

int ppoll(struct pollfd *fds, nfds_t nfds, 
         const struct timespec *timeout_ts, const sigset_t *sigmask);
```
`poll()` 执行与 `select()` 类似的任务：它等待一组文件描述符中的一个准备好执行 I/O 。
要监视的文件描述符集在 `fds` 参数中指定，该参数结构如下：
``` c
struct pollfd {
    int fd;  /* file descriptor */
    short events;  /* requested events */
    short revents;  /* returned events */
}
```
调用者应通过 `nfds` 指定 `fds` 数组中的项目数。
字段 `fd` 包含打开的文件的文件描述符。如果 `fd` 为负数，则忽略相应的 `events` 字段，并且 `revents` 字段返回零。（这提供了一种简单的方法来忽略单个 `poll()` 调用的文件描述符：只需取负 `fd` 字段。

字段 `events` 是输入参数，是用于指定应用程序对文件描述符 `fd` 上感兴趣的事件的位掩码。如果将此 `events` 指定为零，则忽略 `fd` 的所有事件，并且 `revents` 返回零。

字段 `revents` 是输出参数，由内核填充实际发生的事件。`revents` 中返回的位可以包括 `events` 中的任何位，或者 `POLLERR` 、`POLLHUP`或 `POLLNAVAL` 值之一。

如果所有文件描述符都没有发生请求的任何事件（并且没有错误），则 `poll()` 会阻塞，直到其中一个事件发生。`timeout` 参数指定 `poll()` 将阻塞的最小毫秒数，`timeout` 为负意味着无限超时；为零会导致 `poll()` 立即返回，即使没有准备好文件描述符。

可在 `events` 和 `revents` 中设置/返回的位（在 `<poll.h>` 中定义）
- `POLLIN`  有数据要读取
- `POLLPRI`    有紧急数据需要读取（如，TCP套接字上的带外数据）
- `POLLOUT`     现在写入不会阻塞
- `POLLERR`      错误条件（仅输出）
- `POLLHUP`       Hang up （仅输出）
- `POLLNVAL`     请求无效（fd not open）
- `POLLRDHUP` （Since Linux 2.6.17）Stream socket peer closed connection, or shut down writing half of connection. The \_GNU\_SOURCE feature test macro must be defined (before including any header files) in order to obtain this definition.