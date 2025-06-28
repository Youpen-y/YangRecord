### 切莫死板追求一致性
> A foolish consistency is the hobgoblin of little minds. —— 爱默生

Guido：“代码读取的频率比编写代码的频率要高得多。”
一致性工作很重要，项目内的一致性更重要，一个模块或函数内的一致性是最重要的。


### 代码布局
缩进：4空格
Tabs or Spaces ? ：空格是首选的缩进方法；Python 不允许混合 tabs 和 spaces 进行缩进
最大行长度：行最大长度限制为 79 个字符

### 二元运算符前还是后换行？
Python 代码中，只要约定一致，可以在二元运算符之前或之后中断。对于新代码，建议使用 Knuth 样式。
```Python
# Knuth 样式：二元运算符之前中断
income = (gross_wages
          + taxable_interest
          + (dividends - qualified_dividends)
          - ira_deduction
          - student_loan_interest)
```


### 空行
- 顶级函数/类（不在任何其他函数或类中定义的函数或类）前后各**空两行**
```python
import os


def my_function():
    pass


class MyClass:
    pass
```
- 类内部的函数前后**空单行**
```python
class MyClass:
    def method_one(self):
        pass

    def method_two(self):
        pass
```
- 函数内部，使用空行来指示逻辑部分

### 源文件编码
核心 Python 发行版中的代码应始终使用 `UTF-8` ，并且不应具有编码声明。
在标准库中，非 UTF-8 编码应仅用于测试目的。谨慎使用非 ASCII 字符，最好只用于表示地点和人名。

### 导入
- 导入放在单独的行上
```python
# Correct:
import os
import sys

# Wrong:
import sys, os

# Correct:
from subprocess import Popen, PIPE
```
 - 导入总是放在文件的顶部，在任何模块注释和文档字符串之后，以及模块全局变量和常量之前。
	 - 导入顺序：标准库 > 第三方导入 > 特定于本地应用程序/库的导入（每组导入之间放置一个空行）
- 建议使用绝对导入（可读性好、更好的错误消息提示），标准库代码应避免复杂的包布局，并始终使用绝对导入。
```python
import mypkg.sibling
from mypkg import sibling
from mypkg.sibling import example
```
以下是显示相对导入——使用 `.` 和 `..` 表示相对路径导入包内模块。
```python
from . import sibling           # 导入同一目录的 sibling.py
from ..subpackage import tool   # 导入上层包中的 subpackage.tool
```
示例结构：
```python
project/
|---package/
|   |---utils/
|   |   |---helper.py
|   |---core/
|       |---processor.py
```
在 `processor.py` 中想用 `helper.py`
- 绝对导入： `from package.utils import helper`
- 相对导入：`from ..utils import helper`

### 模块级别的 `Dunder` 变量名
模块级别使用的双下划线开头和结尾（`__xxx__`）的特殊变量名。`Dunder` 是 `double underscore` 的缩写。
如:
```
__name__
__file__
__all__
__version__
__author__
```
模块级别的 `dunders` 应该放在模块文档字符串（`docstring`）之后，但在所有 import 语句之前，除了 `from __future__ import ...` 导入语句可以在它们之前。
示例：
```python
"""模块的说明文档字符串（module docstring）"""

from __future__ import annotations  # 特殊允许的未来导入放最前面

__author__ = "Yang"
__version__ = "0.0.1"
__all__ = ["foo", "bar"]

import os
#...
```

### 字符串引用
在 Python 中，单引号字符串和双引号字符串是相同的。尽量使用同一种。
对于三引号字符串，请始终使用双引号字符。

### 表达式和语句中的空格
赋值（或其他）运算符周围有多个空格，以将其与另一个运算符对齐
```python
# Correct:
x = 1
y = 2
long_variable = 3

# Wrong:
x             = 1
y             = 2
long_variable = 3
```

其他建议
- 避免在任何地方尾随空格
- 始终在两侧使用单个空格将这些二元运算符括起来：assignment(`=`), augmented assignment(`+=`, `-=` etc), comparisons(`==`, `<`, `>`, `!=`, `<=`, `>=`, `in`, `not in`, `is`, `is not`), Booleans(`and`, `or`, `not`)。
- 如果使用了不同优先级的运算符，考虑在优先级最低的运算符周围添加空格。
- 函数注解（在函数定义中标注参数和返回值类型）中冒号前不加空格，冒号后视情况而定
```python
# Correct:
def add(x: int, y: int) -> int:
    return x + y

# Wrong:
def add(x : int, y : int) -> int:
```
- 当 `=` 用于函数参数默认值或关键字参数调用时，不要在 `=` 两边加空格。
```python
# Correct
def connect(host, port=3306, user='root'):
    pass

connect(host='localhost', port=5432)    # 函数调用
```
但，对于带类型注解的参数默认值，`=` 周围使用空格。
```python
# Correct
def get_date(limit: int = 100) -> list: ...
def munge(sep: AnyStr = None): ...
def munge(input: AnyStr, sep: AnyStr = None, limit=1000): ...
```
- 不建议使用复合语句
```python
# Correct:
if foo == 'blah':
    do_blah_thing()
do_one()
```

### 何时使用尾随逗号？
尾随逗号通常是可选的，但在创建单元素元组时是必需的。
```python
# Correct:
FILES = ('setup.cfg',)

# Wrong:
FILES = 'setup.cfg',
```

- 多行结构 —— 加
- 单行结构 + 末尾是闭括号 —— 不加
- 单个元素元组 —— 必须加
- 将来可能频繁添加项的结构 —— 加
```python
# Correct:
fruits = [
	'apple',
	'banana',
	'cherry',  # 添加新元素后，git diff 不会将原来行标为修改
]

numbers = [1, 2, 3]
singleton = (42,)

# Wrong:
numbers = [1, 2, 3,]
```

### 注释
- 及时更新
- 句子完整，第一个单词大写（除非是标识符）
- 块注释通常由一个或多个由完整句子构成的段落组成，每个句子以句点结尾。
- 在多句子注释中，您应该在句子结束句点后使用 1 个或 2 个空格，但最后一个句子之后除外。

#### 块注释
块注释通常说明它后面的一段代码的用途或逻辑，并且它的缩进应当和这段代码对齐。
```python
def process_input():
    # 处理用户输入：
    # - 去除空格
    # - 转换大小写
    # - 过滤非法字符
```
#### 内联注释
仅必要时使用

#### 文档字符串
Documentation Strings （`docstrings`）是出现在模块、类、函数、方法内部的**字符串文字**，用于说明**它的用途、行为或参数**，可以被 `help()` 函数读取。
1. 为所有**公共**模块、函数、类和方法编写文档字符串。私有方法（以`_`开头的名称）用注释替代 `docstring`
2. 多行 `docstring` 要用独立的一行放结尾的 `"""`
```python
def add(x, y):
    """Return the sum of x and y."""
    return x + y

def _helper():
    # 仅加普通注释即可
    pass

def fetch_data():
    """
    Fetch data from the remote API.

    This will raise an exception if the network is unavailable.
    """
```

### 命名约定
- `_single_leading_underscore`： 弱内部使用，内部变量，不推荐外部访问，在使用 `from module import *`时，不会导入该变量。
```python
# module.py
_internal_value = 42    # 表内部使用
```
- `single_trailing_underscore_`： Python 不能用保留字（如 `class`, `def`, `lambda`）作为变量名。如果你真的需要使用类似的名字，可以加一个尾部下划线 `_`。
- `__double_leading_underscore`：触发 **名称修饰（name mangling）**，让变量在类外部更难被访问。Python 将把 `__name` 变成 `_ClassName_name`，以避免子类覆盖或意外访问。
```python
class Foo:
    def __init__(self):
        self.__secret = 123  # 实际变成 _Foo__secret

f = Foo()
print(f._Foo__secret)  # 可以访问（但不推荐）

# print(f.__secret) 会报错：AttributeError
```
- `__double_leading_and_trailing_underscore__`：这些名字（如 `__init__`, `__str__`, `__len__`）由 Python 解释器调用。你**不要自定义其他类似的名称**，否则可能和系统行为冲突。

#### 包和模块名
模块应具有简短的全小写名称。如果下划线可以提高可读性，则可以在模块名称中使用下划线。Python 包也应该具有简短的全小写名称，但不建议使用下划线。

当一个用 C 或 C++ 编写的扩展模块（也叫“底层模块”）有一个配套的 Python 模块来封装它，即提供一个更高级别的接口时，这个 C/C++ 扩展模块的名称应该以下划线开头。如 `_socket`。
如：
模块 `_socket` 由 C 编写，提供底层原始接口，性能高但难用
模块 `socket` 由 Python 编写，提供更人性化、面向对象的封装接口。

#### 类名
类名通常用 `CapWords` 约定。首字母大写 +  单词拼接。
```python
class MyClass:
    pass

class BankAccount:
    pass
```
有些类虽然是属于 Class，但主要用途是“被调用”而非建模对象，它就更像是函数。这种类可使用函数风格 `snake_case` 命名。
```python
class make_something:
    def __call__(self):
        # 可调用对象
        pass

make = make_something()
make()  # 像函数一样使用
```
内建名称命名规则

| 内建元素 | 命名风格         | 示例                           |
| ---- | ------------ | ---------------------------- |
| 内建函数 | 全小写（或两个单词拼接） | `int`, `len`, `str`, `super` |
| 异常类  | `CapWords`   | `ValueError`, `TypeError`    |
| 常量   | 全大写（少数情况）    | `True`, `False`, `None`      |

#### 类型变量名
类型变量（Type Variables）命名规范：`CapWords`，首选短名称：`T`, `AnyStr`, `Num` 。
如果在声明协变（covariant）或逆变（contravariant）类型变量时：
- 建议在类型变量名后添加 `_co`或 `_contra` 后缀
	- `_co`：协变
	- `_contra`：逆变
```python
from typing import TypeVar

T_co = TypeVar('T_co', covariant=True)
T_contra = TypeVar('T_contra', contravariant=True)
```

#### 全局变量名称
如果全局变量仅用于一个模块，这些约定与函数的约定大致相同（即 `snake_case`）。
如果模块是专为 `from M import *` 使用而设计的，应使用 `__all__` 机制来限制导出哪些名称。
- 在 Python 中，使用 `from M import *`时，会默认导入模块中所有不以下划线 `_` 开头的名称。
- 但即使这样，可能也会引入一些不想暴露的变量、函数等
解决方案：使用 `__all__` 显式声明哪些名称是“公共的”：
```python
__all__ = ['public_func', 'ImportantClass']

def public_func(): ...
def _internal_helper(): ...
```

#### 函数和变量名称
- 函数/变量的名称应该小写，必要时使用下划线来提高可读性。
- 仅对非公共方法（non-public） 和实例变量使用一个前导下划线
- `mixedCase` 仅在保持向后兼容性时被允许使用。

#### 函数和方法参数
- 实例方法的第一个参数必须命名为 `self`。
```python
class MyClass:
    def method(self, arg):
        # self 指的是该方法所属的实例
        print(self, arg)
```
虽然技术上可以用别的名字（如 `this`），但根据 Python 社区约定统一用 `self`，`self` 表示“这个实例本身”便于可读性和一致性。
- 类方法的第一个参数应命名为 `cls`
```python
class MyClass:
    @classmethod
    def create(cls, arg):
        return cls(arg) # cls 表示类本身
```
类方法接收的不是实例，而是类对象本身；`cls` 是 `self` 的类级别对应物。
- 如果函数参数名与 Python 的关键字冲突，推荐在参数名后添加一个下划线 `_` ，而不是缩写或拼写扭曲。当然，最好选用不冲突的同义词。
```python
def make_class(class_):
    print(f"Creating {class_}")
```

- 使用双前导下划线 `__` 来触发 Python 的名称修饰机制以避免子类中的名称冲突. 如果有人想访问这些变量,可以使用 `类名 + 变量名` 形式访问.
```python
class Base:
    def __init__(self):
        self.__secret = 42  # 会被重命名为 _Base_secret

class Child(Base):
    def __init__(self):
        super().__init__()
        self.__secret = 99  # 会被重命名为 _Child_secret

c = Child()
print(c.__secret)       # X AttributeError
print(c._Child_secret)  # 99
print(c._Base_secret)   # 42
```

#### 常量
常量通常在模块级别定义。全大写，或以下划线分隔。如 `MAX_OVERFLOW`，`ToTAL`。

### 继承的设计
1. 决定属性的可见性
	1. 公开（`public`）属性：期望“无关用户”直接访问，且承诺以后不会做向后不兼容的更改。
	2. 非公开（`non-public`）属性：不打算让外部使用，不做任何稳定性保证。一般在名称前加一个单下划线 `_` 即可。
	3. 子类 API （`protected`）属性：如果一个类专门给子类继承、扩展或修改行为，你需要明确哪些属性是留给子类使用的。这类属性常用单下划线或文档注明，但不一定用双下划线。

| 属性种类               | 命名形式     | 语义                    |
| ------------------ | -------- | --------------------- |
| 公开（public）         | `name`   | 保证稳定、供外部直接使用          |
| 关键字冲突              | `name_`  | 必要时在末尾加一个下划线，避免与关键字冲突 |
| 非公开（non-public）    | `_name`  | 仅内部使用，外部请勿访问          |
| 子类私有（name-mangled） | `__name` | 触发名称修饰，避免子类无意覆盖       |

>真私有并不存在：Python 不支持真正的私有属性，双下划线只是名称修饰（name mangling），主要用于防止子类意外冲突，调试或有需要时仍然可以通过 `_ClassName_attr` 访问。


2. 简单数据属性 vs 访问器（accessor/mutator）
	1. 简单公开数据 —— 直接使用属性，不要为每个字段写一堆 `get_x()/set_x()`
	2. 未来如需功能增强 —— 可使用 `@property` 将逻辑隐藏在属性访问语法下 
```python
class Circle:
    def __init__(self, radius):
        self._r = radius

    @property
    def radius(self):
        # 以后可以加入参数检查、缓存等逻辑
        return self._r
```

### 公共接口与内部接口
- 兼容性保证仅适用于公共接口
- 清晰区分公共接口与内部接口
- 凡在文档中出现的接口，默认视为公共接口，除非文档中明确标注该接口是“试验性”或“内部专用”，不纳入兼容性保证。
- 用 `__all__` 明确声明公共 API。为了支持自动化工具和代码补全，模块应在源代码中用 `__all__` 列出公共名称。若模块没有公共接口，可将 `__all__ = []`。

### 编程建议
- 代码的编写方式不会损害 Python 的其他实现（`PyPy`, `Jython`, `IronPython`, `Cython`, `Psyco`等）

- 与像 `None` 这样的单例进行比较时，应该总是用 `is` 或 `is not`，而不是等号运算符（`==` 或 `!=`）
```python
# 单例（singleton）对象：None/True/False ，的比较应使用身份运算符 `is` 和 `is not`
# 单例对象只有一个实例，即在整个 Python 进程中只有一个对象，使用 is 比较的是身份（identity），即两个引用是否真的指向同一个对象。
# `==` 会调用对象的 `__eq__` 方法。如果你对某些类型重载了等号比较，很可能会得到意想不到的结果；而 `is` 永远比较的是对象的地址，安全可靠。

# 检查变量 x 是否是 None
if x is None:
    ...

if y is not None:
    ...
```

- 当使用富比较（rich comparisons）时，最好实现所有操作（`__eq__`, `__ne__`, `__lt__`, `__le__`, `__gt__`, `__ge__`），而不是依赖其他代码来执行特定的比较。仅在非常简单且不在意性能的场景下，才考虑使用 `@total_ordering`来减少重复代码。
```python
class Version:
    def __init__(self, major, minor, patch):
        self._v = (major, minor, patch)

    def __eq__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._v == other._v

    def __ne__(self, other):
        return not self == other

    def __lt__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._v < other._v

    def __le__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._v <= other._v

    def __gt__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._v > other._v

    def __ge__(self, other):
        if not isinstance(other, Version):
            return NotImplemented
        return self._v >= other._v
```

- 始终使用 `def` 语句，而不是将 `lambda` 表达式直接绑定到标识符。
```python
# Correct:
def f(x): return 2*x

# Wrong:
f = lambda x: 2*x
```

- Python 中自定义异常时，应继承自 `Exception`，而不要直接继承自 `BaseException`。

- 异常链（Exception Chaining）—— 当捕获一个异常，并在 except 块中抛出另一个异常时使用 `raise NewException from OriginalException`方式。
```python
try:
    do_something()
except ValueError as e:
    raise CustomError("更明确的信息") from e
```

- 当捕获异常时应尽可能指定具体的异常类型，而避免使用裸 `except`（即不指定任何异常类型的通用捕获语句）。
> 如果确实有必要捕获所有异常（如在一个守护服务或日志系统中），建议至少用 `except Exception:` 而不是裸 `except:`

- 在捕获操作系统错误（如文件操作、网络访问等）时，应优先使用 Python 3.3 引入的显式异常类层级，而不是通过 `errno` 的值来判断错误类型。
> 从 Python 3.3 开始，`OSError` 及其子类覆盖了大多数操作系统相关错误。如 `FileNotFoundError`、`PermissionError`、`IsADirectoryError`、`NotADirectoryError`、`BlockingIOError`、`ChildProcessError`。

- 对于所有 `try/except` 子句，限制 `try` 子句到绝对最少必要代码量。
```python
# Correct:
try:
    value = collection[key]
except KeyError:
    return key_not_found(key)
else:
    return handle_value(value)

# Wrong:
try:
    # Too broad!
    return handle_value(collection[key])
except KeyError:
    # Will also catch KeyError raised by handle_value()
    return key_not_found(key)
```

- 当资源是特定代码段的本地资源时，请使用 `with` 声明，以确保使用后及时可靠地清理。`try/finally` 语句也可接受。
当一个资源（如文件、网络连接、锁等）只在代码的一小段范围内使用时，应使用 `with` 语句来确保它在使用后被及时且可靠地释放。如果 `with` 不适用，也可以用 `try/finally` 实现相同的资源清理逻辑。
```python
with open("data.txt") as f:
    contents = f.read()
# 文件自动关闭，无需手动调用 f.close()
# with 自动调用资源的 `__enter__()` 和 `__exit__()` 方法

# 替代方案
f = open("data.txt")
try:
    contents = f.read()
finally:
    f.close()
```

- 每当上下文管理器（`with` 语句）执行除获取和释放资源之外的操作时，应通过单独的函数或方法调用上下文管理器。 `with` 语句明确只用于资源管理，函数封装负责逻辑。
```python
# 单独定义一个上下文管理器函数
def locked_resource():
    lock.acquire()
    try:
        yield
    finally:
        lock.release()

# 使用 contextlib.contextmanager 简化写法
from contextlib import contextmanager

@contextmanager
def managed_database_connection():
    conn = connect_db()
    try:
        yield conn
    finally:
        conn.close()

# 在其他地方使用
with managed_database_connection() as conn:
    conn.query("SELECT * FROM users")
```

- `return` 语句保持一致。要么函数中所有返回语句应该返回一个表达式，或者都不返回。如果一个返回语句返回了表达式，则没有值返回的语句应该显式声明为 `return None`，并且显式 `return` 语句应放在函数末尾。
```python
# Correct:

def foo(x):
    if x >= 0:
        return math.sqrt(x)
    else:
        return None

def bar(x):
    if x < 0:
        return None
    return math.sqrt(x)

# Wrong:

def foo(x):
    if x >= 0:
        return math.sqrt(x)
```

- 在检查字符串前缀或后缀时，使用 `str.startwith()` 和 `str.endwith()`，而不是字符串切片。
```python
filename = "example.txt"

if filename.endswith(".txt"):
    print("This is a text file.")

if filename.startswith("ex"):
    print("This starts with 'ex'.")

# 检查多个可能的前缀/后缀
if filename.endswith((".txt", ".md", ".rst"))
    print("This is a readable text file")
```

- 对象类型比较应该始终使用 `isinstance()`，而不是直接比较类型
```python
def process(item):
    if isinstance(item, str):
        print("This is a string")

# 1. 如果对象是给定类型的子类实例， isinstance() 会返回 True
# 2. 支持传入多个类型，例如 isinstance(x, (int, float))
# 3. 支持多态，与类层次结构良好配合

# Wrong:
if type(item) == str:
    ...
```

- 对于序列类型（如字符串、列表、元组），空序列为 `False`
```python
if items:
    print("This list is not empty")

if not message:
    print("The message is empty.")

# Wrong:
if len(seq):
if not len(seq):
```

- 不要使用 `==` 将布尔值与 `True` 或 `False` 进行比较
```python
# Correct:
if greeting:

# Wrong:
if greeting == True:

# Wrose:
if greeting is True:
```

- 不建议在 `try...finally` 的 `finally` 代码块中使用 `return/break/continue` 等流程控制语句，尤其当它们跳出 `finally` 块时。
```python
def example():
    try:
        raise ValueError("Something went wrong")
    finally:
        return 42   # X 把异常吞掉了

print(example())    # 输出 42，而不是抛出异常

for i in range(3):
    try:
        raise Exception("Oops")
    finally:
        break   # X 会吞掉异常

# 推荐做法：不在 finally 中使用跳出结构的语句，尤其当存在异常传播时
```