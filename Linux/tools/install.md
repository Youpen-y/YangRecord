`install` - 复制文件并设置属性

语法
```bash
install [OPTION]... SOURCE... DIRECTORY
```

描述
`install` 拷贝文件到选定的目的位置。在 GNU/Linux 系统上，如果想要下载和安装一个 ready-to-use 包，应该使用包管理器 `yum` 或 `apt-get` 。


常用选项
- `-c`：（无实际效果，兼容 BSD 的）
- `-D`：创建目标路径中缺失的所有目录
- `-d, --directory`：创建目录，即使它们不存在
- `-m, --mode=MODE`：设置权限模式。如 `-m 755` 为目标文件设置权限
- `-o, --owner=OWNER`：设置文件的所有者
- `-g, --group=GROUP`：设置文件的组
- `-t, --target-directory=DIRECTORY`：将文件安装到指定目录
- `-v`：显示执行过程
- `-s`：对目标文件进行 `strip` （去除调试信息，适用于二进制）

示例：
1. 复制文件并设置权限（将 my_program 复制到 `/usr/local/bin/` 并设置其权限为 755 ）
```bash
install -m 755 my_program /usr/local/bin/

# 类似于
# cp my_program /usr/local/bin/ && chmod 755 /usr/local/bin/my_program
```

2. 创建目录（并设置权限）
```bash
install -d /path/to/new_directory
# install -d -m 755 /usr/local/share/my_app/data
```

3. 安装多个文件
```bash
install -m 644 file1.txt file2.txt /destination_directory/
```

4. 安装并 `strip` 可执行文件（移除调试符号）
```shell
install -s -m 755 build/my_app /usr/bin/my_app
```






