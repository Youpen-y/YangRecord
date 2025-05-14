`ltrace` - 跟踪程序运行时库函数调用（library call）的命令行工具。显示程序执行过程中调用的动态库函数，包括函数名称、参数和返回值。

基本用法：
```bash
ltrace <command>
```

将输出重定向到文件
```bash
ltrace -o output.txt <command>
```

仅跟踪特定的库函数调用
```bash
ltrace -e <library_function> <command>

# 统计 malloc 和 free 函数的调用次数和执行时间
# ltrace -e malloc,free ls
```

跟踪已经运行的进程
```bash
ltrace -p <pid>
```

打印每个库函数调用的执行时间戳
```bash
ltrace -tt <command>
```

打印每个调用函数的时间开销
```bash
ltrace -T <command>
```

统计库函数调用
```bash
ltrace -c <command>
```

---
应用场景：
- 调试进程。当程序无法正常工作时，使用 `ltrace` 跟踪库函数调用，找出问题所在。
- 分析性能瓶颈。使用 `ltrace` 分析程序的性能瓶颈，找出库函数调用的执行时间。
- 监控内存分配。`ltrace -e malloc,free ls`
- 监控动态链接库加载。