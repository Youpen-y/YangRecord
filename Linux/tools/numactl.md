`numactl` - 控制进程或共享内存的 NUMA 策略

### 语法
```bash
numactl [--interleave nodes] [--preferred node] [--membind nodes] [--cpunodebind nodes] [--physcpubind cpus] [--localalloc] [--] command {argument ...}
```
如果使用可能会混淆的命令选项，在命令之前使用 `--`.
- `--interleave=nodes, -i nodes` 在 `nodes` 上循环分配
- `--membind=nodes, -m nodes` 只从 `nodes` 上分配内存
- `--cpunodebind=nodes, -N nodes` 仅在 `nodes` 的 CPU 上执行命令
- `--physcpubind=cpus, -C cpus` 只在 `cpus` 上执行进程。使用 `/proc/cpuinfo` 的处理器字段中显示的 CPU 编号。`all` 代表当前 `cpuset` 中的所有 `cpu` 。物理 `cpu` 可以指定为 `N,N,N` 或 `N-N` 或 `N,N-N`等等。相对 `cpu` 可以指定为 `+N,N,N`，
	- `+`表示 `cpu` 编号相对于当前 `cpuset` 允许的 `cpu` 集。
	- `!` 表示逆，除了指定的之外所有 `cpu`
- `--localalloc, -l` 始终在当前节点上分配
- `--preferred=node`  最好在_节点上分配内存，但如果无法分配内存，则回退到其他节点。此选项仅需要一个节点号。可以使用相对符号。

### 描述
`numactl` 使用一个特定的 NUMA 调度或内存放置策略运行进程。该策略为 `command` 设置，并由其所有孩子继承。此外，它可以为共享内存段或文件设置持久性策略。

默认情况下，进程在第一个 NUMA 节点上运行，内存从系统范围的内存池中分配。`numactl` 提供了在进程级别指定 NUMA 策略的选项，例如进程应该在哪个 NUMA 节点上运行，或者应使用哪个 NUMA 节点来分配其内存。

`numactl` 对于提高具有特定 NUMA 要求的应用程序或由于系统 NUMA 架构而导致性能下降的应用程序的性能非常有用。通过为进程或共享内存指定 NUMA 策略，`numactl` 可以帮助最大限度地减少延迟并提供整体系统性能。

### 示例
1. 在节点 0 上运行 `command` ，并在节点 0 和 1 上分配内存
```bash
numactl --cpunodebind=0 --membind=0,1 -- command comand_arguments
```
2. 在当前 `cpuset` 的 CPU（核）0-4核8-12上运行命令
```bash
numactl --physcpubind=+0-4,8-12 -- command command_arguments
```
3. 在所有 CPU 上运行内存交错的命令
```bash
numactl --interleave=all -- command command_arguments
```

4. 显示当前进程的 NUMA 策略设置
```bash
numactl --show
```

