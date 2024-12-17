`sem_wait`, `sem_timedwait`, `sem_trywait` - 锁定（lock）信号量

```c
#include <semaphore.h>

int sem_wait(sem_t *sem);
int sem_trywait(sem_t *sem);
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);
```

描述
`sem_wait()` 递减（lock ）`sem` 指向的信号量。如果信号量的值大于零，则可以递减，并且函数立即返回。如果信号量当前的值为零，则调用将阻塞，直到可以执行递减（即信号量上升到零以上），或者信号处理程序中断此次调用。

`sem_trywait()` 与 `sem_wait()` 相同，只不过如果不能立即执行递减，则调用返回错误（`errno` 设置为 EAGAIN）而不是阻塞。

`sem_timedwait()` 与 `sem_wait()` 类似，不同的是通过 `abs_timeout` 指定了如果不能立即执行递减则该调用允许的最大阻塞时长，该结构指定自纪元 1970-01-01 00:00:00 以来的绝对超时（以 s 和 ns 为单位）。
```c
struct timespec {
    time_t tv_sec;    /* seconds */
    long   tv_nsec;   /* Nanoseconds [0 .. 999999999] */
};
```

如果在调用时 `abs_timeout` 已经到期，并且该信号量无法立即被锁定，则 `sem_timedwait()` 将失败并出现超时错误（`errno` 设置为 ETIMEDOUT）。
如果操作可以立即执行，则 `sem_timedwait()` 永远不会因超时错误而失败，无论`abs_timeout` 的值如何。此外，在这种情况下，不检查 `abs_timeout` 的有效性。

返回值
所有这些函数成功时返回 0 ；错误时，返回 -1 ，并保持信号量的值不变，并设置 `errno` 来指示错误。