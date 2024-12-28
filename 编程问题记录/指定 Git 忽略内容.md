`.gitignore` 文件用于指定 `Git` 应忽略的文件和目录。

常用语法和规则：
#### 通用规则
忽略文件——直接写文件名，忽略项目根目录下的所有 `file.txt` 文件。（包括子目录下的）
```
file.txt
```

忽略目录——目录名后加 `/`
```
build/
```

忽略某种文件类型——使用通配符 `*` （忽略所有 `.log` 文件）
```
*.log
```

#### 通配符
- `*` （匹配任意字符）
```
*.tmp
```

- `?` （匹配单个字符）
```
file?.txt
```
匹配 `file1.txt`，`fileA.txt`，但不匹配 `file10.txt`

- `[abc]` （匹配集合中的任意字符）
```
file[1-3].txt
```

- `**` （匹配任意层级目录）
```
logs/**
```
忽略 `logs` 目录及其所有子目录和文件。

#### 取消忽略
重新包括文件或目录——使用 `!` 指定不忽略的内容
```
*.log
!important.log
```
忽略所有 `.log` 文件，但包含 `important.log`

如果父目录被忽略，则需要先取消忽略父目录
```
build/
!build/dir/
```
忽略 `build` 目录，但包括 `build/dir` 及其内容

#### 特定路径
匹配根目录——使用 `/` 限定只匹配根目录
```
/debug.log
```
只忽略项目根目录下的 `debug.log` 文件

匹配子目录
忽略某子目录的文件
```
docs/*.md
```
忽略 `docs` 目录下的所有 `.md` 文件

#### 注释
以 `#` 开头的行是注释行

#### 实践建议
- 检查忽略规则
```bash
git check-ignore -v <file>
```

- 全局 `.gitignore`，如果某些规则适用于所有项目，可创建全局 `.gitignore` 文件
```bash
git config --global core.excludesfile ~/.gitignore_global
```