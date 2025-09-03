目的：把当前 shell （及其任何子进程）的所有本地化设置为一次性锁定为最基本的 UTF-8 环境，既保证 7-bit ASCII 兼容性，又支持 UTF-8 多字节字符。

1. `LC_ALL` 具有最高优先级，将覆盖其他所有 `LC_*` 变量
`LC_ALL` 是最强的 locale 变量，只要它被设置，系统会忽略 `LANG` 、`LC_CTYPE`、`LC_COLLATE` 等所有其余本地化设置。

2. 解决传统 C locale 的编码痛点
经典 C locale 的默认编码是 ASCII ，无法表示非 ASCII 字符，Python3，grep，sort 等工具在多语言文本下会报错或行为异常。
`C.UTF-8` 在保留最小、可预测的 POSIX 语义的同时，把字符集升级为 UTF-8 ，从而两全其美。

3. 脚本/CI 中的“安全沙箱”
在脚本开头 `export LC_ALL=C.UTF-8` 可以确保无论用户系统语言如何，工具链始终按统一的字节序、排序规则和 UTF-8 编码工作，避免“在我电脑能跑”这类问题。

4. 性能优化
对 `grep`、`sort` 等大量使用字节级比较的工具来说，`C.UTF-8`（或纯 `C`）跳过了复杂的本地化规则，速度通常明显快于 `en_US.UTF-8` 等全功能 locale。

5. 容器、SSH、CI 默认环境的常用补救
很多最小容器镜像或远程主机只提供 `C`，没有其它 locale；手动 `export LC_ALL=C.UTF-8` 即可让 Python、Node、Perl 等运行时不再抱怨 “ANSI_X3.4-1968” 编码。


| 维度              | C                    | C.UTF-8                                                        |
| --------------- | -------------------- | -------------------------------------------------------------- |
| **字符集**         | 纯 ASCII（0–127）       | **UTF-8**（支持全部 Unicode）                                        |
| **来源/目的**       | POSIX 强制规定的最小 locale | 各发行版（Debian 率先，后 Fedora、Ubuntu 等跟进）为了 **既保持可预测性又支持 UTF-8** 而引入 |
| **LC\_CTYPE**   | 仅单字节                 | 处理多字节 UTF-8                                                    |
| **LC\_COLLATE** | 按字节值排序               | 按 Unicode 码点排序                                                 |
| **是否内置**        | glibc **必须**内置       | glibc ≥ 2.35 起 **已内置**                                         |
| **可移植性**        | 所有 POSIX 系统都有        | **“hit or miss”**：Debian/Ubuntu/Fedora/Alpine 有；Arch、macOS 缺   |
| **设置方法**        | `export LC_ALL=C`    | `export LC_ALL=C.UTF-8`（或 `C.utf8`）                            |
> 当需要 脚本结果可移植、排序可预测，又想正确处理 emoji/中文时，用 `C.UTF-8` 比传统 `C` 更合适；




