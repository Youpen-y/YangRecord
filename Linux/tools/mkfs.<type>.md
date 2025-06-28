`mkfs.<type>` —— Linux 中用于格式化设备为特定文件系统的命令工具。
`type`：常见文件系统类型（`ext4`, `xfs`, `btrfs`, `vfat`, `ntfs`等）

语法：
```bash
mkfs.<type> [options] <device_or_file>
```
其中 `<type>` 是文件系统类型，`<device_or_file>` 是