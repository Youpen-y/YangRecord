Git 进行版本控制时，配置分为三个级别
- `--system`，系统级别，适用于系统上所有用户和仓库，`/etc/gitconfig`
- `--global`，全局级别，适用于当前用户的所有仓库。`~/.gitconfig`
- `--local`，仓库级别，仅适用于当前仓库。

### 配置全局忽略文件 ：其中指定应在所有仓库中忽略的文件模式。

```bash
# 创建全局忽略文件
touch ~/.gitignore_global
# 在全局配置中设置忽略文件
git config --global core.excludesfile ~/.gitignore_global
```

### 设置代理

```bash
# 设置 HTTP 代理
git config --global http.proxy http://proxy.example.com:8080

# 设置 HTTPS 代理
git config --global https.proxy https://proxy.example.com:8080

# 取消设置代理
git config --global --unset http.proxy
git config --global --unset https.proxy
```

### 设置提交模板

```bash
touch ~/.gitmessage.txt
git config --global commit.template ~/.gitmessage.txt
```

### 设置推送行为（默认推送行为，避免意外推送所有分支）

```bash
# 设置默认推送当前分支
git config --global push.default current
```

### `git stash`
```bash
# 保存当前工作目录的修改
git stash save "description info"

# 查看现有的 stash 列表
git stash list

# 恢复最近的 stash
git stash pop	stash@{n}		# 恢复并删除 stash@{n}
git stash apply stash@{n}	# 只恢复不删除 stash@{n}

# 删除 stash
 git stash drop stash@{n}	# 删除 stash@{n}
 git stash clear				# 删除所有的 stash


# 查看最近一次 stash 的改动概要
git stash show

# 查看最近一次 stash 的详细改动内容
git stash show -p

# 查看指定 stash{n} 的改动
git stash show [-p] stash@{n}

# 查看具体某个文件的改动
git stash show [-p] stash@{n} path/to/file
```

### 跟踪远程分支
```bash
# 查看远程分支
git branch -r

# 创建本地分支时设置追踪
git checkout -b <本地分支名> <远程名>/<远程分支名>

# 设置现有的本地分支追踪远程分支
git branch --set-upstream-to=<远程名>/<远程分知名> <本地分支名>

# 查看分支的追踪状态
git branch -vv

# 修改追踪关系
git branch --set-upstream-to=origin/<新远程分支名> <本地分支名>

# 删除追踪关系，然后删除本地分支
git branch --unset-upstream
git branch -d <本地分支名>
```
### `git bisect` - 查找引入错误的提交
首先确定 `bad` 提交和 `good` 提交，`git bisect` 在亮点之间选择一个提交，要求你确定该提交是 `good` 还是 `bad`，重复该过程直到找到提交。
```bash
git bisect start
git bisect bad						# Current version is bad
git bisect good v2.6.13.rc2	# v2.6.13.rc2 is known to be good
```

#### `git commit --amend` - 修改最近的提交
- 修改提交信息。`git commit --amend -m "new commit msg"
- 添加遗漏文件后，。
```bash
git add forgotten-file 
git commit --amend
```
Note：使用 `--amend` 会改变提交的 `SHA-1` 值，因此在已经推送到远程仓库的分支上使用时要小心。可能会导致其他协作者的仓库出现问题。

#### `git rebase` 
作用：保持干净且线性的提交历史记录；更新功能分支
语法
```bash
git rebase [-i | --interactive] [options] [--exec cmd] [--onto newbase | --keep-base] [upstream [branch]]
```
`rebase` 是将一系列提交移动或组合到新的基提交（base commit）的过程。即将分支的 base 从一个提交更改为另一个提交。
示例：将 feature 分支变基
```bash
git rebase main feature
# 假如当前分支为 feature , 可使用 git rebase master 获得同样的效果
# 变基后，main 落后 feature 一个提交通过 git merge 将 feature 中的提交合并到 main 中
git merge feature 
```

![[git-rebase.png]]

#### 删除远程分支
```bash
# 方案1
git push origin --delete <branch_name>

# 方案2
git push origin :<branch_name>
```

#### `git tag` 标签
```bash
git tag -a v1.0.0 -m "release a tag v1.0.0"
git push origin v1.0.0
git push origin --delete v1.0.0
git push origin --tags 	# 推送所有标签
```
- `-a` 表示创建一个附注标签，包含作者信息和日期
- `-m` 用于添加标签说明
- `-d` 删除一个标签

#### `git rm`
`git rm` 命令用于从 Git 仓库中删除文件。该命令可以同时将文件从工作目录和索引（暂存区）中删除。
```bash
git rm [options] <file>...

git rm -r directory_name	# 递归删除目录及其内容
git rm --cached file			# 仅从索引中删除文件，同时保留工作目录中的文件
```

#### `git revert`
`git revert` 用于撤销 Git 中某个特定提交的命令。与 `git reset` 不同，`git revert` 会创建一个新的提交，该提交的内容是撤销指定提交的更改。》确保历史记录保持完整，不会丢失其他提交的信息。
```bash
git revert HEAD					# 撤销最新的提交
git revert abc1234				# 撤销特定的提交
git revert -n HEAD~3..HEAD		# 撤销多个提交
# 撤销过程中可能会出现冲突，解决完冲突后，
# 1. 使用 git add . 添加解决后的文件
# 2. git revert --continue 继续
```


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
- 全局设置（显示原始中文字符，而非 Unicode 转义序列）
```bash
git config --global core.quotePath false
```
- 在某仓库下设置
```bash
git config core.quotePath false
```

###  一种情形：`git` 如何将一个分支的前几个提交 `merge` 到另一分支上？
假如，`main` 分支比较落后，`test` 分支有些技术已经测试成熟，但有些还不可用。这时需要将成熟技术 `merge` 到主分支上来，方案？
- 临时分支合并
```bash
# (1). 从 test 分支创建临时分支
git checkout -b temp test

# (2). 在临时分支上移除不想合并的改动
git revert <不想要的提交>

---
# 1. 或者 直接 checkout 到某个提交，创建新分支
git checkout -b temp
---

# 2. 切换到 main 分支并合并临时分支
git checkout main
git merge temp

# 3. 完成后删除临时分支
git branch -d temp
```

- `git cherry-pick`
```bash
# 1. 确认哪些提交包含了成熟的技术
git checkout test
git log
# 2. 找到成熟技术对应的提交 commit hash
# 3. 切换到 main 分支并进行 cherry-pick
git checkout main
git cherry-pick <commit-hash1> <commit-hash2> ...
# 或使用范围
git cherry-pick <start-commit>..<end-commit>
```