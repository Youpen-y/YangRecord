#### pyenv
一款 Python 版本管理工具。
优势：
- 尝试新的语言特性;
- 构造使用不同 Python 版本的项目
- 安装 Python 预发布版本，测试 bug

> 系统Python —— 操作系统自带的 Python 。
> 这个Python 实际上属于操作系统，位置为/usr/bin/python3 。不过这可能不是需要的Python 版本。如果为其安装软件包，需要运行sudo pip install，将为全局安装。
> 同一软件包存在多个版本，不同软件依赖的版本可能不同。

安装
==========================
使用 pyenv-installer
```shell
curl https://pyenv.run | bash
```
将安装
1. pyenv：实际的 pyenv 应用程序
2. pyenv-virtualenv：pyenv 和虚拟环境的插件
3. pyenv-update：用于更新 pyenv 插件
4. pyenv-doctor：用于验证 pyenv 和构建依赖项是否已安装的插件
5. pyenv-which-ext：自动查找系统命令的插件

运行结束后，将 pyenv 添加到路径，并初始化 pyenv/pyenv-virtualenv 自动完成功能。
之后，重新加载 shell
```shell
exec "$SHELL"
```

使用 pyenv 安装 Python
===========================

查看所有可用的 CPython 3.6 到 3.15
```shell
pyenv install --list | grep " 3\.[678]"
```

查看所有 Jython 版本
```shell
pyenv install --list | grep "jython"
```

安装指定版本的 Python
```shell
pyenv install -v 3.13.3
```
- `-v`：显示源码构建所有输出

安装位置 `~/.pyenv/versions/`
```shell
ls ~/.pyenv/versions/
3.13.3  3.14-dev

pyenv versions

```shell
pyenv versions
  system
* 3.13.3 (set by /home/yongy/.pyenv/version)
  3.14-dev
```

删除版本
```shell
rm -rf ~/.pyenv/versions/3.13.3
```
或
```shell
pyenv uninstall 3.13.3
```

pyenv 命令
============================
查看命令完整列表
```shell
pyenv commands
activate
commands
completions
deactivate
doctor
exec
global
help
hooks
init
install
latest
local
prefix
rehash
root
shell
shims
uninstall
update
--version
version
version-file
version-file-read
version-file-write
version-name
version-origin
versions
virtualenv
virtualenv-delete
virtualenv-init
virtualenv-prefix
virtualenvs
whence
which
```
命令详细提示
```shell
pyenv shims --help
Usage: pyenv shims [--short]

List existing pyenv shims
```

- `install`：安装特定版本的 `Python`
```shell
pyenv install 3.10.0
```
`install` 支持的标志有
1. `-l/--list` 列出所有可供安装的 Python 版本
2. `-g/--debug` 构建 Python 的调试版本
3. `-v/--verbose` 详细模式：将编译状态打印到标准输出

- `version`：显示所有当前安装的 Python 版本

- `which`：确定系统可执行文件的完整路径
```shell
pyenv which pip
```

- `global`：设置全局 Python 版本。确保默认使用特定的 Python 版本
```shell
pyenv global 3.13.3
```
该命令将 `~/.pyenv/version` 设置为 3.13.3

- `local`：用于设置特定与应用程序的 Python 版本
```shell
pyenv local 3.14-dev
```
该命令会在当前目录中创建一个 .python-version 文件。如果环境中已启用 pyenv ，则此文件将自动激活此版本

- `shell`：用于设置特定于 shell 的 Python 版本。
```shell
pyenv shell 3.14-dev
```
该命令通过设置 `PYENV_VERSION` 环境变量来激活指定的版本。此命令将覆盖任何应用程序设置或全局设置。如果想提供该版本，可使用 `--unset` 标志。

指定 Python 版本
============================
修改正在使用的 python 版本的三种方法
1. pyenv shell ($PYENV_VERSION)
2. pyenv local (.python-version file)
3. pyenv global (~/.pyenv/version)

自上而下可覆盖配置

虚拟环境和 pyenv
============================
- `pyenv` 管理 Python 自身多个版本
- `virtualenv/venv` 管理特定 Python 版本的虚拟环境
- `pyenv-virtualenv` 管理不同版本 Python 的虚拟环境

创建虚拟环境
```shell
pyenv virtualenv <python_version> <environment_name>
```
`<python_version>` 是可选的，`<environment_name>` 是自定义的环境名。

示例：
```shell
pyenv virtualenv 3.13.3 myproject
```

激活版本
创建虚拟环境后需要激活使用
```shell
pyenv local myproject
```
pyenv local 指定环境，将在当前工作目录中创建一个 .python-version 文件，由于在环境中运行了 `eval "$(pyenv virtualenv-init -)"`，该环境将自动激活

也可以手动激活/停用 Python 版本
```shell
pyenv activate <environment_name>
pyenv deactivate
# 这两个操作是 pyenv-virtualenv 进入或退出包含 .python-version 文件目录时会做的操作
```

在多种环境下工作
============================
示例：
1. project1 支持 python2.7 和 3.6
2. project2 支持 python3.6 和 3.8-dev 进行实验

为项目1创建虚拟环境
```shell
cd project1/
pyenv virtualenv 3.6.8 project1
...
pyenv local project1
```
当前使用 cd 退出目录时，将返回系统默认 Python

为项目2创建虚拟环境
```shell
cd project2/
pyenv virtualenv 3.8-dev project2
...
pyenv local 3.8-dev
```
当在项目之间 cd 时，pyenv 将负责自动激活环境

同时激活多个版本
===========================
假设 project2 使用了 Python 3.8 中的实验性功能，希望测试代码在 Python 3.6 仍然有效。
```shell
pyenv local project2 3.6.8
```
这向 pyenv 表明希望使用虚拟环境 project2 作为首选。如果一个命令（例如python）可以在两个环境中解析，它将优先选择 project2 而不是 3.6.8 。
```shell
python3.6 -V
Python 3.6.8
```
pyenv 尝试查找 python3.6 命令，由于它在活动的环境中找到该命令，因此允许该命令执行。
假设发现了库的兼容性问题，希望进行一些本地测试。测试需要安装所有依赖项。按如下步骤创建新环境：
```shell
pyenv virtualenv 3.6.8 project2-tmp
pyenv local project2-tmp
```

切换回默认环境：
```shell
pyenv local project2 3.6.8
```
