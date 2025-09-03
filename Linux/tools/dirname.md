`dirname` - 用于提取给定路径中的目录部分，即去掉文件名，只返回路径中的目录部分。

使用：
```bash
dirname [OPTION] NAME...
```

描述：
输出每个 `NAME` 中的 non-slash （非斜杠）部分，尾随的斜杠也删除。 如果 `NAME` 不包含 `/` ，输出 `.` （即当前目录）。

选项：
- `-z/--zero`：当你使用 `dirname` 处理多个路径时，默认每个输出是以换行符 (`\n`) 结尾的。但如果加上 `-z` 选项，输出的每一行将以 NUL 字符 (`\0`) 结尾，而不是换行符。

示例：
1. 提取文件的目录路径
```bash
dirname /home/user/documents/file.txt
# 输出：/home/user/documents
```

2. 路径末尾有斜杠
```bash
dirname /home/user/documents/
# 输出：/home/user
```

3. 相对路径
```bash
dirname ./folder1/folder2/file.txt
# 输出：./folder1/folder2
```

4. 只有文件名时
```bash
dirname file.txt
# 输出：.
```