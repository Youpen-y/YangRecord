`prctl` - 对进程或线程的操作

```c
#include <linux/prctl.h>    /* PR_* 常量定义 */
#include <sys/prctl.h>

int prctl(int op, ...)
```

描述
`prctl()` 操纵调用（calling）线程或进程的行为的各个方面。
`prctl()` 的第一个参数描述要做什么，以及其他参数的重要性取决于第一个参数。
`op`:

返回值
成功时返回非负值；出错时返回 -1 ，并设置 `errno` 来指示错误。

如 `EINVAL` ，系统无法识别或不支持 op 的值。

EXAMPLE
```c
#include <sys/prctl.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    // 设置进程名称
    prctl(PR_SET_NAME, (unsigned long)"my_process_name", 0, 0, 0);
    
    // 获取进程名称
    char name[32];
    prctl(PR_GET_NAME, (unsigned long)name, 0, 0, 0);
    printf("Process name: %s\n", name);

    // 设置进程在父进程退出后接收的信号
    // （在子进程中调用，下行代码将将确保当前进程接收 SIGTERM 信号）
    // prctl(PR_SET_PDEATHSIG, (unsigned long)SIGTREM, 0, 0, 0)

    // 设置进程的 Dumpable 标志（允许生成 core 文件）
    prctl(PR_SET_DUMPABLE, (unsigned long)1, 0, 0, 0);

    // 获取当前的定时模式 (PR_TIMING_STATISTICAL / PR_TIMING_TIMESTAMP)
    int timing_mode = prctl(PR_GET_TIMING, 0, 0, 0, 0);

    
}

void * thread_example(void *arg) {
    // 为线程命名
    prctl(PR_SET_NAME, (unsigned long)"thread_name"));
    // ...
    
    // 设置进程 Ambient Capability （将 CAP_SYS_PTRACE 能力添加到当前线程的 Ambient 能力集合中）
    prctl(PR_CAP_AMBIENT, PR_CAP_AMBIENT_RAISE, CAP_SYS_PTRACE, 0, 0);
    return NULL;
}


```