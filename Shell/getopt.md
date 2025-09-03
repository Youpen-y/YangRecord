`getopt` - 解析命令选项；把脚本收到的原始位置参数（`$@`）按短/长选项规则重新整理成易于 case 处理的格式。

语法：
```bash
getopt optstring parameters
getopt [options] [--] optstring parameters
getopt [options] -o|--options optstring [options] [--] parameters
```

描述：
`getopt` 用于解析命令行中的选项，以便 shell procedures 进行解析，并检查选项是否有效。
`getopt` 的命令参数可以分成两部分：定义“怎么解析”的参数和 “要被解析”的参数；两部分以 `--` 为界。如果没有明确指定解析规则，`getopt` 会把“被解析”的第一项当成规则去解析后续参数。
- 第一部分：解析规则
	- `-o` 或 `--options`：定义短选项字符串
	- `--long`：定义长选项
	- 其他影响解析行为的设置（如 `-n`, `-q`）
- 第二部分：希望 `getopt` 解析的参数
	- 以 `--` 为界，后面是要解析的真实参数。
示例：
```bash
PARSED=$(getopt -o ab:c --long alpha,bravo: -- "$@" || usage 1
eval set -- "$PARSED"
# 如果选项无参数，仅使用字母；单冒号(`:`)表示强制参数，双冒号(`::`)表示可选参数
```
- `-o ab:c` 告诉 `getopt` 有短选项 `-a, -c`（无参数），`-b` 后面跟一个参数
- `--long alpha,bravo:` ：定义两个长选项，`--alpha`（无参数），`--bravo`（必须有参数）
- `--`：分隔符
- `--alpha -a -b value file.txt`：被解析参数部分
- `eval set -- "$PARSED"`：`getopt` 返回的是“被重新排序”的字符串；`eval set --` 把它再次变成 `$1, $2, ...`，供后续 `case` 使用。