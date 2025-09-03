GNU Make --- 由 Richard Stallman 和 Roland McGrath 开发，自 3.76 版后，开发工作由 Paul D. Smith 负责。

任何可以使用 shell 命令运行编译器的编程语言都可以使用 `make` 。事实上，`make` 并不局限于程序，可以使用它描述任何需要某些文件在其他文件更改时自动更新的任务。

`makefile` 用于指示 `make`，通常会告诉 `make` 如何编译和链接程序。

---
`make` 如何处理 `Makefile`？
默认情况下，make 会从第一个目标开始执行（但不会选择以 ‘.’ 开头的目标，除非它们的名称中还包含一个或多个 ‘/’）。这个第一个要执行的目标称为默认目标（default goal）。
> 默认目标（即运行 `make` 时自动执行的目标）是 `Makefile` 中**第一个有效的目标**，但如果这个目标的名称以 `.` 开头，且**不包含路径符 `/`**，它将被跳过。如果它的名字中含有路径，比如 `./.foo` 或 `some/.dir/file`，**即包含一个或多个 `/`，则可以被视为默认目标**。

这个机制是为了避免 `make` 默认执行某些**特殊的或隐藏的规则**，例如 `.PHONY`、`.SUFFIXES` 等内部使用的目标。
如果某个规则不是默认目标（或它依赖的任何东西，或这些依赖的依赖）的依赖项，那么这个规则就不会被执行，除非你明确告诉 make 去执行它（比如使用 `make clean` 命令）。

---
### 编写 `Makefile`
`Makefile` 包含五种内容：显式规则（explicit rules）、隐式规则（implicit rules）、变量定义、指令（directive）和注释。
- 显式规则，规定何时以及如何 remake 一个或多个文件（称为规则的目标）。它列出了目标所依赖的其他文件（目标的先决条件），以及用于创建或更新目标的配方。
- 隐式规则，规定何时以及如何根据文件名称 remake 一类文件。它描述了目标如何依赖于与目标名称相似的文件，并提供了创建或更新此类目标的方法。
- 变量定义，一行指定变量的文本字符串值，该值随后可以在文本中替换。
- 指令，指示 `make` 执行某项操作的指令
	- 读取另一个 `makefile`
	- 根据变量的值决定是否使用或忽略 `makefile` 的某个部分
	- 定义多行变量
- 注释，以 `#` 开头的行。

---
当 `make` 查找 `makefile` 时，会按顺序尝试：`GNUmakefile`，`makefile` 和 `Makefile` 。
如果 `make` 找不到这些名称，它就不会使用任何 `makefile` 。这时，必须使用命令参数指定目标，然后 `make` 将尝试找出如何仅使用其内置的“隐式规则”。
使用 `-f` 或 `--file` 选项指定 Makefile 的文件名。
```bash
make -f build.mk
# 或者
make --file=build.mk
# 或者 按顺序连接使用
make -f common.mk -f project.mk
```
---
包含其他 `Makefile` ：`include filenames...`，指示 `make` 暂停读取当前 `makefile` ，并读取一个或多个其他 `makefile` ，然后再继续执行。

---
`make` 如何读取 `Makefile`？
GNU `make` 的工作分为两个不同的阶段。
- 第一阶段：解析阶段。会逐行读取所有 `makefile` 文件、包含的 `makefile` 文件等，并内部化所有变量及其值、隐式和显式规则，然后构建所有目标及其依赖项（prerequisites）的依赖关系图。
- 第二阶段：构建阶段。`make` 会根据依赖关系决定需要更新的哪些目标，并执行相关命令。
这两个阶段直接影响变量和函数的扩展方式：
1. 如果扩展发生在第一阶段，称为立即扩展（immediate expansion）：`make` 会在解析 `makefile` 时扩展该构造部分。典型例子：`define`、`:=`，规则定义等结构中的变量引用。
```makefile
FOO := $(shell echo NOW)

define HELLO
    echo Hello $(NAME)
endef

NAME = Alice
all:
    $(HELLO)
```
- `:=` 是立即赋值，所以 `$(shell ...)` 会在 `makefile` 被解析时就运行。
- 每次运行 `make` ，这个值都是当时的时间，但只展开一次。
2. 如果扩展发生在第二阶段，称为延迟扩展（deferred expansion）：延迟扩展的构造部分会被延迟到需要使用时，要么在立即上下文中引用它，要么是第二阶段需要它。典型示例：`=` 定义的变量（递归变量）、命令中的变量引用
```makefile
FOO = $(shell date) # `=`定义的是递归变量
```
- `FOO` 是递归变量，`make` 不会立即展开。
- 直到真正执行 `show` 规则的命令时，`$(FOO)` 才会展开。每次执行 `make show` 都得到不同结果。
```makefile
CC = gcc
CFLAGS = -O2
COMPILE = $(CC) $(CFLAGS)

special.o: CC = clang   # 构建 special.o 时就会用 clang
```
如果改为 `COMPILE := $(CC) $(CFLAGS)`，那就会立即展开为 `gcc -O2`，之后无论怎么改 `CC` 和 `CFLAGS`，`COMPILE` 都不会变。
