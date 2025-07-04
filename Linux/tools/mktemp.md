`mktemp` - 创建临时文件或目录

语法：
```bash
mktemp [OPTION]... [TEMPLATE]
```

描述：
安全创建临时文件或目录，并打印它的名字。
`TEMPLATE` 默认使用 `tmp.XXXXXXXXXX` ，如果指定的话，在尾部至少包含三个连续的 `X` 。
参数：
`-d, --directory`：创建目录，而非文件
`-u, --dry-run`：不实际创建，仅打印名字
`-p DIR, --tmpdir[=DIR]`：指定临时文件所在的目录。如果 `DIR` 为指定，使用 `$TMPDIR`；如果 `$TMPDIR` 也未指定，使用 `/tmp` 。
`-t`：指定临时文件的文件名模板，模板的末尾必须至少包含三个连续的 `X` 字符，表示随机字符。

示例1：直接运行，创建临时文件
```bash
$ mktemp
/tmp/tmp.lJ3WCw2JqI
```

示例2：脚本中使用 `mktemp` ^fe019b
```bash
#!/bin/bash

TMPFILE=$(mktemp)   # 为确保临时文件创建成功，最好使用 OR 运算符（`||`），保证创建失败时退出脚本 TMPFILE=$(mktemp) || exit 1
echo "Temp file is $TMPFILE"
```

示例3：更改临时文件的文件名模板
```bash
$ mktemp -ut mytemp.XXXXXXXX
/tmp/mytemp.B4SwKBWL
```
