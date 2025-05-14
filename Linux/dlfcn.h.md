`dlfcn.h` - 动态链接

描述：
至少包含以下符号用于构造 `dlopen()` 的 `mode` 参数
- `RTLD_LAZY` - 延迟解析符号，首次使用时绑定，重定位依赖于实现的时间执行
- `RTLD_NOW` - 立即解析所有符号，当对象首次被加载时，执行重定位
- `RTLD_GLOBAL` - 导出符号供后续加载的库使用
- `RTLD_LOCAL` - 符号仅对当前库可见（默认）
- `RTLD_NEXT` - 查找后续依赖链中的符号（用于函数拦截）

函数
```c
void *dlopen(const char *file, int mode);
```
`dlopen()` - 打开动态库，获取对可执行目标文件的访问权限，返回一个不透明句柄 `handle` 供后续操作。
返回值：成功则返回句柄；失败返回 `NULL` ，通过 `dlerror()` 获取错误信息。

```c
void *dlsym(void *handle, const char *symbol);
```
`dlsym()` - 从已打开的库句柄中查找符号（函数或变量）的地址
参数：
- `handle`：`dlopen()` 返回的句柄
- `symbol`：符号名称（如函数名 `cos` 或变量名 `global_var`）
返回值：成功返回符号地址；失败则返回 `NULL`，可通过 `dlerror()` 判断是否真错误（符号值为 0 时可能与错误混淆）

```c
int dlclose(void *);
```
`dlclose()` - 关闭动态库。减少库的引用计数，当计数归零时卸载库。
参数：`dlopen()` 返回的句柄
返回值：成功返回 0 ；失败返回非零值

```c
char *dlerror(void);
```
`dlerror()` - 获取最近一次动态链接操作的错误信息，并清空错误缓冲区。
返回值：无错误时返回 `NULL` ；有错误时返回描述错误的字符串（需及时调用，否则可能被后续操作覆盖）


示例：
#### 创建动态库
##### 编写库代码
```c
// hello.c
#include <stdio.h>

void sayHello() {
	printf("Hello from dynamic library!\n");
}
```

##### 编译生成动态库（`.so`）
```bash
gcc -fPIC -shared hello.c -o libhello.so
```
- `-fPIC`：生成位置无关代码（Position Independent Code），适配动态库特性。
- `-shared`：指定生成共享库文件。

#### 主程序代码
```c
#include <stdio.h>
#include <dlfcn.h>

int main() {
	void *handle;   // 动态库句柄
	char *error;    // 错误信息指针
	void (*sayHello)(); // 函数指针定义

	// 1. 打开动态库
	handle = dlopen("./libhello.so", RTLD_LAZY); // 使用相对路径加载库
	if (!handle) {
		fprintf(stderr, "Error opening library: %s\n", dlerror());
		return 1;
	}

	// 2. 清空之前的错误信息
	dlerror();

	// 3. 获取符号地址
	sayHello = (void (*))dlsym(handle, "sayHello"); // 强制转换函数指针
	error = dlerror();
	if (error) {
		fprintf(stderr, "Error loading symbol: %s\n", error);
		dlclose(handle);
		return 1;
	}

	// 4. 调用动态库中的函数
	sayHello();

	// 5. 关闭动态库
	if (dlclose(handle)) {
		fprintf(stderr, "Error closing library: %s\n", dlerror());
		return 1;
	}

	return 0;
}
```

编译
```bash
gcc main.c -o main -ldl  # 必须链接 libdl 库
```

运行
```bash
./main
```


注意：如果调用带参数的函数，需在函数指针中明确参数类型：如 `int add(int a, int b)`
```c
typedef int (*AddFunc)(int, int);  // 定义函数指针类型
AddFunc add = (AddFunc)dlsym(handle, "add")
int result = add(3, 5);
```