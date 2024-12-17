#### 作业控制命令
一、作业控制命令
```bash
command &    # 在后台运行 command
```

```bash
bg          # 将一个已暂停的进程放到后台运行
fg          # 将后台进程调至前台运行
jobs        # 列出当前终端会话中的后台作业

// 根据 jobs 输出的结果可使用 kill %1 操控
```

二、发送信号

```bash
kill -l     # 列举信号

kill -STOP PID    # 暂停进程（等价于 Ctrl+Z）
kill -CONT PID    # 继续已暂停的进程
kill -KILL PID    # 强制终止进程 == kill -9 PID
```

三、进程优先级台哦正
```bash
nice
renice
```

四、根据 PID 得到进程信息
```
ps -p [pid] -o user,%cpu,%mem,cmd
```

五、快捷键发送信号
```c
Ctrl-C	SIGINT （进程终止）
Ctrl-Z	SIGTSTP（进程暂停执行）
Ctrl-\	SIGQUIT（进程终止并 dump core）
Ctrl-T	SIGINFO（）
```