—— 类方法，静态方法和实例方法的比较
实例方法（Instance methods）使用 `self` 对单个对象进行操作；类方法（class methods）使用 `cls` 访问类层级的数据；而静态方法（Static method）提供不依赖与类或实例状态的组织结构。


| 类型   | 装饰器             | 第一个参数  | 访问实例数据 | 访问类数据 | 用例                                                      |
| ---- | --------------- | ------ | ------ | ----- | ------------------------------------------------------- |
| 实例方法 | 不需要             | `self` | ✅      | ✅     | 操作实例数据                                                  |
| 类方法  | `@classmethod`  | `cls`  | ❌      | ✅     | Factory methods，alternative constructors, 或任何处理类级别数据的方法 |
| 静态方法 | `@staticmethod` | 无特殊参数  | ❌      | ❌     | 不需要实例或类数据的工具函数                                          |

示例：
```python
class MyClass:
    class_variable = "我是类变量"
    
    # 1. 实例方法（Instance Method）
    def instance_method(self):
        """需要实例调用，self 指向实例"""
        return f"实例方法，self 是 {type(self).__name__} 的实例"
    
    # 2. 类方法（Class Method）
    @classmethod
    def class_method(cls):
        """可以通过类或实例调用，cls 指向类本身"""
        return f"类方法，cls 是 {cls.__name__} 类"
    
    # 3. 静态方法（Static Method）
    @staticmethod
    def static_method():
        """既不需要 self 也不需要 cls"""
        return "静态方法，没有特殊参数"


# 调用方式对比
obj = MyClass()

# 实例方法：必须通过实例调用
print(obj.instance_method())  # ✅ 正常
# print(MyClass.instance_method())  # ❌ TypeError: 缺少 self 参数

# 类方法：可以通过类或实例调用
print(MyClass.class_method())  # ✅ 推荐方式
print(obj.class_method())      # ✅ 也可以，但不常用

# 静态方法：可以通过类或实例调用
print(MyClass.static_method())  # ✅ 推荐方式
print(obj.static_method())      # ✅ 也可以
```

类方法的常见用途
1. 替代构造器（Alternative Constructors）
```python
from datetime import datetime

class Person:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    @classmethod
    def from_birth_year(cls, name, birth_year):
        """Create Person from brith year"""
        age = datetime.now().year - birth_year
        return cls(name, age)   # call __init__

    @classmethod
    def from_string(cls, person_str):
        """Create Person from string"""
        name, age = person_str.split(',')
        return cls(name, int(age))

    def __repr__(self):
        return f"Person('{self.name}', {self.age})"
```
在 REPL 上执行：
```python
>>> from alternative_constructor import Person
>>> p1 = Person("Alice", 30)
>>> p2 = Person.from_birth_year("Bob", 1995)
>>> p3 = Person.from_string("Yong,24")
>>> print(f"{p1}\n{p2}\n{p3}")
Person('Alice', 30)
Person('Bob', 30)
Person('Yong', 24)
```

2. 工厂方法（Factory Methods）— 遵循 Don't Repeat Yourself 原则，提供多个创建对象的方式，让类的实例化更灵活、可读。如下，`margherita()` 和 `pepperoni()` 是工厂方法。
```python
class Pizza:
    def __init__(self, ingredients) -> None:
        self.ingredients = ingredients

    @classmethod
    def margherita(cls):
        """bake margherita pizza"""
        return cls(["mozzarella", "tomatoes", "basil"])

    @classmethod
    def pepperoni(cls):
        """bake pepperoni pizza"""
        return cls(["mozzarella", "tomatoes", "pepperoni"])

    def __repr__(self) -> str:
        return f"Pizza({self.ingredients})"
```