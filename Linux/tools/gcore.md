#core 
`gcore` - 生成正在运行的程序的核心转储文件 （core）

语法
```bash
gcore [-a] [-o prefix] pid1 [pid2 .. pidN]
# [ ] 中的参数为可选参数
```

描述
生成一个或多个正在运行的程序的核心转储（core dump），其进程 ID 为 pid1, pid2 等。
`gcore` 生成的 `core file` 等价于进程崩溃时由内核生成的 `core file` （而且同时，`ulimit -c` 设置适当的核心转储限制）。然而，与崩溃后不同的是，`gcore` 完成其工作后，程序仍保持运行状态，没有任何变化。

选项
`-a`  -  转储所有内存映射。该选项的实际效果取决于操作系统。在 GNU/Linux 上，它将禁用 `"use-coredump-filter"` 并启用 `"dump-exclued-mappings"`。

`-o prefix`  -  可选参数 `prefix` 指定了在组成核心转储的文件名时要使用的前缀。文件名由 `prefix.pid` 构成，其中 `pid` 是 `gcore` 正在分析的 running program 的进程ID。如果未指定 `prefix` ，前缀默认为 `gcore` 。


示例- 进程运行时产生核心转储文件
```c
#include <stdio.h>       // snprintf
#include <stdlib.h>      // system, 
#include <signal.h>      // signal
#include <unistd.h>      // getpid, sleep
#include <sys/types.h>

void generate_core_dump(int signum) {
    char cmd[50];
    pid_t pid = getpid();

    snprintf(cmd, sizeof(cmd), "gcore -o core %d", pid);
    system(cmd);

    printf("Core dump generated. Continuing execution...\n");
}

int main() {
    signal(SIGUSR1, generate_core_dump);

    printf("Process ID: %d\n", getpid());
    printf("Send SIGUSR1 to generate a core dump\n");

    while (1) {
        sleep(1);
        printf("Still running ...\n");
    }
    
    return 0;
}
```
