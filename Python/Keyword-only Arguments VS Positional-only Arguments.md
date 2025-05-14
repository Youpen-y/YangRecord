Keyword-only Arguments （仅关键字参数）和 Positional-only Arguments （仅位置参数）
- 用于限制参数传递方式的语法特性
- 允许开发者更精确地控制函数参数的传递规则，从而提高代码的可读性、健壮性和向后兼容性。

##### Keyword-only Arguments
- 仅关键字参数（`*`）从 Python 3.0 开始支持。
- 仅关键字参数必须在函数调用时通过参数名显式指定，不能通过位置传递。
- 在函数定义中，使用 `*` 号作为分隔符，`*` 之后的参数必须用关键字传递。
```python
def func(a, b, *, c, d):
	# a, b 可以是位置或关键字参数
	# c, d 必须通过关键字传递
	pass

def create_user(name, *, age, email):
	pass

# 必须显式指定 age 和 email
create_user("Alice", age=30, email="alice@example.com")
```

##### Positional-only Arguments
- 仅位置参数（`/`）在 Python 3.8+ 中引入。
- 仅位置参数必须在函数调用时通过位置传递，不能使用参数名显式指定。
- 在函数定义中，使用 `/` 符号作为分隔符，`/` 之前的参数必须通过位置传递。

```python
def func(a, b, /, c, d):
	# a, b 必须通过位置传递
	# c, d 可以是位置或关键字参数
	pass

def power(x, y, /):
	return x ** y

power(2, 3)     # 正确
power(x=2, y=3) # 错误：x, y 必须通过位置传递
```

##### 注意：
1. Python 版本兼容性
2. 参数顺序，`/` 必须在 `*` 之前
3. 使用场景
	1. 仅位置参数可能降低代码可读性，适合需要严格控制的场景（如底层库）
	2. 仅关键字参数更适合公开 API，强制用户明确关键参数



