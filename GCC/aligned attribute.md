`__attribute__((aligned()))` 是 GCC 编译器特有的扩展属性，用于控制变量、结构体成员或类型的内存对齐方式。

语法
```c
__attribute__((aligned(n)))
```
其中 `n` 是对齐的字节数，必须为 2 的幂次方（1, 2, 4, 8, 16, 32, 64 ...）。

使用方式
1. 变量对齐
```c
// 将变量对齐到 16 字节边界
int var __attribute__((aligned(16)));

// 在声名时使用
__attribute__((aligned(16))) int var;
```

2. 结构体对齐
```c
// 整个结构体按 32 字节对齐
struct data {
	int a;
	char b;
} __attribute__((aligned(32)));

// 结构体成员对齐
struct data {
    int a __attribute__((aligned(8)));
    char b;
};
```

3. 类型定义对齐
```c
typedef struct {
	int x;
	int y;
} point_t __attribute__((aligned(16)));
```

实际应用场景
- SIMD 优化
```c
// SSE/AVX 指令要求数据 16/32 字节对齐
float vector[4] __attribute__((aligned(16)));
```

- 缓存行对齐
```c
// 避免 false sharing ，按缓存行大小对齐
struct cache_line_data {
	int counter;
	char padding[60];
} __attribute__((aligned(64)));
```

- DMA 缓冲区
```c
// DMA 操作通常需要特定对齐
char dma_buffer[1024] __attribute__((aligned(32)));
```




