`sysbench` - 跨平台多线程基准测试工具，广泛用于评估系统性能。可以测试 CPU、内存、文件 I/O 和数据库性能。

安装
```bash
sudo apt install sysbench
```

主要测试类型
1. CPU 性能测试——测试 CPU 计算素数的能力
```bash
sysbench cpu --cpu-max-prime=20000 run

# 指定线程数和时间
sysbench cpu --cpu-max-prime=20000 --threads=4 --time=60 run
```
>[!NOTE]
>CPU 计算素数是典型的计算密集型任务，需要 CPU 进行大量的整数运算、逻辑判断和循环操作，对 CPU 的整数运算能力、分支预测和缓存效率有较高的要求。
>不足：评估维度局限于 CPU 的整数运算性能。
>CPU 性能除了整数运算，还包括：浮点运算能力（FLOPS）、内存访问性能、多线程/并行处理能力、特定指令集、I/O 性能等。专业 CPU 基准测试工具（如 SPEC CPU、Cinebench、Geekbench等）可以提供更全面的评估。

2. 内存性能测试 —— 测试内存的读写速度
```bash
# 内存顺序访问测试
sysbench memory --memory-total-size=10G run

# 内存随机访问测试
sysbench memory --memory-total-size=10G --memory-access-mode=rnd run

# 指定读写操作类型
sysbench memory --memory-total-size=10G --memory-oper=write --threads=4 run
```

3. 文件 I/O 性能测试 —— 测试磁盘读写性能
```bash
# 准备测试文件
sysbench fileio --file-total-size=5G prepare

# 随机读写测试
sysbench fileio --file-total-size=5G --file-test-mode=rndrw --max-time=300 --max-requests=0 run

# 顺序写测试
sysbench fileio --file-total-size=5G --file-test-mode=seqwr --max-time=300 run

# 清理测试文件
sysbench fileio --file-total-size=5G cleanupash

```

4. 数据库性能测试 —— 以 MySQL 为例
```bash
# 准备测试数据
sysbench oltp_read_write --table-size=100000 --mysql-db=testdb --mysql-user=user --mysql-password=password prepare

# 运行读写混合测试
sysbench oltp_read_write --table-size=100000 --mysql-db=testdb --mysql-user=user --mysql-password=password --time=300 --threads=4 run

# 清理测试数据
sysbench oltp_read_write --table-size=100000 --mysql-db=testdb --mysql-user=user --mysql-password=password cleanup
```


通用参数：
- `--threads=N`: 指定并发线程数
- `--time=N`: 测试持续时间（秒）
- `--events=N`: 执行的事件总数
- `--rate=N`: 平均每秒执行的事件数

CPU 测试参数：
- `--cpu-max-prime=N`: 计算素数的最大值

内存测试参数：
- `--memory-total-size=SIZE`: 传输的数据总量
- `--memory-access-mode=MODE`: 访问模式（seq/rnd）
- `--memory-oper=TYPE`: 操作类型（read/write/none）

文件 I/O 测试参数：
- `--file-total-size=SIZE`: 测试文件总大小
- `--file-test-mode=MODE`: 测试模式（seqwr/seqrd/rndwr/rndrd/rndrw）
- `--file-num=N`: 测试文件数量
- `--file-block-size=SIZE`: 文件块大小

性能测试脚本：
```bash
#!/bin/bash
echo "=== CPU 性能测试 ==="
sysbench cpu --cpu-max-prime=20000 --threads=4 --time=60 run

echo "=== 内存性能测试 ==="
sysbench memory --memory-total-size=10G --threads=4 run

echo "=== 磁盘 I/O 性能测试 ==="
sysbench fileio --file-total-size=5G prepare
sysbench fileio --file-total-size=5G --file-test-mode=rndrw --threads=4 --time=300 run
sysbench fileio --file-total-size=5G cleanup
```

