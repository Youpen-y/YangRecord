`NumPy`（Numerical Python）—— Python 中用于科学计算的基础包。
一个 Python 库，提供多维数组对象、各种派生对象（如掩码数组和矩阵）以及用于快速操作数组的各种例程（routines）。

Why Numpy?
- 速度、内存
> Python List 是通用容器，可以“异构”，即包含各种类型的元素。
- 丰富的例程


基本数组类：`ndarray` （N-dimensional array）
```python
import numpy as np

print(np.__version__) # 查看 numpy 版本

arr = np.array([1, 2, 3, 4, 5]) # 使用列表创建数组
arr2 = np.array(（（0, 0), (1, 1))) # 使用元组创建数组；二维和更高维数组可以从嵌套的 Python 序列初始化

print(arr)
print(type(arr))
```
输出
```
2.0.2 
[1 2 3 4 5] 
[[0 0]
 [1 1]]
<class 'numpy.ndarray'>
```

数组和列表或列表的列表之间的区别：
- 列表的切片索引会将元素复制到一个新列表中，而数组的切片会返回一个 `view`（视图）：一个指向原始数组数据的对象。可以使用该视图修改原始数组。
- 数组元素可以通过在方括号中指定每个轴的索引来访问，轴之间用逗号分隔
```
>>> a[1, 3] # 访问第一行第三列
8
```

数组属性
- `ndarray.ndim`：数组的维数
- `ndarray.data`：指向数组数据开头的 Python 缓冲区对象
- `ndarray.shape`：数组的形状（一个非负元组），指定每个维度上的元素数量
- `ndarray.size`：数组中的元素数量
- `ndarray.dtype`：数组元素类型
- `ndarray.itemsize`：数组元素大小
- `ndarray.nbytes`：数组元素消耗的总字节数
```python
arr = np.array([[1, 2], [3, 4]])
print("arr {\n\t array dimensions: ", arr.ndim, \
            "\n\t buffer object->start of array's data: ", arr.data, \
            "\n\t array shape: ", arr.shape, \
            "\n\t data number: ", arr.size, \
            "\n\t data type: ", arr.dtype, \
            "\n\t item size: ", arr.itemsize, \
            "\n\t consumed: ", arr.nbytes,\
            "\n}")
```


不同维度的数组
- `0-D`数组：亦称标量，是数组中的元素。
- `1-D`数组：向量。
- `2-D`数组：矩阵或二阶张量。
- `3-D`数组：三维数组/三阶张量
```python
# 在创建数组时，使用 ndmin 参数定义维数
arr = np.array([1, 2, 3, 4, 5], ndmin=5)
print(arr)
print(arr.shape)
"""
[[[[[1 2 3 4 5]]]]] 
(1, 1, 1, 1, 5)
"""
```

数组切片：`[start:end:step]` 将每`step`步切片 `[start, end)`
- 未指定 `start` ，则视为 0
- 未指定 `end` ，则视为该维度的数组长度
- 未传递  `step`，则默认为 1

`NumPy` 数据类型
数值类型：
- 布尔型：`bool_`
- 整数：
	- 有符号：`int8`，`int16`，`int32`，`int64`
	- 无符号：`uint8`，`uint16`，`uint32`，`uint64`
- 浮点数：
	- `float16`（半精度）
	- `float32`（单精度）
	- `float64`（双精度，常见默认）
	- `float128`（扩展精度，依赖平台）
- 复数：
	- `complex64`（实部和虚部分别是 `float32`）
	- `complex128`（实部和虚部 `float64`）
	- `complex256`（扩展）
字符串与 Unicode
- `string_`（定长字节串，例如 `S10` 表示 10 字节）
- `unicode_`（定长 Unicode，例如 `U10` 表示 10 个字符）
对象类型
- `object_`（存放 Python 对象指针）
时间类型
- `datetime64`
- `timedelta64`

类型代码（type code）

| 代码    | 类型         |
| ----- | ---------- |
| `'b'` | int8       |
| `'B'` | uint8      |
| `'h'` | int16      |
| `'H'` | uint16     |
| `'i'` | int32      |
| `'I'` | uint32     |
| `'l'` | int64      |
| `'L'` | uint64     |
| `'f'` | float32    |
| `'d'` | float64    |
| `'F'` | complex64  |
| `'D'` | complex128 |
示例：
```python 
np.array([1,2,3], dtype='f')    # float32
```

字节序
- `<` 小端
- `>` 大端
- `=` 当前平台字节序
```python
np.dtype('<i4') # 小端 32 位整数
np.dtype('>f8') # 大端 64 位浮点数
```

`ndarray.astype()` - 转变现有数组的数据类型
```python
import numpy as np

arr = np.array([1.1, 2.1, 3.1])

newarr = arr.astype('i') # 或者 newarr = arr.astype(int)

print(newarr)
print(newarr.dtype)
```

---
Copy && View of array
===
区别：
- 数组的拷贝是一个新数组；而视图是原数组的视图。这意味着 Copy 拥有数据，而 View 不拥有数据。
```python
arr = np.array([1, 2, 3, 4, 5])
x = arr.copy()
y = arr.view()
arr[0] = 42

print(f"original array: {arr}")
print(f"copy array: {x}")
print(f"view array: {y}")

# 通过 ndarray 的 base 属性来判断是否拥有数据
print(x.base)   # None
print(y.base)   # [42 2 3 4 5]

"""
输出：
original array: [42 2 3 4 5] 
copy array: [1 2 3 4 5] 
view array: [42 2 3 4 5]
"""
```


数组重塑
===
通过重塑（Reshape），可以添加或删除维度，或者更改每个维度中的元素数量。

```python
numpy.reshape(a, newshape, order='C')
ndarray.reshape(newshape, /, *, order='C') 
# Returns an array containing the same data with a new shape. 
```
- `a`：要重塑的数组
- `newshape`：新的形状，可以是整数或整数元组
- `order`：读取和放置元素的顺序
	- `C`：按行优先（C风格）顺序
	- `F`：按列优先顺序
	- `A`：如果数组在内存中是 `Fortran` 连续的则用 `F`，否则用 `C`

- 1-D 到 2-D
```python
import numpy as np

arr = np.array([1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12])

newarr = arr.reshape(4, 3)
print(newarr)
print(newarr.shape)
print(newarr.base)  # View of original array

"""
[[ 1 2 3] 
 [ 4 5 6] 
 [ 7 8 9] 
 [10 11 12]] 
(4, 3)
[ 1 2 3 4 5 6 7 8 9 10 11 12]
"""
```
注意：重塑返回的数组是原数组的视图

在 `reshape` 里，必须保证 **新形状的各维度乘积 = 元素总数**。
未知维度（Unknown dimension）：使用 `-1` 让 `NumPy` 自动推导某个维度的大小。
```python
a = np.arange(12)
b = a.reshape(3, -1)    # -1 代表这个维度我不想算，你来算
print(b.shape)          # (3, 4)

c = a.reshape(-1, 6)
print(c.shape)

d = a.reshape(5, -1)    # 将报错：ValueError: cannot reshape array of size 12 into shape (5,newaxis)
```
限制条件：只能有一个 `-1` 。

- N-D to 1-D
```python
arr = np.array([[1, 2, 3], [4, 5, 6]])

# 展开
newarr = arr.reshape(-1) # 或：newarr = arr.reshape(6)
print(newarr)   # [ 0 1 2 3 4 5 6 7 8 9 10 11]

# C风格重塑（按行读取）
arr_c = arr.reshape(3, 2, order='C')
print("\nC风格 reshape(3, 2):")
print(arr_c)

# F风格重塑（按列读取）
arr_f = arr.reshape(3, 2, order='F')
print("\nF风格 reshape(3, 2):")
print(arr_f)

"""
[1 2 3 4 5 6] 
C风格 reshape(3, 2): 
[[1 2] 
 [3 4] 
 [5 6]] 

F风格 reshape(3, 2): 
[[1 5] 
 [4 3] 
 [2 6]]
"""
```
---
遍历：
- 遍历每个标量元素 - `nditer()`
```python
arr = np.array([[[1, 2], [3, 4]], [[5, 6], [7, 8]]])

for x in np.nditer(arr):
    print(x, type(x))
```
如果想在迭代时用别的类型：如 `S`
```python
arr = np.array([1, 2, 3])

for x in np.nditer(arr, flags=['buffered'], op_dtypes=['S'])
    print(x)
```
这里 `op_types` 允许在迭代时，将元素“视为”某种指定的数据类型（本例中，表示迭代时将元素转换成字符串类型）
为什么需要 `buffered` ？
数组里的元素在内存中并不是以字符串形式存的，如 `1, 2, 3` 实际存储为整数。如果强行将它们当作字符串来迭代，`NumPy` 需要
1. 额外的内存空间（缓冲区 buffer）来存放转换后的结果
2. 在迭代时，把每个元素从整数转换成字符串，放进缓冲区，再返回给迭代器

使用不同的步长迭代：（遍历二维数组的每个标量元素，跳过 1 个元素）
```python
arr = np.array([[1, 2, 3, 4], [5, 6, 7, 8]])

for x in np.nditer(arr[:, ::2]):
    print(x)
```

使用 `ndenumerate()` 进行枚举迭代 `ndarray enumerate`（迭代时显示元素索引）
```python
arr = np.array([[1, 2, 3, 4], [5, 6, 7, 8]])

for idx, x in np.ndenumerate(arr):
    print(idx, x)
    
"""
(0, 0) 1 
(0, 1) 2 
(0, 2) 3 
(0, 3) 4 
(1, 0) 5 
(1, 1) 6 
(1, 2) 7 
(1, 3) 8
"""
```
---
合并数组
1.  `concatenate()`
一维：
```python
arr1 = np.array([1, 2, 3])
arr2 = np.array([4, 5, 6])

print(np.concatenate((arr1, arr2)))
"""
[1 2 3 4 5 6]
"""
```
二维：
```python
arr1 = np.array([[1, 2], [3, 4]])
arr2 = np.array([[5, 6], [7, 8]])
print("arr1:\n", arr1)
print("arr2:\n", arr2)

arr = np.concatenate((arr1, arr2), axis=0)  # 按行拼接
print("按行拼接：\n", arr)

arr = np.concatenate((arr1, arr2), axis=1)  # 按列拼接
print("按列拼接：\n", arr)
"""
arr1:
[[1 2] 
 [3 4]]
arr2: 
[[5 6] 
 [7 8]]
按行拼接：
[[1 2] 
 [3 4] 
 [5 6] 
 [7 8]] 
按列拼接：
[[1 2 5 6] 
 [3 4 7 8]]
"""
```
2. `stack()` - 把数组沿着一个新的维度堆叠起来。
与 `concatenate()` 的区别是：`stack` 会增加一个新的维度。
```python
arr1 = np.array([1, 2, 3])
arr2 = np.array([4, 5, 6])

print(np.stack((arr1, arr2), axis=0))   # 在“行”方向
print(np.stack((arr1, arr2), axis=1))   # 在“列”方向
"""
[[1 2 3]
 [4 5 6]]
 
[[1 4]
 [2 5]
 [3 6]]
"""
```
3. `hstack()` 沿行堆叠；`vstack()` 沿列堆叠；
```python
arr1 = np.array([1, 2, 3])
arr2 = np.array([4, 5, 6])
arr = np.hstack((arr1, arr2))
print(arr)
"""
[1 2 3 4 5 6]
"""

varr = np.vstack((arr1, arr2))
print(varr)
"""
[[1 2 3]
 [4 5 6]]
"""

darr = np.dstack((arr1, arr2))
print(darr)
"""
[[[1 4]
  [2 5]
  [3 6]]]
"""
```
---
拆分数组
1. `array_split()`：`def array_split(ary, indices_or_sections, axis=0)`
```python
arr = np.array([1, 2, 3, 4, 5, 6])
newarr = np.array_split(arr, 3)
print(newarr)
"""
[array([1, 2]), array([3, 4]), array([5, 6])]
"""

newarr1 = np.array_split(arr, 4)
print(newarr1)
"""
[array([1, 2]), array([3, 4]), array([5]), array([6])]
"""
```
2. `hsplit()`
```python
arr = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9], [10, 11, 12], [13, 14, 15], [16, 17, 18]])  
newarr = np.hsplit(arr, 3)  
print(newarr)
"""
[array([[ 1],
       [ 4],
       [ 7],
       [10],
       [13],
       [16]]), array([[ 2],
       [ 5],
       [ 8],
       [11],
       [14],
       [17]]), array([[ 3],
       [ 6],
       [ 9],
       [12],
       [15],
       [18]])]
"""
```
---
搜素数组
1. `where()`
```python
arr = np.array([1, 2, 3, 4, 5, 4, 4])
x = np.where(arr == 4)
print(x)
""" 返回一个元组，其中包含了搜索值的索引
(array([3, 5, 6]),)
"""
```
2. `searchsorted()` - 在有序数组中进行二分查找，并返回在维持搜索顺序前提下插入该值的索引
原型：`def searchSorted(a，v，side='left'，sorter=none)`
```python
arr = np.array([6, 7, 8, 9])
x = np.searchsorted(arr, 5)
print(x)    # 0

y = np.searchsorted(arr, [2, 6, 10])
print(y)    # [0 0 4]
```
---
排序数组
1. `sort()`
```python
arr1 = np.array([3, 2, 0, 1])
print(np.sort(arr1))    # [0 1 2 3]

arr2 = np.array([[4, 9, 1], [5, 2, 6]]) # 在二维数组上使用 sort() 方法，两个数组都将被排序
print(np.sort(arr2))
"""
[[1 4 9]
 [2 5 6]]
"""
```
---
过滤数组
1. 使用布尔索引列表过滤数组
```python
arr = np.array([41, 42, 43, 44])
x = [True, False, True, False]

newarr = arr[x]

print(newarr)   # [41, 43]
```
2. 根据条件创建过滤列表
```python
arr = np.array([41, 42, 43, 44])

filter_list = []

for element in arr:
    if element > 42:
        filter_list.append(True)
    else:
        filter_list.append(False)

newarr = arr[filter_list]

print(filter_list)  # [False, False, True, True]
print(newarr)       # [43, 44]
```
3. 直接从数组创建过滤器
```python
arr = np.array([41, 42, 43, 44])

filter_arr = arr > 42

newarr = arr[filter_arr]

print(filter_list)  # [False, False, True, True]
print(newarr)       # [43, 44]
```
---
`NumPy` 使用 `random` 模块来处理随机数。
1. `rand`

