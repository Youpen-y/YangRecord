`sendfile` - 在文件描述符之间传输数据

```c
#include <sys/sendfile.h>

ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
```

描述
`sendfile()` 在一个文件描述符和另一个文件描述符之间复制数据。**由于此复制是在内核内完成的，因此 `sendfile()` 比 `read()` 和 `write()` 的组合更有效，后者需要在用户空间之间传输数据。**
`in_fd` 是打开的用于读取的文件描述符，`out_fd` 是打开的用于写入的文件描述符。

当 `sendfile()` 返回时，该变量将被设置为读取的最后一个字节之后的字节的偏移量。
如果 `offset` 不为 NULL ，则它指向一个保存文件偏移量的变量，`sendfile()` 将从该偏移量开始从 `in_fd` 读取数据。当 `sendfile()` 返回时，该变量（即 `offset` ）将被设置为读取的最后一个字节后面那个字节的偏移量。

如果 `offset` 非 NULL，`sendfile()` 不会更改 `in_fd` 的偏移（此处指的是文件的属性偏移，而非函数变量`offset`）。
如果 `offset` 为 NULL，则从 `in_fd` 文件 offset 开始处读取数据，文件偏移量将由调用更新。

`count` 是文件描述符之间复制的字节数。

`in_fd` 参数必须对应于支持 `mmap-like` 的操作（即它不能是一个 socket）。


在 Linux 2.6.33 之前，`out_fd` 必须指向一个 socket 。自从 Linux 2.6.33 可以为任意文件。如果它是 seekable，则 `sendfile()` 适当地改变文件偏移。

返回值
如果传输成功，返回写入 `out_fd` 的字节数。注意，对 `sendfile()` 的一次成功调用可能写入的比请求的少。调用者应该准备 retry the call ，如果有未发送的字节。
错误时，返回 -1 ，并设置 `errno`。


HISTORY
最初的 Linux `sendfile()` 系统调用并不是为了处理大文件偏移。Linux 2.4 添加了 `sendfile64()` ，参数`offset` 具有更宽的类型。而 `glibc` 的 `sendfile()` 包装函数透明地处理这种内核差异。

NOTES
`sendfile()` 最多传输 0x7ffff000 bytes 。实际传输的字节数被返回。（这在 32 bit 和 64 bit 系统上均如此）

如果您打算使用**sendfile** () 将文件发送到 TCP 套接字，
但需要在文件内容前面发送一些标头数据，
您会发现使用**TCP_CORK**选项很有用，如下所述
在 tcp(7)中，最大限度地减少数据包数量并进行调整
表现。

在 `sendfile()` 出现 `EINVAL` 和 `ENOSYS` 失败情况时，程序可能希望回退到 `read` 和 `write` 。

如果 `out_fd` 指向具有 zero-copy 支持的 socket 或 pipe，调用者必须确保 `in_fd` 所指向的文件的传输部分保持不变直到另一端的读者 `out_fd` 已消耗了传输的数据。

Linux 特定的 `splice`[[]] 调用支持在在任意文件描述符之间传输数据，只要其中一个（或两个）是一个管道。


#### 示例

```c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define SOURCE_FILE "source.txt"
#define DEST_FILE1 "dest_null_offset.txt"
#define DEST_FILE2 "dest_non_null_offset.txt"
#define BUFFER_SIZE 1024

void create_source_file() {
    FILE *fp = fopen(SOURCE_FILE, "w");
    if (fp == NULL) {
        perror("Error creating source file");
        exit(1);
    }
    fprintf(fp, "This is a test file for sendfile demonstration.\n");
    fprintf(fp, "It contains multiple lines to show offset behavior.\n");
    fprintf(fp, "The sendfile function will be used to copy this content.\n");
    fclose(fp);
}

void sendfile_with_null_offset() {
    int source_fd = open(SOURCE_FILE, O_RDONLY);
    int dest_fd = open(DEST_FILE1, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (source_fd == -1 || dest_fd == -1) {
        perror("Error opening files");
        exit(1);
    }

    struct stat stat_buf;
    fstat(source_fd, &stat_buf);
    
    ssize_t ret = sendfile(dest_fd, source_fd, NULL, stat_buf.st_size);
    
    if (ret == -1) {
        perror("sendfile failed");
    } else {
        printf("sendfile with NULL offset: transferred %zd bytes\n", ret);
    }

    close(source_fd);
    close(dest_fd);
}

void sendfile_with_non_null_offset() {
    int source_fd = open(SOURCE_FILE, O_RDONLY);
    int dest_fd = open(DEST_FILE2, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    
    if (source_fd == -1 || dest_fd == -1) {
        perror("Error opening files");
        exit(1);
    }

    struct stat stat_buf;
    fstat(source_fd, &stat_buf);
    
    off_t offset = 0;
    ssize_t ret = sendfile(dest_fd, source_fd, &offset, stat_buf.st_size);
    
    if (ret == -1) {
        perror("sendfile failed");
    } else {
        printf("sendfile with non-NULL offset: transferred %zd bytes\n", ret);
        printf("After sendfile, offset is at: %ld\n", (long)offset);
    }

    close(source_fd);
    close(dest_fd);
}

int main() {
    create_source_file();
    sendfile_with_null_offset();
    sendfile_with_non_null_offset();
    return 0;
}
```