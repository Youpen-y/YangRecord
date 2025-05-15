`chown` —— Change Owner，更改文件或目录的所有者（owner）和所属组（group）

基本语法：
```bash
chown [OPTION] owner[:group] file...
```

常用选项：
- `-R, --recursive` 递归地更改指定目录及其所有子文件和子目录
- `-v, --verbose` 显示详细操作信息
- `-h, --no-dereference` 更改符号链接本身而不是其指向的文件

使用示例：
查看文件的所有者
```bash
drwxrwxr-x 3 yongy yongy      4096 May 15 10:48 .
drwxrwxr-x 5 yongy yongy      4096 May 15 10:51 ..
```

1. 更改文件所有者
```bash
# 将 file 的所有者更改为 user1
sudo chown user1 file
```

2. 同时更改所有者和所属组
```bash
sudo chown user1:group1 file
```

3. 只更改所属组
```bash
sudo chown :group1 file
```

4. 递归更改目录及所有内容
```bash
# 递归地将 documents 目录及其所有内容的所有者更改为 user1
sudo chown -R user1 documents/
```

5. 更改符号链接本身
```bash
# 更改符号链接而非其他指向的文件
sudo chown -h user1 symlink_file
```




