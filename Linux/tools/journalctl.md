`journalctl` - 打印 `systemd` 日志中的日志条目

语法
```
journalctl [OPTIONS...] [MATCHES...]
```

描述
`journalctl` 用于打印 `systemd-journald.service` 和 `systemd-journal-remote.service` 存储在日志中的日志条目。

示例
- 查看特定服务的日志
```
journalctl -u nginx.service
```

- 查看最近 100 条日志
```
journalctl -n 100
```

- 实时查看日志
```
journalctl -f
```

- 查看特定时间的日志
```
journalctl --since "2023-01-01 12:00:00"
```

- 查看特定时间之后的日志
```
journalctl --since "1 hour ago"
```

- 查看特定时间之前的日志
```
journal --until "2023-01-01 13:00:00"
```

- 查看特定优先级（等级）的日志
```
journal -p err
```

- 查看内核日志
```
journalctl -k
```