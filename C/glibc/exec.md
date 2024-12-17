```
execl, execlp, execle, execv, execvp, execvpe - 执行一个文件
```

```c
#include <unistd.h>

extern char **environ;

int execl(const char *path, const char *arg, ... /*, (char *) NULL */);
int execlp(const char *file, const char *arg, ... /*, (char *) NULL */);
int execle(const char *path, const char *arg, ..., /*, (char *) NULL, char *const envp[] */);
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execvpe(const char *file, char *const argv[], char *const envp[]);
```
描述
`exec()` 系列函数用新的进程映像替换当前进程映像。可以使用这些函数在`fork()`创建子进程后执行新程序

这些函数的初始参数是要执行的文件的名称。

函数可以根据 `exec` 后面的字母进行分组：
后面的 `e` 代表着新进程映像的环境通过参数 `envp` 指定。
`p` - 在 PATH 环境变量中指定的以冒号分隔的目录路径名列表中查找文件。
`l` - `execl()`, `execlp()`, `execle()`
`const char *arg` 和随后的省略号 `...` 可以被认为是 arg0, arg1, ..., argn 。它们一同描述了一个或多个指向 null-terminated strings 的指针列表，这些字符串代表可用于执行程序的参数列表。
```c
if (execl("/bin/ls", "ls", "-l", NULL) == -1) {
    perror("execl failed");
}

// Environment variables
char *env[] = {"MYVAR=123", NULL};
if (execle("/usr/bin/env", "env", NULL, env) == -1) {
    perror("execle failed");
}
// 相比于 execl, execle 允许通过额外的参数 envp 为新进程指定环境， env 是表示环境变量的字符串数组


if (execlp("data", "date", NULL) == -1) {
    perror("execlp failed");
}
// 而 execlp 允许在 PATH 环境变量中的目录下搜索可执行文件，这使得可以仅指定可执行文件的名称，而不是完整路径
```

`v` - `execv()`, `execvp()`, `execvpe()`
`char const argv[]` 参数是一个指向空终止字符串的指针数组，这些字符串表示新程序可用的参数列表。按照惯例，第一个参数应该指向与正在执行的文件相关联的文件名。指针数组必须以空指针终止。
```c
char *args[] = {"ls", "-l", NULL};
if (execv("/bin/ls", args) == -1) {
    perror("execv failed");
}

// Execute the "date" command using execvp (no need for full path)
char *args[] = {"date", NULL}; 
if (execvp("date", args) == -1) { 
    perror("execvp failed"); 
}

// Environment variables 
char *env[] = {"MYVAR=Hello", "YOURVAR=World", NULL}; 
// Execute the "env" command with a custom environment variable 
char *args[] = {"env", NULL}; 
if (execvpe("env", args, env) == -1) {
    perror("execvpe failed"); 
}
```

返回值
`exec()` 函数在错误时返回 -1 ，并设置 `errno` 来指示错误。