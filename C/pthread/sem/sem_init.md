`sem_init` - 初始化一个未命名的信号量

```c
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value);
```

描述
`sem_init()` 初始化 `sem` 指向的信号量，`value` 参数用于指定信号量的初始值。

`pshared` 参数指示
- `pshared=0` -  该信号量是在进程的线程们之间共享，此时信号量位于所有线程可见的某个地址，如全局变量或堆上动态分配的变量；
- `pshared非零` -  在进程之间共享，并且应该位于共享内存区域中（参阅 `shm_open`，`mmap`，`shmget`）。由于 `fork()` 创建的子进程继承其父进程的内存映射，因此它也可以访问信号量。任何可以访问共享内存区域的进程都可以使用 `sem_post()` 和 `sem_wait()` 等对信号量进行操作。

初始化已初始化的信号量会导致未定义的行为。

返回值
成功时返回 0 ；错误时返回 -1 并设置 `errno` 。

错误
`EINVAL` - value 超过了 `SEM_VALUE_MAX`
`ENOSYS` - `pshared` 非零，但系统不支持进程共享信号量。