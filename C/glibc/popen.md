`popen, pclose` - 将流用管道传送至进程或从进程传出

语法：
```c
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);
```

`popen()`：启动一个子进程，执行给定的 `command`，并建立一个管道用于通信。由于管道在定义上是单向的，因此 `type` 参数只能指定读或写，而不能同时指定读或写；结果流相应地是只读或只写的。
参数：
- `command`：是一个以 `'\0'` 结尾的 C 字符串，表示要执行的 Shell 命令行。该命令实际通过 `/bin/sh -c "your command"` 执行。命令的解析、管道、重定向、变量展开等都由 Shell 来处理。
- `type`
	- `"r"`：从子进程的 `stdout` 读取数据
	- `"w"`：向子进程的 `stdin` 写入数据
`pclose()`：等待关联进程终止并返回命令（`command`）的退出状态。

注意： `popen()` 流默认是块缓冲的。

返回值：
`popen()` 成功时返回一个指向打开的流的指针，该流可用于读取或写入管道。如果 `fork(2)` 或 `pipe(2)` 调用失败，或者函数无法分配内存，则返回 NULL 。
`pclose()` 成功时返回命令的退出状态；如果 `wait4(2)` 返回错误，或者检测到其他错误，则返回 -1 。