`git diff` - 显示提交之间的、提交和工作树等的更改。

> Show changes between the working tree and the index or a tree, changes between the index and a tree, changes between two trees, changes resulting from a merge, changes between two blob objects, or changes between two files on disk.

概念解释：
- `workspace`：是查看和编辑的（源）文件的目录树。
- `index`：（又称为 Staging area，暂存区域），是位于`<baseOfRepo>/.git/index` 中的一个单独的大型二进制文件，其中列出了当前分支中的所有文件、它们的 *sha1* 校验和、时间戳和文件名。
- `local repository`：一个隐藏目录（`.git`），其中包括一个 `objects` 目录，其中包含存储库中每个文件的所有版本（local branches and copies of remote branches）作为一个压缩的 `blob` 文件。

![[Git Data Transport.png]]
