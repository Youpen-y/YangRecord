`-pedantic` 和 `-pedantic-errors` 是 GCC 编译器中的两个用于控制编译器行为的选项。强调代码符合标准的严格程度。

| 选项                 | 作用简述                   |
| ------------------ | ---------------------- |
| `-pedantic`        | 报告所有违反标准的为警告（Warnings） |
| `-pedantic-errors` | 报告所有违反标准的问题为错误（Errors） |

示例：
```bash
gcc -std=c99 -pedantic -Wall code.c
g++ -pedantic -Wall code.cpp
```