`tkill`，`tgkill` - 向一个线程发送一个信号

头文件
```c
#include <signal.h>   /* SIG* 常数定义 */
#include <sys/syscall.h>  /* SYS_* 常数定义 */
#include <unistd.h>
```

语法
```c
int tkill(int tid, int sig);
int tgkill(pid_t tgid, pid_t tid, int sig);
```

_Note_ : `glibc` 没有为 `tkill()` 提供包装函数，因此需要使用 `syscall()` 。
如果 `tgid` 指定为 -1 ，则 `tgkill()` 等效于 `tkill()` 。
`tkill()` 和 `tgkill()` 是特定于 Linux 的，不应用
于要求移植性的程序中。

描述
`tgkill()` 将信号 `sig` 发送到 `tgid` 线程组中线程 ID 为 `tid`中。（相比之下，可以使用 `kill` 将信号发送给一个进程（i.e. 线程组），这个信号将被传送给该进程中的任意一个线程中）。
`tkill()` 是 `tgkill()` 的废弃的前身。它允许只指定目标线程 ID 。这可能会导致给错误的线程发送信号（该 `tid` 已被回收）。避免使用此系统调用。

返回值
成功时，返回 0 ；失败时，返回 -1 ，并且设置 `errno` 指示错误。


示例：
```c
// thread create
pid_t pid = getpid();
int ret = syscall(SYS_tgkill, pid, thread_tid, CUSTOM_SIG);
if (ret == -1) {
	perror("tgkill");
	return 1;
}
```

参见