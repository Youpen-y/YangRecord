`asm` 关键字允许在 C 代码中嵌入汇编指令。最大限度地提高时间敏感代码的性能或 使用 C 程序不易使用的汇编指令。
GCC 提供两种内联 `asm` 语句
- Basic `asm` 语句（无操作数）
- Extended `asm` 语句（包括一个或多个操作数），是在函数中混合 C 和汇编语言的首选。**必须位于函数内部。**
还可以使用 `asm` 关键字覆盖 C 符号的汇编程序名称，或将 C 变量放置在特定寄存器中。

## Basic Asm 
语法
```c
asm asm-qulifiers (AssemblerInstructions)
```
对于 C 语言来说，`asm` 关键字是 GNU 扩展。当编写可以使用 `-ansi` 和 `-std` 的编译选项的 C 代码时，使用 `__asm__` 而不是 `asm`。对于 C++，`asm` 是关键字，但 `__asm__` 可以用于使用 `-fno-asm` 编译的代码。

Qulifiers
- `volatile` - 可选的 `volatile` 限定符没有任何作用。所有 Basic `asm` 块都隐含了`volatile`。
- `inline` - 出于内联的目的，`asm` 语句的大小将被视为可能的最小的大小
参数
`AssemblerInstructions` - 是指定汇编代码的文字字符串（literal string）。该字符串可以包含 assembler 可以识别的任何指令，包括 `directives`。GCC 本身并不解析汇编指令，也不知道它们的含义，甚至不知道它们是否是有效的汇编输入。

可以将多个汇编指令放在一个 `asm` 字符串中，并用系统汇编代码中通常使用的字符分割。

## Extended Asm
带 C 表达式操作数的汇编指令
使用 `extended asm` 可以从汇编程序读取和写入 C 变量，并执行从汇编程序代码到 C labels 的跳转。`extended asm` 语法使用 colons(':') 来分割后面的操作数参数：
```c
asm asm-qualifiers ( AssemblerTemplate
                   : OutputOperands
                   [: InputOperands
                   [: Clobbers]])

asm asm-qualifiers ( AssemblerTemplate
                   : OutputOperands
                   : InputOperands
                   : Clobbers
                   : GotoLabels)
```

在后面一种形式中，`asm-qualifiers` 包含 `goto`，而第一种形式中则不包含。

Qualifiers
`volatile` - Extended asm 语句的典型用途是操作输入值以产生输出值。然而，`asm` 语句也可能产生副作用，可能需要 `volatile` 限定符来禁用某些优化。
`inline` - 同上
`goto` - 此限定符通知编译器，`asm` 语句可以执行跳转到 `GotoLabels` 中列出的标签之一。

参数
`AssemblerTemplate` - 汇编代码的模板。是固定文本和指向