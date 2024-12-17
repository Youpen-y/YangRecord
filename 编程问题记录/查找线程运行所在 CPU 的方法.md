1. 使用 `/proc/<pid>/task/<tid>/stat`
其中字段含义分别是：

获取 CPU 命令
```bash
cat /proc/<pid>/task/<tid>stat | awk '{print $39}'
```

2. 使用 `sched_getcpu()` 
在 C 程序中使用 `sched_getcpu()` 函数来确定线程正在运行的 CPU 。

如，
```c
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	while(1) {
		int cpu = sched_getcpu();
		printf("Thread is running on CPU: %d\n", cpu);
		sleep(1);
	}
	return 0;
}
```

3. 使用 `htop` 或 `top`
```
1. htop
2. 按 H 键切换线程视图（如果线程尚不可见）
3. 查看每个现成的 CPU% 列。
```

`top` 命令
```
top -H -p <pid>
```

4. 使用 `perf` 
```bash
perf stat -e task-clock -p <tid>
```

5. 使用 `pidstat` 工具
```bash
pidstat -t -p <pid> 1
```
`-t` - 启用线程监视
`-p <pid>` - 指定进程 ID
`1` - 指定工具每秒更新一次，显示 CPU 使用率和运行线程特定的 CPU 。