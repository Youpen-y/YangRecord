`alloca` - 分配自动释放的内存

```c
#include <alloca.h> // #include <stdlib.h>

void *alloca(size_t size);
```

描述
`alloca()` 函数在调用者的栈帧中分配 `size` 大小的空间。当调用 `alloca()` 的函数返回给它的调用者时，该临时空间会被自动释放。

返回值
`alloca()` 函数返回一个指向已分配空间的起始位置的指针。如果分配导致堆栈溢出，则程序行为未定义。

>[!NOTES]
>- `alloca()` 依赖机器和编译器。由于它从栈上分配，所以比 `malloc()/free()` 快
>- 简化使用 `longjmp()/siglongjmp()` 的应用程序中的内存释放
>- `alloca()` 分配的空间是在栈上分配的，如果函数返回被调用 `longjmp()/siglongjmp()` 跳过，该空间将自动释放
>- 如果指向 `alloca()` 的指针超出了作用域，那么 `alloca()` 分配的空间不会自动释放。
>- 不要试图释放由 `alloca()` 分配的空间
>- `alloca()` 是一个内置函数，意味着不可能通过链接到不同的库来获取其地址或更改其行为
>- 
>  