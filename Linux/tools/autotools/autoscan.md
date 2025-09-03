`autoscan` 是 `autoconf` 的附带工具程序。—— 帮助生成或检查 `configure.ac` 
将扫描从给定目录 `SRCDIR` 开始的整个源码目录树。如果没有提供目录，它会默认从当前目录开始扫描。

`autoscan` 在扫描后会做以下几件事：
- 检查源码中是否有一些跨平台兼容性问题，比如使用了不可移植的函数或头文件，然后根据检测结果，建议添加相应的 `AC_CHECK_HEADERS`, `AC_CHECK_FUNCS` 等宏到 `configure.ac`
- 如果已经有了一个粗略的 `configure.ac` ，它会检查是否遗漏了必要的检查项。
- 最后，它会生成一个 `configure.scan` 文件，该文件是一个草稿式 `configure.ac` 。可重名它为 `configure.ac`。

总结：

| 项目  | 说明                                                              |
| --- | --------------------------------------------------------------- |
| 工具  | `autoscan`（Autoconf 提供）                                         |
| 输入  | 当前目录或指定目录的源代码                                                   |
| 功能  | 1. 查找跨平台问题 2. 检查 `configure.ac` 是否缺失配置 3. 生成草稿 `configure.scan` |
| 输出  | `configure.scan` 文件（初步的 `configure.ac`）                         |

选项：
- `-v/--verbose`：详细报告处理
- `-d/--debug`：不移除临时文件

库目录：
- `-B/--prepend-include=DIR`：将目录 DIR 添加到搜索路径的前面
- `-I/--include=DIR`：追加 目录 DIR 到搜索路径