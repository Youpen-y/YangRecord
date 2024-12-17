一个 `git` 子模块是一个完整的存储库，嵌套在另一个存储库中。任何存储库都可以是另一个存储库的子模块。

子模块只能固定到特定的提交（即，使用特定的提交）。

递归克隆
```bash
git clone --recursive [url]
```



添加子模块
```bash
git submodule add https://github.com/dmazin/library.git library
```
该命令将添加（或更新）本地项目的 `.gitmodules` 文件，下载 `library` ，并将本项目指向 `library` 的最新提交。