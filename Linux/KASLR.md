`ASLR` （Address Space Layout Randomization），启用了`ASLR` 功能的程序，代码和数据将在随机位置加载。这是一种用来防止通过代码地址和数据地址进行攻击的方法。如果没有`ASLR`，程序的代码和数据在执行时将加载到固定地址（除了动态共享库，动态共享库将加载到任意地址）。
`ASLR` 不是由硬件实现的，而是由软件（Linux kernel）实现的。当 `ASLR-enabled` 的程序加载到虚拟内存地址空间时，Linux 会随机定位其代码/数据段。

Position Independent Code 通常成为 PIC，所有代码段/数据段都与位置无关的程序称为 PIE （Position-independent executable）。可以通过`file`查看程序是否为`pie`，可以通过查看`cat /proc/pid/maps` 查看程序内存布局。如
``` shell
$ file /bin/sleep
/bin/sleep: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=7ca63edbda68518a8b021ac3a4e8b6e0be94c915, for GNU/Linux 4.4.0, stripped
```
通过 `gcc` 启用 `-fpic` 选项构建PIE，使用此选项构建的程序，所有数据/代码都位于随机地址。
>`-fPIE` 与 `-fPIC` 的区别
>`-fPIE` 告诉 gcc 生成位置无关的可执行文件。意味着生成的可执行文件可在内存中任何位置运行，而不需要重定位。
>`-fPIE` 通常用于生成动态链接的可执行文件，它确保可执行文件在加载时可以被加载到随机的内存地址，从而增强了安全性
>在大多数现代系统中，`-fPIE` 是默认选项。
>---
>`-fPIC` 告诉 gcc 生成位置无关的代码，这些代码可用于创建共享库（动态库）
> 与 `-fPIE` 相比，`-fPIC` 生成的代码可能会更大，因为它需要包含更多的重定位信息，以确保代码可以在任何位置运行。
> `-fPIC` 是创建共享库时的推荐选项，因为共享库被设计为在多个程序之间共享，并且可能被映射到不同的地址空间。在创建需要在不同程序中共享的库时，`-fPIC` 是更安全的选择。

PIE 在过去不被使用，是由于 pie 程序相比于位置相关的程序更慢，因为需要额外的工作来处理内存。但最近的Linux发行版倾向于使其所有或大部分二进制文件可重定位。原因是安全的健壮性比性能的细微改进更重要。

Linux 默认启用 `ASLR` 。要禁用 `ASLR` ，设置 sysctl 参数 `kernel.randomize_va_space` 为 0 ，该参数为默认配置为 2 （默认启用）。

`KASLR`（Kernel Address Space Layout Randomization，内核地址空间布局随机化）。
启用`KASLR`时，内核被加载到内存的随机位置，防止依赖于内核地址的攻击。`KASLR` 的效果可以通过内核中符号的地址来确认。所有 symbols 和它们的地址都写入 `System.map` 文件。在 `Ubuntu` 上，位于 `/boot/System.map-<kernel version>`。 此外，运行时符号信息可以通过 `/proc/kallsyms` 获取。因此，如果 `/proc/kallsyms` 中某个符号的地址与 `System.map` 中的地址不同，则系统运行的内核启用了 `KASLR` 。

如果你想显式地禁用 `KASLR`，你应该设置 `nokaslr` 内核引导参数。