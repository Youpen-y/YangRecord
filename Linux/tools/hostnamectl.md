`hostnameclt` - 用来查询和改变系统 `hostname` 和相关设置

```bash
$ hostnamectl
 Static hostname: Surge
       Icon name: computer-laptop
         Chassis: laptop 💻
      Machine ID: 3734a41523e74c25975f765035759209
         Boot ID: 0d2294a56bf848dfa076edbfe307cce8
Operating System: Ubuntu 24.04.1 LTS              
          Kernel: Linux 6.8.0-51-generic
    Architecture: x86-64
 Hardware Vendor: Lenovo
  Hardware Model: Yoga Pro 14s ARH7
Firmware Version: JVCN38WW
   Firmware Date: Thu 2023-09-21
    Firmware Age: 1y 3month 1w 1d
```

### 设置主机名
```bash
sudo hostnamectl set-hostname NEW_HOSTNAME
```

### 设置 `pretty` 主机名
```bash
sudo hostnamectl set-hostname "My Pretty Hostname" --pretty
```