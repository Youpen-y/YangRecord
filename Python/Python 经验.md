1. 整除：`//` 运算符，等价于 `int((1 + 2) / 2)`
```python
>>> (1 + 2) / 2
1.5
>>> (1 + 2) // 2
1
```

2. 打开文件并查看每行内容
```python
with open(filename, "r") as f:
    for line in f:
        print(repr(line))
```