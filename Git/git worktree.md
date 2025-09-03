`git worktree` - 管理多个工作树

---
### 概述
首先要理解
- `working tree`：代表项目的当前快照，包含 `Git` 当前正在跟踪的所有文件和目录，以及任何修改、新文件或未跟踪的资源。
- `worktree`：`working tree` + 元数据

一个 Git 仓库可以与多个 `working tree` 相关联。`git worktree` 工具用于管理附加到同一存储库的多个 `working tree` ，为每个树创建不同的目录，使得可以同时检出多个分支，而无需来回切换。
- `main worktree`：如果一个仓库不是裸仓库（bare repository） ，其包含默认的主工作树。
- `linked worktree`：链接工作树，使用 `git worktree add` 添加的工作树。

多 `worktrees` 的使用场景：
1. 在分支之间快速切换以进行测试或比较，而无需存储（stash）或提交（commit）正在进行的工作。
2. 隔离 new feature 上的工作，同时保持主分支清洁且可部署。
3. 在继续 feature branch 上开放的同时，处理生产分支上的修补程序或紧急更改。

---
### 命令
`git worktree add`：简单讲“创建一个 `worktree`”。将一个新的 `working tree` 与仓库关联，并添加额外的元数据，用于区分该工作树与同一仓库中的其他工作树。
```bash
git worktree add <path> # 自动创建新分支（分支名为 path 的最后一个组成部分）
git worktree add <path> <branch>  # 在新工作树中处理现有分支
git worktree add -d <path> # 在新工作目录中检出一个 detached HEAD（游离 HEAD）状态
git worktree add <path> <commit-ish> # 在 path 处创建一个工作树，并将 commit-ish 签出到其中。
```

示例：
```bash
$ git worktree add ../demp-feature-a
Preparing worktree (new branch 'demp-feature-a')
HEAD is now at 42da243 Add feature-xyz as a single squashed commit

$ git worktree add ../demp-test test
Preparing worktree (checking out 'test')
HEAD is now at 42da243 Add feature-xyz as a single squashed commit
```

---
`git worktree lock`：锁定某个 `worktree` ，防止 `worktree` 被无意删除或修改。
```bash
git worktree lock [--reason <reason>] <path>
git worktree unlock <path>
```
- `<path>`：需要锁定的 `worktree` 路径
- `--reason <reason>`：为锁定添加原因（备注）

`git worktree unlock`：解锁 `worktree` 。
```bash
git worktree unlock <path>
```
---
`git worktree remove`：移除 `worktree`。只能移除干净的 `worktrees` （即没有未跟踪文件且已跟踪文件未发生任何修改）。
不干净的 `worktree`（或带有子模块的 `worktree`） 可以通过 `--force` 删除，`main worktree` 无法删除。

```bash
git worktree remove <path>

# 示例：
git worktree remove ../feature-a
```

---
`git worktree list`：列出所有工作树。
```bash
git worktree list [-v | --porcelain [-z]]
```

---
`git worktree prune`：删除 Git 仓库中已经失效的 `worktree`。比如使用 `rm -rf` 手动删除了 `worktree` 目录，但 Git 仓库的 `.git/worktrees` 目录中还残留旧数据，运行 `git worktree prune` 可以清理这些记录。
```bash
git worktree prune [-n] [-v] [--expire <expire>]
```
---
`git wortree move`：将工作树移动到新位置。
```bash
git worktree move <worktree> <new-path>
```
注意：`main worktree` 或包含子模块的 `linked worktree` 无法使用此命令移动。

---
`git worktree repair`：修复 `worktree` 元数据。
当发生一些特殊情况：
- `.git/worktrees/` 目录中的元数据丢失或损坏
- 手动拷贝、移动、还原了 Git 仓库
- 某些操作（如同步、备份、文件恢复）导致 Git 不再识别原来的 `worktree`
扫描 `.git/worktrees` 中丢失的 `worktree` 信息，并根据当前目录结构重新注册，恢复 `worktree` 的状态。

---
### 演示
问题：假如正在 `feature-a` 分支开发一项新功能，但需要快速切换到 `main` 分支来验证某些功能在那里是如何实现的。不能丢失当前进度，也不必提交正在进行的更改。

如果没有 `worktrees`：需要使用 `git stash` 临时保存更改，切换分支（main），然后切换回来并重新应用 stash 。

使用 `worktrees`：
1. 为 `main` 分支创建一个新的 `worktree`
```bash
git worktree add ../main-branch main
```

2. 切换到新的工作树并验证实现
```bash
cd ../main-branch
# 在该目录下验证
```

3. 返回原有分支的 `worktree`
```bash
cd ../original-repo-feature-new-ui
```