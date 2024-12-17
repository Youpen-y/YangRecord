`sem_post` - 解锁信号量

```c
#include <semaphore.h>

int sem_post(sem_t *sem);
```

描述
`sem_post()` 递增（解锁） `sem` 指向的信号量。如果信号量的值因此变得大于零，则在`sem_wait()`调用中阻塞的另一个进程或线程将被唤醒并继续锁定信号量。

返回值
`sem_post()` 成功则返回 0 ；发生错误时，信号量保持不变，返回 -1，并设置 `errno` 来指示错误。

`sem_post()` 是异步信号安全的（async-signal-safe）；可以在信号处理程序中安全地调用它。