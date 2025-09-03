`zip` —— 打包和压缩（归档）文件

语法：
```bash
zip [OPTION] [COMPRESS_NAME.zip] [FILES_OR_DIR]
```

常用选项：

| 选项          | 描述                          |
| ----------- | --------------------------- |
| `-r`        | 递归压缩，包含子目录及其所有内容            |
| `-d`        | 从 `zip` 文件中删除指定文件           |
| `-u`        | 更新 `zip` 文件中已存在的文件，或添加新文件   |
| `-e`        | 加密 `zip` 文件，需要输入密码          |
| `-m`        | 压缩完成后删除原始文件                 |
| `-g`        | 将文件添加到现有的 `zip` 归档中         |
| `-0` 到 `-9` | 设置压缩级别，`0` 表示无压缩，`9` 表示最佳压缩 |
| `-q`        | 安静模式，不显示压缩过程中的信息            |
| `-v`        | 显示详细信息                      |

使用示例：
1. 压缩单个文件
```bash
zip archive.zip file.txt
```

2. 压缩多个文件（同样可用于向已有压缩包中添加新文件）
```bash
zip archive.zip file1.txt file2.txt file3.txt image.png
```

3. 压缩整个目录
```
# 1. 包含目录路径
zip -r archive.zip folder/

# 2. 不包含目录路径
cd folder/
zip -r ../archive.zip .
```

4. 加密压缩
```bash
zip -e archive.zip secretfile.txt
```

5. 排除某些文件
```bash
zip -r archive.zip folder/ -x "*.log"
```

`zip` 命令主要负责压缩，而对应的解压操作通常使用 `unzip` 命令。
- 解压文件到当前目录
```bash
unzip archive.zip
```

- 解压文件到指定目录
```bash
unzip archive.zip -d /path/to/destination/
```

- 查看压缩包内容
```bash
unzip -l archive.zip
```



