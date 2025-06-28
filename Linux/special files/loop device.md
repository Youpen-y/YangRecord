`loop device`（环回设备） 是一种伪设备，使计算机文件可以作为块设备（block device）访问。简单讲，loop device 将一个文件“伪装”成硬盘或分区。

工作原理
当执行 `losetup /dev/loop0 file.img` 时，内核会创建一个从 `/dev/loop0` 到 `file.img` 的映射。之后对 `/dev/loop0` 的所有读写操作都会被转发到对应的文件位置。内核的 loop 驱动负责处理这种地址转换和 I/O 重定向。

基本使用方法
```bash
# 查看可用的 loop 设备
losetup -f

# 将文件关联到 loop 设备
losetup /dev/loop0 disk.img

# 挂载 loop 设备
mount /dev/loop0 /mnt/point

# 卸载并断开链接
umount /mnt/point
losetup -d /dev/loop0
```

现在 Linux 系统通常预配置了多个 loop 设备，可以通过 `/sys/module/loop/parameters/max_loop` 参数调整数量。`loop device`支持只读和读写两种模式，可以设置偏移量来访问文件的特定部分。







