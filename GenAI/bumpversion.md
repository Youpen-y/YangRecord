安装：`pip install --upgrade bumpversion`

命令行工具：只需一个命令即升级软件，通过以正确的增量更新源代码中的所有版本字符串来简化软件的发布。

特点：
- 版本格式高度可配置
- 无需任何 VCS 即可工作
- 只处理文本文件，不特定于任何编程语言

使用：
有两种操作模式：
- 在命令行操作单个文件
可选参数：

| 参数                  | 说明             |
| ------------------- | -------------- |
| `--current-version` | 指定当前版本号（不依赖文件） |
| `--new-version`     | 直接指定新版本号       |
| `--commit`          | 自动提交修改         |
| `--tag`             | 自动创建 `git` 标签  |
| `--allow-dirty`     | 允许在未提交状态下运行    |
```bash
# 例，当前版本是 1.2.3
bumpversion patch   # 将版本号改为 1.2.4，输出修改信息，但不会更改文件，除非配置文件中指定
bumpversion minor   # 1.2.3 -> 1.3.0
bumpversion major   # 1.2.3 -> 2.0.0

bumpversion --new-version 1.5.0 release     # 自定义

bumpversion patch --commit --tag    # 快速修改版本并提交
```
- 使用配置文件（`.bumpversion.cfg` / `pyproject.toml`）进行多文件操作
方式一：在 `.bumpversion.cfg` 中配置
```.bumpversion.cfg
# .bumpversion.cfg
[bumpversion]
current_version = 1.2.3
commit = True
tag = True

[bumpversion:file:setup.cfg]
[bumpversion:file:my_package/__init__.py]
```
执行 `bumpversion patch` 将：自动找到配置文件；在 `setup.cfg` 和 `__init__.py` 中同步更新版本号；自动 `commit` 并打 `tag` 。

方式二：在 `pyproject.toml` 中配置
```pyproject.toml
[tool.bumpversion]
current_version = "1.2.3"
commit = true
tag = true

[[tool.bumpversion.files]]
filename = "pyproject.toml"

[[tool.bumpversion.files]]
filename = "my_package/__init__.py"
```
执行 `bumpversion patch` 将达到同样效果。
