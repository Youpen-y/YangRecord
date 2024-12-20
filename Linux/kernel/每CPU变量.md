[Per-CPU](https://0xax.gitbooks.io/linux-insides/content/Concepts/linux-cpu-1.html)
Per-CPU variables，创建一个变量，每个处理器核心都有自己的该变量的副本。

创建 Per-CPU 变量的 API —— `DEFINE_PER_CPU` 宏
```c
#define DEFINE_PER_CPU(type, name)	\
			DEFINE_PER_CPU_SECTION(type, name, "")

#define DEFINE_PER_CPU_SECTION(type, name, sec)	\
			__PCPU_ATTRS(sec) PER_CPU_DEF_ATTRIBUTES	\
			__typeof__(type) name

#define __PCPU_ATTRS(sec)	\
			__percpu __attribute__((section(PER_CPU_BASE_SECTION sec)))	\
			PER_CPU_ATTRIBUTES

#define PER_CPU_BASE_SECTION ".data..percpu"
```
示例：
```c
DEFINE_PER_CPU(int, per_cpu_n);
```