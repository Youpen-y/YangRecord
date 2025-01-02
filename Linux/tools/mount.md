`mount` 用于将文件系统挂载到指定的目录下，从而使用户可以访问该文件系统。可以用于本地设备、网络文件系统等的挂载。

在一台计算机中，可以在很多外存设备上部署多种类型的文件系统，但是整个操作系统只有一个全局命名空间（这个命名空间所属的文件系统被称为根文件系统，Root File System，它在系统启动时被初始化）。当想要访问一个外存设备上的文件系统时，需要先将这个文件系统合并到全局命名空间中——挂载，mount 操作。
当我们将一个文件系统挂载到全局命名空间的一个目录上后，所有对该目录下的访问都会被这个文件系统处理。
》[[外存设备]]

用法
```bash
mount [option] <设备> <挂载点>
```
- `<设备>` ：要挂载的设备路径，例如 `/dev/sda1`
- `<挂载点>` ：设备挂载到的目录，例如 `/mnt`

1. 挂载设备（`/dev/sda1`）到挂载点（`/media/yongy/data2`）
```bash
mount /dev/sda1 /media/yongy/data2
```

2. 指定文件系统类型
```bash
mount -t ext4 /dev/sda1 /mnt
```

常见文件系统类型：
- `ext4`：常用的 Linux 文件系统
- `ntfs`：Windows 文件系统
- `vfat`：FAT文件系统（如 USB 设备）
- `nfs`：网络文件系统
- `iso9660`：光盘镜像文件系统

3. 只读挂载
```bash
mount -o ro /dev/sda1 /mnt
```

4. 挂载网络文件系统
```bash
mount -t nfs 192.168.103.1:/shared /mnt
```
将 NFS 服务器上的目录 `/shared` 挂载到 `/mnt`

5. 挂载 ISO 文件
```bash
mount -o loop file.iso /mnt
```

6. 使用特定选项挂载
```bash
mount -o rw,noexec /dev/sda1 /mnt
```
- `rw`：以读写模式挂载
- `noexec`：禁止执行挂载点上的二进制文件

### 查看
1. 查看当前所有挂载点
```bash
mount
```

2. 查看特定设备或挂载点
```bash
findmnt /mnt
```

3. 查看 `/etc/fstab` 中配置的挂载信息
```bash
cat /etc/fstab
```
通过 `/etc/fstab` 可以实现设备在启动时自动挂载。每行定一个挂载项，格式如下：
```bash
<设备>  <挂载点>  <文件系统类型>  <挂载选项>  <dump>  <fsck>
/dev/sda1  /mnt  ext4  defaults  0  2
```
- `defaults`：使用默认挂载选项（读写、允许执行等）
- `0`：是否需要 dump 备份
- `2`：是否使用 `fsck` 检查文件系统

### 卸载挂载

1. 卸载挂载点
```bash
umount /mnt
```

2. 卸载设备（无需指定挂载点）
```bash
umount /dev/sda1
```

3. 卸载所有挂载点
```bash
umount -a
```


