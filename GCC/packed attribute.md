`__attribute__((packed))` 是 GCC 和 Clang 提供的一个结构体内存布局控制属性，用于告诉编译器“不要添加对齐填充字节（padding）”，以节省内存空间，达到紧凑存储的目的。


示例：
```c
#include <stdio.h>

struct Normal {
	char a;
	int b;
};

struct __attribute__((packed)) Packed {
	char a;
	int b;
};

int main() {
    printf("sizeof(Normal) = %lu\n", sizeof(struct Normal)); // 输出通常为 8
    printf("sizeof(Packed) = %lu\n", sizeof(struct Packed)); // 输出为 5
}
```

使用方式
1. 匿名结构体 + `typedef`（常见）
```c
typedef struct {
	char a;
	int b;
} __attribute__((packed)) MyStruct;
```

2. 命名结构体
```c
struct __attribute__((packed)) MyStruct {
	char a;
	int b;
};

typedef struct MyStruct MyStruct;
```

3. 使用 `aligned` 搭配 `packed`
```c
struct __attribute__((packed, aligned(1))) Tiny {
	char a;
	int b;
};
```



