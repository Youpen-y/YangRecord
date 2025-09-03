`fstab` - 有关文件系统的静态信息

位置：`/etc/fstab`

描述：
文件 `fstab` 包含系统可挂载的文件系统的描述信息。`fstab` 只能由程序读取，而不能写入；正确创建和维护此文件是系统管理员的职责。
`fstab` 中记录的顺序非常重要，因为 `fsck(8)`，`mount(8)`，`umount(8)` 会依次遍历 `fstab` 执行各自的操作。

在基于 `systemd` 的系统上，建议在修改 `fstab` 后使用 `systemctl daemon-reload` 命令，因为许多守护进程或工具使用该文件。

每个文件系统单独占一行描述，字段（fields）之间使用制表符或空格分隔。行在解析之前会被拆分成多个字段，这意味着字段中的任何空格或制表符都必须使用 `\040` 或 `\011` 进行转义，即使在带引号的字符串中也是如此（如 LABEL="foo\040bar"）。

以 `#` 开头的行是注释，空行将被忽略。

示例：
```
LABEL=t-home2   /home       ext4    defaults,auto_da_alloc  0   2
```

六个字段
```
<file system>   <mount point>       <type>  <options>    <dump>    <pass>
```
第一个字段（`fs_spec`）：描述将挂载的 block special device，remote filesystem 或 loop device 的文件系统镜像，或者是将启用的交换文件（swap file）或交换设备（swap device）。
可以用 `LABEL=<label>` 或 `UUID=<uuid>` 来代替设备名称。
也可以用 `PARTUUID=` 和 `PARTLABEL=` ，这些分区标识符支持 GUID 分区表。

第二个字段（`fs_file`）：此字段描述文件系统的挂载点。对于交互区域，此字段应指定为 `node`。

第三个字段（`fs_vfstype`）：此字段描述文件系统的类型。`swap` 表示要用于交换的文件或分区。

第四个字段（`fs_mntops`）：描述与文件系统相关的挂载选项。通常包含挂载类型（`ro` 或 `rw`，默认为 `rw`），以及文件系统适合的任何其他选项。
- `defaults`：使用默认选项。默认值取决于内核和文件系统。
- `noauto`：当指定 `mount -a` 时，不要挂载。
- `user`：允许用户挂载。
- `owner`：允许设备所有者挂载。
- `comment`：供 `fstab-maintaining` 程序或 `x-<name>`使用。
- `nofail`：如果设备不存在，不要报告该设备的错误。

第五个字段（`fs_freq`）：`dump(8)` 使用该字段来确定需要转储的文件系统。如果不存在，则默认为零（不转储）。

第六个字段（`fs_passno`）：此字段由 `fsck(8)` 用来决定启动时文件系统检查的顺序。根文件系统应指定 `fs_passno` 为 1 。其他文件系统应指定 `fs_passno` 为 2 。同一驱动器内的文件系统将按顺序检查，但不同驱动器上的文件系统将同时检查，以利用硬件提供的并行机制。如果不存在，则默认为零（不检查文件系统）。