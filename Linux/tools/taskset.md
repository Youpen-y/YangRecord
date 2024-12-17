`taskset` - 设置或检索进程的 CPU 亲和性（CPU affinity）

### 语法
```bash
taskset [options] mask command [argument...]
taskset [options] -p [mask] pid
```

### 描述
`taskset` 命令用于在给定 `pid` 的情况下设置或检索正在运行的进程的 CPU 亲和性，或使用给定的 CPU 亲和性启动新命令。
CPU 亲和性是一种调度程序属性，它将进程“绑定”到系统上给定的一组 CPU 。Linux 调度程序将遵循给定的 CPU 关联性，并且该进程不会在任何其他 CPU 上运行。请注意，Linux 调度程序还支持自然地（natural） CPU 亲和性：出于性能原因，调度程序会尝试将进程保持在同一 CPU 上，只要可行。因此，强制特定的 CPU 关联性仅在某些应用程序中有用。无法设置某些进程（例如内核 per-CPU 线程）的关联性。

CPU亲和性被表示为位掩码（`bitmask`），最低位对应于第一个逻辑CPU，最高位对应于最后一个逻辑CPU。并非所有 CPU 都可能存在于给定系统上，但掩码可能会指定比当前数量更多的 CPU。检索到的掩码将仅反映与系统上物理上的 CPU 相对应的位。
如果给出了无效掩码（即，对应于当前系统上没有有效 CPU 的掩码），则会返回错误。

掩码可以以十六进制指定（带或不带 `0x`），或使用
`--cpu-list` 选项指定为 CPU 列表。
如：
`0x00000001` 处理器 0
`0x00000003` 处理器 0 和 1

`--cpu-list 0-2,6` 处理器 0，1，2，和 6
`--cpu-list 0-10:2` 处理器 0，2，4，6，8，10

当 `taskset` 返回时，可以保证给定的程序已被调度到合法的 CPU 上。

### 权限
用户必须拥有 `CAP_SYS_NICE` 才能更改进程的 CPU 亲和性，而任何用户都可以检索亲和力掩码。

### 示例：
检索现有任务的 CPU 亲和性：
```bash
taskset -p pid
```

设置 `pid` 的 CPU 亲和性
```bash
taskset -p mask pid
```

为 `pid` 指定 `cpu-list`
```bash
taskset -pc cpu-list pid
```

`--cpu-list` 形式仅适用于启动新命令
```bash
taskset --cpu-list cpu-list command
```


### 返回值
只要提供的 PID 存在，`taskset` 就会在其亲和力获取模式下返回 0 。


