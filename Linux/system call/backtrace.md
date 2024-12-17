`backtrace`, `backtrace_symbols`, `backtrace_symbols_fd` - 支持应用程序自调试

``` c
#include <execinfo.h>

int backtrace(void **buffer, int size);

char **backtrace_symbols(void *const *buffer, int size);

void backtrace_symbols_fd(void *const *buffer, int size, int fd);
```
`backtrace()` 将调用者的回溯返回到 `buffer` 指向的数组中。而回溯是程序的一系列当前活跃的函数调用。`buffer` 数组中每一项都是 `void` 类型，并且是来自相应 `stack frame` 的返回地址。`size` 参数指定 `buffer` 中可以存储的最大地址数。如果回溯大于 `size` ，则返回最近 `size` 个函数调用对应的地址。要获得完整的回溯，需要确保 `buffer` 和 `size` 足够大。
给定 `backtrace()` 在 `buffer` 中返回的地址集，`backtrace_symbols()` 将地址转换为以符号描述地址的字符串数组。`size` 参数指定 `buffer` 中的地址数。每个地址的符号表示由函数名称（如果可以确定的话）、函数的十六进制偏移量和实际返回地址（十六进制）组成。字符串指针数组的地址作为 `backtrace_symbols` 的结果返回，该数组由 `backtrace_symbols` 分配，必须由调用者释放。（数组中的字符串指针不需要也不应该被释放）
`backtrace_symbols_fd()` 与 `backtrace_symbols()` 采用相同的 `buffer` 和 `size` ，但它不是向返回字符串数组，而是将字符串（每行一个）写入文件描述符 `fd` 。`backtrace_symbols_fd()` 不调用 `malloc` ，因此 `malloc` 可能调用失败的情况下使用。

----
返回值
`backtrace()` 返回 存入`buffer` 中的地址数，不大于 `size` 。如果返回值小于 `size` ，则存储完整的回溯；如果返回值等于 `size` ，则可能已被截断。

成功时，`backtrace_symbols()` 返回一个指针，指向调用 `malloc` 分配的指针；出错时，返回 `NULL` 。

---
Notes
这些函数对函数的返回地址如何存储在堆栈上做了一些假设。

- 省略栈指针（如 `gcc` 的非零优化等级隐含的那样或加入了栈指针优化参数 `-fomit-frame-pointer`）可能会导致违反这些假设
- 内联函数没有`stack frame`
- 尾部调用优化（`tail call optimization`）导致一个 `stack frame` 替换另一个
如果不适用特殊的链接器选项，符号名称可能不可用。对于使用 GNU 链接器的系统，**必须使用 `-rdynamic` 链接器选项**。请注意，静态函数的名称不会公开，并且在回溯中不可用。

---
Example
``` c
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 100
void myfunc3(void) {
    int j, nptrs;
    void *buffer[SIZE];
    char **strings;

    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

    strings = backtrace_symbols(buffer, nptrs);
    if(strings == NULL){
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for(j = 0; j < nptrs; j++){
        printf("[%d]: %s\n", j, strings[j]);
    }

    free(strings);
}

static void myfunc2(void) {
    myfunc3();
}

void myfunc3(int ncalls){
    if(ncalls > 1)
        myfunc(ncalls - 1);
    else
        myfunc2();
}

int main(int argc, char *argv[]){
    if(argc != 2){
        fprintf(stderr, "%s num-calls\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    myfunc(atoi(argv[1]));
    exit(EXIT_SUCCESS);
}
```