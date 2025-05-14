### VSCode 调试控制台（DEBUG Console）常用调试命令
#### 基础命令
- `-exec help` - 显示帮助信息
- `-exec continue` - 继续执行程序
- `-exec next` - 单步执行（跳过函数调用）
- `-exec step` - 单步执行（进入函数调用）
- `-exec finish` - 运行到当前函数返回
- `-exec quit` - 退出调试会话

#### 调用栈和帧
- `-exec backtrace` 或 `-exec bt` - 显示调用栈
- `-exec frame n` 或 `-exec f n` - 选择栈帧
- `-exec info locals` - 显示当前栈帧中的局部变量
- `-exec info args` - 显示当前函数的参数

信号处理
- `-exec info signals` - 显示信号处理设置
- `-exec handle SIGSEGV nostop noprint pass` - 设置忽略段错误信号

数据结构
- `-exec ptype variable` - 显示变量类型信息
- `-exec print *pointer@10` - 显示数组前 10 个元素
- `-exec set print pretty on` - 美化结构体和数组的打印格式

#### 调试时忽略某个信号（以 SIGSEGV 为例）
方法1：修改 `launch.json` 文件
如果使用 GDB，
```json
// 在 "setupCommands" 对应的值中添加指令

"configurations": [
	{
		"setupCommands": {
			{
				"description": "忽略 SIGSEGV 信号",
				"text": "handle SIGSEGV nostop noprint pass",
				"ignoreFailures": true
			}, 
		}
	}
]
```
方法2：在 DEBUG CONSOLE 面板输入指令
如果使用 GDB，输入
```
-exec handle SIGSEGV nostop noprint pass
```
如果使用 LLDB，输入
```
-exec process handle SIGSEGV --pass true --stop false --notify false
```
上述指令将告诉调试器在遇到 SIGSEGV 信号时：
- 不停止程序执行（`nostop/--stop false`）
- 不打印信号信息（`noprint/--notify false`）
- 将信号传递给程序继续处理（`pass/--pass true`）

> VS Code 的 Debug Console 是 GDB 的 MI 接口封装层，它只允许输入“变量查询”、“堆栈查看”这类高级指令，而不是原生 GDB 命令。

---

#### 设置内存断点（watchpoint）监视变量（以 `globaladdr` 为例）何时被修改
1. 使用 DEBUG Console 
GDB，
```
-exec watch globaladdr
```
LLDB，
```
-exec watchpoint set variable globaladdr
```

2. UI 界面添加

更精确地监控特定条件下的变化，还可以使用条件监视点：
- GDB：`-exec watch globaladdr if globaladdr < 1000`
- LLDB：`-exec watchpoint set variable globaladdr -C "globaladdr < 1000"

如果知道了在何处地址（如 `0x55555556950d`）修改了`globaladdr` 的值，检查触发点的代码。
- 在 DEBUG Console 输入 `-exec disassemble 0x55555556950d`
