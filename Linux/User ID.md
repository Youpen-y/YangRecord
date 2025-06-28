Linux 系统中，每个进程都与三个用户 IDs 关联：
- 真实用户 ID （real uid, ruid）—— 进程所有者（即启动应用程序的用户，通常是 shell 登录用户）。真实 UID 永远不会改变。
- 有效用户 ID （effective uid, euid）—— 决定访问权限（系统检查该 ID 判断权限是否足够）
- 保留用户 ID （saved uid, suid）—— 存储先前的用户 ID ，以便以后恢复
有时还包含 `fsuid` （文件系统 UID） —— 只用于访问文件时检查权限，不影响其他系统调用，NFS，FUSE 用于安全隔离