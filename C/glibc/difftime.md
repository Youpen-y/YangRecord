`difftime()` - 计算时间差

语法
```c
#include <time.h>
double difftime(time_t time1, time_t time2);
```

描述
`difftime()` 函数返回从 `time1` 到 `time0` 经过的时间秒数，用 `double` 类型表示。每个时间都在日历时间中指定，这意味着其值是相对于Epoch {1970-01-01 00:00:00 +0000(UTC)}（以秒为单位）

笔记
在 POSIX 系统上，`time_t` 是算术类型，当减法中不会溢出时可以定义
```c
#define difftime(t1,t0) (double)(t1 - t0)
```

