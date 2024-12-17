当使用 signal-driven I/O 时，当 `fd` 上发生某些事情时，内核会使用信号通知我们。

使用信号驱动 I/O 的三个步骤
- 为 SIGIO 信号编写特定的信号处理程序（使用 signal/sigaction 注册）
- 设置 socket 文件描述符 owner 
```c
fcntl(fd, F_SETOWN, getpid());

// arg > 0: process ID == arg
// arg < 0: process group ID = |arg|
```
- 为该 socket 启用信号驱动 I/O （fcntl or ioctl）
```c
fcntl(fd, F_SETFL, O_ASYNC);

// int on = 1;
// ioctl(fd, FIOASYNC, &on);

// 二者的区别在于一些系统不支持 fcntl
```
