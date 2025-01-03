`python` 虚拟环境允许单独管理不同项目的依赖关系，防止冲突并保持更清晰的设置。
- `python` 的 `venv` 模块
- 

可以创建使用不同版本库或 Python 本身的隔离环境。

1. 创建 `.venv` 虚拟环境
```bash
python -m venv .venv/
```

2. 激活虚拟环境
```bash
source .venv/bin/activate		# 在 vscode 中，无法直接执行myvenv/bin/activate
```

3. 在虚拟环境中安装包
```bash
python -m pip install <package_name>
```

4. 退出虚拟环境
```bash
deactivate
```

`.venv` 目录树
```bash
.venv
├── bin
├── include
│   └── python3.12
├── lib
│   └── python3.12
│       └── site-packages
│          ├── pip
├── lib64 -> lib
├── pyvenv.cfg
```

- `include/` 用来包含可能安装的依赖于 C 扩展的包的 C 头文件
- `lib/` 包含`site-packages\`文件夹，这是创建虚拟环境的主要原因之一。您将在该文件夹中安装要在虚拟环境中使用的外部包。默认仅安装 `pip`
- `scripts/` 包含虚拟环境的可执行文件。
- `pyvenv.cfg` 配置虚拟环境的重要文件。
```cfg
home = /usr/bin
include-system-site-packages = false
version = 3.12.3
executable = /usr/bin/python3.12
command = /usr/bin/python3 -m venv /home/yongy/Notes/python-exercises/.venv
```
