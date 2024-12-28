

### `git stash`


### `HEAD` 用法
`HEAD` 简单来说是一个指针，指向当前所在的分支或特定的提交。
#### 基本概念
- `HEAD` 通常指向当前分支的最新提交
- 当切换分支时，`HEAD` 会跟着移动到新分支的最新提交
- `HEAD` 的位置决定了你的工作目录内容
#### 使用技巧
- `HEAD^` 表示父提交
- `HEAD~n` 表示向上第 n 个提交
- `HEAD@{n}` 表示 HEAD 的第 n 个历史位置
#### 常见用法
```bash
git symbolic-ref HEAD		# 显示 HEAD 指向的分支

git show HEAD		# 显示当前提交的信息
git show HEAD~1	# 显示上一个提交
git show HEAD~2	# 显示上上个提交

# 版本回退
git reset HEAD~1	# 回退一个提交(mixed，默认模式)
git reset --soft HEAD~1		# 软回退，
git reset --hard HEAD~1		# 硬回退

git diff HEAD..Some_branch	# 查看分支之间的差异
git diff HEAD..Some_branch --stat	# 查看文件统计信息（增删改了多少行）

git checkout <commit-hash>	# 直接切换至某个提交

git reflog show HEAD		# 查看 HEAD 的移动历史
```
#### 提交比较（`diff`）
```bash
# 语法
git diff <from>..<to>
```
其中 `<from>` 和 `<to>` 可以是 `HEAD` ，`SHA 前缀`，分支名。

#### 三种版本回退的差异
`git reset HEAD~1` 默认是 `--mixed` 模式
- 撤销 commit
- 撤销暂存区的修改至工作区
- 保留工作区的修改
`git reset --soft HEAD~1` ，`--soft` 模式
- 仅撤销 commit
- 保留暂存区的修改
- 保留工作区的修改
`git reset --hard HEAD~2` ，`--hard` 模式
- 撤销提交
- 撤销暂存区的修改
- 撤销工作区的修改（会丢失未提交的修改）

### `git status` 如何显示中文文件名？（配置 `core.quotePath` ）
全局设置（显示原始中文字符，而非 Unicode 转义序列）
```bash
git config --global core.quotePath false
```

某仓库下设置
```bash
git config core.quotePath false
```
