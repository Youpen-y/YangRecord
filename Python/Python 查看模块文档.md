- 查看模块文档
```bash
python3 -m pydoc os
python3 -m pydoc sys
python3 -m pydoc pathlib
```

- 通过浏览器查看交互式文档
```bash
python3 -m pydoc -p 1234
```

- 在交互式解释器里用 `help()`
```bash
>>> import os
>>> help(os)
```

- 查看 Python 的标准库路径
`Python` 安装时会把标准库放在一个目录下，如：
```bash
python3 -c "import sys; print(sys.prefix)"
```