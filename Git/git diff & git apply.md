`git diff` - 显示提交之间的、提交和工作树等的更改。

> Show changes between the working tree and the index or a tree, changes between the index and a tree, changes between two trees, changes resulting from a merge, changes between two blob objects, or changes between two files on disk.

单个 `git` 存储库可以跟踪任意数量的分支，但你的 `working tree` 仅与其中一个分支（`current` 或 `checked out` 分支）相关联，并且 `HEAD` 指向该分支。

概念解释：
- `Working tree`：是查看和编辑的（源）文件的目录树。
- `Index`：（又称为 Staging area，暂存区域），是位于`<baseOfRepo>/.git/index` 中的一个单独的大型二进制文件，用于存储下次提交的内容信息，其中列出了当前分支中的所有文件、它们的 *sha1* 校验和、时间戳和文件名。当运行 `git add` 命令时，文件会被添加到暂存区。
- `local repository`：一个隐藏目录（`.git`），其中包括一个 `objects` 目录，其中包含存储库中每个文件的所有版本（local branches and copies of remote branches）作为一个压缩的 `blob` 文件。

![[Git Data Transport.png]]

使用示例：
- 查看尚未 `add` 的改动（比较 `Working tree` 和 `Index` 区域）
```bash
git diff

git diff > uncommitted.patch    # 打补丁
git apply uncommitted.patch     # 补丁应用
```

- 查看已 `add` 但尚未 `commit` 的改动
```bash
git diff --staged
# or
git diff --cached
```

- 查看工作区与最新提交的差异
```bash
git diff HEAD
```

- 比较任两次 `commit` 之间的差异
```bash
git diff a1b2c3d..e4f5g6h
```

- 比较两个分支
```bash
git diff main feature/login
```

- 只看某个文件在两 `commit` 间的差异
```bash
git diff HEAD~3 HEAD -- <file>
```