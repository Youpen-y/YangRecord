`time` 处理时间和日期的核心模块，支持高精度计时、时区管理、格式化解析、定时任务等场景。

#### 核心类型与基础操作
- 时间点 - `time.Time` 表具体时刻，包含年月日、时分秒、纳秒和时区信息。
```go
now := time.Now()  // 获取当前时间
t := time.Data(2023, 8, 1, 10, 30, 0, 0, time.UTC)
year, month, day := now.Date()   // 分解日期分量
hour, min, sec := now.Clock()    // 分解时间分量
```
- 时间戳 - `now.Unix()` 秒级 和 `now.UnixNano()` 纳秒级

- 时间段 - `time.Duration` 表示时间间隔（如 `2*time.Hour + 30*time.Minute`），支持加减运算。
```go
duration := 5 * time.Minute
future := now.Add(duration)  // 时间加法
diff := t2.Sub(t1)  // 时间差计算
```

- 加载时区 - `time.LoadLocation` 显式指定时区，避免隐式依赖系统时区
```go
loc, _ := time.LoadLocation("Asia/Shanghai")
shanghaiTime := now.In(loc)
```

#### 时间格式化与解析
- 格式化规则 - 魔法数字（以 `2006-01-02 15:04:05` 作为格式模板）
```go
formatted := now.Format("2006-01-02 15:04:05 MST")
```