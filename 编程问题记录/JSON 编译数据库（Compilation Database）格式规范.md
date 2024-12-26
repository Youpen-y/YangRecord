一种独立于构建系统重现单次编译的格式。
> 单次编译：针对一个源文件（`.c/.cpp`）的独立编译过程，而不是整个项目的构建
> 重现：指重现这个编译过程，通常是通过记录下构建系统对这个翻译单元的编译指令（如编译器调用、选项、宏定义、头文件路径等）
> 独立于构建系统：该格式不依赖于具体的构建系统（如，`Makefile`，`CMake`，`Bazel`等），即该格式为每个翻译单元提供独立的信息，因此即使不运行原始的构建系统，也可以通过读取这些信息手动或自动完成编译。

### 背景
基于 C++ 抽象语法树的工具需要解析翻译单元的完整信息。通常，此信息在构建系统中隐式可用，但将工具作为构建系统的一部分不一定是最佳解决方案。
> 翻译单元，编译器处理的最小源代码单元。通常是单个源文件（如，`.c` 或 `.cpp` 文件）及其包含的所有头文件和宏定义的组合。

原因：
- 构建系统本质上是由 change 驱动的，因此在同一代码库上运行多个工具而不更改代码并不适合许多构建系统的架构。
- 确定事物是否发生变化是一个 IO 密集型过程；这使得基于构建系统构建低延迟终端用户工具很困难。
- 构建系统在构建图中本质上是顺序的，例如由于生成的源代码。虽然独立于构建运行的工具仍然需要生成的源代码存在，但在不变的源上多次运行工具不需要根据构建依赖关系图对运行进行序列化。

### 支持的系统
- `Clang` 可以通过 `-MJ argument` 生成编译数据库片段。示例如下：
```sh
$ clang -MJ file1.o.cmd -c file1.c
$ cat file1.o.cmd 
{ 
"directory": "/home/yongy/temp", 
"file": "file1.c", 
"output": "file1.o", 
"arguments": ["/usr/lib/llvm-18/bin/clang", "-xc", "file1.c", "-o", "file1.o", "-c", "--target=x86_64-pc-linux-gnu"]
},
```
- `CMake`，支持使用选项 `CMAKE_EXPORT_COMPILE_COMMANDS` 为 `Unix Makefile` 构建（`Ninja` 正在构建）生成编译数据库。
- `Bear`，对于 Linux 上的项目，可以使用 `Bear` 拦截编译器调用来捕获编译数据库。
- `Bazel`，可以通过提取器扩展（`bazel-compile-commands-extractor`）导出编译数据库。`Bazel` 的构建过程是高度自定义和沙箱化的，它通过自己的方式调用编译器，并严格控制构建环境。因此，Bear 很难像在传统构建系统（如 Make 或 CMake）中那样轻松地拦截编译器调用。`Bazel` 的构建流程可能没有直接暴露编译器调用的所有细节，使得 Bear 和类似工具难以正常工作。
- Clang's 工具接口支持读取编译数据库；（如，）。`libclang` 及其 `python` 绑定也支持这一点。

### 格式
编译数据库（`compilation database`）是一个 JSON 文件，由一组“命令对象”组成，其中每个命令对象指定翻译单元在项目中编译的一种方式。
如：
```json
[
  { "directory": "/home/user/llvm/build",
    "arguments": ["/usr/bin/clang++", "-Irelative", "-DSOMEDEF=With spaces, quotes and \\-es.", "-c", "-o", "file.o", "file.cc"],
    "file": "file.cc" },

  { "directory": "/home/user/llvm/build",
    "command": "/usr/bin/clang++ -Irelative -DSOMEDEF=\"With spaces, quotes and \\-es.\" -c -o file.o file.cc",
    "file": "file2.cc" },
  ...
]
```
命令对象中字段
- `directory` - 编译的工作目录。`command` 或 `file` 字段中指定的所有路径必须是绝对路径或相对于该目录的路径。
- `file` - 此次编译步骤处理的主要翻译单元源。工具将其用作编译数据库的密钥。同一个 `file` 可以有多个命令对象（使用不同的配置编译同一源文件）。
- `arguments`（required） - 编译命令 `argv` 作为字符串列表。
- `command` （required） - 作为单个 shell 转义字符串的编译命令。参数可以按照平台约定进行 shell 引用和转义，其中 `"` 和 `\` 是唯一的特殊字符。
- `output` - 编译步骤输出的名称。

> 对于简单的项目，`Clang` 项目还可以识别 `compile_flags.txt` 文件。


示例：包含 `bear` 命令的脚本 `bear.sh`
```sh
#!/bin/bash
# shellcheck disable=SC2129
# shellcheck disable=SC2046

cd ./.vscode/ || exit
make clean -C ../lib/linux/
bear --append -- make all -C ../lib/linux/
bear --append -- make all -C ../apps/ep/linux/
bear --append -- make all -C ../apps/is/linux/
bear --append -- make all -C ../apps/lu/linux/
bear --append -- make all -C ../apps/mm/linux/
bear --append -- make all -C ../apps/pi/linux/
bear --append -- make all -C ../apps/sor/linux/
bear --append -- make all -C ../apps/tsp/linux/
bear --append -- make all -C ../apps/water/linux/
cd .. || exit
```