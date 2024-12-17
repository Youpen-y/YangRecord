`pthread_kill` - 向线程发送信号

概要
```c
#include <signal.h>

int pthread_kill(pthread_t thread, int sig);
```

描述
`pthread_kill()` 函数将信号 `sig` 发送给 `thread` ，该线程与调用者位于同一进程中。该信号被异步定向到 `thread` 。
如果 `sig` 为 0 ，则不会发送任何信号，但仍会执行错误检查。

返回值
成功时，返回 0 ；出错时，返回错误号，且不会发送信号。

