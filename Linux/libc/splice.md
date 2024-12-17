`splice` - splice data to/from a pipe （将数据拼接到管道或从管道拼接数据）

```c
#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#include <fcntl.h>

ssize_t splice(int fd_in, off_t *off_in,
               int fd_out, off_t *off_out,
               size_t len, unsigned int flags);
```

描述
`splice()` 在两个文件描述符之间移动数据，而不在内核地址空间和用户地址空间之间进行复制。它将最多 `len` 个字节的数据从文件描述符 `fd_in` 传输到文件描述符 `fd_out` 。文件描述符之一必须引用管道。