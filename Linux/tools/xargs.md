`xargs` - build and execute command lines from standard iput.
`xargs` （exteneded arguments）是给命令传递参数的一个过滤器，也是组合多个命令的一个工具。
`xargs` 可以将管道或标准输入数据转换成命令行参数，也可以从文件的输出中读取数据。
`xargs` 也可以将单行或多行文本输入转换为其他格式，如多行变单行，单行变多行。

选项
`-a file` ：从文件中读入作为 `stdin` 。
``` shell
xargs -a test.txt echo "Processing line: "
Processing line:  a b c d e f g h i j k l m n o p q r s t u v w x y z
```
`-e flag` ：有的时候可能是 `-E` ，`flag` 必须是一个以空格分隔的标志，当 `xargs` 分析到含有 `flag` 这个标志的就停止。
``` shell
echo -e "apple\nbanana\ncherry" | xargs -E '\n' echo "Processing fruit: "
Processing fruit:  apple banana cherry
```
`-p` ：当每次执行一个 `argument` 的时候询问一次用户。
`-n num` ：表示命令在执行的时候一次用的 `argument` 的个数，默认是用所有参数。
`-t` ：表示先打印命令，然后再执行。
`-i / -I` ：将 `xargs` 的每项名称，一般是一行一行赋值给 `{}` ，可以用 `{}` 代替。
``` shell
ls | xargs -I {} mv {} /new_directory
```

`-r no-run-if-empty` ：当 `xargs` 的输入为空的时候则停止 `xargs` ，不用再执行了。
`-s num` ：命令行的最大字符数，指的是 `xargs` 后面那个命令的最大命令行字符数。
`-L num` ：从标准输入一次读取 `num` 行送给 `command` 命令。
``` shell
cat test.txt | xargs -L 1 echo "Processing line: "
Processing line:  a b c d e f g
Processing line:  h i j k l m n
Processing line:  o p q
Processing line:  r s t
Processing line:  u v w x y z
```
`-d delim` ：默认的 `xargs` 分隔符是回车，`argument` 的分隔符是空格，这里修改的是 `xargs` 的分隔符。
`-0` ：处理以 null 字符结尾的输入
``` shell
find . -type f -print0 | xargs -0 rm
```

---
`xargs` 默认的命令是 `echo` ，这意味着通过管道传递给 `xargs` 的输入将会包含换行和空白，但通过 `xargs` 的处理，换行和空白将被空格取代。
``` shell
$ cat test.txt
a b c d e f g
h i j k l m n
o p q
r s t
u v w x y z

$ cat test.txt | xargs
a b c d e f g h i j k l m n o p q r s t u v w x y z

$ cat test.txt | xargs -n3
a b c
d e f
g h i
j k l
m n o
p q r
s t u
v w x
y z

```
