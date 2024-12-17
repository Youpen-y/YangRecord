`getenv`, `secure_getenv` - 获取环境变量

```c
#include <stdlib.h>

char *getenv(const char *name);
char *secure_getenv(const char *name);  // 测试宏要求 _GNU_SOURCE
```

描述
`getenv()` 函数搜索环境列表以查找环境变量 `name` ，并返回指向相应 `value` 字符串的指针。

示例：
