`rdma_get_devices` - 获得当前可用的 RDMA 设备列表。

```c
struct ibv_context **rdma_get_devices(int *num_devices);
```
参数
- `num_devices` - 如果非空，设置为返回设备的数目。

描述
返回打开的 RDMA 设备的以 NULL-终止的数组。调用者可以使用此例程在特定 RDMA 设备上分配资源，这些资源将在多个 `rdma_cm_id` 之间共享。

返回值
返回可用 RDMA 设备的数组，如果请求失败，则返回 NULL。失败时，将设置 `errno` 以指示失败原因。

注意
返回的数组必须通过调用 `rdma_free_devices` 释放。

```c
void rdma_free_devices(struct ibv_context **list);
```