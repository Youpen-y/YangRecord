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

