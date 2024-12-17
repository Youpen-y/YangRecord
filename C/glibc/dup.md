```c
dup, dup2, dup3 - duplicate a file descriptor
```

语法
```c
#include <unistd.h>

int dup(int oldfd);
int dup2(int oldfd, int newfd);

#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
int dup3(int oldfd, int newfd, int flags);
```

描述
`dup()` 系统调用分配一个新的文件描述符并返回，该描述符引用与描述符 `oldfd` 相同的打开文件描述。新文件描述符保证是调用进程中未使用的编号最小的文件描述符。

成功返回后，新旧文件描述符可以互换使用，它们共享文件偏移量和文件状态标志；如果在其中一个文件描述符上使用 `lseek()` 修改文件偏移量，则另一个文件描述符的偏移量也会更改。

但这两个文件描述符不共享文件描述符标志（`close-on-exec` 标志）。复制的fd 的 `close-on-exec` 是关闭的。

---
`dup2()` 实现类似任务，但并不使用编号最小的的未使用文件描述符，而是使用 `newfd` 指定的文件描述符编号。换句话说，文件描述符 `newfd` 将被调整，以便它现在引用与 `oldfd` 相同的打开文件描述（open file description）。

如果 `newfd` 此前已打开，则在重用之前将会静默将其关闭（即，`dup2()` 不会报告关闭期间的任何错误）。

关闭和重用文件描述符 `newfd` 的步骤是以原子方式执行的。因为如果使用 `close()` 和 `dup()` 实现相同的功能可能会存在 race condition，因为 `newfd` 可能在这两个函数之间被重用（主程序被分配文件描述符的信号处理程序中断/并行线程分配文件描述符）。

Note:
- 如果 `oldfd` 无效，则调用失败，而且 `newfd` 不会被关闭
- 如果 `oldfd` 有效，并且 `newfd` 与 `oldfd` 具有相同的值，则 `dup2()` 不执行任何操作，并返回 `newfd`

---
`dup3()` 与 `dup2()` 的区别
- 调用者可以通过在 `flags` 中指定 `O_CLOEXEC` 来强制为新文件描述符设置 `close-on-exec` 标志。
- 如果 `oldfd` 等于 `newfd` ，`dup3()` 将失败并出现错误 `EINVAL` 。 


示例
```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main() {
    // 打开一个文件，如果文件不存在则创建它
    int fd = open("example.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // 复制文件描述符
    int new_fd = dup(fd);
    if (new_fd == -1) {
        perror("dup");
        close(fd);
        return 1;
    }

    // 使用新的文件描述符写入数据
    const char *message = "Hello, World!\n";
    ssize_t bytes_written = write(new_fd, message, strlen(message));
    if (bytes_written == -1) {
        perror("write");
    } else {
        printf("Written %zd bytes.\n", bytes_written);
    }

    // 从文件的开头读取数据
    lseek(new_fd, 0, SEEK_SET);
    char buffer[128];
    ssize_t bytes_read = read(new_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read == -1) {
        perror("read");
    } else {
        buffer[bytes_read] = '\0'; // 确保字符串以空字符结尾
        printf("Read data: %s\n", buffer);
    }

    // 关闭文件描述符
    close(fd);
    close(new_fd);

    return 0;
}
```