`.` 在 `shell` 中是一个内建命令，作用是“运行或加载一个脚本文件的内容到当前 `shell` 环境”
- 它与 `source` 命令功能相同（在现代 `shell` 中 `. file` 等价于 `source file`）

#### 引用库（代码重用）
类似于 C 语言中的 `#include<stdio.h>` 
```bash
# common.lib
STD_MSG="About to rename some files..."

rename() {
	# expects to be called as: rename .txt .bak
	FROM=$1
	TO=$2

    for i in *$FROM
    do
        j=`basename $i $FROM`
        mv $i ${j}$TO
    done
}
```
而且其他文件中，可以引用该库
```bash
#!/bin/bash
# function2.sh
. ./common.lib
echo $STD_MSG
rename .txt .bak
```
