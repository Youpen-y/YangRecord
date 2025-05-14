Go 语言的 `testing` 包提供了强大的测试功能，包括单元测试、性能测试和并发测试等。

#### 使用方法
1. 创建测试文件：测试文件通常与被测试的源文件放在同一目录下，并以 `_test.go` 作为后缀。
2. 编写基本的测试函数：测试函数的命名必须以 `Test` 开头，并接收一个 `*testing.T` 类型的参数。
3. 运行测试：使用 `go test` 命令运行测试。

示例：
```go
package math

import "testing"

func TestSum(t *testing.T) {
	result := Sum(2, 3)
	if result != 5 {
		t.Errorf("Sum(2, 3) = %d; want 5", result)
	}
}
```

测试指令：`go test`

在编写测试时，需要判断 `go test` 测试是否成功。成功的测试函数将返回，当测试失败时可使用以下函数发出信号。
- `func (t *T) Fail()` - `Fail` 将测试函数标记为失败但继续执行
- `func (t *T) FailNow()` - `FailNow` 将测试函数标记为失败并停止其执行。将跳过此文件中的任何剩余测试，并继续执行下一个测试。
- `func (t *T) Log(args ...interface{})` - `Log` 使用默认格式设置其参数的格式，类似于 `Print()` ，并将文本记录在错误日志中。
- `func (t *T) Fatal(args ...interface{})` - `Fatal` 等价于 `Log()` 后跟 `FailNow()s`

示例：
```go
// Entering the twilight zone
func TestEven(t *testing.T) {
	if Even(2) {
		t.Log("2 should be odd!")
		t.Fail()
	}
}
```

