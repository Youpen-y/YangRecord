`dmesg` —— 显示从内核环形缓冲区（kernel ring buffer）检索到的内核相关消息。环形缓冲区在系统启动期间存储有关硬件、设备驱动程序初始化和内核模块消息的信息。

> 内核将其日志保存在环形缓冲区中。
> 缘由：可以在 syslog 守护程序启动前，保存系统启动的日志。其内容会在 syslog 守护程序启动时保存到 `/var/log/dmesg` 。

语法
```c
dmesg [options]
```

选项
- `-C, --clear` ，清除环形缓冲区
- `-c, --read-clear` ， 打印内容后清除
- `-f, --facility [list]` ，将输出限制为指定的逗号分隔 设施 `[list]`
- `-H, --human` ，启用人类可读的输出
- `-L, --color[=auto|never|always]` ，为输出添加颜色。默认为 auto 
- `-l, --level [list]` ，将输出限制为指定的逗号分隔级别列表
- `-S, --syslog` ，指示 `dmesg` 使用 `syslog` 内核接口读取内核消息。默认使用 `/dev/kmsg` 而非 `syslog`
- `-w, --follow`，保持 `dmesg` 运行并等待新消息，该功能仅在具有可读 `/dev/kmsg` 文件的系统上可用。

#### 示例
1. 选择时间戳格式（`ctime`, `reltime`, `delta`, `notime`）
```bash
sudo dmesg --time-format=iso
```

2.  `dmesg` 输出限制为特定设施
- `kern` 内核消息
- `user` 用户级消息
- `mail` 邮件系统消息
- `daemon` 有关系统守护程序的消息
- `auth` 授权消息
- `syslog` 内部 `syslogd` 消息
- `lpr` 行式打印机子系统消息
- `news` 网络新闻子系统消息
列出多个设施的消息，以逗号分隔
```bash
sudo dmesg -f syslog,daemon
```

3. 过滤日志 Level
- `emerg` 紧急消息
- `alert` 需要立即采取措施的警报
- `crit` 危机事件
- `err` 错误消息
- `warn` 警告消息
- `notice` 正常但严重的情况
- `info` 通知性消息
- `debug` 调试级消息
```bash
sudo dmesg -l info
```

4. 结合设备和 Level
```bash
sudo dmesg -x
```