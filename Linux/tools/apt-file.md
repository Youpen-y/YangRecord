`apt-file` - APT 包查询工具

语法：
```bash
apt-file [options] search pattern
apt-file [options] show package
```

描述：



使用：
1. 定期更新
```bash
sudo apt-file update
```

2. 查找文件属于哪个包
- 查找确切文件路径
```bash
apt-file search /usr/bin/vim
```

- 查找文件名（模糊匹配）
```
```