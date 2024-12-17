## 可变参函数（Variadic Function）
ISO C 定义了一种语法，用于声明函数以接受可变数量或类型的参数。（此类函数称为可变参数函数）。但语言本身没有为此函数提供访问其非必须参数的机制；相反，可以使用定义的变量参数宏 `stdarg.h`。

使用场景：（某些函数执行的操作可以有意义地接收不定数量的参数，k）
- 如 `printf` 系列打印函数
```c
int printf(const char *format, ...);
// 函数在 format 的控制下打印参数（不定参数目）
```
- 如自定义函数 `init_array` 用初始化列表初始化
```c
void init_array(int *array, int count, ...);
void init_array(int *array, int count, ...) {
    va_list args;
    va_start(args, count); // 初始化为 args 为最后一个参数之后的参数

    for (int i = 0; i < count; ++i) {
        array[i] = va_arg(args, int); // 获取可变参数列表中的下一个参数
    }
    va_end(args); // 清理工作
}

// int *arr = malloc(5 * sizeof(int)); // 分配空间
// init_array(arr, 5, 1, 2, 3, 4, 5);
```

---
可选参数没有名称，只有`...`。访问可选参数的唯一方法是按顺序（它们的写入顺序），并使用来自 `<stdarg.h>` 的宏做如下几步：
1. 使用 `va_start` 初始化 `va_list` 类型的参数指针变量。初始化后的参数指针指向第一个可选参数。
```c
// Macro
void va_start(va_list ap, last-required)
//  该宏将 ap 初始化为指向当前函数的第一个可选参数；last-required 必须是函数的最后一个必需的参数。
```
`va_list` 是一种数据类型。它像一种特殊的列表，包含了传递给函数的所有额外参数。
2. 通过调用 `va_arg` 来访问可选参数，第一次调用 `va_arg` 会给出第一个可选参数，第二次调用给出第二个。（可随时停止，忽略剩余的可选参数）
```c
// Macro
type va_arg(va_list ap, type)
// va_arg 宏返回下一个可选参数的值，并修改 ap 的值以指向后续参数。va_arg 返回值的 type 是调用中指定的 type。而 type 必须是与实际参数的类型匹配的自我提升类型（不是 char 或 short int 或 float）
```
3. `va_end` - 结束 `va_list` 并清理内容
```c
// Macro
void va_end(va_list ap)
// ap 的使用结束。在 va_end 调用后，使用同一 ap 进行进一步的 va_arg 调用可能不起作用。
// 在 GNU C 库中，va_end 不执行任何操作，除非出于可移植性的原因，否则不需要使用。
```

---
有时需要多次解析参数列表，或者想要记住参数列表中的某一位置。为此，必须复制参数的当前值。但 `va_list` 是一种不透明类型，不一定将 `va_list` 类型的一个变量的值分配给同一类型的另一个变量。
- `va_copy` - 用于复制  `va_list`
```c
// Macro
void va_copy(va_list dest, va_list src)
// va_copy 宏允许复制 va_list 类型的对象，即使不是整型。dest 中的参数指针被初始化为指向与 src 中的指针相同的参数。
```

由于原型没有指定可选参数的类型，因此在调用可变参数函数是，将对可选参数值执行默认参数提升。（char/short int => int/unsigned int, float => double）
### 调用约定
函数并没有通用方法来确定调用它时使用的可选参数的数量和类型。=》 设计可变参函数时的调用约定
- 将可选参数的数量作为固定参数之一传递。（所有参数都具有相同类型）
```c
void my_func(char *str, int count, ...);
```
- 在必须的参数之一提供可选的参数的位掩码
```c
#define PARAM_INT 0x01
#define PARAM_FLOAT 0x02
#define PARAM_DOUBLE 0x04
#define PARAM_CHAR 0x08

void process_arguments(int params, ...);
int main() {
    int params = PARAM_INT | PARAM_FLOAT;
    int int_val = 42;
    float float_val = 3.14f;

    process_arguments(params, int_val, float_val);
    return 0;
}

void process_arguments(int params, ...)
{
    va_list args;
    va_start(args, params);

    if (params & PARAM_INT) {
        int value = va_arg(args, int);
        printf("Integer value: %d\n", value);
    }
    if (params & PARAM_FLOAT) {
        float value = va_arg(args, double); // 注意：float在va_arg中按double处理
        printf("Float value: %f\n", value);
    }
    if (params & PARAM_DOUBLE) {
        double value = va_arg(args, double);
        printf("Double value: %lf\n", value);
    }
    if (params & PARAM_CHAR) {
        char value = va_arg(args, int); // char在va_arg中按int处理
        printf("Char value: %c\n", value);
    }

    va_end(args); // 清理工作}
```
- 将必需参数用作指定可选参数的数量和类型的模式（`printf` 函数）
- 传递“end marker” 值作为最后一个可选参数（`execl` 函数）

示例2 - 求多参数和
```c
#include <stdarg.h>
#include <stdio.h>

int add_em_up (int count, ...)
{
    va_list ap;
    int i, sum;

    va_start(ap, count);    // Initialize the argument list
    sum = 0;
    
    for (i = 0; i < count; i++) {
        sum += va_arg(ap, int); // Get the next argument value
    }

    va_end(ap);     // Clean up
    return sum;
}

int main(void) {
    printf("%d\n", add_em_up(3, 5, 5, 6));
    printf("%d\n", add_em_up(10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10));
    return 0;
}
```


## 可变参宏 (Variadic Macro)
使用省略号 `...` 作为最后一个参数。语法如下：
```c
#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#define CHECK(x, ...) if(!(x)) {printf(__VA_ARGS__);}

// ... 来替换 __VA_ARGS__
```

关于 尾随逗号
Trailing Commas: 在 C99 及更高版本中，如果没有参数传递给可变参数宏，则可能会因尾随逗号而导致语法错误。
解决方案：
- 至少提供一个参数
- GCC扩展（不可移植）：`##__VA_ARGS__` removes the `,`
其中`##` 是一个预处理操作符，称为“粘贴”操作符（token-pasting operator）。可以将两个预处理标记（tokens）粘贴在一起，好像它们是单个标记一样。
```c
#define debug(M, ...) fprintf(stderr, M "\n", ##__VA_ARGS__)

/*
debug("message"); will be expanded to fprintf(stderr, "message");, 而不是 fprintf(stderr, "message",);
*/
```
- `__VA_OPT__`：如果存在一个或多个可变参数，则 `__VA_OPT__(x)` 扩展为 `x` ；否则，它扩展为空。
```c
#define LOG(fmt, ...) printf(fmt __VA_OPT__(,) __VA_ARGS__)
// 如果在没有附加参数的情况下调用 LOG, 则省略逗号，以防止语法错误。
#define M(X, ...) X __VA_OPT__(,) __VA_ARGS__
// M(3) 扩展为 3， 而 M(3, 4) 扩展为 3, 4
```







