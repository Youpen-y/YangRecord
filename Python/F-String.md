Python 的 f-string （formatted string literals, 格式化字符串字面量）—— 简化字符串方法 `format()` 的使用。

`format()` 使用示例：
```python
a = 123
b = 'abc'
print('{} and {}'.format(a, b))     # 123 and abc
print('{first} and {second}'.format(first=a, second=b)) # 123 and abc
```
#### 基本语法
- 定义方式：在字符串前添加前缀 `f` 或 `F`，允许直接在字符串内部使用 `{}` 包裹变量或表达式
- 变量插入：直接引用变量名或表达式，运行时自动计算并转换为字符串

和普通字符串字面量一样，`f-string` 可以使用单引号 `''` ，双引号`""` 和三引号 `'''`、`"""`
#### 表达式嵌入
f-string 支持在 `{}` 内嵌入任意合法表达式，包括：
- 算数运算：`{a + b}`
- 函数调用：`{name.upper()}`
- 对象属性/方法：`{user.get_age()}`
- 条件表达式（三元操作符）：`{"Adult" if age >= 18 else "Child"}`
- 字典键值：`{data["key"]}`

#### 格式规范
在 `{}` 中使用 `:` 指定格式规则，语法为 `{expression:format_spec}`，支持以下常见控制：
`f'{obj!s}'`：对象将使用 `str()` 转换为字符串（调用对象的 `__str__` 方法）
`f'{obj!r}'`：对象将使用 `repr()` 转换（调用对象的 `__repr__` 方法）
`f'{obj!a}'`：对象将使用 `ascii()` 转换

1. 数值格式化
- 百分号：`{value:.2%}`（保留两位小数的百分数）
```python
f = 0.123
print(f'percent: {f:.2%}`)
# percent: 12.30%
```
- 小数精度与科学计数：`{value:.2f}`（保留两位小数）；`{value:.3g}`（保留 3 位有效数字）；`{value:.3e}`（3 位小数科学计数）；`{value:.3E}`
```python
f = 12.3456
print(f'decimal places    : {f:.3f}')
print(f'significant digits: {f:.3g}')
# decimal places    : 12.346
# significant digits: 12.3

print(f'exponent(lower): {f:.3e}')
print(f'exponent(upper): {f:.3E}')
# exponent(lower): 1.235e+01
# exponent(upper): 1.235E+01
```
- 进制转换：`{num:b}`（二进制）、`{num:x}`（十六进制）
```python
i = 255
print(f'bin        : {i:b}')
print(f'oct        : {i:o}')
print(f'hex(lower) : {i:x}')
print(f'hex(upper) : {i:x}')
# bin        : 11111111
# oct        : 377
# hex(lower) : ff
# hex(upper) : FF

print(f'bin       : {i:08b}')
print(f'oct       : {i:08o}')
print(f'hex(lower): {i:08x}')
print(f'hex(upper): {i:08X}')
# bin       : 11111111
# oct       : 00000377
# hex(lower): 000000ff
# hex(upper): 000000FF

print(f'bin       : {i:#010b}')
print(f'oct       : {i:#010o}')
print(f'hex(lower): {i:#010x}')
print(f'hex(upper): {i:#010X}')
# bin       : 0b11111111
# oct       : 0o00000377
# hex(lower): 0x000000ff
# hex(upper): 0X000000FF
```

- 千位分隔符：`{amount:,}`（如 `1,000,000`）

2. 字符串对齐与填充
- 对齐方向：`<`(左对齐)，`>`(右对齐)，`^`(居中)
- 填充字符：指定填充字符（如 `*`），与对齐方向结合使用。
```python
s = 'abc'

# 使用 * 填充
print(f'right : {s:*>8}')
print(f'center: {s:*^8}')
print(f'left  : {s:*<8}')

# 使用 0 填充
i = 1234
print(f'zero padding: {i:08}')
# zero padding: 00001234

# 使用分隔符 
i = 1234567890
print(f'comma: {i:,}')
# comma: 1,234,567,890
```

#### 多行字符串
使用 `"""` 或 `'''` 定义多行 f-string

```python
pi = 3.14159
num = 25134151

print(f"圆周率: {pi:.2f}")  # 保留两位小数
print(f"二进制: {num:b}")  # 转换为二进制
print(f"十六进制: {num:x}")  # 转换为十六进制
print(f"千位分隔符: {num:,}")  # 添加千位分隔符

text = "Hello"
print(f"左对齐: {text:<10}")  # 左对齐，宽度为10
print(f"右对齐: {text:>10}")  # 右对齐，宽度为10
print(f"居中对齐: {text:=^10}")  # 居中对齐，宽度为10

name = "Bob"
message = f"""
Hello, {name}!
Today's tasks:
- Task 1: {"Completed" if True else "Pending"}
- Task 2: {"Pending"}
"""
print(message)
```

- 日期和时间（`datetime`）
```python
import datetime

dt = datetime.datetime(2020, 1, 5, 20, 15, 30)

print(f'datetime: {dt}')
# datetime: 2020-01-05 20:15:30

print(f'datetime: {dt:%A, %m/%d/%Y %I:%M:%S %p}')
# datetime: Sunday, 01/05/2020 08:15:30 PM

print(f'datetime: {dt.isoformat()}')
# datetime: 2020-01-05T20:15:30
```

注意：要在 `f-string` 中包含括号 `{` 和 `}` ，需要加倍使用 `{{` 和 `}}`，反斜杠无效。
```python
n = 123
print(f'{{}}-{n}-{{{n}}})
# {-}-123-{123}
```

从 `Python 3.8` 之后，`f-string` 支持一个 `=` 指定符，它将打印变量名及其对应的值。
```python
i = 123

print(f'{i=}')
# i=123

print(f'{i = }')    # 空格将被保留
# i = 123

print(f'{i = :#b}')
# i = 0b1111011
```