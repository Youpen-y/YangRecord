返回 rdma cm 异步事件的字符串表示形式

``` c
#include <rdma/rdma_cmd.h>

char *rdma_event_str(enum rdma_cm_event_type event);
```

`event`：异步事件

返回指向与事件对应的静态字符串的指针。