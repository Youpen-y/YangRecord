`ibv_alloc_dm`, `ibv_free_dm`, `ibv_memcpy_to/from_dm` - 分配或释放设备内存缓冲区（DM）并对其执行内存复制。

```c
struct ibv_dm *ibv_alloc_dm(struct ibv_context *context, struct ibv_alloc_dm_attr *attr);

int ibv_free_dm(struct ibv_dm *dm);

int ibv_memcpy_to_dm(struct ibv_dm *dm, uint64_t dm_offset, void *host_addr, size_t length);

int ibv_memcpy_from_dm(void *host_addr, struct ibv_dm *dm, uint64_t dm_offset, size_t length);
```