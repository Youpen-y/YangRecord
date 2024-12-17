```
mlock, mlock2, munlock, mlockall, munlockall - lock and unlock memory
```

### 概要
```c
#include <sys/mman.h>

int mlock(const void *addr, size_t len);
int mlock2(const void *addr, size_t len, int flags);
int munlock(const void *addr, size_t len);
```
```c
int mlockall(int flags);
int munlockall(void);
```

### 描述
`mlock()`, `mlock2()`, `mlockall()` 将调用进程的部分或全部虚拟地址空间锁定到 RAM (main memory) 中，防止该内存被换到 swap area 。
`munlock()`, `munlockall()` 执行相反的操作，解锁调用进程的部分或全部虚拟地址空间，以便在内核内存管理器需要时可以再次换出指定虚拟地址范围内的页面。

内存的 lock 和 unlock 以整页为单位进行。

---

`mlock()` 锁定从 `addr` 开始并持续 `len` 字节的地址范围内的 pages 。当调用成功返回时，所有包含范围内一部分的 pages 都保证驻留在 RAM 中，直到稍后解锁。

`mlock2()` 同样锁定从 `addr` 开始并持续 `len` 字节的指定范围的页。但调用成功返回后，该范围内包含的页面的 state 取决于 `flags` 参数中的值。
`flags` ：
-  0 : 与 `mlock()` 完全相同
-  MLOCK_ONFAULT : 锁定当前驻留页面并标记整个范围，以便剩余的非驻留页面在出现页面错误时被锁定。

`munlock()` 解锁从 `addr` 开始并持续 `len` 字节的地址范围内的页面。在此调用之后，包含指定内存范围的一部分的所有页面都可以由内核再次移动到外部 swap space 。

---
`mlockall()` 锁定映射到调用进程的地址空间的所有页面。这包括  `the pages of the code, data, and stack segment, as well as shared libraries, user space kernel data, shared memory, and memory-mapped files`。当调用成功返回时，所有映射的页面都保证驻留在 RAM 中；直到之后解锁。

`flags` 参数被构造为以下一个或多个常量的按位或：
- `MCL_CURRENT`  锁定当前映射到进程地址空间的所有页面。
- `MCL_FUTURE` 锁定将来将映射到进程地址空间的所有页面。如，这些可能是不断增长的堆和堆栈所需的新页面以及新的内存映射文件或共享内存区域。
- `MCL_ONFAULT` 与 `MCL_CURRENT`、`MCL_FUTURE` 或 both 一起使用。标记所有当前（使用 `MCL_CURRENT` ）或将来（使用 `MCL_FUTURE`）映射以在页面出现故障时锁定页面。与 `MCL_CURRENT` 一起使用时，所有当前页面都会被锁定，但 `mlockall()` 不会在非当前页面中出现故障。当与 MCL_FUTURE 一起使用时，所有未来的映射在发生故障时都将被标记为锁定页面，但在创建映射时它们不会被锁填充。 MCL_ONFAULT 必须与MCL_CURRENT或 MCL_FUTURE 或两者一起使用。

`munlockall()` 解锁映射到调用进程地址空间的所有页面。

---
在 Linux 中，`mlock()`, `mlock2()`, `munlock()` 自动将 `addr` 向下舍入到最近的页边界。然而，`mlock()` 和 `munlock()` 的 POSIX.1 规范允许实现要求 `addr` 是页对齐的，因此可移植应用程序应该确保这一点。
Linux-specific `/proc/pid/status` 文件中的 `VmLck` 字段显示 ID 为 PID 的进程已使用 `mlock()`，`mlock2()` ，`mlockall()` 和 `mmap()` MAP_LOCKED 锁定了多少 KB 内存。


---
### Notes
内存锁定（Memory locking）的两个主要应用：
- 实时算法：需要确定性的完成时间。与调度一样，换页是程序执行不确定延迟的主要原因之一。（实时应用程序通常也会使用 `sched_setscheduler` 切换到实时调度程序）
- 高安全性数据处理（加密安全软件）：通常将密码或密匙等关键字节作为数据结构进行处理。而换页将这些密码转移到持久性 swap 存储介质上，在安全软件擦除 RAM 中的密码并终止后，敌人就可以访问它们。（请注意，笔记本电脑和某些台式计算机上的挂起模式会将系统 RAM 的副本保存到磁盘，而不管是否 memory locks）