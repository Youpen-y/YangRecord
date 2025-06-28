`offsetof` - 某个结构体成员的偏移

```c
#include <stddef.h>

size_t offsetof(type, member);
```

描述
宏 `offsetof()` 在不实例化结构体的情况下，计算成员（`member`）相对于结构体（`type`）起始地址的偏移量。以字节为单位。

可能的实现
```c
// 可能的实现
#define offsetof(type, member) ((size_t)&(((type *)0)->member))
```
- `(type *)0`：构造一个“指向地址为 0 的该结构体的指针”，不会真的解引用。
-  `&(...)`：取该字段的地址。
- `->member`：访问结构体字段。
- 最终得到的是“该字段相对于结构体起始地址（0）”的偏移