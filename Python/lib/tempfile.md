`tempfile` - 生成临时文件和目录的模块

- 高级接口（自动清理）：`TemporaryFile`, `NamedTemporaryFile`, `TemporaryDirectory`,  `SpooledTemporaryFile`
- 低级接口（手动清理）：`mkstemp()`, `mkdtemp()`
- 其他接口：`gettempprefix()`, `gettempprefixb()`, `gettempdir()`, `gettempdirb()`
以上接口在使用时不会有竞争条件（race condition），除了 `mktemp`（仅为了向后兼容旧的代码）

1. `TemporaryFile` - 创建并返回一个临时文件
```python
def TemporaryFile(mode='w+b', buffering=-1, encoding=None,
                      newline=None, suffix=None, prefix=None,
                      dir=None, *, errors=None)
```
参数：

| 参数名         | 作用      | 默认值    | 说明                                   |
| ----------- | ------- | ------ | ------------------------------------ |
| `mode`      | 打开文件的模式 | `w+b`  | 二进制可读写模式（覆盖旧内容）。                     |
| `buffering` | 缓冲策略    | `-1`   | 与 `io.open()` 一样，`-1` 表示使用系统默认缓冲区大小。 |
| `encoding`  | 文本编码    | `None` | 仅在文本模式（如 `w+`）下有效。二进制模式下忽略。          |
| `newline`   | 换行符控制   | `None` | 控制读写时换行符的处理方式。与 `io.open()` 相同。      |
| `suffix`    | 文件名后缀   | `None` | 临时文件的文件名后缀，如 `.txt`。                 |
| `prefix`    | 文件名前缀   | `None` | 临时文件的文件名前缀，如 `tmp_`。                 |
| `dir`       | 文件目录    | `None` | 临时文件放在哪个目录中。默认在系统临时目录中。              |
| `errors`    | 错误处理策略  | `None` | 对编码/解码错误的处理方式，与 `io.open()` 一样。      |
示例：
```python
import tempfile

with tempfile.TemporaryFile(mode='w+t') as tmp:
    tmp.write("Hello, temporary file!\n")
    tmp.seek(0)
    print(tmp.read())
```

2. `NamedTemoraryFile` - 创建一个带名字的临时文件并返回一个文件对象
```python
def NamedTemporaryFile(mode='w+b', buffering=-1, encoding=None,
                       newline=None, suffix=None, prefix=None,
                       dir=None, delete=True, *, errors=None,
                       delete_on_close=True)
```
与 `TemporaryFile()` 相比，`NamedTemporaryFile()` 的临时文件在文件系统中可见（有文件名）；而 `TemporaryFile()` 的文件则可能没有名字或立即被删除。
- `delete`：是否在使用完后自动删除文件（默认为 `True` => 删除）
- `delete_on_close`：在 `delete=True` 的前提下，决定何时删除文件：
	- `True`：文件关闭时就删除（默认）
	- `False`：延迟删除（在 `with` 块退出或对象销毁时）
示例：
```python
import tempfile

with tempfile.NamedTemporaryFile(mode='w+t') as tmp:
    print("临时文件名: ", tmp.name)
    tmp.write("Hello, world!")
    tmp.seek(0)
    print(tmp.read())
# 退出 with 块后，文件会自动删除
```

3. `class TemporaryDirectory` - 用于创建临时目录
```python
class TemporaryDirectory:
    def __init__(self, suffix=None, prefix=None, dir=None,
                ignore_cleanup_errors=False, *, delete=True)
“”“ TemporaryDirectory 在 mktemp() 的基础上：
- 自动清理目录
- 支持 with 语法
- 更加安全、方便
”“”
```
示例：
```python
import tempfile, os

with tempfile.TemporaryDirectory() as tmpdir:
    print("临时目录: ", tmpdir)
    open(os.path.join(tmpdir, "test.txt"), "w").write("hello")

# 离开 with 块后，该临时目录及文件都会删除
```

4. `SpooledTemporaryFile` -  可自动切换的临时文件包装类
```python
class SpooledTemporaryFile(_io.IOBase):
    """Temporary file wrapper, specialized to switch from BytesIO
    or StringIO to a real file when it exceeds a certain size or
    when a fileno is needed.
    """
    def __init__(self, max_size=0, mode='w+b', buffering=-1,
                 encoding=None, newline=None,
                 suffix=None, prefix=None, dir=None, *, errors=None)
```
- `SpooledTemporaryFile` 是一个“可自动切换的临时文件包装类”。
	- 一开始，像 `io.BytesIO` （或 `io.StringIO`）那样在内存中存储数据。
	- 当写入的数据超过一定大小（`max_size`），或需要底层文件描述符（如 `.fileno()`）时，它会自动切换到真正的临时磁盘文件（由 `tempfile` 模块创建）。
> `SpooledTemporaryFile` 一开始是“内存文件”，当内容太大或需要文件句柄时，变成“磁盘文件”。 

示例1：
```python
import tempfile

with tempfile.SpooledTemporaryFile(max_size=1024, mode='w+t') as tmp:
    tmp.write("Hello, world!")
    tmp.seek(0)
    print(tmp.read())
```
示例2：
```python
import tempfile

with tempfile.SpooledTemporaryFile(max_size=10, mode='w+t') as tmp:
    tmp.write("This is a long string exceeding ten characters.")
    tmp.seek(0)
    print(tmp.read())
```
示例3：（即使内容很小，因为调用了 `.fileno()`，仍会自动切换到磁盘文件）
```python
import tempfile

with tempfile.SpooledTemporaryFile() as tmp:
    tmp.write(b"abc")
    fd = tmp.fileno()
    print("file descriptor", fd)
```
---
底层接口：
1. `mkstemp` - 创建并返回一个唯一且安全的临时文件
```python
def mkstemp(suffix=None, prefix=None, dir=None, text=False)
"""返回一个二元组 (fd, name). 
fd: 是 os.open 返回的文件描述符
name: 文件的完整路径名（字符串或字节串）
- suffix: 文件名后缀，不指定则没有
- prefix: 文件名前缀，默认常为 'tmp'
- dir: 临时文件存放的目录，默认使用系统临时目录（`tempfile.gettempdir()`）返回的目录
- text=False, 二进制模式；text=True，文本模式 
  
如果 suffix, prefix 和 dir 非空，它们必须是同一类型（bytes 或 str）
"""
```

2. `mkdtemp` - 创建一个安全、唯一的目录
```python
def mkdtemp(suffix=None, prefix=None, dir=None) -> str: ...
"""返回目录的完整路径字符串
"""
```

3. `gettempdir` - 返回 `tempfile.tempdir` 字符串
```python
def gettempdir() -> str: ...
示例：
>>> tempfile.gettempdir()
'/tmp'
```

4. `gettempprefix` - 返回临时目录的默认前缀
```python
def gettempprefix() -> str: ...
示例：
>>> tempfile.gettempprefix()
'tmp'
```