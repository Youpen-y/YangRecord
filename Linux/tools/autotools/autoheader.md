`autoheader` - 为 `configure` 脚本创建模板头文件（template header），它根据 `configure.ac` 中 `AC_CONFIG_HEADERS` 来确定模板的名称（如果有多个，使用第一个）。

`autoheader` 会记录所有可能使用的符号，这些符号通过 `AC_DEFINE` 或 `AC_DEFINE_UNQUOTED` 调用。

宏：
```c
AC_DEFINE(variable, value, [description])
AC_DEFINE(variable)
# 描述仅在使用 AC_CONFIG_HEADERS 时有用。这时，描述将被放入生活的 `config.h.in` 作为宏定义之前的注释。
# 未给出 value 和 描述 时，value 默认为 1 而非空字符串（保持向后兼容）

AC_DEFINE_UNQUOTED(variable, value, [description])
AC_DEFINE_UNQUOTED(variable)
# 对变量和值执行一次 three shell expansions：变量扩展（"$"）、命令替换（"`"）、和反斜杠转义（"\"）
```

语法
```bash
autoheader [OPTION]... [TEMPLATE-FILE]
```

如果希望 `autoheader` 读取 `configure.ac`（或旧项目中的 `configure.in` ，默认） 以外的文件，请指定输入文件。

`autoheader` 将根据 `configure.ac` 中的 `AC_CONFIG_HEADER([config.h])` 生成一个头文件模板 `config.h.in`，后续

如：`configure.ac`
```c
AC_INIT([MyProject], [1.0])
AC_CONFIG_HEADERS([config.h])
AC_DEFINE([ENABLE_DEBUG], [1], [Enable debugging])
AC_DEFINE([PACKAGE_NAME], ["MyProject"], [Package name])
AC_OUTPUT
```
首先调用 `autoheader`，将生成 `config.h.in` 如下
```c
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Enable debugging */
#undef ENABLE_DEBUG

/* Define to the address where bug reports for this package should be sent. */
#undef PACKAGE_BUGREPORT

/* Package name */
#undef PACKAGE_NAME

/* Define to the full name and version of this package. */
#undef PACKAGE_STRING

/* Define to the one symbol short name of this package. */
#undef PACKAGE_TARNAME

/* Define to the home page for this package. */
#undef PACKAGE_URL

/* Define to the version of this package. */
#undef PACKAGE_VERSION
```
随后调用 `autoconf`；`./configure` 将生成 `config.h` 头文件
```c
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Enable debugging */
#define ENABLE_DEBUG 1

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Package name */
#define PACKAGE_NAME "MyProject"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "MyProject 1.0"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "myproject"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.0"
```