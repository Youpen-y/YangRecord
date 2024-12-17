`setbuf`, `setbuffer`, `setlinebuf`, `setvbuf` - 流缓冲操作

标准 C 库 （`libc`, `-lc`）

```c
#include <stdio.h>

void setbuf(FILE *stream, char *buf);
void setbuffer(FILE *stream, char *buf, size_t size);
void setlinebuf(FILE *stream);
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
```

描述
三种类型的缓冲可用：
- unbuffered （无缓冲）
- block buffered （块缓冲）
- line buffered （行缓冲）
当输出流无缓冲时（unbuffered），信息一旦写入就会出现在目标文件或终端上；
当其为块缓冲时（block buffered），许多字符被保存并作为块写入；
当其为行缓冲时，许多字符将被保存，直到输出换行符或从连接终端设备的任何流（通常是 `stdin`）读取输入。
函数 `fflush()` 可用于强制块提早输出（force the block out early）。

通常，所有文件都是块缓冲的。

当对文件进行第一次 IO 操作时，`malloc()` 被调用 ，获得一个缓冲区。

如果是涉及终端的流（如 `stdout`），则它是行缓冲的。

默认情况下，标准错误流 `stderr` 是无缓冲的（unbuffered）。

---
```c
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
```
`setvbuf()` 函数可用于任何打开的流来更改其缓冲区。
`mode` 参数可为：
- `_IONBF` （unbuffered）
- `_IOLBF` （line buffered）
- `_IOFBF` （fully buffered）

除了无缓冲的文件， `buf` 参数应指向一个长度至少为 `size` 字节的缓冲区。将使用此缓冲区代替当前缓冲区。如果参数 `buf` 为 NULL，则仅 `mode` 受影响。将在下一次读或写操作时分配新的缓冲区。`setvbuf` 函数只能在打开流之后且在对其执行任何其他操作之前使用。

其他三个调用实际上是 `setvbuf()` 调用的别名。 
`setbuf()` 函数等价于
```c
setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
//setbuf(stream, NULL) == setvbuf(stream, NULL, _IONBF, BUFSIZ), 设置 stream 为无缓冲的
```

`setbuffer()` 相同，只不过缓冲区大小由调用者决定，而不是由默认的 `BUFSIZ` 确定。
`setlinebuf()` 等价于
```c
setvbuf(stream, NULL, _IOLBF, 0);
```

返回值
`setvbuf()` - 成功时返回 0 ；失败时返回非 0 （mode 无效或 请求无法得到满足）并设置 `errno`
其他函数不返回值

