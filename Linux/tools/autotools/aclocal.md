`aclocal` - 通过扫描 `configure.ac`（或旧的 `configure.in`） 生成 `aclocal.m4`

`automake` 包含许多可以在您的包中使用的 `autoconf` 宏。这些宏必须在 `aclocal.m4` 中定义，否则不会被 `autoconf` 看到。

`aclocal` 提供了一种获取 `automake` 提供的宏的便捷方法，而无需到处搜索。同时，`aclocal` 机制允许其他包提供自己的宏，可以使用它来维护自定义的宏。

启动时，`aclocal` 会扫描可以找到的所有 `.m4` 文件以寻找宏定义；然后它扫描 `configure.ac`，将定义的宏以及需要的宏放入 `aclocal.m4`。








