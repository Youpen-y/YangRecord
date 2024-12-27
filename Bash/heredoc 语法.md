```bash
cat << EOF
这里可以写多行
文本
EOF
```
NOTE
- `EOF` 可以换成任何词，如 `END`, `STOP`
- 如果 `EOF` 加引号，即，`cat << 'EOF' ... EOF` 其中的变量不会展开。
常见用法：
- 生成配置文件
- 创建脚本
- 多行注释
- 直接执行命令
示例
```bash
# 创建一个配置文件
cat << EOF > config.txt
host: localhost
port: 8080
user: admin
password: 123456
EOF

# 创建一个脚本
cat << 'EOF' > script.sh
#!/bin/bash
echo "Hello World"
date
pwd
EOF
```