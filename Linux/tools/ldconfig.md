`ldconfig` - 管理动态链接库。

`ldconfig` 用于根据 `/etc/ld.so.conf` 中存在的 lib 目录创建、更新和删除当前共享库的符号链接，并更新动态链接器的缓存文件。

`ldconfig` 配置文件
- `/etc/ld.so.conf` - 主配置文件，列出了额外需要扫描的库路径。默认路径 `/lib` 和 `/usr/lib` 通常不需要手动配置。
- `/etc/ld.so.conf.d/` - 存放的是 `.conf` 后缀的配置文件，每个文件可以定义额外的库路径，便于分模块管理。

示例
1. 从缓存中显示当前库
```bash
ldconfig -p | head -5
```

2. 显示每个目录中的库（扫描所有目录，并打印目录名称以及在其下创建的所有链接）
```bash
ldconfig -v | head
```