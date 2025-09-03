`rar` - 归档并压缩文件，创建并操作 `.rar` 存档。

使用示例：
1. 压缩单个文件
```bash
rar a archive.rar file.txt
```
- `a`：表示添加/压缩

2. 压缩整个目录
```bash
rar a archive.rar folder/
```

3. 压缩多个文件
```bash
rar a archive.rar file1.txt file2.png image.jpg
```

4. 创建带密码的 `RAR`文件
```bash
rar a -p archive.rar secretfile.txt
```

`.rar` 格式为专有格式。使用 `unrar` 或 `unrar-free` 解压。
```bash
unrar x archive.rar
```
- `x`：保留原始目录结构

只查看内容，不解压
```bash
unrar l archive.rar
```

解压到指定目录
```bash
unrar x archive.rar /path/to/destination/
```
