Secure Shell Protocol 是一种加密网络协议，用于在不安全的网络上安全地操作网络服务。其最著名的应用是远程登录和命令行执行。
`SSH` - Secure Shell，基于 client-server 架构，将 SSH client 与 SSH 服务器连接起来。要建立 SSH 连接，远程计算机必须运行一个名为 SSH daemon (`sshd`) 的软件，该软件侦听特定网路端口（默认22），验证连接请求，并在用户提供正确的凭据时生成适当的环境。

而用户的计算机必须具有 SSH client ，它知道如何使用 SSH 协议进行通信。

### 基于 SSH 密钥的身份验证
SSH 密钥是一组相匹配的用于身份验证的加密密钥，每组包含一个公钥和一个私钥。公钥可以随便共享，而私钥需个人保护。
要使用 SSH 密钥进行身份验证，用户必须在其本地计算机上具有 SSH 密钥对。而在远程服务器上，必须将公钥复制到主目录中的 `~/.ssh/authorized_keys` 文件中。此文件包含有权登录此账户的公有密钥列表，每行一个。
![[SSH.png]]

### 使用
#### 生成 SSH 密钥对
许多加密算法可用于生成 SSH 密钥（默认为 2048 位，可通过 `-b 4096` 指定），包括Ed25519、 RSA（默认）、DSA 和 ECDSA。
```bash
$ ssh-keygen -t ed25519
# 之后分别是 选择文件位置、设置密码短语、确认密码短语
# 默认，私钥将被保存在 ~/.ssh/id_ed25519；公钥将被保存在 ~/.ssh/id_ed25519.pub
```
为了使用 SSH 登录服务器，需要将公钥（`id_ed25519.pub`）的内容添加到 `~/.ssh/authorized_keys` 文件中。

#### `ssh-copy-id` 
使用 `ssh-copy-id` 实用程序可以轻松向服务器传输公钥
```bash
$ ssh-copy-id username@remote_host
# 将提示输入密码，之后 公钥的内容将附加到服务器 remote_host 上 username 的 ~/.ssh/authorized_keys
```

#### 将公钥复制到没有 `ssh-copy-id` 的服务器
```bash
$ cat ~/.ssh/id_rsa.pub | ssh username@remote_host "mkdir -p ~/.ssh && cat >> ~/.ssh/authorized_keys"
```  

### 登录到具有不同端口的服务器
```bash
$ ssh -p port_num username@remote_host
```

为避免每次登录远程服务器都需执行此操作，可以在 `~/.ssh/config` 添加：
```
Host remote_machine_alias
    HostName remote_host
    User username
    Port port_num
```

将 SSH 密钥添加到 SSH agent 以避免键入密码：
如果您的私有 SSH 密钥上有密码，则每次使用密码连接到远程主机时，系统都会提示您输入密码。
为避免重复执行此操作，您可以运行 SSH agent。这个小实用程序会在您首次输入密码后存储您的私钥。它将在您的终端会话期间可用，允许您在将来连接而无需重新输入密码。
启用 SSH Agent
```bash
$ eval $(ssh-agent) # 启动 SSH agent
Output
Agent pid 10891
$ ssh-add  # 添加私钥到 agent
```


### 服务器端配置选项
#### 禁用口令验证
如果您已配置、测试了 SSH 密钥并正常工作，则禁用密码身份验证可能是一个好主意。这将阻止任何用户使用密码通过 SSH 登录。
通过设置 `/etc/ssh/sshd_config` 中的 `PasswordAuthentication no` 以禁用密码登录

#### 更改 `sshd` 运行的默认端口
减少服务器受到自动爬虫程序的身份验证的尝试次数
通过设置 `/etc/ssh/sshd_config` 中的 `Port 4444`

#### 限制可通过 SSH 连接的用户
通过设置 `/etc/ssh/sshd_config` 中的 `AllowUsers user1 user2`
