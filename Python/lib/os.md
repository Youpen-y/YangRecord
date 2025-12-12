`os` 模块提供了一种可移植的使用依赖操作系统的函数的方式。
> 注意：该模块中的所有函数在文件名和路径无效或不可访问的情况下，或者包含正确类型但操作系统不接受的其他参数时引发 `OSError` 错误（或者其子类）

```python
>>> os.error
<class 'OSError'>
>>> os.name
'posix'
>>> os.path
<module 'posixpath' (frozen)>
>>> os.curdir
'.'
>>> os.pardir
'..'
>>> os.sep
'/'
>>> os.extsep
'.'
>>> print(os.altsep)
None
>>> os.pathsep
':'
>>> os.linesep
'\n'
>>> os.defpath
'/bin:/usr/bin'
>>> os.devnull
'/dev/null'
```

- `os.error`：是 `OSError` 的别名
- `os.name`：导入 os 模块的操作系统，`posix/nt/java`
- `os.path`：一个模块，提供一系列用于处理文件路径的函数，“动态别名”会根据当前操作系统自动指向不同的实现
- `os.curdir`：代表当前目录的字符串（通常是 `.`）
- `os.pardir`：代表父目录的字符串（通常是 `..`）
- `os.sep`：路径名分隔符（`/` 或 `\\`）
- `os.extsep`：扩展分隔符（通常是 `.`）
- `os.altsep`：可选的路径分隔符（`None` 或 `/`）
- `os.pathsep`：在 `$PATH` 中使用的分隔符
- `os.linesep`：文本文件中的行分隔符（`\n`或`\r\n`）
- `os.defpath`：可执行文件的默认搜索路径
- `os.devnull`：空设备的文件路径（`/dev/null`）

文件定位：`os.SEEK_SET`、`os.SEEK_CUR`、`os.SEEK_END`
当在 Python 中操作文件时，如下：其中 `whence` 参数决定从哪里开始移动文件指针。
```python
f.seek(offset, whence)

with open("example.txt", "rb") as f:
    f.seek(0, os.SEEK_END)  # 移动到文件某尾
    size = f.tell()         # 当前指针位置 = 文件长度
    print(f"File size: {size} bytes")
    
    fseek(-10, os.SEEK_END) # 从末尾向前移动 10 字节
    print(f.read())         # 读取最后 10 个字节
```
`whence`可能取值：
- `os.SEEK_SET`：0，从文件开头开始偏移
- `os.SEEK_CUR`：1，从当前位置开始偏移
- `os.SEEK_END`：2，从文件末尾开始偏移
Linux 系统上还支持：（稀疏文件定位常量，高效定位实际存储的数据或空洞部分）
- `os.SEEK_DATA`：3，找到下一个有数据的偏移，跳过空洞
- `os.SEEK_HOLE`：4，找到下一个空洞的偏移，跳过数据
```sql
Offset:
0----1000----2000----3000----4000
内容:
[DATA]  [HOLE]  [DATA]  [HOLE]
lseek(fd, 0, SEEK_DATA)         -> 0 （第一个数据块）
lseek(fd, 500, SEEK_DATA)       -> 2000 （下一个数据块）
lseek(fd, 0, SEEK_HOLE)         -> 1000 （第一个空洞）
lseek(fd, 2500, SEEK_HOLE)      -> 3000（下一个空洞）
```

---
`os.path` 模块
常用函数：
1.  `join`
```python
def join(a, *p) -> str
"""连接两个或多个路径部分，需要时插入 '/'.
如果某部分是绝对路径，则其前面的路径部分均被丢弃.
最后部分如果为空，将输出一个以分隔符为结尾的路径.
"""
```

2. `split`
```python
def split(p) -> tuple(str | None, str | None)
"""分割路径.
返回元组 "(head, tail)"，其中 'tail' 是最后一个 '/' 后的所有部分，'head' 是最后一个 '/' 前的部分（不包括最后一个 '/'），两部分均可能为空.
"""
```

3. `splitext`
```python
def splitext(p) -> tuple(str, str)
"""分割路径的根与其扩展名（扩展名是 '.' 加上扩展名后缀），根是 '.' 前面的所有部分（不含 '.'）
"""
```

4. `basename`
```python
def basename(p) -> str
"""返回路径名的最后一部分"""
```

5. `dirname`
```python
def dirname(p) -> str
"""返回路径名的目录部分"""
```

6. `abspath`
```python
def abspath(path) -> str
"""返回绝对路径"""
```

7. `realpath`
```python
def realpath(filename, *, strict=False) -> str
"""返回文件（或目录）的规范化路径.即解析所有符号链接后得到的实际路径.
当 strict=True 时，路径中的任一部分不存在，就会抛出 FileNotFoundError
"""
```

8. `relpath`
```python
def relpath(path, start=None) -> str
"""返回 path 相对于 start 的相对路径.
如果不指定 start，默认以当前目录 os.getcwd() 为起点
```

9. `commonpath`
```python
def commonpath(paths: list[str]) -> str
"""给定一组路径序列，返回它们的最长公共自路径(longest common sub-path)"""
```

---
高级目录工具
1. `makedirs` - 递归创建目录，等价于 Linux 的 `mkdir -p`
```python
def makedirs(name: StrOrBytesPath, mode: int = 0o777, exist_ok: bool = False) -> None: ...
"""默认情况下，exist_ok=False，如果目录已存在，会抛出异常 FileExistsError
而 exist_ok=True 可使得忽略已存在的目录
"""
```

2. `removedirs` - 递归删除目录
```python
def removedirs(name: StrOrBytesPath) -> None: ...
"""如果上级目录也变空，会继续向上删除；一直删除到非空或无权限为止
"""
```

3. `renames` - 递归重命名
```python
def renames(old: StrOrBytesPath, new: StrOrBytesPath) -> None: ...
"""如果 new 的父目录不存在，会自动创建；如果 old 的父目录在重命名后变空，会自动删除
"""
```

---
文件系统遍历
1. `walk` - 面向应用层的目录树遍历器
```python
def walk(top: str, topdown: bool=True, onerror: callable=None, followlinks: bool=False) -> tuple(str, list[str], list[str])
"""目录树生成器，递归遍历整个目录树.
每次迭代返回一个三元组 (dirpath, dirnames, filenames)
参数：
- top: 起始目录（根目录）
- topdown: 是否“自上而下”遍历（默认自上而下，设置为 False 则自下而上）
- callable: 出错时的回调函数
- followlinks: 是否跟随符号链接（设置为 True，如果是符号链接也会跟随遍历）
返回值：
- dirpath: str, 当前目录路径
- dirnames: list[str], 当前目录下的子目录名列表
- filenames: list[str], 当前目录下的普通文件列表
```

示例：
```python
for dirpath, dirnames, filenames in os.walk('project'):
    if 'docs' in dirnames:
        dirnames.remove('docs') # 不进入 docs/
```

2. `fwalk` - 与 `walk()` 类似，但会返回目录文件描述符，一些低级系统调用（如 `os.fstat()`, `os.open()`, `os.unlink()`可以直接使用文件描述符进行操作）
```python
def fwalk(top=".", topdown=True, onerror=None, *, follow_symlinks=False, dir_fd=None)
"""file descriptor walk, 返回四元组 (dirpath, dirnames, filenames, dirfd)
"""
```

---
`exec` 系列函数 - 底层系统调用接口的封装；在当前进程中直接执行一个新的可执行文件，替换掉当前进程的映像。
1. `execl` - 示例：`os.execl('/bin/ls', 'ls', '-l')`
```python
def execl(file, *args)
"""执行可执行文件 file, with 参数列表 args，替换当前进程"""
```

2. `execle`
```python
def execle(file, *args)
"""执行可执行文件 file, with 参数列表 args，以及 env=args[-1]，替换当前进程"""
```

3. `execlp`
```python
def execlp(file, *args)
"""执行可执行文件 file（在 $PATH 中查询），with 参数列表 args，替换当前进程"""
```

4. `execlpe`
```python
def execlpe(file, *args)
"""执行可执行文件 file（在 $PATH 中查询），with 参数列表 args，以及 env=args[-1]，替换当前进程"""
```

5. `execvp`
```python
def execvp(file, args)
"""执行可执行文件 file（在 $PATH 中查询），with 参数列表 args（字符串列表或字符串元组），替换当前进程"""
```

6. `execvpe`
```python
def execvp(file, args, env)
"""在 $PATH 中搜索文件，并指定环境变量"""

示例：
args = ["ls", "-l"]
env = {"PATH": "/bin:/usr/bin"}
os.execvpe("ls", args, env)
```

---
常用功能
0. `access` - 检验权限模式
```python
def access(
    path: FileDescriptorOrPath, mode: int, *, dir_fd: int | None = None, effective_ids: bool = False, follow_symlinks: bool = True
) -> bool: ...
"""参数 path 是被检测的是否具有访问权限的路径.
参数 mode:
- os.F_OK: 测试 path 是否存在
- os.R_OK: 测试 path 是否可读
- os.W_OK: 测试 path 是否可写
- os.X_OK: 测试 path 是否可执行
```

1. `getcwd` - 获取当前工作目录
```python
def getcwd() -> str: ...
示例：
>>> os.getcwd()
'/home/yongy'
```

2. `chdir()` - 改变当前工作目录
```python
def chdir(path: FileDescriptorOrPath) -> None: ...
```

3. `listdir` - 列出当前目录内容；不提供 path 参数，默认列出当前工作目录的内容
```python
@overload
def listdir(path: StrPath | None = None) -> list[str]: ...
@overload
def listdir(path: BytesPath) -> list[bytes]: ...
@overload
def listdir(path: int) -> list[str]: ...
```

4. `mkdir` - 创建目录；如果目录已存在，会抛出 `FileExistsError` 异常
```python
def mkdir(path: StrOrBytesPath, mode: int = 0o777, *, dir_fd: int | None = None) -> None: ...
```

5. `rmdir` - 删除目录；如果目录不为空，会抛出 `OSError` 异常
```python
def rmdir(path: StrOrBytesPath, *, dir_fd: int | None = None) -> None: ...
```

6. `remove` - 删除文件；如果文件不存在，会抛出 `FileNotFoundError` 异常
```python
def remove(path: StrOrBytesPath, *, dir_fd: int | None = None) -> None: ...
```

7. `rename` - 重命名文件或目录
```python
def rename(src: StrOrBytesPath, dst: StrOrBytesPath, *, src_dir_fd: int | None = None, dst_dir_fd: int | None = None) -> None: ...
```

8. `getenv` - 获取指定环境变量的值。示例：`home_dir = os.getenv("HOME")`
```python
@overload
def getenv(key: str) -> str | None: ...
@overload
def getenv(key: str, default: _T) -> str | _T: ...
```

9. `system` - 执行系统命令，返回命令的退出状态。
```python
def system(command: StrOrBytesPath) -> int: ...
```

10. `link` - 创建硬链接
```python
def link(
    src: StrOrBytesPath,
    dst: StrOrBytesPath,
    *,
    src_dir_fd: int | None = None,
    dst_dir_fd: int | None = None,
    follow_symlinks: bool = True,
) -> None: ...
```

11. `symlink` - 创建软链接
```python
def symlink(
    src: StrOrBytesPath, dst: StrOrBytesPath, target_is_directory: bool = False, *, dir_fd: int | None = None
) -> None: ...
```

12. `write` - 写入，将数据 `data` 写入文件描述符 `fd` 对应的文件中，返回实际写入的字符串长度
```python
def write(fd: int, data: ReadableBuffer, /) -> int: ...
```

13. `scandir` - 迭代目录条目，返回一个迭代器，每个元素是 `os.DirEntry` 对象
```python
@overload
def scandir(path: None = None) -> _ScandirIterator[str]: ...
@overload
def scandir(path: int) -> _ScandirIterator[str]: ...
@overload
def scandir(path: GenericPath[AnyStr]) -> _ScandirIterator[AnyStr]: ...
```
每个 `DirEntry` 对象提供的方法和属性
- `.name`: 文件或目录名称（不包含路径）
- `.path`: 完整路径
- `is_file(follow_symlinks=True)`: 是否为普通文件
- `is_dir(follow_symlinks=True)`: 是否为目录
- `is_symlimk()`: 是否为符号链接
- `.stat(follow_symlinks=True)`: 获取文件状态信息（比 `os.stat()` 更快）
示例：
```python
files = [entry.name for entry in os.scandir('.') if entry.is_file()]
dirs = [entry.name for entry in os.scandir('.') if entry.is_dir()]
```

14. `chflags` - 设置路径 `path` 的标志
```python

```

- `os.environ`：一个映射对象（mapping object/字典对象），其中键和值代表进程环境的字符串。如 `os.environ['HOME']` 返回主目录。
	- `os.environ` 是在 Python 启动时捕获的一份系统环境变量的“快照”；启动后系统环境的改变不会自动反映到它上面，但你可以直接修改 `os.environ` 来影响后续子进程的环境。
```python
>>> os.environ
environ({'SHELL': '/bin/bash', 'WINDOWID': '96469006', ..., 'http_proxy': 'http://127.0.0.1:7897', 'XDG_RUNTIME_DIR': '/run/user/1000', ...})
```