`notify-send` - Linux 桌面环境中用于发送桌面通知的小工具。

语法：
```bash
notify-send [OPTIONS] {summary} [body]
```
选项：
- `-a/--app-name=APP_NAME`：指定通知的 APP 名称。
- `-A/--action=[NAME=]Text...`：
- `-u/--urgency=LEVEL`：指定紧急程度（`low`，`normal`，`critical`）
- `-t/--expire-time=TIME`：通知出现在屏幕上的持续时间，以毫秒为单位；并不是所有实现都使用这个参数。GNOME Shell 和 Notify OSD 通常忽略它，但 Plasma 会为紧急程度1极高的通知忽略该选项。
- `-i/--icon=ICON`：指定要显示的图标文件名或库存图标。
- `-c/--category=TYPE[,TYPE...]`：指定通知类别
- `-h/--hint=TYPE:NAME:VALUE`：指定要传递的基本额外数据。有效类型为 `BOOLEAN`、`INT`、`DOUBLE`、`STRING`、`BYTE` 和 `VARIANT`
- `-p/--print-id`：打印通知 ID 
- `-r/--replace-id=REPLACE_ID`：将取代通知的 ID
- `-w/--wait`：等待通知关闭后再退出。如果设置了 `expire-time` ，则该时间将作为最长等待时间
- `-e/--transient`：显示临时通知。临时通知会绕过服务器的持久化功能（如果有）。因此，在用户确认之前，通知不会被保留。

描述：
从命令行通过通知守护进程向用户发送桌面通知。通常用于脚本或终端中显示临时提示。它是库 `libnotify` 的命令行前端，支持基本的通知内容发送。

[桌面通知规格书](https://specifications.freedesktop.org/notification-spec/latest/)

示例：
1. 发送一个简单通知
```bash
notify-send "Build Completed" "Your program has been compiled successfully"
```

2. 设置通知等级
```bash
notify-send -u low "Low Priority" "nothing special"
notify-send -u normal "Normal priority" "normal"
notify-send -u critical "Critical" "something special happened"
```

3. 设置通知持续时间（毫秒）
```bash
notify-send -t 5000 "Notification" "This notification will show 5 minutes"
```