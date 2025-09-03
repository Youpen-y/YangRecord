0. 版权声明
```python
# Copyright (C) 2025 Yang Yupeng
# Written by Yang Yupeng <yongy@outlook.com>

```
1. `__all__`：定义模块的公共 API，当其他文件使用 `from module import *` 时，只有 `__all__` 里列出的名字会被导入；如果模块里未定义，那将会导入所有非下划线开头的名字。
```my_module.py
__all__ = ['foo']

def foo():
    return "public"
    
def bar():
    return "private"
```
```test.py
from my_module import *
print(foo())   # ✅ 正常
print(bar())   # ❌ NameError: name 'bar' is not defined
```

2. **Tricks for Function**：当默认参数是可变对象，或构造成本较高时，用 `None` 作为哨兵值，再在函数内部按需实例化。
```python
# 函数模板
def func(arg: T | None = None) -> T:
    arg = <default_instance> if arg is None else arg


# 需要空列表
def f(x=None):
    x = x or []
    
# 需要空集合
def f(x=None):
    x = x or set()
    
# 需要空自定义对象
def f(x=None):
    x = x or MyContainer()
    
# 同时兼容 0、False 等合法假值
def f(x=None):
    x = [] if x is None else x  # 等价于| if x is None:
                                #      |    x = []
                                #      | else:
                                #      |    x = x    
    
# 更具体的例子
def f(x=None):
    x = x or []
    print(type(x))

f(0)    # <class 'list'>
f(1)    # <class 'int'>
f(False)    # <class 'list'>

print("---------------")

def func(x=None):
    x = [] if x is None else x
    print(type(x))
    
func(0) # <class 'int'>
func(1) # <class 'int'>
func(False) # <class 'bool'>
```

3. **Module: `dataclasses`**
引入：`Python 3.7`
Data Class（数据类）：通常是主要包含数据的类（但实际上并无限制）。
How TO Create Data Class? : 通过 `@dataclass` 装饰器创建。

| 对象                       | 类型  | 描述           |
| ------------------------ | --- | ------------ |
| `@dataclasses.dataclass` | 装饰器 | 修饰类以将其转换为数据类 |
| `dataclasses.field()`    | 函数  | 自定义数据类的各个字段  |
| `dataclasses.asdict()`   | 函数  | 将数据类实例转换为字典  |
| `dataclasses.astuple()`  | 函数  | 将数据类实例转换为元组  |

```python
from dataclasses import dataclass

@dataclass
class Point:
    x: int
    y: int

point = Point(1, 2)
print(point)    # Point(x=1, y=2)

# 默认情况下，dataclass 修饰的类对应的实例可变
point.x = 3
print(point)    # Point(x=3, y=2)
```

特点：
- 数据类为一些常见的特殊方法，如 `__init__()`，`__repr__()`，`__eq__()` 和 `__hash__()`，这使得无需编写关联的样板代码。
```python
# 传统写法
class Point:
    def __init__(self, x, y):
        self.x, self.y = x, y
    def __repr__(self):
        return f'Point({self.x!r}, {self.y!r})'
    def __eq__(self, other):
        return isinstance(other, Point) and (self.x, self.y) == (other.x, other.y)
"""
# 默认情况（@dataclass 不带 frozen）
# - 不生成 __hash__()，示实例不可哈希

# 显示要求
# - @dataclass(unsafe_hash=True): 强制生成 __hash__
# - @dataclass(frozen=True): 或继承的父类也是 frozen，自动生成可用的 __hash__，使其可哈希
"""
        
# 使用 @dataclass （✅相当简明）
from dataclasses import dataclass
@dataclass
class Point:
    x: int
    y: int
 ```
 - 提供一种创建不可变数据类的方法
```python
@dataclass(frozen=True)
class Point:
    x: int
    y: int

point = Point(1, 2)
point.x = 3 # <-- FrozenInstanceError
```
- 支持字段的默认值和默认工厂
```python
# 默认值，类似于在常规类的 __init__() 方法的定义中指定默认值
@dataclass
class Config:
    host: str = "0.0.0.0"
    port: int = 8000

# 默认工厂（每次实例化都会调用 list() 得到新列表，避免“共享可变默认参数”陷阱）
# def field(*, default=MISSING, default_factory_MISSING, init=True, repr=True, hash=None, compare=True, metadata=None, kw_only=MISSING)

from dataclasses import field
@dataclass
class Job:
    tasks: list[str] = field(default_factory=list)
    
# field() 函数除了 default_factory，还能挂 metadata, 比较（__eq__）/哈希(__hash__)/repr 开关
@dataclass
class User:
    id: int = field(metadata={"description": "主键"})
    password: str = field(repr=False)
```
- 将数据类实例转换为字典或元组以进行序列化
```python
from dataclasses import asdict, dataclass

@dataclass
class Point:
    x: int
    y: int
    
point = Point(5, 6)
print(asdict(point))    # {'x': 1, 'y': 2}
```

4. `assert` - 断言（调试程序）
```python
# 语法
assert expression[, assertion_message]
```
在条件不满足时，触发 `AssertionError`，并打印 `assertion_message`。
注意：
- 全局禁用断言：`python -0 program.py` 将删除所有 `assert` 语句。

5. `raise, try, except, else, finally` 关键字
- `raise`：触发指定异常
- `try`：监视执行
- `except`：捕获异常
- `else`：无异常是执行
- `finally`：不管有无异常，都执行
当在 `except` 子句中使用 `raise` 时，可以使用 `raise ExceptionName from None` 抛出指定的异常
```python
try:
    [][1]
except IndexError:
    raise ValueError("Bad index")
"""
IndexError                                Traceback (most recent call last)

[/tmp/ipython-input-3479684022.py](https://localhost:8080/#) in <cell line: 0>()
      1 try:
----> 2   [][1]
      3 except IndexError:

IndexError: list index out of range

During handling of the above exception, another exception occurred:

ValueError                                Traceback (most recent call last)

[/tmp/ipython-input-3479684022.py](https://localhost:8080/#) in <cell line: 0>()
      2   [][1]
      3 except IndexError:
----> 4   raise ValueError("Bad index")

ValueError: Bad index
"""

try:
    [][1]
except IndexError:
    raise ValueError("Bad index") from None
"""
ValueError                                Traceback (most recent call last)

[/tmp/ipython-input-3183076268.py](https://localhost:8080/#) in <cell line: 0>()
      2   [][1]
      3 except IndexError:
----> 4   raise ValueError("Bad index") from None

ValueError: Bad index
"""
```

5. 异常
- 创建自定义异常
```python

class MyException(Exception):
    """Docstring"""
    pass
```
- 异常继承树
```txt
BaseException
├── BaseExceptionGroup      # 创建一个异常组，包装任何异常
├── GeneratorExit           # 当 generator 或协程关闭时发生
├── KeyboardInterrupt       # 用户按下中断键组合（如`Ctrl+C`）
├── SystemExit              # 调用 sys.exit() 函数的结果
└── Exception
	├── ArithmeticError            # 算术错误异常
	│    ├── FloatingPointError
	│    ├── OverflowError         # 溢出
	│    └── ZeroDivisionError     # 除零
	├── AssertionError             # 断言错误
	├── AttributeError             # 当指定对象未定义尝试访问的属性或方法
	├── BufferError
	├── EOFError
	├── ExceptionGroup [BaseExceptionGroup]
	├── ImportError                        # import 语句无法加载模块
	│    └── ModuleNotFoundError           # import 语句无法定位模块（确保模块在 sys.path 中列出）
	├── LookupError                        # 查找错误异常
	│    ├── IndexError                    # 索引越界
	│    └── KeyError                      # 访问字典中不存在的键
	├── MemoryError
	├── NameError                  # 访问当前命名空间不存在的名称
	│    └── UnboundLocalError
	├── OSError
	│    ├── BlockingIOError
	│    ├── ChildProcessError
	│    ├── ConnectionError
	│    │    ├── BrokenPipeError
	│    │    ├── ConnectionAbortedError
	│    │    ├── ConnectionRefusedError
	│    │    └── ConnectionResetError
	│    ├── FileExistsError       # 尝试创建已存在的文件或目录
	│    ├── FileNotFoundError     # 文件不存在
	│    ├── InterruptedError
	│    ├── IsADirectoryError
	│    ├── NotADirectoryError
	│    ├── PermissionError       # 访问没有足够权限的文件或目录
	│    ├── ProcessLookupError
	│    └── TimeoutError
	├── ReferenceError
	├── RuntimeError
	│    ├── NotImplementedError       # 子类未实现抽象父类的方法
	│    ├── PythonFinalizationError
	│    └── RecursionError
	├── StopAsyncIteration
	├── StopIteration
	├── SyntaxError                    # 语法错误异常
	│    └── IndentationError          # 缩进错误异常
	│         └── TabError             # 混合使用制表符和空格符
	├── SystemError
	├── TypeError          # 将某个操作或函数应用于不支持的该操作的对象
	├── ValueError         # 操作或函数获得正确类型但不适当值的参数
	│    └── UnicodeError
	│         ├── UnicodeDecodeError
	│         ├── UnicodeEncodeError
	│         └── UnicodeTranslateError
	└── Warning
		├── BytesWarning
		├── DeprecationWarning        # 使用已废弃的功能
		├── EncodingWarning
		├── FutureWarning
		├── ImportWarning
		├── PendingDeprecationWarning
		├── ResourceWarning
		├── RuntimeWarning
		├── SyntaxWarning
		├── UnicodeWarning
		└── UserWarning
```