`sysconf` - 在运行时获取配置信息

头文件
``` c
#include <unistd.h>
```

函数定义
``` c
long sysconf(int name);
```

描述

POSIX 允许应用在编译或运行时测试某些选项是否支持，或某些配置常量或限制的值是什么？\

在编译时，通过 包含 `<unistd.h>` 和/或 `<limits.h>` 并测试某些 macros 的值来完成 \

在运行时，可以使用函数 `sysconf()` 来询问某些数值型的值。可以通过使用 `fpathconf(3)` 和 `pathconf(3)` 来询问取决于文件所在的文件系统的数值型的值。可以使用 `confstr(3)` 来询问一些字符串值。

从这些函数得到的值是系统配置常量，它们在进程运行期间不改变。\

对于选项，通常，对于选项，通常有一个常量_POSIX_F00，可以在`<unistd.h>` 中定义。如果未定义，应该在运行时询问。

- 如果定义为-1，则不支持该选项。

- 如果将其定义为0，则存在相关的函数和头文件，但必须在运行时询问可用的支持程度。

- 如果它被定义为-1或0以外的值，则支持该选项。

通常，该值(如200112L)表示描述该选项的POSIX版本的年份和月份。只要POSIX版本尚未发布，Glibc就使用值1来表示支持。sysconf()参数将是_SC_FOO。有关选项列表，请参见posixoptions(7)。

  

POSIX.1 变量 （给定变量名，`sysconf()` 函数查询相应变量的值）

- ARG_MAX - `_SC_ARG_MAX`

  - `exec(3)`函数族的参数的最大长度。不能小于_POSIX_ARG_MAX（4096）

- CHILD_MAX - `_SC_CHILD_MAX`  

  - 每个用户ID的最大并发进程数。不能小于_POSIX_CHILD_MAX

- HOST_NAME_MAX - `_SC_HOST_NAME_MAX`

  - `gethostname(2)`返回的主机名的最大长度，不包括终止空字节。不能小于_POSIX_HOST_NAME_MAX（255）

- LOGIN_NAME_MAX - `_SC_LOGIN_NAME_MAX`

  - 登录名的最大长度，包括终止空字节。不能小于_POSIX_LOGIN_NAME_MAX（9）

- clock ticks - `_SC_CLK_TCK`

  - 每秒时钟滴答的次数。相应的变量已过时。它被称为CLK_TCK。(Note：宏CLOCKS_PER_SEC不提供信息：它必须等于1000000)

- OPEN_MAX - `_SC_OPEN_MAX`

  - 进程在任何时候可以打开的最大文件数。必须不小于_POSIX_OPEN_MAX（20）

- PAGESIZE - `_SC_PAGESIZE`

  - 以字节为单位的页大小。不得小于1。(Some系统使用PAGE_SIZE。)

- RE_DUP_MAX - `_SC_RE_DUP_MAX`

  - `regexec（3）`和`regcomp（3）`允许的BRE重复出现的次数。不能小于_POSIX 2_RE_DUP_MAX（255）

- STREAM_MAX - `_SC_STREAM_MAX`

  - 进程在任何时候可以打开的最大流数。如果定义了，它与标准C宏FOPEN_MAX具有相同的值。不能小于_POSIX_STREAM_MAX（8）

- SYMLOOP_MAX - `_SC_SYMLOOP_MAX`

  - 在解析返回CNOOP之前，在路径名中看到的符号链接的最大数量。必须不小于_POSIX_SYMLOOP_MAX（8）

- TTY_NAME_MAX - `_SC_TTY_NAME_MAX`

  - 终端设备名称的最大长度，包括终止空字节。不能小于_POSIX_TTY_NAME_MAX（9）

- TZNAME_MAX - `_SC_TZNAME_MAX`

  - 时区名称中的最大字节数。不能小于_POSIX_TZNAME_MAX（6）

- _POSIX_VERSION - `_SC_VERSION`

  - 表示POSIX.1标准批准的年份和月份，格式为YYYYMML;值199009 L表示1990年9月修订

  

POSIX.2 variables