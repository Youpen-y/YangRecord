以普通用户身份使用 `GDB` 连接 `Linux` 上的进程可能会失败。并显示 `ptrace: Operation not permitted` 。默认情况下，`Linux` 不允许附加到不是由调试器启动的进程。

解决方案：
- 以超级用户身份运行以下命令
```bash
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope
```
这会将 `ptrace` 级别设置为 0 ，之后使用用户权限即可附加到调试器未启动的进程。
永久更改：请编辑 `/etc/sysctl.d/10-ptrace.conf` 
更改 `kernel.yama.ptrace_scope = 1` 为 `kernel.yama.ptrace_scope = 0`

- 在没有 `yama` 的发行版上，可以使用 `libcap2-bin` 将 `ptrace` 权限分配给特定的可执行文件：`sudo setcap cap_sys_ptrace=eip /usr/bin/gdb`
- 或者，以超级用户身份启动 `GDB` 并附加到进程。使用 `root@machine` 进行登陆。由于安全原因，很多发行版配置为禁止通过 `ssh` 进行 `root` 登录。

对于 `Docker` 容器，有必要在创建容器时添加该功能，以便允许 `gdb` 附加到进程。具体做法：使用 `docker run` 启动容器时添加 `--cap-add=SYS_PTRACE` 标志。

---
`Yama` —— 一个 Linux 安全模块（`LSM`，Linux Security Module），旨在通过强制执行 core kernel 本身未处理的**附加安全策略**来增强系统范围的自主访问控制 `DAC` 保护。
为了启用 `yama` ，在构建内核时启用 `CONFIG_SECURITY_YAMA`
> LSM 是 Linux 内核中的一个框架，允许将安全策略实现为模块。
> LSM 用于实施超出 Linux 中默认 DAC 机制的额外安全措施。
> 常见的 LSM 包括 AppArmor, SELinux, Yama
> DAC，Discretionary Access Control 自主访问控制
> DAC 是 Linux/Unix 中默认的权限模型。它使文件或资源的所有者能够设置权限（读、写、执行）并控制对资源的访问。不过 DAC 不足以防范某些安全威胁，如特权升级或进程未经授权的内存访问
> Core Kernel，Linux 内核提供了 DAC 的基本机制，包括文件权限和用户/组所有权。
> 但，某些高级或系统范围的安全策略不属于核心内核的 DAC 模型的范围。

`yama` 主要目标是通过添加控制措施来强化 Linux 安全模型，以防止滥用系统资源。侧重于系统范围的策略，而不是每个资源或每个用户的策略。
- `ptrace` 限制。（即使一个应用程序被破坏，也无法附加到其他正在运行的应用程序，浏览器、加密钱包读取/写入敏感数据或更改流程）
	- `ptrace` 系统调用允许一个进程观察或控制另一个进程。`Yama` 引入了对 `ptrace` 系统调用的限制。
	- 默认情况下，`ptrace` 仅限于同一用户拥有的进程。 `Yama` 进一步限制这一点以避免权限升级攻击。
`ptrace` 范围（`/proc/sys/kernel/yama/ptrace_scope`）
- `0` —— 无附加限制（默认 `DAC` 行为），一个进程可以 `PTRACE_ATTACH` 到任何其他在相同 `uid` 下运行的进程，只要它是可转储的（即没有转换 `uid` ，启动特权或调用 `prctl(PR_SET_DUMPABLE...` ）
- `1` —— 将 `ptrace` 限制为仅限子进程
- `2` —— 将 `ptrace` 限制为仅通过 `prctl` 明确允许的进程
- `3` —— 完全禁用 `ptrace` 
- 进程内存限制
	- `Yama` 强制执行更严格的 DAC 策略，如即使 DAC 权限允许，也阻止对资源的访问。


附：[A Brief Tour of Linux Security Modules](https://www.starlab.io/blog/a-brief-tour-of-linux-security-modules)
