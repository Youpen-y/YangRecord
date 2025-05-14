`os` 包是 Go 语言标准库中用于与操作系统交互的核心模块，涵盖了文件操作、环境变量管理、命令行参数处理、进程控制等功能。

#### 文件与目录操作
##### 创建与打开文件
- 创建文件 - `os.Create()`，若存在则清空内容
```go
file, err := os.Create("test.txt")
if err != nil {
	panic(err)
}
defer file.Close()
```
- 打开文件 - `os.Open()` 以只读方式打开文件；`os.OpenFile()` 支持更多模式（如追加、读写）
```go
file, err := os.OpenFile("data.txt", os.O_RDWR|os.O_APPEND, 0644)
if err != nil { panic(err) }
defer file.Close()
```

##### 文件读写与信息获取
- 读取文件内容 - `os.ReadFile()` 直接读取全部内容
```go
data, err := os.ReadFile("test.txt")
if err != nil { panic(err) }
fmt.Println(string(data))
```
- 写入文件 - `os.WriteFile()` 快速写入字节数据
```go
content := []byte("Hello, World!")
err := os.WriteFile("output.txt", content, 0644)
if err != nil { panic(err) }
```
- 获取文件信息 - `os.Stat()` 返回文件大小、权限等元数据
> 返回一个 `os.FileInfo` 对象，其中包含文件的详细信息，如文件名、大小、修改时间等
> `func Stat(name string) (FileInfo, error)`
> `os.FileInfo` 是一个接口，包含文件名：`Name()` ，文件大小：`Size()`，文件权限：`Mode()`，文件最后的修改时间：`ModTime()`，是否为目录：`IsDir()`，底层系统接口：`Sys()`

```go
info, err := os.Stat("test.txt")
if err != nil {
	if os.IsNotExist(err) {
		fmt.Println("文件不存在")
	} else {
		fmt.Println("错误：", err)
	}
	return
}
fmt.Println("Name: ", info.Name())
fmt.Println("Size: ", info.Size())
fmt.Println("Modified Time: ", info.ModTime().Format(time.RFC3339))
fmt.Println("Dir? ", info.IsDir())
fmt.Println("Mode: ", info.Mode())
```

##### 文件管理
- 重命名与删除 - `os.Rename()` 和 `os.Remove()`
```go
err := os.Rename("old.txt", "new.txt")
err := os.Remove("temp.txt")
```
- 截断文件 - `os.Truncate()` 调整文件大小
```go
err := os.Truncate("log.txt", 1024)
```

#### 环境变量操作
##### 读取与判断变量
- 获取环境变量 - `os.Getenv()` 和 `os.LookupEnv()`
```go
path := os.Getenv("GOPATH")
value, exists := os.LookupEnv("API_KEY")
```
- 列出所有环境变量 - `os.Environ()` 返回 `key=value` 格式的切片
```go
for _, env := range os.Environ() {
	fmt.Println(env)
}
```

##### 设置与删除变量
- 设置变量 - `os.Setenv()`
```go
err := os.Setenv("DEBUG_MODE", "true")
```
- 删除变量 - `os.Unsetenv()`
```go
err := os.Unsetenv("TEMP_TOKEN")
```

##### 环境变量展开
- 替换字符串中的变量 - `os.ExpandEnv()`
```go
str := os.ExpandEnv("当前用户：$USER, 目录：${HOME}")
```

#### 命令行参数与进程管理
##### 命令行参数解析
- 获取参数 - `os.Args` 存储命令行输入的参数（第一项为程序路径）
```go
args := os.Args  // os.Args 是一个 []string 类型的切片
if len(args) > 1 {
	fmt.Println("参数列表:", args[1:])
}
// args[0]永远都是程序本身的路径,args[1:]包含所有参数
```

##### 进程信息与控制
- 获取进程 ID - `os.Getpid()`
```go
pid := os.Getpid()
fmt.Println("进程Id：", pid)
```
- 切换工作目录 - `os.Chdir()`
```go
err := os.Chdir("/tmp")
```
- 退出程序 - `os.Exit()`
```go
os.Exit(1)  // 退出当前执行的进程
```

#### 其他实用
- 获取操作系统的页面大小 - `os.Getpagesize()`
- 获取主机名称 - `os.Hostname()`

##### 临时目录与路径
- 获取临时目录 - `os.TempDir()`
```go
tmpDir := os.TempDir()
fmt.Println("临时目录：", tmpDir)
```
- 获取当前工作目录 - `os.Getwd()`
```go
dir, err := os.Getwd()
if err != nil { panic(err) }
```

##### 用户与权限
- 获取用户信息 - `user.Current()` 需导入 `os/user` 包
```go
u, _ := user.Current()
fmt.Println("用户名：", u.Username)
```
- 修改文件权限 - `os.Chmod`
```go
err := os.Chmod("script.sh", 0755)  // 设置可执行权限
```