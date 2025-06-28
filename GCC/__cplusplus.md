`__cplusplus` 是 C++ 编译器自动定义的宏（如 `g++` 会定义，而 `gcc` 不会）
目的：用于检查编译器设置


| ISO 标准 | Value                        |
| ------ | ---------------------------- |
| C++98  | `#define __cplusplus 199711` |
| C++11  | `#define __cplusplus 201103` |
| C++14  | `#define __cplusplus 201402` |
| C++17  | `#define __cplusplus 201703` |
| C++20  | `#define __cplusplus 202002` |
| C++23  | `#define __cplusplus 202302` |

1. 条件编译，根据 C++ 版本代码选择不同的实现方式（或检测 C++ 环境）
```c
#ifndef __cplusplus
 #error C++ is required
#elif __cplusplus > 199711
 // C++11 (or newer) code goes here
#else
 // old code goes here
#endif
```

2. 让 C++ 编译器用 C 语言方式导出符号（许多 C 库需要在 C++ 项目中使用）
```c
#ifdef __cplusplus
extern "C" {
#endif

// some code

#ifdef __cplusplus
}
#endif
```
- `extern "C"` 告诉 C++ 编译器，用 C 的方式（而不是 C++ 的 name mangling）来编译以下声明。
- 用 `gcc` 编译时，宏 `__cplusplus` 未定义
- 用 `g++` 编译时，宏 `__cplusplus` 已定义，启用 `extern "C"` ，防止 C++ name mangling ，方便与 C 链接