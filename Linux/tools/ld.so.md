`ld.so`, `ld-linux.so` - 动态链接器/加载器

动态链接器
- 可通过运行动态链接程序或共享对象（shared object）间接运行，这种情况不能传递命令行参数给动态链接器，在 ELF 情况下，动态链接器存储在被执行程序的 `.interp` 部分。
- 或执行运行 
```shell
/lib/ld-linux/so.* [OPTIONS] [PROGRAM [ARGUMENTS]]
```

描述：
程序 `ld.so` 和 `ld-linux.so*` 查找并加载一个程序所需的共享对象（共享库），准备运行程序，以及运行它。

Linux 二进制文件需要动态链接（运行时链接），除非在编译时为 `ld` 提供了 `-static` 选项。



文件：
- `/lib/ld.so` - `a.out` 动态链接器/加载器
- `/lib/ld-linux/so.{1,2}` - ELF 动态链接器/加载器
- `/etc/ld.so.cache` - 包含要搜索的目录的编译列表的文件，将在这些目录中搜索共享对象和有序列表的候选共享对象。
- `/etc/ld.so.preload` - 文本文件，列出系统启动时需要强制加载的共享库（`.so`）路径，这些库将在所有 ELF 程序启动之前被加载，就像设置了一个系统级的 `LD_PRELOAD` 环境变量一样。如果同时设置了环境变量 `LD_PRELOAD` 和 `/etc/ld.so.preload`，会先加载 `LD_PRELOAD`。
- `lib*.so*` - 共享对象