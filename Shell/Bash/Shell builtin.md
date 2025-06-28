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