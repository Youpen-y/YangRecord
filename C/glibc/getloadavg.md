`getloadavg` - 获取系统平均负载

```c
#include <stdlib.h>

/* Put the 1 minute, 5 minute and 15 minute load averages into the first NELEM
element elements of LOADAVG. Return the number written (never more than three, but maybe less than NELEM), or -1 if an error occurred. */ 
int getloadavg(double loadavg[], int nelem);
```

描述
`getloadavg()` 函数返回系统运行队列中（system run queue）各个时间段内平均的进程数。最多检索 `nelem` 个样本并将其分配给 `loadavg[]` 的连续元素。系统最多施加 3 个样本，分别代表过去 1 分钟、5 分钟、15 分钟的平均值。

返回值
如果无法获得平均负载，则返回 -1；否则 ，返回实际检索到的样本数量

符合
不在 POSIX.1-2001 中。目前存在于 BSD 和 Solaris 上。