#### 1. 宏 —— 定义栈
特点：可指定类型、大小
```c
#define my_stack(T, n)    struct { int idx; T items[n];}
```
组成：
1. `my_stack(T, n)` 带参数宏
	- `T` 是类型参数，表示栈中存储的元素类型
	- `n` 是大小参数，表示栈的最大容量
2. 展开后的结构包含：
	- `int idx`：当前栈顶的索引（指向下一个可用的位置）
	- `T items[n]`：一个固定大小的数组，用于存储栈中的元素

使用示例：
```c
// 创建一个能存储 10 个整数的栈
my_stack(int, 10) i_stack = {0};    // 初始化 idx 为 0
// 创建一个能存储 5 个浮点数的栈
my_stack(float, 5) f_stack;

// 压入数据
i_stack.items[i_stack.idx++] = 22;

// 弹出数据
int value = i_stack.items[--i_stack.idx];

// 结构体栈
typedef struct {
	int x;
	int y;
} Point;

// 创建一个能存储 10 个点的栈
my_stack(Point, 10) point_stack = {0};

// 压入点
Point p1 = {10, 20};
point_stack.items[point.idx++] = p1;

// 弹出点
Point popped = point_stack.items[--point_stack.idx];
```

带检查的压入和弹出操作：
```c
// 安全的压入操作
int safe_push(my_stack(int, 10) *stack, int value) {
	if (stack->idx < 10) {
		stack->items[stack->idx++] = value;
		return 0;
	}
	return -1;
}

// 安全的弹出操作
int safe_pop(my_stack(int, 10) *stack) {
	if (stack->idx > 0) {
		return stack->items[--stack->idx];
	}
	return -1;
}
```

遍历与清空
```c
// 遍历并打印栈中所有内容
void print_stack(my_stack(int, 10) *stack) {
	for (int i = 0; i < stack->idx; i++) {
		printf("Element %d: %d\n", i, stack->items[i]);
	}
}

// 清空栈
void clear_stack(my_stack(int, 10) *stack) {
	stack->idx = 0;
}
```

#### 2. 宏 —— 限制数值范围
```c
#define min(a, b)            ((a) < (b) ? (a) : (b))
#define max(a, b)            ((a) > (b) ? (a) : (b))
#define my_clamp(x, a, b)    min(b, max(a, x))
```
作用：确保值 `x` 被限制在 `[a, b]` 范围内。
工作原理：
- 首先用 `max(a, x)` 确保值不小于 `a`
- 然后用 `min(b, ...)` 确保值不大于 `b`
- 结果 `x` 被限制在 `a` 和 `b` 之间

使用示例：
```c
int value = 15;
int min_value = 10;
int max_value = 20;

int clamped = my_clamp(value, min_value, max_value);  // 结果：15

value = 5;
clamped = my_clamp(value, min_value, max_value);  // 结果：10

value = 25;
clamped = my_clamp(value, min_value, max_value);   // 结果：20
```

#### 3. 宏 —— 消除未使用变量警告
```c
#define unused(x) ((void) (x))
```
作用：
显式地标记一个参数或变量为未使用，以避免编译器发出 `-Wunused-parameter`的警告。

好处：
- 提高代码可读性
- 明确表明变量是有意未使用的
- 避免编译器警告
- 保持代码整洁


#### 4. 内存缓冲日志
```c
#include <stdio.h>
#include <string.h>

#define LOGBUF_SIZE 64000

static char logbuf[LOGBUF_SIZE];
static int logbuf_updated = 0;

// 追加日志（带换行）
static void write_log(const char *text) {
	size_t current_len = strlen(logbuf);
	size_t text_len = strlen(text);
	// 当前为空则不需要添加 '\n'，否则需要添加 '\n'
	size_t needed_len = text_len + (current_len > 0 ? 1 : 0);

	// 检查是否有足够空间，'+1' 的目的是为了给字符串末尾的空字符`\0`留出空间
	if (current_len + needed_len + 1 >= LOGBUF_SIZE) {
		fprintf(stderr, "Log buffer overflow. Message dropped: %s\n", text);
		return;
	}

	if (current_len > 0) {
		strcat(logbuf, "\n");
	}
	strcat(logbuf, text);

	logbuf_updated = 1;
}

// 将日志缓冲写入文件，并清空缓冲
static void flush_log_to_file(const char *filename) {
	if (!logbuf_updated) {
		printf("No new logs to write.\n");
		return;
	}

	FILE *fp = fopen(filename, "a");
	if (!fp) {
		perror("Failed to open log file");
		return;
	}

	fprintf(fp, "%s\n", logbuf);
	fclose(fp);

	logbuf[0] = '\0';    // 清空缓冲
	logbuf_updated = 0;  // 重置更新标志
	printf("Logs written to %s\n", filename);
}

int main() {
    write_log("Program started");
    write_log("Loading configuration...");
    write_log("Configuration loaded successfully");

    flush_log_to_file("mylog.txt"); // 写入日志文件
    flush_log_to_file("mylog.txt"); // 再次调用：没有新日志

    return 0;
}
```