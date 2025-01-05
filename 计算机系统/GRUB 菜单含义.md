ubuntu 22 GRUB 命令参数
```bash
setparams 'Ubuntu'

    recordfail
    load_video
    gfxmode $linux_gfx_mode
    insmod gzio
    if [ x$grub_platform = xxen]; then insmod xzio; insmod lzopio; fi
    insmod part_msdos
    insmod ext2
    set root='hd0,msdos9'
    if [ x$feature_platform_search_hint = xy ]; then
	search --no-floppy --fs-uuid --set=root --hint-bios=hd0,msdos9 --hint-efi=hd0,msdos9 --hint-baremetal=ahci0,msdos9 cc577e9a-5308-4884-9779-fad0f2c080cf
    else
	search --no-floppy --fs-uuid --set=root cc577e9a-5308-4884-9779-fad0f2c080cf
    fi
    linux 	/vmlinuz-6.8.0-49-generic root=UUID=2b6a1cbd-dc32-42a7-8ea3-d0b643a52a17 no quiet splash $vt_handoff
    initrd 	/initrd.img-6.8.0-49-generic
```

**参数解释：**

- `setparams 'Ubuntu'`: 设置系统名称为 Ubuntu。
- `recordfail`: 记录失败信息，用于调试。
- `load_video`: 加载视频模式，用于显示启动画面。
- `gfxmode $linux_gfx_mode`: 设置图形模式，具体模式由变量 `linux_gfx_mode` 决定。
- `insmod gzio, xzio, lzopio`: 加载压缩模块，用于解压缩内核和 initrd 镜像。
- `insmod part_msdos, ext2`: 加载分区和文件系统模块，用于识别硬盘分区和 ext2 文件系统。
- `set root='hd0,msdos9'`: 设置根设备为第一个硬盘的第 9 个分区。
- `search ...`: 搜索指定 UUID 的分区，并将其设置为根设备。
- `linux /vmlinuz-6.8.0-49-generic ...`: 加载 Linux 内核，并传递给内核一系列参数：
    - `root=UUID=2b6a1cbd-dc32-42a7-8ea3-d0b643a52a17`: 指定根分区 UUID。
    - `no quiet splash`: 不显示启动信息和进度条。
    - `$vt_handoff`: 将控制权交给虚拟终端。
- `initrd /initrd.img-6.8.0-49-generic`: 加载 initrd 镜像，用于初始化系统。

**其他可能出现的参数：**

- `title`: 设置启动菜单项的标题。
- `timeout`: 设置启动菜单的超时时间。
- `default: 设置默认启动项。
- `password`: 设置启动密码。
- `hidden`: 隐藏启动菜单项。

**参数功能：**

这些参数共同作用，告诉 GRUB 如何加载内核和初始化系统。通过配置这些参数，可以定制系统的启动过程，例如选择不同的内核、设置不同的启动选项等。

**更详细的解释：**

- `insmod`: 加载模块，用于支持不同的文件系统、硬件设备等。
- `set`: 设置变量的值。
- `search`: 搜索指定的分区。
- `linux`: 加载 Linux 内核。
- `initrd`: 加载 initrd 镜像。

**需要注意的是：**

- **参数的顺序和格式很重要**，错误的配置会导致系统无法启动。
- **不同的 GRUB 版本和 Linux 发行版可能支持不同的参数**。
- **修改 GRUB 配置文件需要谨慎**，建议备份后再进行修改。

Note：
GRUB 中的模块和 Linux 内核中的模块完全不同，主要区别如下：

|      | GRUB 模块                  | Linux 内核模块                         |
| ---- | ------------------------ | ---------------------------------- |
| 运行环境 | GRUB bootloader 阶段       | Linux 内核空间                         |
| 用途   | 支持文件系统识别、网络启动、磁盘访问等      | 扩展内核功能，如驱动程序、文件系统等                 |
| 加载命令 | `insmod` （GRUB 专用）       | `modprobe` / `insmod`              |
| 模块语言 | GRUB 特定二进制格式（`.mod` 文件）  | Linux 内核模块格式（ELF 格式 `.ko`文件）       |
| 依赖环境 | 独立于 Linux 内核，仅服务于引导过程    | 必须与 Linux 内核兼容                     |
| 存储位置 | `/boot/grub/x86_64-efi/` | `/lib/modules/$(uname -r)/kernel/` |
| 生效范围 | 仅在 GRUB 阶段生效，内核启动后失效     | 内核加载完成后失效，提供系统运行功能                 |

内核中的模块现在都使用 `zstd` 压缩，文件后缀为 `.ko.zst`，`modprobe`工具加载模块时会自动解压加载。