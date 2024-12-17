`sched_setaffinity`，`sched_getaffinity` - 设置和获取线程的 CPU 关联掩码

概要
```c
#define _GNU_SOURCE
#include <sched.h>

int sched_setaffinity(pid_t pid, size_t cpusetsize, const cpu_set_t *mask);

int sched_getaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);
```


描述
线程的 CPU 亲和掩码决定了该线程可以在其上运行的 CPU 集。在多处理器系统上，设置 CPU 亲和掩码可用于获得性能优势。如，通过将一个 CPU 专用于特定线程（即，设置该线程的亲和掩码以指定单个 CPU，并将所有其他线程的亲和掩码设置为排除该 CPU），可以确保该线程的最大执行速度。将线程限制为在单个 CPU 上运行还可以避免线程停止在一个 CPU 上执行，然后在另一个 CPU 上重新开始执行时发生的缓存失效导致的性能成本。

CPU 亲和掩码（CPU affinity mask）在 `cpu_set_t` 结构中描述。一组用于操作 CPU 集的宏在 `CPU_SET(3)` 中描述。

`sched_setaffinity()` 将 ID 为 `pid` 的线程的 CPU 亲和掩码设置为 `mask` 指定的值。如果 `pid` 为零，则操作调用线程。参数 `cpusetsize` 是 `cpu_set_t` 结构的长度，即 `sizeof(cpu_set_t)`。

`sched_getaffinity()` 将 ID 为 `pid` 的线程的 CPU 亲和掩码写入 `mask` 指向的 `cpu_set_t` 结构体。`cpusetsize` 同上。如果 `pid` 为零，则返回调用线程的掩码。

返回值
成功时，`sched_setaffinity()` 和 `sched_getaffinity()` 返回 0 。失败时，返回 -1，并设置 `errno` 以指示错误。
成功时，`sched_setaffinity()` 和 `sched_getaffinity()` 返回 0 。失败时，返回 -1，并设置 `errno` 以指示错误。