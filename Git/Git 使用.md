Git 进行版本控制时，配置分为三个级别 ^3f1ed4
- `--system`，系统级别，适用于系统上所有用户和仓库，`/etc/gitconfig`
- `--global`，全局级别，适用于当前用户的所有仓库。`~/.gitconfig`
- `--local`，仓库级别，仅适用于当前仓库，`.git/config`。
### 连接本地存储库到GitHub上远程存储库
```bash
$ git remote add origin repository-url
```
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

### `git stash` 使用
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

### 删除远程分支
```bash
# 方案1
git push origin --delete <branch_name>

# 方案2
git push origin :<branch_name>
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

#### `git rebase` 使用
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

# 在命令行中不小心将 file 加入了暂存，如果撤销
git rm --cached file		# 仅从索引中删除文件，同时保留工作目录中的文件
git rm -f file              # 从索引中删除文件，并且不保留工作目录中的文件
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

---
### 更改提交的描述（message）
1. 更改最新的提交消息（尚未推送到远端）
```bash
git commit --amend
```
执行该命令后，会打开默认的文本编辑器以便修改提交的消息。修改后，保存并退出编辑器即可。
2. 更改历史提交的消息（已推送到远端）- `git rebase -i`
Note：修改历史提交的消息会改变提交的哈希值，因此如果这些提交已经推送到远端，需小心使用，因为可能影响其他协作者的工作。
步骤1：
```bash
git rebase -i HEAD~n
```
其中 `n` 是要回溯的提交数量。如，`HEAD~3`表示想修改过去3个提交。
步骤2：选择修改的提交。运行 `git rebase -i` 后，会打开一个文本编辑器，显示最近 `n` 个提交。每个提交前面都有一个动作指令（`pick`）。找到你要修改消息的提交，把 `pick` 改为 `reword`。
```bash
pick e3a1b35 Commit message 1
reword b4c4c9f Commit message 2 # 修改这个提交的消息 
pick 9c4d90a Commit message 3
```
步骤3：修改提交消息，保存并退出。
3. 推送更改到远程仓库
```bash
git push --force
```

---
问题：已经将本地提交推送到远端，发现有需要包含的遗漏文件？怎么办？
解决方案1：
1. 撤销此次本地提交（保留暂存区和工作区的修改）
```bash
git reset --soft HEAD~1
```
2.  添加遗漏文件
```bash
git add tempfile
```
3. 强制推送到远端仓库
```bash
git push origin <branch_name> --force
# git push -f
```

解决方案2：
1. 添加遗漏文件到暂存区
```bash
git add tempfile
```
2. 修改上一次的提交
```bash
git commit --ammend
```
3. 强制推送到远程仓库
```bash
git push origin <branch_name> --force
# 或简写
git push -f
```


### `git archive`
将某个提交（`commit`）或分支（`branch`）的内容打包为压缩文件（如 `.zip` 或 `.tar`）。常用于发布源代码或备份特定版本，而不包含 `.git` 目录（即不包含版本历史）。

基本语法：
```bash
git archive [options] <commit> [<path>...]
```
- `<commit>`：可以是分支名、标签名、commit 哈希值等
- `<path>`（可选）：只归档指定路径的文件或目录
- 常用选项
	- `--format=tar` 或 `--format=zip`：指定压缩格式（默认为 `tar`）
	- `-o <filename>`：指定输出文件名
	- `--prefix=<dir>/`：在归档文件中为所有文件加上前缀目录名


常见示例：
1. 打包主分支为 `tar` 文件
```bash
git archive --format=tar --output=project.tar main
```
2. 打包标签 `v1.0` 为 `zip` 文件
```bash
git archive --format=zip --output=project.zip v1.0
```
3. 只打包某个子目录
```bash
git archive -o src-only.zip HEAD src/
```














