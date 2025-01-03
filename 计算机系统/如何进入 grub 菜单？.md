`GRUB` 启动菜单在系统启动时按特定的键触发。
- 系统使用 BIOS 引导——在启动时按 `Shift` 键来显示 GRUB 启动菜单
- 系统使用 UEFI 引导——在启动时按 `Esc` 键
[[如何知道系统引导方式？]]
进入 GRUB 菜单后操作
选择一、使用箭头键选择选择要启动的操作系统或内核版本，然后 `Enter` 启动
选择二、编辑启动选项（如 `Ubuntu Linux 5.15.0-43-generic）
- 选择一个启动项，按 `e` 进入编辑模式
- 在编辑模式下，可以修改内核启动参数。找到 `linux` 或 `linux16` 开头的内核启动行，在该行末尾添加 `init=/bin/sh` （临时设置）
- 然后按 `Ctrl+x` 或 `F10` 启动
选择三、进入 GRUB 命令行
- 在 GRUB 启动菜单中，按 `c` 键进入 GRUB 命令行界面，这里可以手动输入命令来启动操作系统、修复启动问题等。

如果希望每次启动时都使用新的启动选项，可以修改 GRUB 配置文件 `/etc/default/grub`。
- 修改 `GRUB_CMDLINE_LINUX` 来永久添加启动参数。 `GRUB_CMDLINE_LINUX="quiet splash"`
- 更新 GRUB 配置。`sudo update-grub`
- 重启系统。`sudo reboot`

---
当在 GRUB 中使用 `init=/bin/bash` 启动一个 Bash shell 时，系统会跳过常规的 `init` 进程，直接启动一个 shell 。该过程强制用 Bash 来替代传统的初始化进程 `init` ，而 `init` 是 Linux 系统启动过程中的关键进程，负责管理系统的各种服务和进程。
在这种情况下，使用 `exit` 命令将试图退出当前的 Bash shell 。这将导致 `try to kill init`