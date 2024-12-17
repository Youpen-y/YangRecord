`strace` - 用于监视和改变进程与 Linux 内核之间的交互（系统调用、信号传递和进程状态的改变）

`strace` 的操作是通过 `ptrace` 内核功能实现的。每个系统调用的名称，参数，返回值，调用时间通过标准错误输出或者 `-o` 选项写入到文件中。

### 使用

#### `strace -p pid` - 跟踪 `pid` 
```bash
$ strace -p 26380
strace Process 26380 attached 
```

#### `strace -o file -p pid` - 重定向 `strace` 的输出到 `file`

#### `strace -i` - 查看系统调用入口指针
```bash
$ strace -i ls
[00007f82ce4d508b] execve("/usr/bin/ls", ["ls"], 0x7fffdaeee638 /* 27 vars */) = 0
[00007f4a7e006a8b] brk(NULL)            = 0x55d330c97000
```

#### `strace -t` - 跟踪系统调用，同时查看系统调用时间戳（ `-t` 精确到 s ，`-tt` 精确到 `us` ， `-ttt` 打印 `us` 且前导部分打印自纪元以来的秒数）
```bash
$ strace -t ls
19:47:53 execve("/usr/bin/ls", ["ls"], 0x7ffe410253e8 /* 27 vars */) = 0
19:47:53 brk(NULL)                      = 0x559006d0f000

$ strace -tt ls
19:49:29.541775 execve("/usr/bin/ls", ["ls"], 0x7ffe4e371e98 /* 27 vars */) = 0
19:49:29.542259 brk(NULL)               = 0x564408e48000
```

#### `strace -T` - 查看系统调用花费的时间
```bash
$ strace -T ls
execve("/usr/bin/ls", ["ls"], 0x7ffd7498a9d8 /* 27 vars */) = 0 <0.000236>
brk(NULL)                               = 0x562ba5d67000 <0.000044>
```

#### `strace -c` - 统计每个系统调用时间，调用次数，错误次数 （与`-S sortby` 结合可按指定栏排序， `sortby` 合法值：time(默认), calls, name）
```bash
$ strace -c ls
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
 22.51    0.000255          14        18           mmap
 11.39    0.000129          64         2           getdents64
 10.24    0.000116          23         5           read
  9.44    0.000107          15         7           mprotect
  7.94    0.000090          11         8           newfstatat
  7.41    0.000084           9         9           close
  6.71    0.000076          10         7           openat
  4.85    0.000055          13         4           pread64
  4.50    0.000051          25         2           write
  2.65    0.000030          15         2         2 statfs
  2.03    0.000023          11         2           ioctl
  1.85    0.000021          21         1           munmap
  1.68    0.000019           6         3           brk
  1.32    0.000015           7         2         2 access
  1.06    0.000012          12         1           getrandom
  0.88    0.000010           5         2         1 arch_prctl
  0.88    0.000010          10         1           set_tid_address
  0.88    0.000010          10         1           set_robust_list
  0.88    0.000010          10         1           prlimit64
  0.88    0.000010          10         1           rseq
  0.00    0.000000           0         1           execve
------ ----------- ----------- --------- --------- ----------------
100.00    0.001133          14        80         5 total
```

#### `strace -f` - 跟踪当前跟踪的进程由于 `fork()` 系统调用而创建的子进程

`strace -ff` - 如果 `-o filename` 选项有效，则每个进程跟踪都会写入 `filename.pid` ，其中 `pid`  是每个进程的数字进程 ID 。但这与 `-c` 不兼容。

#### `strace -P path` - 仅跟踪访问 path 的系统调用
#### `strace -e expr` 
过滤
`expr` 格式：`[qualifier=][!]value1[,value2]`
`qualifier` 可为：`trace`（默认）, `abbrev`, `verbose`, `raw`, `signal`, `read`, `write` 之一

> 注意： `-eopen` 等价于 `-e trace=open` 仅跟踪 open 系统调用
> `-etrace=!open` 跟踪除 open 之外的每个系统调用

```bash
strace -e trace=<set>    仅跟踪指定的系统调用集，如 -e trace=open,close,read,write
                file     跟踪所有以文件名作为参数的系统调用 （进程在引用哪些文件？）
                process  跟踪所有涉及进程管理的系统调用
                network  跟踪所有与网络相关的系统调用
                signal   跟踪所有与信号相关的系统调用
                ipc      跟踪所有 IPC 相关的系统调用
                desc     跟踪所有与文件描述符（fd）相关的系统调用
                memory   跟踪所有内存映射相关的系统调用
                （推荐使用 %file, %process, %network, ...）
        
        -e trace-fd=set
        -e fd=set        仅跟踪对 fds 的特定子集进行操作的系统调用
        

strace -e abbrev=set     缩写大型结构的打印输出，默认 abbrev=all, -v 等价于 abbrev=none
          signal=set     仅跟踪指定的信号子集。默认为 signal=all。如 signal=!SIGIO

        --status=set     仅打印具有指定返回状态的系统调用 （set: successful, failed, unfinished, unavailable, detached）

