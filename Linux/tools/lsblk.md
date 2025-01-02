`lsblk` 用于列出系统中所有块设备及其详细信息的命令。

#### 列出所有的块设备，默认（名称、主次设备号、类型（0：不可移动，1：可移动）、大小、只读设备（1：只读，0：读写）、类型、挂载点）
```bash
lsblk
NAME 		MAJ:MIN 		RM	SIZE 	RO TYPE MOUNTPOINT
sda           8:0    0   1.8T  0 disk 
├─sda1        8:1    0  11.5G  0 part [SWAP]
├─sda2        8:2    0   256G  0 part 
├─sda3        8:3    0 186.6G  0 part /home
├─sda4        8:4    0     1G  0 part /boot/efi
├─sda5        8:5    0 300.8G  0 part /
```

#### 显示设备详细信息
```bash
lsblk -f
```

#### 查看设备树
```bash
lsblk -t
```

#### 设备详细属性（包括没有分区或未挂载）
```bash
lsblk -a
```

#### 输出特定字段（`-o`）
```bash
lsblk -o NAME,SIZE,UUID,MOUNTPOINT
```