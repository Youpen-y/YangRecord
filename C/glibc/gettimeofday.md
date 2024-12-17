`gettimeofday` -  获取日期和时间

```c
#include <sys/time.h>

int gettimeofday(struct timeval *restrict tp, void *restrict tzp);
int settimeofday(const struct timeval *tp, const struct timezone *tzp);
```

描述
`gettimeofday()` 函数应获取当前时间，以自 Epoch （纪元）以来的秒和微妙表示，并将其存储在 `tp` 指向的 `struct timeval` 中。系统时钟的分辨率未指定。
如果 `tzp` 不是 NULL ，则行为未指定。

返回值
`gettimeofday()` 函数应返回 0 ，并且不保留任何值来指示错误。

```c
struct timeval {
    time_t tv_sec;      /* seconds */
    long tv_usec;     /* microseconds */
};
```

```c
long long current_timestamp(){
    long long milliseconds;
    struct timeval te;
    
    gettimeofday(&te, NULL); // get current time
    milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // caculate milliseconds
    return milliseconds;
}
```