`git format-patch` 用于生成补丁文件（patch files）的命令，它将提交（commits）转换成标准的邮件格式（mbox 格式），可以通过邮件发送给他人，便于代码审阅、应用（`git am`）等场景。

基本语法：
```bash
git format-patch [OPTION] <commit-range>
```
将指定范围内的提交生成一组 `.patch` 文件，每个提交生成一个对应的补丁文件。

场景用途：
- 提交代码给社区（如 Linux Kernel、coreutils）
- 离线发送补丁进行代码审查
- 跨仓库传输提交
- 与 `git am` 搭配使用，实现补丁应用

常见用法：
1. 生成最近一次提交的补丁
```bash
git format-patch -1
```

2. 生成最后三次提交的补丁
```bash
git format-patch -3
```

3. 从某个提交开始生成补丁（不包含该提交）
```bash
git format-patch abc123
```

4. 指定提交区间（生成的是 `(start, end]`）
```bash
git format-patch abc123..HEAD
```
或包含起始提交
```bash
git format-patch abc123^..HEAD
```

5. 添加邮件信息（用于发送）
```bash
git format-patch --cover-letter -o patches/ origin/main
```


生成的补丁文件格式示例：
```bash
0001-commit_message_description.patch
```
内容包含：
- `From:` 作者信息
- `Subject:` 提交标题
- `Date:` 提交时间
- `diff --git` 内容
- `Signed-off-by:` （如果使用了 `--signoff, -s` 选项）

常用选项详解：

|选项|说明|
|---|---|
|`-n`|指定生成补丁数量|
|`-o <dir>`|指定输出目录|
|`--stdout`|将所有补丁输出到标准输出|
|`--cover-letter`|创建一个用于邮件封面的 `0000-cover-letter.patch`|
|`--subject-prefix="PATCH"`|修改邮件主题前缀（默认是 "PATCH"）|
|`--signature=`|自定义签名尾部内容|
|`--thread`|设置邮件的引用关系，形成邮件讨论串结构|
|`--no-stat`|不输出 diffstat|
|`--attach`|用于将补丁作为附件（用于 `git send-email`）|
|`--numbered`|补丁文件加上序号（默认行为）|

配合 `git am` 使用
```bash
git am 0001-commit_message_description.patch
```
应用 `patch` 文件中的更改和提交信息。



