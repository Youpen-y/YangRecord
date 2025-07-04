#### `read`
作用：从标准输入（或指定的文件描述符）读取一行文本，并将其内容赋值给一个或多个变量。
语法：
```bash
read [options] [variable...]
```
将输入赋值给指定变量或默认变量（`REPLY`）

选项：
- `-p prompt`：在读取输入之前显示提示
```bash
read -p "Please input your name: " name
```
- `-s`：（silent）静默模式（隐藏输入，例如：用于密码）
- `-r`：（raw）禁用反斜杠转义；如果未指定 `-r` 选项，则 `\` 应充当转义符。
```bash
test_input="Hello\nWorld\tTab\\Backslash"
read var1 <<< "$test_input"
echo "read result: '$var1'"     # read result: 'HellonWorldtTabBackslash'
echo "length: ${#var1}"         # length: 24

read -r var2 <<< "$test_input"
echo "read result: '$var2'"     # read result: 'Hello\nWorld\tTab\Backslash'
echo "length: ${#var2}"         # length: 27
```
- `-n num`：仅读取 `num` 个字符而不是整行
```bash
$ read -n 3 letter
abcdefghij
$ echo $letter
abc
```
- `-a ARRAY`：将读取到的行根据内部字段分隔符（`IFS`变量，默认为空格、制表符、换行符）分割为单词，并存储到指定的数组中。
```bash
echo "apple banana cherry" | while read -r -a fruits; do
    echo "fruit1: ${fruits[0]}"
    echo "fruit2: ${fruits[1]}"
    echo "fruit3: ${fruits[3]}"
done
```

- `-t TIEMOUT`：设置一个超时时间（以秒为单位）。如果在指定的时间内没有输入，`read` 命令会返回非零状态码（等价于假），并停止等待。
```bash
echo "你需要在 5 秒内输入你的名字："
if read -t 5 name; then
    echo "你好，$name！"
else
    echo "抱歉，你输入太慢了！"
fi
```
- `-d DELIM`：指定一个单字符分隔符，而不是默认的换行符（`\n`）。`read`会一直读取，直到遇到这个分隔符或文件末尾。
- `-u FD`：从文件描述符 `FD` 读取而非标准输入。
返回值：`read`的退出状态
- 0 （成功）：当 `read` 成功读取到数据（没有到文件末尾或遇到错误）时，返回 0 。
- 非 0 （失败）：当 `read` 遇到文件末尾（EOF）或者发生错误（例如，超时、指定的文件描述符无效）时，它返回非 0 。

#### `exit`
`exit` - 终止脚本的执行。可以选择性地在其后面跟一个整数值，该值被称为退出状态码（Exit Status）或返回码（Return Code）。

用法：
1. 直接使用 `exit`。默认返回上一个执行命令的退出状态码。隐式使用 `$?` 的值。
2. `exit [0-255]`：0 表示成功（条件判断中，0 被视为真）；非 0 表示失败。
3. `exit $?`：返回上一个执行命令的退出状态码。

#### `export`
把一个 shell 变量“导出”为环境变量，使得它对当前 shell 启动的所有子进程可见。
- 普通变量只在当前 shell 会话中有效
- 用 `export` 后，普通变量变为环境变量，子进程也能读取
示例：
```bash
MY_VAR="hello"
bash -c 'echo $MY_VAR'  # 输出为空，`MY_VAR` 没有被子进程继承
export MY_VAR
bash -c 'echo $MY_VAR'  # 输出 hello 

# 变量赋值和输出单步完成
export TEST_VAR="test var"
```

取消 `export` ：变量还存在，但不再作为环境变量传递
```bash
export -n MY_VAR
```

>[!NOTE]
>当使用 `export` 输出变量后，子 Shell 继承这些变量。但如果子 Shell 修改继承的变量，不会影响父 Shell 。
#### `shift`
用于处理位置参数的内建命令。
语法：
```bash
shift [n]
```
重命名位置参数 `$N+1, SN+2, ...` 为 `$1, $2, ...` 。如果未给定 `N` ，假定为 1 。
退出状态：
除非 N 是负数或大于 `$#` ，否则返回成功。
示例：循环读取参数
```bash
#!/bin/bash
while [[ $# -gt 0 ]]; do
    echo "Current argument: $1"
    shift
done

# bash test.sh one two three
# output:
# current argument: one
# current argument: two
# current argument: three
```
注意：`shift` 会丢弃前面的参数，无法恢复。
示例2：解析 `--key value` 形式的参数
```bash
#!/bin/bash
while [[ $# -gt 0 ]]; do
    case "$1" in
        --name)
        NAME="$2"
        shift 2
        ;;
        --age)
        AGE="$2"
        shift 2
        ;;
        *)
        echo "Unknown option: $1"
        shift
        ;;
    esac
done

echo "Name: $NAME"
echo "Age: $AGE"
```

#### 作业控制——`fg, bg, jobs`
作业控制指可以选择性地终止（`suspend`）进程执行并在稍后恢复（`resume`）其执行。用户通过操作系统内核的终端驱动和 `bash` 联合提供的交互式接口来实现作业控制。
内核终端驱动负责管理终端设备的输入和输出。将监控后台进程的终端访问行为，并在必要时发送信号。
Shell 将每个流水线与一个作业关联起来（流水线可能包含多个进程，它们同属于一个作业）。维护来一个正在执行作业的表，可通过 `jobs` 命令列出。
当 `bash` 异步启动一个作业（异步意味着在后台），将打印如下信息：
```
$ cursor . &
[1] 66881
```
其中 `[1]` 表明作业号为 1，而 66881 是与该作业关联的流水线的最后一个进程的 PID。

操作系统维护了一个当前终端进程组 ID（Terminal process group ID，TPGID）的概念，用以指示当前处于前台的进程组（该进程组下进程的 PGID == TPGID）。该进程组中进程将收到键盘产生的信号（如 `SIGINT`）。
后台进程是那些 PGID 与 TPGID 不同的进程。这些进程对键盘产生的信号免疫。

如果用户启用了 `stty tostop`，只有前台进程允许读取或写入终端；当后台进程尝试从终端读取或写入时，内核将通过终端驱动程序向该进程发送 SIGTTIN（对应读取） 或 SIGTTOU 信号。如果进程没有捕获这些信号，这些信号的默认行为是将进程挂起（`suspend`）。

在 Shell 中，指代作业的方式：
- `%n` ：作业号为 `n` 。
- 作业名称或作业前缀：`%prefix`，如 `%ce` 指命令名以 `ce` 为前缀的作业。
- `%?ce`：命令中包含 `ce` 的作业。
- 特殊表示：`%+` 表示最近一个后台作业；`%-`表示次近一个后台作业。
`bash` 进行作业控制：
1. 将作业放到后台运行
```bash
command &
```

2. 暂停作业
	- 使用 `Ctrl-Z` 暂停当前前台运行的作业
```bash
[1]+ Stopped        command
```

3. 将暂停的作业放到后台运行
```bash
bg %job_number
```

4. 查看后台作业
```bash
jobs

jobs -l # 查看作业的详细状态
```

5. 将作业放到前台运行
```bash
fg %job_number
```

#### `declare`
`declare` 用于设置变量值和属性

> Declare variables and give them attributes. If no NAMEs are given, display the attributes and values of all variables.

语法：
```bash
declare [OPTION] [VARIABLE[=value]...]
```

主要选项：
- `-a`：声明数组变量
- `-f`：输出所有函数定义
```bash
declare -f
```
- `-F`：输出所有函数名
- `-i`：声明整数变量
```bash
declare -i val1=12 val2=5
```
- `-l`：声明变量为小写字母
- `-p`：查看变量信息
- `-r`：声明只读变量，无法改变变量值，也不能 `unset` 变量
- `-u`：声明变量为大写字母，自动把变量值转成大写字母
- `-x`：该变量输出为环境变量，等同于 `export`

注意点：
1. 使用 `+` 号而非 `-` 号来关闭给定的属性
2. 当在函数中使用时，声明的变量只在函数内部有效，等同于 `local` 命令

#### `readonly`
`readonly` 命令等同于 `declare -r`，用来声明只读变量，不能改变变量值，也不能 `unset` 变量。
`readonly`命令有三个参数。
- `-f`：声明的变量为函数名。
- `-p`：打印出所有的只读变量。
- `-a`：声明的变量为数组。

#### `let` 
`let` 命令声明变量时，可以直接执行算术表达式。
```bash
$ let foo=1+2
$ echo $foo
3
```
如果 `let` 命令的参数表达式包含空格，需要使用引号
```bash
$ let "foo = 1 + 2"
```


#### `pushd, popd, dirs`
操作目录堆栈的 shell builtin 。
显示目录堆栈：
```bash
$ dirs
```

`pushd` 和 `popd` 使用：
- `pushd` 类似于 `cd` 命令，可以进入指定的目录。
```bash
$ pushd [-n] [+N | -N | dir]
```
上述命令会进入目录 `dirname` ，并将该目录放入堆栈。
第一次使用 `pushd` 命令时，会将当前目录先放入堆栈，然后将所要进入的目录也放入堆栈，位置在前一个的上方。

- `popd`命令
语法：
```bash
popd [-n] [+N | -N]
```
从目录桟移除条目。
当没有参数时，移除栈顶条目，并切换到新的顶部目录下。
- `popd -n`：仅操作堆栈，但不切换目录。
- 整数参数：当采用整数作为参数时，该整数表示堆栈中指定位置的记录（从 0 开始）。`pushd` 命令会把这条记录移动到栈顶，同时切换到该目录；而 `popd` 则从堆栈中删除这条记录，不会切换目录。
示例：
```bash
$ pushd +3      # 从栈顶算起的 3 号目录（从 0 开始）
$ pushd -3      # 从桟底开始的 3 号目录（从 0 开始）
$ popd +3       # 删除从栈顶算起的 3 号目录
$ popd -3       # 删除从桟底算起的 3 号目录
```

#### `alias, unalias`
`alias` 命令用来定义或显示别名；`unalias` 用于移除别名。
```bash
alias [-p] [name[=value] ... ]
unalias [-a] name [name ...]   # 返回成功，除非 NAME 是不存在的别名
```
示例：
```bash
$ alias today='date'
$ today
Thu Jul  3 07:24:49 PM CST 2025

$ alias rm='rm -i'  # 删除文件前询问

$ unalias today
$ unalias -a    # 移除所有别名定义
```

#### `trap`
`trap` 命令用于捕获信号和其他事件。可以定义信号处理程序并在收到信号或其他条件时激活信号处理程序。
语法：
```bash
trap [-lp] [[arg] signal_spec ...]
```

参数 `arg` 指定了当 shell 收到 `signal_spec` 信号时要读取和执行的命令。当 `arg` 包含多条命令时，应使用引号把多个处理语句括起来，并把信号名放在后面。
```bash
trap 'command1; command2; ...' SIGNAL1 SIGNAL2 ...
```

选项：
`-l`：打印信号名和其对应的编号
`-p`：显示和每个信号 `signal_spec` 关联的 trap 命令。

每个 `signal_spec` 要么是 `<signal.h>` 中的信号名或信号编号。信号名大小写不敏感而且 `SIG` 前缀可选。

`trap`命令必须在接收信号或可能退出的代码之前设置，否则就无法捕获那些在`trap`设置之前发生的信号或中断。

示例：更多可参见[[代码片段#^222313]]
```bash
$ trap -l
 1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
 2) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	10) SIGUSR1
3) SIGSEGV	12) SIGUSR2	13) SIGPIPE	14) SIGALRM	15) SIGTERM
4) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
5) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
6) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
7) SIGSYS	34) SIGRTMIN	35) SIGRTMIN+1	36) SIGRTMIN+2	37) SIGRTMIN+3
8) SIGRTMIN+4	39) SIGRTMIN+5	40) SIGRTMIN+6	41) SIGRTMIN+7	42) SIGRTMIN+8
9) SIGRTMIN+9	44) SIGRTMIN+10	45) SIGRTMIN+11	46) SIGRTMIN+12	47) SIGRTMIN+13
10) SIGRTMIN+14	49) SIGRTMIN+15	50) SIGRTMAX-14	51) SIGRTMAX-13	52) SIGRTMAX-12
11) SIGRTMAX-11	54) SIGRTMAX-10	55) SIGRTMAX-9	56) SIGRTMAX-8	57) SIGRTMAX-7
12) SIGRTMAX-6	59) SIGRTMAX-5	60) SIGRTMAX-4	61) SIGRTMAX-3	62) SIGRTMAX-2
13) SIGRTMAX-1	64) SIGRTMAX	
```