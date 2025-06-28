`__typeof__` 是 GCC 提供的一个 GNU C 扩展，用于在编译期获取一个表达式或变量的类型，类似于 C++ 的 `decltype` 。

语法：
```c
__typeof__(expression)
```
返回 `expression` 的类型，可以用来声明其他变量或函数，使其类型与 `expression` 一致。

示例：
1. 自动复制变量类型
```c
int x = 42;
__typeof__(x) y = 5; // y 的类型是 int
```
等价于
```c
int y = 5;
```
使用 `__typeof__` 可以在不知道变量类型的情况下复制它的类型，非常适合用于宏中。

2. 函数类型复制
```c
int add(int a, int b) { return a + b; }

__typeof__(add) another_add;
```
相当于声明：
```c
int another_add(int a, int b);
```

创建函数别名时配合 `__attribute__((alias))` 使用：
```c
extern __typeof__(add) sum __attribute__((weak, alias("add")));
```

3. 配合 `typeof(*(x))` 推导指针指向类型
```c
int *ptr;
__typeof__(*ptr) val = 10; // val 是 int 类型
```