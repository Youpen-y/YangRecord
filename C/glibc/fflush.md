`fflush` - 刷新流

语法：
```c
#include <stdio.h>

int fflush(FILE *stream);
```

- 如果 `stream != NULL` ，刷新该流
- 如果 `stream == NULL` ，刷新所有输出流
- 成功返回 0 ，失败返回 `EOF` 并设置 `errno`

描述：
不会影响流的开放状态；
对于输出流，`fflush()` 会把用户空间缓冲区中的数据立即写入底层文件（或设备），即出触发系统调用 `write()` 。
```c
FILE *fp = fopen("out.txt", "w");
fprintf(fp, "hello");   // 数据进入用户态缓冲区
fflush(fp);             // 马上写入文件中（未必持久，可能写入到内核缓冲区）
```

对于输入流（即在 `fread()` 或 `fgets()` 中读文件的情况）：
- 如果这个文件是可定位的（seekable），如普通磁盘文件（不是管道或终端）
	- `fflush()` 会丢弃已经从文件中读到用户缓冲区但还没用掉的部分。
示例：
```c
FILE *fp = fopen("data.txt", "r");
char buf[100];

// 设想文件前 100 字节被 fread 预读流，但只使用前 20 字节
fread(buf, 1, 20, fp);

// 此时，80 字节仍在用户缓冲区
fflush(fp); // 丢弃那 80 字节，下次 fread 会重新从磁盘读取
```

>[!NOTE]
>注意 C 库提供的用户空间缓冲区与内核缓冲区 Page Cache 的区别
```
fwrite() / fprintf()
     ↓       ← 用户空间缓冲（行缓冲 / 块缓冲）
fflush()
     ↓       ← write() 系统调用
Page Cache   ← 内核内存缓冲
     ↓       ← bdflush / flush 线程
物理磁盘      ← 最终写入磁盘
```

