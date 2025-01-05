`install` - 复制文件并设置属性

语法
```bash
install [OPTION]... SOURCE... DIRECTORY
```

常用选项
- `-d, --directory`：创建目录，即使它们不存在
- `-m, --mode=MODE`：设置权限模式。如 `-m 755` 为目标文件设置权限
- `-o, --owner=OWNER`：设置文件的所有者
- `-g, --group=GROUP`：设置文件的组
- `-t, --target-directory=DIRECTORY`：将文件安装到指定目录

示例：
1. 复制文件并设置权限（将 my_program 复制到 `/usr/local/bin/` 并设置其权限为 755 ）
```bash
install -m 755 my_program /usr/local/bin/
```

2. 创建目录
```bash
install -d /path/to/new_directory
```

3. 安装多个文件
```bash
install -m 644 file1.txt file2.txt /destination_directory/
```






