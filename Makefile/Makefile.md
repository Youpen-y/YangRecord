![[C 编译步骤]]

### `rules` - 规则语法
```
targets: prerequisites
    command
    command
```

---

### `variables` 赋值：`:= / =`
```
files := file1 file2
some_file : $(files)
    command
```
其中引用变量  `$(var)` 等价于 `${var}` 。

---
### 自动变量
`$@` - 引用 target
`$?` - 所有比 target 更新的 prerequisites
`$^` - 所有 prerequisites 
`$<` - 第一个 prerequisites

`$@` - 引用目标
```
all := file1 file2
all :
    # 命令前的 @ 使得不输出执行的 command, $@ 引用 all 目标
    @echo $@  # 输出 all
    @echo $($@) # 输出 file1 file2
```
当有一条规则有多个目标时
```
obj1 obj2:
    @echo $@
# make obj1 将输出 obj1
# make obj2 将输出 obj2
```

### 内置变量
1. `MAKECMDGOALS` - 包含用户在命令行上指定的所有目标（targets）
如果运行 `make clean all`，那么 `$(MAKECMDGOALS)` 的值就是 `clean all`。
如果只运行 `make`（没有指定目标），那么 `$(MAKECMDGOALS)` 的值是空字符串。

2. `CURDIR` - 代表 `make` 命令当前的工作目录（始终是绝对路径；动态变化）
当直接在一个包含 `Makefile` 的目录中运行 `make` 时，`CURDIR` 的值就是这个目录的绝对路径。
当使用 `-C` 选项切换目录时，`CURDIR` 的值会改变为 `make` 实际操作的那个目录。

---
### 两个特殊的目标 （`all` 和 `clean`）
由于它们没有任何先决条件，并且项目中没有名为 `all` 或 `clean` 文件，因此它们始终被认为比其依赖项更旧，并且始终被执行。
但如果目录中创建一个名为 `all` 和 `clean` 文件，`make` 就会感到困惑。由于 `all` 或 `clean` 文件存在，并且目标没有先决条件，因此它们将被视为比先决条件 newer 。因此该规则永远不会运行，为解决该问题，可以创建一个虚拟的目标`.PHONY`
```makefile
.PHONY: all clean
```

---
### Wildcard - 通配符
#### `*` - 在文件系统中搜索匹配的文件名，和 `wildcard` 函数一同使用 。
> 注意： `*` 不能直接用于变量定义中

wildcard 函数基本语法（其中pattern... 是一个或多个文件名模式）
```make
$(wildcard patterns...)
```

##### 示例1：
目录下，包含如下文件
```bash
$ ls
file1.txt
file2.txt
file3.txt
file4.bin
```
可以使用 `wildcard` 函数来匹配所有以 `.txt` 结尾的文件
```
TXT_FILES := $(wildcard *.txt)

# 在这个示例中，TXT_FILES 变量将包含 file1.txt file2.txt file3.txt
```

##### 示例2：
匹配当前目录下所有 `.c` 文件
```
C_SOURCES := $(wildcard *.c)
```

匹配特定目录下所有 `.h` 文件
```
HEADERS := $(wildcard include/*.h)
```

匹配当前目录及子目录下的所有 `.cpp` 文件
```
CPP_SOURCES := $(wildcard **/*.cpp)
```


#### `%` 模式匹配通配符 的使用
用于指定一个规则可以匹配多个目标文件。


---
### 隐含规则
编译 C 程序使用如下命令将 `n.c` 变为 `n.o`  文件
```
$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@
```

编译 CPP 程序使用如下命令将 `n.cc` 或 `n.cpp` 变为 `n.o`
```
$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $^ -o $@
```

链接一个目标文件：使用如下命令将 `n.o` 变为 `n`
```
$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@
```

隐式规则使用的重要变量有：
- `CC` - 编译 C 程序的程序；默认为 `cc`
- `CXX` - 编译 C++ 程序的程序；默认为 `g++`
- `CFLAGS` - 提供给 C 编译器的额外标志
- `CXXFLAGS` - 提供给 C++ 编译器的额外标志
- `CPPFLAGS` - 提供给 C 预处理器的额外标志
- `LDFLAGS` - 当编译器应该调用链接器时提供给编译器的额外标志

示例：
```
CC = gcc
CPPFLAGS = -I./include  # 将当前目录下的 include 子目录添加到头文件的搜索路径中
CFLAGS = -g

blah: blah.o

blah.c:
    echo "int main() {return 0; }" > blah.c

clean:
    rm -f blah*
```

### 常见语法
1. `ifeq`, `ifneq`, `ifdef`, `ifndef` - 条件语句
```Makefile
ifeq (<arg1>, <arg2>)
    # 如果 <arg1> 等于 <arg2> ，则执行这里的代码
#endif

ifneq(<arg1>, <arg2>)
    # 如果 <arg1> 不等于 <arg2> ，则执行这里的代码
#endif

ifdef <variable-name>
    # 如果定义了变量 variable-name ，则执行这部分
#endif

# 结合 else 和 else if 使用，执行时可通过 make OS=Linux 传入 OS 变量
ifeq ($(OS), Linux)
    # Linux 特定设置
else ifeq ($(OS), Darwin)
    # macOS 特定的设置
else
    # 其他操作系统的通用设置
endif
```

2. `filter` 函数 - 从一个列表中过滤出符合特定模式的单词
语法：
```Makefile
$(filter <pattern...>, <text>)
```
- `<pattern...>`：一个或多个模式（`pattern`），用空格分隔。这些模式可以是文件名匹配模式，如 `%.c`, `foo%`, `bar` 等。
- `<text>`：待过滤的文本列表，其中包含多个单词，用空格分隔。
`filter` 将遍历 `<text>` 中的每一个单词。如果单词匹配 `<pattern...>` 中的任何一个模式，那么它就会被保留下来，并包含在函数的结果中。不匹配任何模式的单词会被丢弃。结果是一个新的单词列表，单词之间用单个空格分隔。

示例：从文件列表中找出所有的 `.c`, `.h`文件
```Makefile
SOURCES = main.c foo.h bar.c test.txt utils.c

C_FILES = $(filter %.c %.h, $(SOURCES))

all:
        @echo "All source files: $(SOURCES)"
        @echo "C source files: $(C_FILES)"
```

互补函数 `filter-out` ：
`filter-out` 用来排除匹配的单词
```Makefile
# 排除 .c 文件
NON_C_FILES = $(filter-out %.c, $(ALL_SOURCES))
```

示例：
```Makefile
ifeq ($(filter output-sync,$(.FEATURES)),)
$(error GNU Make >= 4.0 is required. Your Make version is $(MAKE_VERSION))
endif
```
- `.FEATURES` 是 GNU Make 的一个特殊变量，下面测试列出当前 make 支持的所有特性
```Makefile
test:
    @echo $(.FEATURES)
# make test 将输出
# target-specific order-only second-expansion else-if shortest-stem undefine oneshell nocomment grouped-target extra-prereqs archives jobserver output-sync check-symlink load
```
- `ifeq ($(VAR),)` 检查变量 VAR 的值是否为空字符串？
- 检查当前 GNU Make 是否支持 `output-sync` 特性，如果不支持，将报错终止 make 过程
- `MAKE_VERSION` 是当前 make 的版本号变量

3. `error` 函数 - 停止 `make` 命令的执行，并打印出指定的错误消息
语法：
```Makefile
$(error <text>)
```
-  `<text>`：希望作为错误消息打印出的字符串，可以是纯文本，或包含 `Makefile` 变量或函数的引用

示例：
```Makefile
ifndef CC
    $(error CC (C compiler) is not defined. Please set it, e.g., 'make CC=gcc')
endif
```

4. `info` 函数， `warning` 函数
语法：
```Makefile
$(info <text>)      # 打印 <text> 到标准输出；调试，用户指导
$(warning <text>)   # 打印 <text> 到标准错误输出作为警告；指示潜在问题
```

5. `if` 条件函数
语法：
```Makefile
$(if COND, THEN, ELSE)
```
- `COND`：是一个条件表达式。`make` 会对其进行评估。
	- 如果 `COND` 展开后不为空，则条件为真。
	- 如果 `COND` 展开后为空字符串，则条件为假。
- `THEN`：如果 `COND` 为真，`$(if ...)` 函数的返回值就是 `THEN` 部分的内容
- `ELSE`（可选）：如果 `COND` 为假，`$(if ...)` 函数的返回值就是 `ELSE` 部分的内容。如果 `ELSE` 部分被省略且 `COND` 为假，那么 `$(if ...)` 函数将返回空字符串。

示例：
```Makefile
# 如果 MY_VAR 没有被定义或为空，则设置为 'default_value'
MY_VAR ?= $(if $(MY_VAR), $(MY_VAR), default_value)

# 或者更常见的用法，利用 ?= 的特性
# MY_VAR ?= default_value

# 在命令行传入 DEBUG=1 或 DEBUG=true 来启用调试
DEBUG_CFLAGS = $(if $(DEBUG),-g -DDEBUG_MODE,)
```

6. `lastword` 函数 - 返回一个空格分隔的文本列表中最后一个单词（字符串处理函数）
```Makefile
$(lastword <text>)
```
- `<text>`：由一个或多个单词组成的字符串，单词之间通常用空格分隔。

示例：
```Makefile
FILES = file1.txt file2.c main.o final_report.pdf

LAST_FILE = $(lastword $(FILES))

all:
        @echo "All files: $(FILES)"
        @echo "Last file in the list: $(LAST_FILE)"

# 运行结果: 
# All files: file1.txt file2.c main.o final_report.pdf 
# Last file in the list: final_report.pdf
```

7. `dir` 函数 - 从一个或多个文件路径中提取出它们的目录部分
语法：
```Makefile
$(dir <names...>)
```
- `<names...>`：由一个或多个文件路径组成的列表，路径之间通常用空格分隔。
`dir` 函数会遍历 `<names...>` 中的每一个文件路径。对于每个路径：
- 它会移除路径中最后一个斜线 `/` 之后的所有内容（包括斜线本身）
- 如果路径中没有斜线，则返回 `./`（表当前目录）
- 如果路径是目录，例如 `src/` ，它会返回该目录本身，即 `src/` 。
- 如果路径为空，则返回空字符串。

8. `realpath` 函数 - 用于获取文件或目录的规范化绝对路径。
语法：
```Makefile
$(realpath <names...>)
```
 - `<names...>`：一个由一个或多个文件或目录路径组成的列表，路径之间通常用空格分隔。
`realpath` 函数会遍历 `<names...>` 中的每一个路径。对于每个路径，它会：
- 将其转换为绝对路径：如果输入是相对路径，它会基于当前 `make` 进程的工作目录转换为绝对路径。
- 移除冗余成分：例如，`./` 会被移除，`../` 会被正确解析。重复的路径分隔符（如 `//`）会被规范化为单个 `/`。
- 解析符号链接 (symlinks)：这是 `realpath` 与 `abspath` 的关键区别。如果路径中包含符号链接，`realpath` 会解析这些链接，返回它们所指向的真实文件或目录的路径。
- 要求文件或目录存在：如果 `realpath` 函数所处理的路径指向的文件或目录**不存在**，它将返回空字符串。这是另一个与 `abspath` 的重要区别，`abspath` 即使文件不存在也能生成绝对路径。

9. `export` 和 `unexport`
`export` 指令用于将一个 `Makefile` 变量导出为环境变量，使其在 `make` 命令所执行的子进程中可见。
```Makefile
export VARNAME
```
或者，在定义变量的同时导出：
```Makefile
export VARNAME = value
export VARNAME := value
export VARNAME ?= value
export VARNAME += value
```

`unexport` 指令与 `export` 相反，用于阻止一个 `Makefile` 变量被导出为环境变量。

10. `origin` 函数 - 用于获取一个变量的起源或来源。该变量是如何被定义的。
语法：
```Makefile
$(origin <variable-name>)
```
返回值：
- `undefined`：变量从未被定义过
- `default`：变量是 `make` 的内置变量，由 `make` 自身默认定义。如 `$(MAKE)`、`$(CC)`等
- `environment`：变量是从环境中导入的，即在 `make` 命令执行前，已经在 `shell` 的环境变量中定义
- `environment override`：变量是从环境中导入的，当由于 `make -e` 选项的使用，它覆盖了 `Makefile` 中的同名定义
- `file`：变量是在 `Makefile` 文件中定义的（包括通过 `include` 包含的文件）
- `command line`：变量是在 `make` 命令行上定义的。如 `make VAR=value`
- `override`：变量在 `Makefile` 中使用 `override` 指定定义（如，`override VAR = value`），这意味着即使被命令行定义，也会覆盖命令行定义
- `automatic`：变量是自动变量，由 `make` 在执行规则时自动设置（如 `$(@)`, `$(<)`, `$(^)`
使用示例：
```Makefile
# 强制用户在命令行设置 PROJECT_NAME
ifeq ($(origin PROJECT_NAME), undefined)
    $(error PROJECT_NAME must be defined on the command line, e.g., 'make PROJECT_NAME=MyProject')
endif
ifeq ($(origin PROJECT_NAME), environment)
    $(warning PROJECT_NAME is from environment, consider defining on command line for clarity.)
endif
```

11. `findstring` - 允许在一个字符串（`SUBSTRING`）中搜索另一个字符串（`TEXT`）的出现。
语法：
```Makefile
$(findstring SUBSTRING,TEXT)
```
- `SUBSTRING`：要搜索的字符串
- `TEXT`：要在其中搜索的字符串（或单词列表）
`findstring` 在 `TEXT` 中搜索 `SUBSTRING`，如果找到会返回 `SUBSTRING` 本身；否则返回一个空字符串。

示例：
```Makefile
# Makefile
BUILD_TARGET = debug
# BUILD_TARGET = release
# BUILD_TARGET = profile

ifeq ($(findstring, debug,$(BUILD_TARGET)),debug)
    CFLAGS += -g -DDEBUG_BUILD
    LDFLAGS += -ldl
    $(info Debug build enabled.)
else ifeq ($(findstring profile,$(BUILD_TARGET)),profile)
    CFLAGS += -pg
    $(info Profile build enabled.)
else
    CFLAGS += -O2
    $(info Release build enabled.)
endif
```

12. `word` - 用于从一个空格分隔的文本列表中提取出第 `N` 个单词
语法：
```Makefile
$(word N,TEXT)
```
- `N`：一个正整数，表示想要提取的单词的序号。第一个单词是 `1` ，第二个单词 `2` ...。
- `TEXT`：待处理的文本列表，其中包含一个或多个单词，单词之间通常用空格分隔。
```Makefile
FILE_LIST = file1.c file2.h file3.txt file4.o

FIRST_FILE = $(word 1, $(FILE_LIST))
THIRD_FILE = $(word 3, $(FILE_LIST))
FIFTH_FILE = $(word 5, $(FILE_LIST))

all:
    @echo "File list: $(FILE_LIST)"
    @echo "First file: $(FIRST_FILE)"
    @echo "Third file: $(THIRD_FILE)"
    @echo "Fifth file: $(FIFTH_FILE)"
```
其他列表处理函数
- `$(firstword TEXT)`：提取列表中的第一个单词（== `$(word 1, TEXT)`)
- `$(lastword TEXT)`：提取列表中的最后一个单词
- `$(words TEXT)`：返回 `TEXT` 中单词的总数
- `$(wordlist START,END,TEXT)`：提取从 `START` 到 `END` 位置的一系列单词

13. `foreach` - 对一个列表中每个元素执行迭代操作，并生成一个新的列表。
语法：
```Makefile
$(foreach VAR,LIST,TEXT)
```
- `VAR`：临时变量的名称。每次迭代中，它都会被赋值为 `LIST` 中的当前元素
- `LIST`：一个空格分隔的单词列表。`foreach` 函数将对这个列表中的每一个单词进行迭代
- `TEXT`：一个字符串。每次迭代时，`VAR` 会被替换为 `LIST` 中的当前元素，然后 `TEXT` 会被评估。每次评估 `TEXT` 的结果会被连接起来，形成 `foreach` 函数的最终返回值

示例1：
```Makefile
MODULES = foo bar baz

# 为每个模块生成一个 .c 文件名
SOURCES = $(foreach M,$(MODULES),$(M).c)

all:
	@echo "Modules: $(MODULES)"
	@echo "Sources: $(SOURCES)"

# 运行结果:
# Modules: foo bar baz
# Sources: foo.c bar.c baz.c
```

示例2：
```Makefile
# 定义要编译的 C 模块
C_MODULES = core network drivers

# 为每个模块定义一个编译规则
$(foreach MODULE, $(C_MODULES), \
$(MODULE).o: $(MODULE).c \
	$(CC) $(CFLAGS) -c $< -o $@ \
)

all: $(patsubst %,%.o,$(C_MODULES))
	@echo "All modules compiled."

# 假设 core.c network.c drivers.c 存在
core.c:
network.c:
drivers.c:
```

14. `call` 函数 - 允许调用一个以变量形式定义的函数或宏
语法：
```Makefile
$(call VARIABLE,PARAM1,PARAM2,...)
```
- `VARIABLE`：这是一个变量的名称，这个变量的值被视为一个函数或宏的定义。在这个变量中，可以使用特殊变量 `$(1)`、`$(2)`、`$(3)`等来引用传递给 `call` 函数的参数。
- `PARAM1, PARAM2,...`：这些是传递给 `VARIABLE` 所定义的函数的参数。它们会分别替换 `VARIABLE` 定义中的 `$(1)`、`$(2)` 等。

示例：
```Makefile
# 定义一个用于打印消息的宏
# $(1) 会被替换为传递给 call 函数的第一个参数
PRINT_MESSAGE = @echo "--- MESSAGE: $(1) ---"

all:
	$(call PRINT_MESSAGE,Hello from call function!)
	$(call PRINT_MESSAGE,Another message here.)

# 运行结果:
# --- MESSAGE: Hello from call function! ---
# --- MESSAGE: Another message here. ---
```

---
其他 make 指令
- `make -C <dir>` 到目录 `dir` 下执行 `make`
- `make -f <file> <target>` 指定 `makefile` 的文件名。当运行 `make` 时，`make` 会在当前目录下查找名为 `Makefile`、`makefile` 或 `GNUmakefile` 的文件。如果文件名非默认，需要使用 `-f` 指定文件。