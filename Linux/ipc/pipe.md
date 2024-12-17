管道，是一种单向（Linux，Posix.1） {Freebsd 则可双向}进程间通信方式。包含一个读端和一个写端。数据从写端写入并从读端读取。
> Each pipe and FIFO is implemented as a "file" mounted on the pipefs filesystem. A FIFO is just a thin wrapper around a pipe

使用 `pipe(2)` 创建管道，它返回两个描述符，`pipefd[0]` 指向管道读取端，`pipefd[1]` 指向管道写端。写入管道的数据由内核缓冲，直到从管道读取端读取。

```c
#include <unistd.h>

int pipe(int pipefd[2]);

#define _GNU_SOURCE
#include <fcntl.h>   /* Definition of O_* constants */
#include <unistd.h>

int pipe2(int pipefd[2], int flags);

/* 在 Alpha, IA-64, MIPS, SuperH, 和 SPARC/SPARC64， pipe() 有如下原型 */
struct fd_pair {
    long fd[2];
};
struct fd_pair pipe(void);
```

其中，如果 `flags = 0` ，则 `pipe2()` 和 `pipe()` 相同。可以在`flags` 中按位或以下位。
- `O_CLOEXEC` - 在两个新文件描述符上设置 `close-on-exec(FD_CLOEXEC)` 标志。
- `O_DIRECT` （since Linux 3.4） - 创建一个以 `packet` 模式执行 I/O 的模式。对管道的每次 `write` 都作为一个单独的数据包进行处理，而来自管道的 `read` 将一次读取一个数据包。不支持此标志的旧内核将返回 `EINVAL` 错误。
    注意：
        1. 大于 `PIPE_BUF` 字节的写入将被分成多个数据包。常量 `PIPE_BUF` 在 `<linux/limits.h>` 中定义。
        2. 如果 `read(2)` 指定的缓冲区的大小  <  下一个数据包，则读取请求的字节数，丢弃数据包中多余的字节。指定 `PIPE_BUF` 的缓冲区大小足以读取最大可能的数据包。
        3. 不支持零长度数据包。（ A `read(2)` that specifies a buffer size of zero is a no-op, and returns 0 ）
- `O_NONBLOCK` - 在新文件描述符引用的打开文件描述（open file descriptions）上设置 `O_NONBLOCK` 文件状态标志。使用此标志可以省去对 `fcntl(2)` 的额外调用以实现相同的结果。
- `O_NOTIFICATION_PIPE` - 从Linux 5.8 开始，一般的通知机制建立在管道的顶部，内核将通知消息拼接到用户空间打开的管道中。管道的所有者必须告诉内核要监视哪些事件源，并且还可以应用过滤器来选择应将哪些子事件放入管道中。

返回值
成功时，返回 0 ；出错时，返回 -1 ，并设置 `errno` 来指示错误，而且 `pipefd` 保持不变。

示例：
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    int    pipefd[2];
    char   buf;
    pid_t  cpid;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    cpid = fork();
    if (cpid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0) {    /* Child reads from pipe */
        close(pipefd[1]);          /* Close unused write end */
        while (read(pipefd[0], &buf, 1) > 0)
            write(STDOUT_FILENO, &buf, 1);
        write(STDOUT_FILENO, "\n", 1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS);
    } else {            /* Parent writes argv[1] to pipe */
        close(pipefd[0]);          /* Close unused read end */
        write(pipefd[1], argv[1], strlen(argv[1]));
        close(pipefd[1]);          /* Reader will see EOF */
        wait(NULL);                /* Wait for child */
        exit(EXIT_SUCCESS);
    }
}
```