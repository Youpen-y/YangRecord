Python 的 f-string （格式化字符串字面量）
#### 基本语法
- 定义方式：在字符串前添加前缀 `f` 或 `F`，并在字符串内部使用 `{}` 包裹表达式
- 变量插入：直接引用变量名或表达式，运行时自动计算并转换为字符串

#### 表达式嵌入
f-string 支持在 `{}` 内嵌入任意合法表达式，包括：
- 算数运算：`{a + b}`
- 函数调用：`{name.upper()}`
- 对象属性/方法：`{user.get_age()}`
- 条件表达式（三元操作符）：`{"Adult" if age >= 18 else "Child"}`
- 字典键值：`{data["key"]}`

#### 格式规范
在 `{}` 中使用 `:` 指定格式规则，语法为 `{expression:format_spec}`，支持以下常见控制：
1. 数值格式化
- 小数精度：`{value:.2f}`（保留两位小数）
- 进制转换：`{num:b}`（二进制）、`{num:x}`（十六进制）
- 千位分隔符：`{amount:,}`（如 `1,000,000`）

2. 字符串对齐与填充
- 对齐方向：`<`(左对齐)，`>`(右对齐)，`^`(居中)
- 填充字符：指定填充字符（如 `*`），与对齐方向结合使用。

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