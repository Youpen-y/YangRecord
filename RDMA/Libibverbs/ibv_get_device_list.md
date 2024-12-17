`ibv_get_device_list` - 获取和释放可用的 RDMA 设备列表

```c
#include <infiniband/verbs.h>

struct ibv_device **ibv_get_device_list(int *num_devices);
void ibv_free_device_list(struct ibv_device **list);
```

描述
`ibv_get_device_list()` 返回当前可用的 RDMA 设备数组（NULL-terminated）。参数 `num_devices` 是可选的；如果不为 NULL，则将其设置为数组中返回的设备数。

`ibv_free_device_list()` 释放 `ibv_get_device_list()` 返回的设备列表数组。

返回值
`ibv_get_device_list()` 返回可用 RDMA 设备的数组，或者在请求失败时返回 NULL 并设置 `errno` 。如果没有找到设备，则 `num_devices` 设置为 0 ， 并返回非 NULL 。

`ibv_free_device_list()` 不返回任何值。

错误
`EPERM` - 没有权限
`ENOSYS` - 内核不支持 RDMA
`ENOMEM` - 内存不足，无法完成操作

NOTES
客户端代码应在调用 `ibv_free_device_list()` 之前使用 `ibv_open_device()` 打开其打算使用的所有设备。一旦使用 `ibv_free_device_list()` 释放数组，它将只能使用打开的设备；指向未打开设备的指针将不再有效。

如果发现 kernel verbs device ，但找不到相应的用户空间驱动程序，则设置环境变量 `IBV_SHOW_WARNINGS` 将导致向 `stderr` 发出警告。

