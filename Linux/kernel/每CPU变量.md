[Per-CPU](https://0xax.gitbooks.io/linux-insides/content/Concepts/linux-cpu-1.html)
Per-CPU variables，创建一个变量，每个处理器核心都有自己的该变量的副本。

两种Per-CPU 变量：静态和动态。
- 静态 Per-CPU 变量，在编译时使用宏定义，使用 `DEFINE_PER_CPU` 宏，将存储在 `.data..percpu` ELF section 部分；使用 `get_cpu_var()` 或 `per_cpu_ptr()` 访问。
- 动态 Per-CPU 变量，在运行时使用 `__alloc_percpu()` 或 `alloc_percpu()` 分配，`__alloc_percpu` 返回变量的每 CPU 地址。使用 `free_percpu(my_var)` 释放。
```c
int __percpu *myvar;
my_var = __alloc_percpu(sizeof(int), alignof(int));
```


| 类型  | 定义方式                          | 生命周期  | 存储位置              |
| --- | ----------------------------- | ----- | ----------------- |
| 静态  | `DEFINE_PER_CPU(type, name)`  | 编译时   | `.data..percpu` 段 |
| 动态  | `__alloc_percpu(size, align)` | 运行时分配 | Slab 分配器管理的区域     |





创建 Per-CPU 变量的 API —— `DEFINE_PER_CPU` 宏
```c
#define DEFINE_PER_CPU(type, name)	\
			DEFINE_PER_CPU_SECTION(type, name, "")

#define DEFINE_PER_CPU_SECTION(type, name, sec)	\
			__PCPU_ATTRS(sec) __typeof__(type) name

#define __PCPU_ATTRS(sec)	\
			__percpu __attribute__((section(PER_CPU_BASE_SECTION sec)))	\
			PER_CPU_ATTRIBUTES

#ifndef PER_CPU_BASE_SECTION
#ifdef CONFIG_SMP   // 支持 SMP（多核）
#define PER_CPU_BASE_SECTION ".data..percpu"
#else   // 单核
#define PER_CPU_BASE_SECTION ".data"
#endif
#endif
```
示例：
```c
DEFINE_PER_CPU(int, per_cpu_n);
```
展开为
```c
// step 1
DEFINE_PER_CPU_SECTION(int, per_cpu_n, "");

// step 2
__PCPU_ATTRS("") __typeof__(int) per_cpu_n

// step 3
__percpu __attribute__((section(PER_CPU_BASE_SECTION ""))) PER_CPU_ATTRIBUTES \
                __typeof__(int) per_cpu_n

// step 4
__percpu __attribute__((section(".data..percpu" ""))) PER_CPU_ATTRIBUTES \
                __typeof__(int) per_cpu_n

// final result
__percpu __attribute__((section(".data..percpu"))) PER_CPU_ATTRIBUTES int per_cpu_n;
```
