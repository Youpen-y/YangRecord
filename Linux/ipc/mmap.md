#### `mmap()` `munmap()` 系统调用

> `mmap()`, `munmap()` - 映射或取消映射 files 或 devices 到内存

``` c
#include <sys/mman.h>

void *mmap(void *addr, size_t length, int prot, int flags,
            int fd, off_t offset);
```

用户进程可以使用 `mmap()` 来请求操作系统内核将 文件 或 设备 映射到该进程的内存（i.e. address space）中，`mmap()` 系统调用还可用于分配内存（an anonymous mapping）。关键点是在引用映射的页面之前，它们实际上不会被带入物理内存中；因此 `mmap() can be used to implement "lazy" loading of pages into memmory(demand paging)`
![[mmap 映射图.png]]

  
`mmap()` 参数
- `addr` 向操作系统内核提示虚拟映射提示虚拟映射应在进程的虚拟内存（虚拟地址空间）中启动的地址。
    - `NULL` ，内核将虚拟映射放在它认为合适的任何位置
    - `non-null`，则 `addr` 应该是页面大小的倍数

- `length` 映射的长度（字节数）

- `prot` 对  mapped memory（映射内存） 的保护(protection level)。是下列 single-bit 值的按位或 
    - `PROT_READ` 使映射内存内容可被进程读取
    - `PROT_WRITE` 使映射内存的内容可由进程写入
    - `PROT_EXEC` 使映射内存的内容可由进程当作CPU机器指令执行
    - `PROT_NONE`

- `flags` 控制映射的各种选项。
  - `MAP_ANONYMOUS` (or `MAP_ANON`)，分配匿名内存；`the pages are not backed by any file`
  - `MAP_FILE` 默认设置，无需指定；映射的区域由常规文件提供支持
  - `MAP_FIXED` 不将 `addr` 解释为提示：将映射准确地放在该地址，该地址必须是页面大小的倍数
  - `MAP_PRIVATE` 对映射内存区域的修改对映射同一文件的其他进程不可见
  - `MAP_SHARED` 对映射内存区域的修改对于映射同一文件的其他进程可见，并最终反映在文件中

- `fd` 要从中填充 memory region 的文件的open file descriptor。如果指定了 `MAP_ANONYMOUS`，则 `fd` 为 -1。

- `offset` 如果不是匿名映射，则内存映射区域将从 `fd` 文件开头的`offset` 字节开始填充。应该是页面大小的倍数。

成功时，`mmap()` 返回执行 mapped area 的指针。出现错误时，将返回 `MAP_FAILED` （即，返回(void)(-1)并设置 `errno` 以指示原因）

---
`munmap()` 是一个系统调用，用于取消先前使用 `mmap()` 映射的内存。该调用从调用进程的地址空间删除映射的内存。

``` c
int munmap(void *addr, size_t length);
```

`munmap()` 参数
- `addr` 要从调用进程的虚拟映射中取消映射的内存地址（应该是页面大小的倍数）
- `length` 从调用进程的虚拟映射中取消映射的内存长度（字节数，页面大小的倍数）

成功时， `munmap()` 返回 0 ，失败时返回 -1，并设置 `errno` 来指示原因。如果成功，之后对未映射的内存区域的访问将导致 segmentation fault (SIGSEGV)。

---
`mmap()` 的优点

使用 `mmap()` 访问磁盘上某些文件的内容有很多优点。
- lazy loading（延迟加载）：如果某个页面内没有内存被引用，那么该页面永远不会加载到物理内存中。

- speed improvements：传统I/O涉及大量系统调用（如，调用 `read()` ） 来将数据加载到内存中。这些调用会产生相关成本，如函数本身的错误检查。将数据加载到主内存中还必须经过多层软件抽象，数据在最终放入内存之前会在操作系统的各个缓冲区复制，这将减慢程序速度。

- versatile dynamic memory allocation：支持通用的动态内存分配。尤其是，`malloc()` 不能在信号处理程序中安全地调用，而 `mmap()` 可以使用匿名映射。此外，通过使用 `mmap()` 分配内存，而不是简单地使用 `sbrk()` 提供进程的 `break` ，可以使用 `malloc()` 本身更加通用。调用 `sbrk()` 需要仅在堆顶部释放内存（i.e., by lowering the break）。但是，使用 `mmap()` 分配内存没有这样的限制，堆中间的内存可以使用 `munmap()` 随意释放。

