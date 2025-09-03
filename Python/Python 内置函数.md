- `type()` - 返回变量类型
- `id()` - 返回变量所指的内存地址
- `ord()` - 接受一个 `Unicode` 字符并返回一个整数（Unicode code point）
- `chr()` - 接受一个整数（Unicode code point）并返回该位置的字符


#### `map()` - “批量加工厂”
**映射**——将转换函数（transformation function）应用于可迭代对象以生成新的可迭代对象。
通俗讲：可在不使用 `for` 循环的情况下，把一个可迭代对象中的每个元素依次交给同一个函数处理，再逐一把结果吐出来。
```python
map(func, iterable[, iterable1, iterable2,..., iterableN])
```
- `func`：转换函数，单参数或多参数的函数（也可以是 `None`），其参数数量等于传递给 `map()` 的可迭代对象数
- `iterable`：一个或多个可迭代对象，元素按位置成组地喂给 `func`
- 返回值：惰性迭代器（Python 3）；想立即拿到结果需套 `list()`，`tuple()` 等
示例：
```python
# 1. 将 abs 应用到列表中的每个元素
numbers = [-2, -1, 0, 1, 2]
abs_values = map(abs, numbers)
print(abs_values)       # <map object at 0x7f7f2aefa860>
print(list(abs_values)) # [2, 1, 0, 1, 2]

# 2. 多迭代对象，转换函数必须接受和可迭代对象数目一样多的参数
a = [1, 2, 3]
b = [4, 5, 6]
print(list(map(pow, a, b))) # 计算 1^4, 2^5, 3^6
# [1, 32, 729]

# 3. 搭配 lambda 快速匿名函数
odd_nums = list(map(lambda x: x * 2 + 1, range(5)))
print(odd_nums) # [1, 3, 5, 7, 9]

# 4. 字符串处理 with str methods
string_it = ["processing", "strings", "with", "map"]
print(list(map(str.capitalize, string_it))) # ['Processing', 'Strings', 'With', 'Map']
print(list(map(str.upper, string_it))) # ['PROCESSING', 'STRINGS', 'WITH', 'MAP']

string_with_spaces = ["hello   ", "  world"]
print(list(map(str.strip, string_with_spaces))) # ['hello', 'world']

string_with_dots = ["hello...", "...world..."]
print(list(map(lambda s: s.strip('.'), string_with_dots))) # ['hello', 'world']

# 5. 

```