EditorConfig 是一个帮助开发者在不同编辑器和 IDE 之间保持一致代码风格的工具。通过在项目根目录创建一个名为 `.editorconfig` 的文件，可以定义和维护各种文件格式的一致编码风格。

#### 主要特点
1. **跨编辑器兼容** - 支持 VSCode、Sublime Text、IntelliJ IDEA、Vim 等多种编辑器
2. **简单配置** - 使用简单的 INI 格式文件定义规则
3. **格式统一** - 可以统一管理缩进风格、行尾字符、文件编码等

EditorConfig Files —— `.editorconfig`，包含键值对。
Cores —— 解析符合该规范的文件，并向插件（`plugins`）提供键值对。
Plugins —— 从 cores 接收键值对，并根据键值对更新编辑器设置。
Editors —— 允许编辑文件，并使用 plugins 来更新正在编辑的文件的设置。

注释：以 `;` 或 `#` 开头。
常用配置属性：
- `indent_style`：设置缩进风格（tab 或 space）
- `indent_size`：设置缩进大小（通常为 2 或 4）
- `end_of_line`：设置行尾字符（`lf`、`cr`或`crlf`）
- `charset`：设置文件编码（`utf-8`，`latin1`等）
- `trim_trailing_whitespace`：是否删除行尾空格
- `insert_final_newline`：是否在文件末尾插入空行

#### 示例配置文件
```
# 顶级 EditorConfig 文件
root = true

# 所有文件通用设置
[*]
charset = utf-8
indent_style = space
indent_size = 2
end_of_size = lf
trim_trailing_whitespace = true
insert_final_newline = true

# 针对 Python 文件的设置
[*.py]
indent_size = 4

# 针对 Markdown 文件的设置
[*.md]
trim_trailing_whitespace = false
```

使用方法：
1. 在项目根目录创建 `.editorconfig` 文件
2. 根据项目需求编写规则
3. 确保编译器支持 EditorConfig

Linux 内核 `.editorconfig` 文件分析
```
# SPDX-License-Identifier: GPL-2.0-only

root = true

# 应用于 C 源文件/头文件、设备树文件（.dts, .dtsi, .dtso）、汇编文件（.s, .S）、AWK脚本（.awk）、Makefiles 和相关文件（.mk, Makefile, Makefile.*）、内核配置文件（Kconfig）
[{*.{awk,c,dts,dtsi,dtso,h,mk,s,S},Kconfig,Makefile,Makefile.*}]
# 使用 UTF-8 编码
charset = utf-8
# 使用 LF 作为行尾字符（Linux/Unix 风格）
end_of_line = lf
# 文件末尾添加空行
insert_final_newline = true
# 使用制表符而非空格进行缩进
indent_style = tab
# 制表符宽度为 8 个空格
indent_size = 8

# 应用于 python 文件、rust 文件、JSON 文件
[*.{json,py,rs}]
charset = utf-8
end_of_line = lf
insert_final_newline = true
# 使用空格而非制表符进行缩进
indent_style = space
# 缩进宽度为 4 个空格
indent_size = 4

# 特定目录下的 python 文件，覆盖上面设置的 python 文件规则
# this must be below the general *.py to overwrite it
[tools/{perf,power,rcu,testing/kunit}/**.py,]
indent_style = tab
indent_size = 8

[*.yaml]
charset = utf-8
end_of_line = lf
insert_final_newline = true
indent_style = space
indent_size = 2
```




