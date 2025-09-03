目的：从文档字符串（`docstring`）里提取代码示例，并运行这些示例来验证结果是否和文档中描述的一致。

> 简而言之，可以在函数、类、模块的文档中写一些交互式的 Python 示例，`doctest` 可以帮助检查这些示例能否正确执行并产生期望的结果。

两个常用接口：
- `doctest.testmod()`
作用：在**当前模块或指定模块**中查找 `docstring` 里的测试并运行。

- `doctest.testfile()`
作用：从一个外部 **文本文件** （通常是文档或说明文件）中读取并执行测试。文件内容必须是交互式 Python 示例的格式（`>>>`）。
示例：
```python
import doctest
doctest.testfile("example.txt")
```

示例：
```python
"""
This is a example module, show how to use doctest.

usage:
    python -m doctest -v example.py
"""

def add(a, b):
    """
    return sum of both num

    example:
    >>> add(2, 3)
    5
    >>> add(-1, 1)
    0
    """
    return a + b

if __name__ == "__main__":
    import doctest
    doctest.testmod(verbose=True)
```
运行得到：
```bash
Trying:
    add(2, 3)
Expecting:
    5
ok
Trying:
    add(-1, 1)
Expecting:
    0
ok
1 item had no tests:
    __main__
1 item passed all tests:
   2 tests in __main__.add
2 tests in 2 items.
2 passed.
Test passed.
```

倘若修改 `add(2, 3)` 期望结果为 6 ，将得到：
```bash
$ python example.py 
Trying:
    add(2, 3)
Expecting:
    6
**********************************************************************
File "/home/yongy/Templates/example-code-2e/01-data-model/my-test/example.py", line 13, in __main__.add
Failed example:
    add(2, 3)
Expected:
    6
Got:
    5
Trying:
    add(-1, 1)
Expecting:
    0
ok
1 item had no tests:
    __main__
**********************************************************************
1 item had failures:
   1 of   2 in __main__.add
2 tests in 2 items.
1 passed and 1 failed.
***Test Failed*** 1 failure.
```