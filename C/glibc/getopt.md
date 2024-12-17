`getopt`, `getopt_long`, `getopt_long_only`, `optarg`, `optind`, `opterr`, `optopt` - 解析命令行选项

概要

``` c
#include <unistd.h>

int getopt(int argc, char *const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

// optstring 示例
// "a:bcdf::"
//  a 后跟随一个冒号，表明该选项必须紧跟一个参数，如 -a value
//  bcd 后不带冒号，表明该选项不需要参数。如 -a 是一个有效的选项
//  f 后跟随两个冒号，表明该选项可以跟一个参数，也可以不跟。如果提供了参数，getopt 会将其存储在 optarg 中；如果没有提供参数，optarg 将为 NULL 
```

描述
`getopt()` 函数解析命令行参数。它的参数 `argc` 和 `argv` 是在程序调用时传递给 `main()` 函数的参数计数和数组。`argv` 中以 `-` 开头的元素（不完全是`-`或`--`）是一个选项元素。此元素的字符（除去首字母`-`）是**选项字符**。如果重复调用 `getopt()`，它将连续返回每个选项元素中的每个选项字符。

变量 `optind` 是 `argv` 中下一个要处理的元素的索引。系统将此值设置为 1 。调用者可以将其重置为 1 以重新开始扫描同一个 `argv`，或者扫描一个新的参数向量。

如果 `getopt()` 找到另一个选项字符，它将返回该字符，更新外部变量 `optind` 和静态变量 `nextchar`，以便下一次调用 `getopt()` 时可以继续扫描剩下的选项字符或 argv-element。

如果没有更多的选项字符，`getopt()` 返回 -1 。然后 `optind` 是 `argv` 中第一个不是选项的 argv-element。

`optstring` 是包含合法选项字符的字符串。如果这样一个字符后面跟一个冒号，则该选项需要一个参数，因此 `getopt()` 在 `optarg` 中放置一个指针，指向同一个 argv-element 中有文本（即，与选项名称本身相同的单词，如"-oarg"），则它将在 `optarg` 中返回，否则 `optarg` 将被设置为 0 。

GNU 扩展，如果 `optstring` 包含 `W` 后跟一个分号，那么 `-W foo` 将视为长选项 `--foo`。（`-W` 选项由POSIX.2保留用于实现扩展，此行为是GNU扩展，在glibc 2之前的库中不可用）。

默认情况下，`getopt()`会在扫描时对 `argv` 的内容进行置换，因此最终所有的nonoptions都位于末尾。

如果 `getopt()` 不能识别选项字符，它将向 `stderr` 输出错误消息，将字符存储在 `optopt` 中，并返回`'?'`。调用程序可以通过将`opterr`设置为0来防止错误消息。

如果 `getopt()` 在 `argv` 中找到一个选项字符，而它没有包含在`optstring` 中，或者如果它检测到一个缺少的选项参数，它将返回`'?'`并将外部变量`optopt`设置为实际的选项字符。如果`optstring`的第一个字符（跟在上面描述的任何可选的'+'或'-'之后）是冒号（':'），那么`getopt()`返回':'而不是'?'指示缺少选项参数。如果检测到错误，并且`optstring`的第一个字符不是冒号，并且外部变量`opterr`为非零（这是默认值），则`getopt()`打印错误消息。

---
长选项

``` c
#include <getopt.h>

int getopt_long(int argc, char *const argv[],

                const char *optstring,

                const struct option *longopts, int *longindex);

  

int getopt_long_only(int argc, char *const argv[],

                    const char *optstring,

                    const struct option *longopts, int *longindex);

```


`getopt_long()` 函数的工作方式与 `getopt()` 类似，只是它也接受以两个破折号开头的长选项。（如果程序只接受长选项，则 `optstring` 应指定为空字符串（""），而不是`NULL`。）如果缩写是唯一的，则可以缩写长选项名称 或者与某个定义的选项完全匹配。一个长的选项可以接受一个参数，形式为`--arg=param` 或 `--arg param`。


`longopts` 指向`<getopt.h>`中`struct option`数据第一个元素的指针

``` c
struct option {
    const char *name;       // the name of the long option

    int         has_arg;    /* no_argument(or 0): option does not take an argument.

                               required_argument(or 1): option require an argument

                               optional_argument(or 2): option takes an optional argument

                            */

    int        *flag;       /* flag - specifies how results are returned for a long option

                                NULL: getopt_long() returns val

                                otherwise, returns 0, and flag points to a variable which is set to val if the option is found, but left unchanged if the option is not found.

                            */

    int         val;        // the value to return, or to load into the variable pointed to by flag

};

```

数组的最后一个元素必须用零来填充。

如果 `longindex` 不为 `NULL`，则它指向一个变量，该变量被设置为 `long` 选项相对于 `longopts` 的索引。

`getopt_long_only()` 类似于 `getopt_long()`，但`'-'` 和 `"--"` 可以表示长选项。如果一个以`'-'`（而不是`"--"`）开头的选项不匹配长选项，但匹配短选项，则会将其解析为短选项。


返回值

如果一个选项被成功找到，那么 `getopt()` 返回选项字符。如果所有命令行选项都已解析，则`getopt()`返回-1。如果 `getopt()` 遇到一个不在 `optstring` 中的选项字符，那么返回 `'?'`。如果 `getopt()` 遇到缺少参数的选项，则返回值取决于`optstring` 中的第一个字符：如果是 `':'`，则返回 `':'`;否则返回`'?'`。

`getopt_long()` 和 `getopt_long_only()` 在识别短选项时也返回选项字符。对于长选项，如果`flag`为`NULL`，则返回`val`，否则返回0。Error和-1返回值对于`getopt()`相同，加上`'?'`表示不明确的匹配或无关参数。