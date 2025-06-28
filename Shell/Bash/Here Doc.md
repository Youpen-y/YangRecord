### `here doc` vs `here strings`
| **特性** | **Here Document** | **Here Strings** |
| ------ | ----------------- | ---------------- |
| 用于传输   | 多行文本              | 单行或短文本           |
| 语法     | `<<[标识符]...[标识符]` | `<<< [字符串]`      |

#### Here Document
`here document` 是一个特殊目的的代码块。使用某种 I/O 重定向形式，将一个命令列表（command list）传递给交互式程序或命令，如 `ftp`，`cat`或 `ex` 文本编辑器。
语法：
```bash
InteractiveProgram_or_COMMAND <<InputComesFromHERE
...
...
...
InputComesFromHERE
```
*limit string* （上述 `InputComesFromHERE`）框住了命令列表，`<<` 符号位于 *limit string* 前，效果是重定向代码块的输出到交互程序的输入。类似于 `interactive-program < command-file` ，其中 `command-file` 包含：
```
command #1
command #2
...
```
对应 `here document`：
```bash
interactive-program <<LimitString
command #1
command #2
...
LimitString
```
>[!NOTE]
>选择 _limit string_ 时，应确保其足够独特，避免与命令列表中可能出现的内容混淆。

如果使用 `<<-LimitString` （多一个 `-`），那么输出中每行开头的 tab （制表符）会被自动去除，但空格不会被去掉。

可以使用 Bash 内置的空命令（no-op command `:`）配合 `here doc` 创建匿名 `here doc` 。
```bash
# 使用 `:` 作为接收者，文本被 shell 解析但最终被丢弃，可用于多行代码块注释或占位
# 由于文本会被解析，如果有错误的变量替换将导致错误，考虑下方提到的禁用替换
: <<EOF
This is an anonymous here document.
It doesn't go anywhere or do anything.
EOF
```

`here doc` 支持参数和命令替换。当使用引号（单引号/双引号）或 `\` 修饰 *limit string* 时，参数和命令替换将被禁用。
```bash
cat <<"Endofmessage"
`ls -l`
$((5 + 3))
\\
Endofmessage
```

示例：`cat` 使用 `here doc` 输出多行
```bash
cat << EOF
这里可以写多行
文本
# 这里面的不再算注释
EOF
```
NOTE
- `EOF` 可以换成任何词，如 `END`, `STOP`
- 如果 `EOF` 加引号，即，`cat << 'EOF' ... EOF` 其中的变量不会展开。
常见用法：
- 生成配置文件
- 创建脚本
- 多行注释
- 直接执行命令
示例
```bash
# 创建一个配置文件
cat << EOF > config.txt
host: localhost
port: 8080
user: admin
password: 123456
EOF

# 创建一个脚本
cat << EOF > script.sh
#!/bin/bash
echo "Hello World"
date
pwd
EOF
```

---
#### Here Strings
`here strings` 可以被认为是 `here doc` 的精简形式。
仅包含：`COMMAND <<< $WORD` ，其中 `$WORD` 被扩展并提供给 `COMMAND` 的标准输入。

常见使用如：
- `echo-grep` 替换
```bash
if grep -q "txt" <<< "$VAR"
then
    echo "$VAR contains the substring sequence \"txt\""
fi

# 而非
if echo "$VAR" | grep -q txt    # 事实上等价于 if [[ $VAR = *txt* ]]
```

- 与 `read` 组合
```bash
String="This is a string of words."

read -r -a Words <<< "$String"

echo "First word in String is:    ${Words[0]}"   # This
echo "Second word in String is:   ${Words[1]}"   # is
echo "Third word in String is:    ${Words[2]}"   # a
echo "Fourth word in String is:   ${Words[3]}"   # string
echo "Fifth word in String is:    ${Words[4]}"   # of
echo "Sixth word in String is:    ${Words[5]}"   # words.
echo "Seventh word in String is:  ${Words[6]}"   # (null)
                                                 # Past end of $String.
```

- 将 `here doc` 的输出传递到循环的标准输入中
```bash
ArrayVar=( element0 element1 element2 {A..D} )

while read element ; do
    echo "$element" 1>&2
done <<< $(echo ${ArrayVar[*]})

# element0 element1 element2 A B C D
```