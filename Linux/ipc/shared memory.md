#ipc
## System V 共享内存
### `shmget` - 分配一个 System V 共享内存段

```c
#include <sys/shm.h>

int shmget(key_t key,   /* the key for the segment */
           size_t size, /* the size of the segment */
           int shmflg); /* create/use flag */
```

描述
`shmget()` 返回与参数 `key` 对应的 System V 共享内存段的标识符。要么是之前创建过的共享内存段的标识符（当 `shmflg` 是 0 ，且 `key` 没有值 `IPC_PRIVATE` ），或者创建一个新的。

当 `key` 包含 `IPC_PRIVATE` 或 `key` 不含 `IPC_PRIVATE`，但`shmflg`包含 `IPC_CREAT` 且 `key` 对应的段不存在时创建一个新的共享内存段。


参数
- `key` : `key_t` 或 `IPC_PRIVATE` （其中 `IPC_PRIVATE` 宏定义为 `#define IPC_PRIVATE  ((__key_t) 0)`），分配给返回的共享内存段的 numeric key 。
- `size` : 请求的共享内存的大小。
- `flag` : 指定共享内存将被使用的方式。
    - IPC_CREAT - 创建一个新的共享内存段。如果未使用该标志，`shmget()` 将找到与 `key` 对应的段，并检查用户是否有权限访问该段
    - IPC_EXCL - 此标志与 IPC_CREAT 一起使用，以确保此函数调用创建区段。如果该段已经存在，则调用失败
    - `SHM_HUGETLB` - 使用 huge 页面分配区段。
    - `SHM_HUGE_2MB` / `SHM_HUGE_1GB(since Linux 3.8)` - 与 `SHM_HUGETLB` 结合使用以选择替代 `hugetlb` 页面的大小（分别为 2 MB 和 1 GB）在支持多个 `hugetlb` 页面大小的系统上。
    - `SHM_NORESERVE` - 此标志的作用与 `mmap` 中的 `MAP_NORESERVE` 标志相同。不要为此段预留交换空间。当交换空间被保留时，用户是可能修改该段的。当未预留 swap space 时，如果写该段时没有物理内存可用，则可能会收到一个 SIGSEGV 信号。
    - `shmflg` 最低 9 位指定授予所有者、组和 others 的权限。目前，系统不使用执行权限。
        ![[文件访问权限.png]]

返回值
成功时返回有效的共享内存标识符；失败时返回 -1 并设置 `errno` 以指示错误。

示例：
```c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int shm_id;     /* shared memory ID */

shm_id = shmget(IPC_PRIVATE, 4*sizeof(int), IPC_CREAT | 0666);
if (shm_id < 0) {
    printf("shmget error\n");
    exit(1);
}

/* now the shared memory ID is store in shm_id */
```
---
#### `shmat` - 将 `shmid` 标识的共享内存段附加到调用进程的地址空间
```c
void *shmat(int shmid, const void *_Nullable shmaddr, int shmflg);
```
附加地址由 `shmaddr` 指定：
- `shmaddr` 为空，则系统选择合适的（未使用的）页对齐的地址以附加段。
- `shmaddr` 不为空，且 SHM_RND 在 `shmflg` 中指定，则在 `shmaddr`  向下舍入到最接近 SHMLBA 倍数的位置上附加。
- 否则，附加发生的`shmadrr` 必须是页面对齐的地址

参数 `shmflg` 除 SHM_RND 外，还可以有如下位掩码参数：
- `SHM_EXEC` - 允许执行该段的内容。调用方必须对分段具有执行权限。
- `SHM_RDONLY` - 附加分段以进行只读访问。该过程必须具有区段的读取权限。如果此标志未被指定，则附加该段以进行读取和写入访问权限，并且进程必须具有 read 和 write 权限。没有 write-only 共享内存段。
- `SHM_REMAP` - 特定于 Linux，此标志指定 segment 的 

 #### `shmdt` - 分离共享内存段
```c
 int shmdt(const void *shmaddr);
```

---
#### `shmctl` - System V 共享内存控制
```c
int shmctl(int shmid, int op, struct shmid_ds *buf);
```
`shmctl()` 对 `shmid` 标识的共享内存段执行 `op` 操作。
`op`:
- `SHM_LOCK` - 锁定内存中指定的共享内存段。进程必须具有超级用户的有效 ID 才能执行此命令
- `SHM_UNLOCK` - 解锁共享内存段。该进程必须具有超级用户的有效 ID 才能执行此命令
- `IPC_STAT` - 从 `shmid` 关联的内核数据结构中复制信息，并将其放入 `buf` 指向的缓冲区中。该进程必须具有该段的读取权限才能执行此命令
- `IPC_SET` - 设置有效的用户和组 id 和 访问权限
- `IPC_RMID` - 删除共享内存段

`buf`参数是指向 `shmid_ds` 结构的指针：
```c
struct shmid_ds {
    struct ipc_perm shm_perm;    /* Ownership and permissions */
    size_t shm_segsz;   /* Size of segment (bytes) */
    time_t shm_atime;   /* Last attach time */
    time_t shm_dtime;   /* Last detach time */
    time_t shm_ctime;   /* Creation time/time of last modification via shmctl() */
    pid_t  shm_cpid;    /* PID of creator */
    pid_t  shm_lpid;    /* PID of last shmat(2)/shmdt(2) */
    shmatt_t shm_nattch;  /* No. of current attaches */
    ...
};

struct ipc_perm {
key_t __key;    /* Key supplied to shmget(2) */
uid_t uid;      /* Effective UID of owner */
gid_t gid;      /* Effective GID of owner */
uid_t cuid;     /* Effective UID of creator */
gid_t cgid;     /* Effective GID of creator */
unsigned short mode;     /* **Permissions** + SHM_DEST and SHM_LOCKED flags */
unsigned short __seq;    /* Sequence number */
};
```


## POSIX 共享内存
`shm_open()`
`shm_unlink()`