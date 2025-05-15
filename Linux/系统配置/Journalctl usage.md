`journalctl` - 与日志进行交互的命令行工具
##### 实时显示日志
```bash
journalctl -f
```

###### 仅显示内核消息
```bash
journalctl -k
```

过滤特定时间间隔的日志
```bash
journalctl --since=yesterday --until=now
```

指定日期或日期时间组合
```bash
journalctl --since "2020-07-10"
```

###### 查看最后几个日志
```bash
journalctl -xe
```
`-x`：显示日志条目的额外信息
`-e`：跳到日志的末尾