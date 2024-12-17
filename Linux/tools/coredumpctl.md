`coredumpctl` - 检索和处理已保存的 core dumps 和 元数据

语法
```
coredumpctl [OPTIONS...] {COMMAND} [PID|COMM|EXE|MATCH...]
```

描述
`coredumpctl` 是一个工具，可以用来检索和处理 `systemd-coredump` 保存的 core dumps 和 metadata

使用示例
- 列出所有 core dumps
```
coredumpctl list
```

- 查看特定 PID 的 core dump 信息
```
coredumpctl info PID
```

- 使用 `gdb` 调试 core dump
```
coredumpctl gdb PID
```

- 导出 core dump 文件
```
coredumpctl dump PID > core.dump
```

