### 第二章
字符串相关（以 `name = "hello world"` 为例），以下方法均调用 `name.METHODS`，本身不会改变 `name` 的值，只返回修改后的值。 

| 方法                     | 含义                                 |
| ---------------------- | ---------------------------------- |
| `capitalize()`         | 将首字母大写，其余变小写。`Hello world`         |
| `casefold()`           | 更彻底的全小写转换（比 `lower()` 更强，处理更多语言情况） |
| `lower()`              | 转为小写。`hello world`                 |
| `upper()`              | 转为大写。`HELLO WORLD`                 |
| `swapcase()`           | 大小写互换。`HELLO WORLD`                |
| `title()`              | 每个单词首字母大写。 `Hello World`           |
| `istitle()`            | 判断是否是 `title` 形式                   |
| `isupper()`            | 判断是否全大写                            |
| `islower()`            | 判断是否全小写                            |
| 查找与判断                  |                                    |
| `find(sub)`            | 返回子串 `sub` 的索引，找不到返回 -1            |
| `rfind(sub)`           | 从右边开始找子串索引，找不到返回 -1                |
| `index(sub)`           | 和 `find` 类似，但找不到会报错                |
| `rindex(sub)`          | 和 `rfind` 类似，但找不到会报错               |
| `startswith(prefix)`   | 判断是否以某前缀开头                         |
| `endswith(suffix)`     | 判断是否以某后缀结尾                         |
| 内容判断                   |                                    |
| `isalnum()`            | 是否全是字母或数字                          |
| `isalpha()`            | 是否全是字母                             |
| `isdigit()`            | 是否全是数字                             |
| `isdecimal()`          | 是否全是十进制数字                          |
| `isascii()`            | 是否全是 ASCII 字符                      |
| `isnumeric()`          | 是否全是数字（包括汉字数字、罗马数字等）               |
| `isidentifier()`       | 是否是合法的标识符（变量名形式）                   |
| `isprintable()`        | 是否都是可打印字符                          |
| `isspace()`            | 是否全是空白字符（空格、换行、制表符）                |
| 修改与替换                  |                                    |
| `replace(old, new)`    | 替换子串                               |
| `removeprefix(prefix)` | 去掉前缀（如果有）                          |
| `removesuffix(suffix)` | 去掉后缀（如果有）                          |
| `strip()`              | 去掉首尾空白                             |
| `lstrip()`             | 去掉左侧空白                             |
| `rstrip()`             | 去掉右侧空白字符（包括空格、`\t`，`\n`）           |
| 对齐与填充                  |                                    |
| `center(width)`        | 居中，并用空格填充                          |
| `ljust(width)`         | 左对齐，右侧填充                           |
| `rjust(width)`         | 右对齐，左侧填充                           |
| `zfill(width)`         | 左侧填充 `0`                           |
| 分割与拼接                  |                                    |
| `split(sep)`           | 按分隔符切分为列表                          |
| `rsplit(sep)`          | 从右边开始切分                            |
| `splitlines()`         | 按行切分，返回一个列表，包含文件中所有的行              |
| `partition(sep)`       | 分隔成（前、分隔符、后）三元组                    |
| `rpartition(sep)`      | 从右侧开始分隔                            |
| `join(list)`           | 用字符串 `sep` 把序列拼接成一个字符串             |
| 格式化                    |                                    |
| `format()`             | 字符串格式化                             |
| `format_map(dict)`     | 用字典映射来格式化                          |
| 编码与转换                  |                                    |
| `encode(encoding)`     | 编码为字节串                             |
| `maketrans()`          | 创建字符映射表（供 `translate()` 用）         |
| `translate(table)`     | 根据映射表替换字符                          |
| 其他                     |                                    |
| `count(sub)`           | 统计子串出现次数                           |
| `expandtabs(tabsize)`  | 将 `\t` 替换为空格（默认 8 个空格）             |

### 第四章
列表相关（以 `test_list = []` 为例）

| 方法                              | 含义                                                            |
| ------------------------------- | ------------------------------------------------------------- |
| `append(x)`                     | 在列表末尾添加一个元素。如 `test_list.append(50)`                          |
| `extend(iterable)`              | 将另一个可迭代对象（如列表、元组）里的元素逐个添加到当前列表。如 `test_list.extend([60, 70])` |
| `insert(i, x)`                  | 在指定位置 `i` 插入元素 `x`。如 `test_list.insert(1, 90)`                |
| `remove(x)`                     | 删除第一个匹配到的元素 `x`（若不存在将报错）                                      |
| `pop([i])`                      | 移除并返回指定位置 `i` 的元素（不传参数则默认最后一个）。注意：其中 `[]` 是可选标志               |
| `clear()`                       | 清空整个列表                                                        |
| `index(x, [start, end]`         | 返回第一个 `x` 元素的下标（可选范围 `start`，`end`）                           |
| `count(x)`                      | 统计元素 `x` 出现的次数                                                |
| `sort(key=None, reverse=False)` | 对列表进行排序                                                       |
| `reverse()`                     | 将列表元素 反转顺序（并不排序）                                              |
| `copy()`                        | 浅拷贝列表                                                         |
> 切片表示法 `[:]` 将创建列表的副本，适用于将列表传递给函数但不想修改列表本身时。示例：`print_models(unprinted_designs[:], completed_models)`

### 第八章
- 可变参数
	- `*形参`（位置可变参数，`var-positional parameter`）—— 用来接收任意数量的位置参数（普通参数），函数内部会把它们打包成一个 `tuple`。
```python
def func(*args):
    print(args)
    print(type(args))
func(1, 2, 3, "hello")
# (1, 2, 3, 'hello')
# <class 'tuple'>
```
- `**形参`（关键字可变参数，`var-keyword parameter`）—— 用来接收任意数量的关键字参数（形如 `key=value` 的参数），函数内部会把它们打包成一个 `dict` 。
```python
def func(**kwargs):
    print(kwargs)
    print(type(kwargs))
    
func(name="Tom", age=20, city="Beijing")
# {'name': 'Tom', 'age': 20, 'city': 'Beijing'}
# <class 'dict'>
```

当结合使用位置实参和任意数量的实参时，注意将匹配“任意数量的实参”的形参放在最后。Python 将先匹配位置实参和关键字实参，再将余下的实参都收集到最后一个形参中。

- 拆包用法（函数调用时）—— 在调用函数时，使用 `*` 和 `**` 拆包容器里的参数，
```python
def func(x, y, z):
    print(x, y, z)
    
lst = [1, 2, 3]
d = {"x": 10, "y": 20, "z": 30}

func(*lst)  # 拆包序列，等价于 func(1, 2, 3)
func(**d)   # 拆包字典，等价于 func(x=10, y=20, z=30)
```

两种导入方法
1. `import module_name` —— 将 `module_name` 模块中的所有函数复制到当前程序中。
```python
import module_name

module_name.function_name() # 调用方式
```

2. `from module_name import function_0, function_1, function_2` —— 导入特定的函数
```python
from module_name import function_0, function_1, function_2

function_0()    # 直接调用
```

3. 使用 `as` 给模块指定别名
```python
import module_name as mn

mn.function_name()
```

4. 使用 `as` 给函数指定别名
```python
from module_name import function_name as fn

fn()
```

5. 导入模块中的所有函数
```python
from module_name import *

function_name()
```

### 第九章
继承（Inheritance）表示一个类从另一个类获取属性和方法。

基本语法：
```python
class Parent:
    def greet(self):
        print("Hello from Parent")

class Child(Parent):
    pass

c = Child()
c.greet()   # Hello from Parent
```

- 方法重写（Override）—— 子类可以重写父类的方法，覆盖掉父类的实现
```python
class Parent:
    def greet(self):
        print("Hello from Parent")
        
class Child(Parent):
    def greet(self):
        print("Hello from Child")

c = Child()
c.greet()   # Hello from Child
```

- `super()` 函数 —— 有时子类重写了方法，但仍想在里面调用父类的实现，这时用 `super()`
常见用法：在子类 `__init__` 里调用父类的 `__init__`，确保父类初始化逻辑也执行。
```python
class Animal:
    def __init__(self, name):
        self.name = name
        print(f"Animal created: {self.name}")
        
class Dog(Animal):
    def __init__(self, name, breed):
        super().__init__(name)  # 调用父类构造函数
        self.breed = breed
        print(f"Dog created: {self.name}, breed {self.breed}")
        
d = Dog("Buddy", "Labrador")

# 输出
# Animal created: Buddy
# Dog created: Buddy, breed Labrador
```

- 多重继承和 `super()`
Python 支持多继承，此时 `super()` 会根据 `MRO` （Method Resolution Order，方法解析顺序）决定调用链。
```python
class A:
    def show(self): print("A")
    
class B(A):
    def show(self):
        print("B")
        super().show()
    
class C(A):
    def show(self):
        print("C")
        super().show()
        
class D(B, C):
    def show(self):
        print("D")
        super().show()
        
d = D()
d.show()

# 输出
# D
# B
# C
# A
```
Python 使用 `C3` 线性化算法 计算 `MRO` ，保证每个父类只被调用一次。
`C3` 算法的核心思想：
1. 优先保证继承链的局部顺序（类定义时的父类顺序）
2. 每个类只出现一次
3. 子类优先于父类，保证一致性

```
# 输入： Class C, 以及它的父类列表 Parents
# 输出：C 的 MRO（方法解析顺序）

function MRO(C):
    if C has no parents:
        return [C, object]  # 没有父类时，默认继承 object
        
    # 各父类的 MRO
    parent_mros = [MRO(P) for P in Parents]
    
    # 在末尾再加一份 Parents 列表，保证局部顺序
    return [C] + merge(parent_mros + [Parents])
    
function merge(seqs):
    result = []
    
    while True:
        # 如果所有序列都空了，结束
        if all seq is empty for seq in seqs:
            return result

        # 在每个序列的第一个元素中，找一个符合条件的 candidate
        for seq in seqs:
            if seq is empty: 
                continue
            candidate = seq[0]

            # 检查 candidate 是否出现在其他序列的尾部
            if candidate not in tail of any other non-empty seq:
                break
            else:
                candidate = None

        if candidate is None:
            raise Error("Inconsistent hierarchy - no valid MRO")
        
        # 把 candidate 加入结果
        result.append(candidate)

        # 从所有列表中删除 candidate
        for seq in seqs:
            if seq is not empty and seq[0] == candidate:
                remove first element of seq
```


### 第十章
`pathlib`
- 读写文本
```python
from pathlib import Path

path = Path("file_name.txt")
contents = path.read_text()
path.write_text("Hello World")  # 会清除原有文件内容

with path.open("a", encoding="utf-8") as f:
    f.write("appended contents\n")
```
注意： Python 只能将字符串写入文本文件。如果要将数值存储到文本文件中，必须使用函数 `str()` 将其转换为字符串格式。

- `try-except` - 如果 `try` 代码块中的代码运行无问题，`Python` 将跳过 `except` 代码块；如果 `try` 代码块中的代码导致错误，`Python` 将查找与之匹配的 `except` 代码块并运行其中的代码。
- `json.dumps()` 和 `json.loads()`
`dumps` = dump string （转储为字符串）
`loads` = load string （从字符串加载）
`json.dumps()` 将 Python 对象转换成 JSON 格式的字符串。
`json.loads()` 将 JSON 格式的字符串解析为 Python 对象。
```python
from pathlib import Path
import json

data = {"name": "Alice", "age": 25, "is_student": False}

path = Path('table.json')
json_str = json.dumps(data)
path.write_text(json_str)

contents = path.read_text()
table = json.loads(contents)
```

### 第十一章
`pytest` ：Python 的流行测试框架之一。
- 测试文件，必须以 `test_` 开头。当使用 `pytest` 运行测试时，将查找 `test_` 开头的文件，并运行其中的所有测试。
- 测试函数，必须以 `test_` 开头。在测试过程中，`pytest` 将找出并运行所有以 `test_` 开头的函数
- 在测试函数中调用被测试的函数
- 通过断言（assertion）判断

| 断言                           | 用途                    |
| ---------------------------- | --------------------- |
| `assert a == b`              | 断言两个值相等               |
| `assert a != b`              | 断言两个值不等               |
| `assert a`                   | 断言 `a` 的布尔求值为 `True`  |
| `assert not a`               | 断言 `a` 的布尔求值为 `False` |
| `assert element in list`     | 断言元素在列表中              |
| `assert element not in list` | 断言元素不在列表中             |

示例：
```python
# test_sample.py
def func(x):
    return x + 1

def test_answer():
    assert func(3) == 4
```
测试：
```bash
$ pytest 
============================= test session starts ==============================
platform linux -- Python 3.13.3, pytest-8.4.1, pluggy-1.6.0
rootdir: /home/yongy/Templates/test/project8
plugins: anyio-4.9.0, nbval-0.11.0
collected 1 item                                                               

test_sample.py .                                                         [100%]

============================== 1 passed in 0.01s ===============================
```

`fixture`（夹具） = 测试前准备 + 测试后清理，创建供多个测试使用的资源。
在 `pytest` 中，要创建夹具，需要编写一个使用装饰器 `@pytest.fixture` 装饰的函数。
```python
import pytest

@pytest.fixture
def sample_data():
    print("prepare data")
    return {"name": "Alice", "age": 25}
    
def test_name(sample_data):
    assert sample_data["name"] == "Alice"
    
def test_age(sample_data):
    assert sample_data["age"] == 25
```