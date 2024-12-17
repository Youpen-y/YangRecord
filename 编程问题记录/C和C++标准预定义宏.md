> `__func__` ：不是宏而是预定义变量，提供当前函数的名称

`__FILE__`：当前输入文件的名称，形式为一个 C 字符串常量。是预处理器打开文件的路径，而不是 `#include` 中指定的短名称或者作为输入文件名参数。如，`/usr/local/include/myheader.h` 是这个的可能扩展宏。

`__LINE__`：行号

```c
fprintf(stderr, "Internal error:"
                "negative string length"
                "%d at %s, line %d.", length, __FILE__, __LINE__);
```

`__DATE__`：预处理器正在运行的日期，宏扩展为字符串常量，包含 11 个字符，如“Feb 12 1996”

`__TIME__`：预处理器正在运行的时间，宏扩展为字符串常量，包含 8 个字符，如 “23:59:01”

`__STDC__`：在正常操作中，该宏扩展为常数 1 ，表示该编译器符合 ISO Standard C 。如果 GNU CPP 与 GCC 以外的编译器，This is not necessarily true。

`__STDC_VERSION__`：该宏扩展为 C 标准的版本号，一个长整数 `yyyymmL` 形式的常量，其中 `yyyy` 和 `mm` 是标准版本的年份和月份。如 `199409L` 表示 1994 年修订的 1989 C标准，这是当前的默认值。

`__cplusplus`：在使用 C++ 编译器时定义的，可以使用 `__cplusplus` 测试头文件是否由 C编译器编译或 C++ 编译器。

``