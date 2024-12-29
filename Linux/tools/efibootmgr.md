`efibootmgr`- 更改 `UEFI` 启动管理器配置

该应用程序可以创建和销毁启动项、更改启动顺序、更改下一个正在运行的启动选项等等。

`Note:` `efibootmgr` 要求内核支持通过 `/sys/firmware/efi/vars` 或 `/sys/firmware/efi/vars` 访问 `EFI` 非易失性变量 `/sys/firmware/efi/efivars/`

#### 查看当前引导项
```bash
sudo efibootmgr
BootCurrent: 0000
Timeout: 2 seconds
BootOrder: 0003,0000,2001,2002,2003
Boot0000* ubuntu	HD(4,GPT,4e3d94b2-8ce5-4c63-af59-94cc0b84f55a,0x18c60000,0x219800)/File(\EFI\ubuntu\shimx64.efi) File(.䍒)
Boot0002* EFI USB Device (WD      My Passport 2626)	UsbWwid(1058,2626,0,57583332413734383656333)/Unit(0)/HD(9,GPT,b7ebb335-e443-4bd3-bca3-e6bd82a13d91,0x5e800800,0x400000)RC
Boot0003* Windows Boot Manager	HD(1,GPT,9743bbca-a610-42d2-86ac-84f71d64ee45,0x800,0x82000)/File(\EFI\Microsoft\Boot\bootmgfw.efi)57494e444f5753000100000088000000780000004200430044004f0042004a004500430054003d007b00390064006500610038003600320063002d0035006300640064002d0034006500370030002d0061006300630031002d006600330032006200330034003400640034003700390035007d00000031000100000010000000040000007fff0400
Boot2001* EFI USB Device	RC
Boot2002* EFI DVD/CDROM	RC
Boot2003* EFI Network	RC
```
#### 修改默认引导项
```bash
sudo efibootmgr -o 0000,0001
```
引导顺序改为先尝试 `0000` ，再尝试 `0001` 。

#### 删除引导项
```bash
sudo efibootmgr -b 0001 -B
```
删除引导项编号为 `0001` 的引导配置（注意，有可能重启后引导项恢复，这时需要手动删除文件（UEFI 分区））
> 手动删除关联的引导文件步骤
> 1. 确定 EFI 分区（`sudo lsblk`， 比如 `/dev/sda1`）
> 2. 挂载 EFI 分区 （`sudo mount /dev/sda1 /mnt`）
> 3. 切换到存储 Ubuntu 启动文件的 EFI 目录。（`cd /mnt/EFI/ubuntu/`）
> 4. 删除损坏的文件（`sudo rm shimx64.efi` 或者移除 `ubuntu`整个目录）
> 5. 卸载分区 （`sudo umount /mnt`）
#### 添加新的引导项
```bash
sudo efibootmgr -c -d /dev/sdX -p Y -L "Ubuntu" -l "\EFI\ubuntu\grubx64.efi"
```
`-d /dev/sdX`：指定磁盘
`-p Y`：指定分区号
`-L "Ubuntu"`：指定引导项名称
`-l "\EFI\ubuntu\grubx64.efi"`：指定引导文件路径

#### 启用引导项
```bash
sudo efibootmgr -b 0001 -a
```
启用 `0001` 的引导项

#### 恢复默认引导项顺序
```bash
sudo efibootmgr -D
```