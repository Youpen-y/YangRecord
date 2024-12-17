`getpagesize`  -  获取内存页大小

库：标准 C 库 （`libc`, `-lc`）

``` c
#include <unistd.h>

int getpagesize(void);
```

描述：`getpagesize()` 函数返回一个内存页中的字节数量，"page" 是固定长度的块，是 `mmap(2)` 执行内存分配和文件映射的单元。

注意：用户程序不应该硬编码页大小，（不能用常量或PAGE_SIZE宏，因为有些架构支持多个页大小）