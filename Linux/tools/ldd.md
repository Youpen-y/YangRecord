`ldd` - 打印共享对象依赖，查看 ELF 的依赖项和共享库。
> `ldd` 是一个命令行工具（本质是 shell 脚本），用于列出程序或共享库的共享库依赖关系。通过设置环境变量 `LD_TRACE_LOADED_OBJECTS = 1` 来调用动态链接器（通常是 `ld-linux.so*`）在加载程序时仅输出依赖信息而不运行程序，从而获取程序的动态依赖信息。

`ld.so`, `ld-linux.so*` - Linux 系统的动态链接器/加载器，负责在程序运行时加载所需的共享库，并解析程序的动态依赖关系。
- `ld.so`：用于处理较老的 `a.out` 二进制格式
- `ld-linux.so*`：用于处理现代的 `ELF` 二进制格式。
工作流程：
1. 动态链接器会根据程序的 `.interp` 段找到对应的动态链接器
2. 加载程序所需的共享库，并解析这些库的依赖关系
3. 如果共享库路径中包含 `/` ，则直接加载该路径；否则，会按照以下顺序搜索：
	1. `DT_RPATH` 或 `DT_RUNPATH`：动态段属性
	2. 环境变量 `LD_LIBRARY_PATH` 
	3. `/etc/ld.so.cache` （由 `ldconfig` 生成的缓存）
	4. 默认路径 `/lib` 和 `/usr/lib` （64 位系统为 `/lib64` 和 `/usr/lib64`）

示例：
```bash
$ ldd $(which ls)
        linux-vdso.so.1 (0x00007ffeb036d000)
        libselinux.so.1 => /lib/x86_64-linux-gnu/libselinux.so.1 (0x00007fd8f0e55000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fd8f0c2c000)
        libpcre2-8.so.0 => /lib/x86_64-linux-gnu/libpcre2-8.so.0 (0x00007fd8f0b95000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fd8f0eb7000)
```
从输出可以看到 `ls` 二进制文件使用了 `libselinux`、`libc`、`libpcre` 库。
- 列出的第一个依赖项 `linux-vdso.so.1` 是虚拟动态共享对象，由内核自动映射到所有用户空间应用程序的地址空间中。
> `linux-vdso.so.1` 是Linux内核将内核代码注入进程内存的一种方式，以便某些系统调用运行得更快。
- `libc.so.6` 是 C 标准库
- 列出的最后一个依赖项 `/lib64/ld-linux-x86-64.so.2` 是动态链接器的位置，`ld-linux-x86-64.so.2` 是动态链接器。

使用 `strace` 跟踪 `ls`
```bash
execve("/usr/bin/ls", ["ls"], 0x7ffe733928b0 /* 38 vars */) = 0
brk(NULL)                               = 0x562af680e000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffc27e6c6c0) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7fe8dfcff000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=63719, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 63719, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7fe8dfcef000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libselinux.so.1", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=166280, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 177672, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fe8dfcc3000
mprotect(0x7fe8dfcc9000, 139264, PROT_NONE) = 0
mmap(0x7fe8dfcc9000, 106496, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x6000) = 0x7fe8dfcc9000
mmap(0x7fe8dfce3000, 28672, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x20000) = 0x7fe8dfce3000
mmap(0x7fe8dfceb000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x27000) = 0x7fe8dfceb000
mmap(0x7fe8dfced000, 5640, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fe8dfced000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P\237\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0 \0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0"..., 48, 848) = 48
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\315A\vq\17\17\tLh2\355\331Y1\0m"..., 68, 896) = 68
newfstatat(3, "", {st_mode=S_IFREG|0755, st_size=2220400, ...}, AT_EMPTY_PATH) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2264656, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fe8dfa9a000
mprotect(0x7fe8dfac2000, 2023424, PROT_NONE) = 0
mmap(0x7fe8dfac2000, 1658880, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7fe8dfac2000
mmap(0x7fe8dfc57000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1bd000) = 0x7fe8dfc57000
mmap(0x7fe8dfcb0000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x215000) = 0x7fe8dfcb0000
mmap(0x7fe8dfcb6000, 52816, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7fe8dfcb6000
close(3)
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libpcre2-8.so.0", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=613064, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 615184, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7fe8dfa03000
```
系统会在加载 `libselinux` 之前检查 `/etc/ld.so.preload` 是否存在
> `access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)`

`LD_PRELOAD`
Linux 动态链接器提供一项名为 `LD_PRELOAD` 的重要能力。`LD_PRELOAD` 包含用户指定的 ELF 共享对象列表。它使用户能够在任何其他共享库之前以及程序本身执行之前将这些共享对象加载到进程的地址空间中。该功能有多种用途，包括调试、测试和运行时检测，可以通过写入 `/etc/ld.so.preload` 文件或利用 `LD_PRELOAD` 环境变量来使用。

`LD_PRELOAD` 与 `/etc/ld.so.preload` 之间的区别？
- `/etc/ld.so.preload` 是一个系统范围的配置文件，适用于所有进程并影响整个系统。
- `LD_PRELOAD` 是一个环境变量，允许各个用户根据每个进程指定要为特定可执行文件或命令预加载的库。
- `LD_PRELOAD` 定义的库早于 `/etc/ld.so.preload` 中的库加载。