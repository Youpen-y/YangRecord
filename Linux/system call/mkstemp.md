`mkstemp`, `mkostemp`, `mkstemps`, `mkostemps` - 创建一个唯一的临时文件

```c
#include <stdlib.h>

int mkstemp(char *template);
int mkostemp(char *template, int flags);
int mkstemps(char *template, int suffixlen);
int mkostemps(cahr *template, int suffixlen, int flags);
```