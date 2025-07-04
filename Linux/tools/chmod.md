`chmod` —— 改变文件模式位

描述
```bash
chmod [OPTION]... MODE[,MODE]... FILE...
chmod [OPTION]... OCTAL-MODE FILE...
chmod [OPTION]... --reference=RFILE FILE...
```

`chmod` 根据给定的 `mode` 改变文件的模式位。
两种模式形式：
- 符号形式：格式 `[ugoa...][[-+=][perms...]...]` ，`perms` 要么是 来自集合 `rwxXst` 中的 0 个或多个字母，或者来自集合 `ugo` 的单个字母。可以使用逗号分隔多个符号形式的模式。
	- `ugoa` 字母的组合控制那些用户对文件的访问将被改变。`u` 代表所有者用户，`g` 代表文件组中的其他用户，`o` 代表不在用户组的其他用户，`a` 代表所有用户。如果未指定，默认为 `a` ，但 `umask` 中禁用的权限位不会被添加。
```bash
chmod +x file   # 未指定，默认为 a ，但会受 umask 限制
chmod u+x file  # 指定作用对象为用户（user），给予执行权限

umask   # 查看当前 umask
0002    # 屏蔽掉 other 的写权限，因此 `chmod +w` 对 other 不生效 
umask 0077  # 屏蔽 group 和 other 的所有权限
```

操作符 ：
- `+`：添加文件模式
- `-`：移除文件模式
- `=`：使之相同（有增删）

字母 `rwxXst` 指定了文件模式位。

| 字母  | 含义（权限位）                                 | 作用                          |
| --- | --------------------------------------- | --------------------------- |
| `r` | Read                                    | 允许读取文件内容或列出目录内容（`ls`）       |
| `w` | Write                                   | 允许修改文件内容或在目录中添加/删除文件        |
| `x` | Execute                                 | 对文件：允许执行；对目录：允许“进入”目录（`cd`） |
| `X` | Execute **only if**: 文件是目录或已设置了某个用户的执行位 | 智能设置执行权限（常用于递归）             |
| `s` | Set UID / GID                           | 设置用户 ID 或组 ID 位（对可执行文件或目录）  |
| `t` | Sticky Bit                              | 限制删除目录中非自己的文件（常见于 `/tmp`）   |
- 八进制数字形式：读权限（4）、写权限（2）、执行权限（1）
