`git send-email` —— 将补丁集合作为电子邮件发送

语法：
```bash
git send-email [<options>] (<file>|<directory>)...
git send-email [<options>] <format-patch-options>
git send-email --dump-aliases
git send-email --translate-aliases
```

描述：
获取命令行中输入的补丁并通过电子邮件发送出去。补丁可以指定为文件、目录（将发送目录中的所有文件）或直接以修订列表的形式发送。
> 直接以修订列表形式发送含义：直接通过指定 Git 修订版本范围来让 `git send-email` 内部生成并发送。`git send-email` 能理解并支持 `git format-patch` 的所有 `<revision range>` 和选项。
> 如：
> ```bash
> git send-email -1 --to="recipient@example.com"  --cc="cc@example.com"
> ```
> `-1` 是 `git format-patch` 的一个常用选项，表示只生成最近一次提交的补丁。
> `--to="recipient@example.com`：指定邮件的发送人
> `--cc="cc@example.com`：指定邮件的抄送人
> 幕后：`git send-email` 会在内部调用 `git format-patch -1` 来生成关于你最近一次提交的补丁，然后用这个补丁来构建并发送一封电子邮件。邮件的主题和正文会根据提交信息自动生成。

两种方式指定电子邮件主题（Subject）：
- 命令行选项：可以在运行命令时通过 `--subject/-s [Subject Description]` 选项直接指定邮件标题。
- 交互式界面提示用户输入标题。如下：
```bash
git send-email --to your-mailing-list@example.com 0001-my-patch.patch
Subject: [Subject Description that you should complete]
```


在 Git 和 `git send-email` 的上下文中，有如下邮箱地址：


1. `user.email` （Git 提交作者/提交者邮箱）：该邮箱是通过 `git config user.email` 设置的邮箱。该邮箱地址会出现在 Git 提交（`commit`）信息中，作为作者（Author）和提交者（`Committer`）的身份。是 Git 仓库内部记录的身份。
2. `sendemail.from`（邮件`From:`头部）：这是 `git send-email` 发送邮件时，邮件的 `From:` 头部显示的邮箱地址。这是邮件客户端看到的发件人地址。
3. `sendemail.smtpUser`（SMTP 认证用户名）：用于 SMTP 服务器认证的用户名，通常是邮箱账户地址，用于实际登录 SMTP 服务器发送邮件。
4. `--to, --cc, --bcc`（邮件收件人，抄送，密送）

如何设置？
`git send-email` 默认会尝试使用你的 `user.email` 作为邮件的 `From:` 地址。但是，可通过如下方式明确指定不同的发件人地址：
1. 通过命令行选项 `--from` 指定：
```bash
git send-email --from "Your Name <Another.email@example.com>" --to mailinglist@example.com 000x-patch.patch
```
这样，邮件的 `From:` 头部将显示为 `Your Name <Another.email@example.com>` 。而你用于 SMTP 认证的账户可以是不同的。

2. 通过 `sendemail.from` 配置项指定
```TOML
[sendemail]
    from = Another.email@example.com
    smtpServer = smtp.gmail.com
    smtpServerPort = 587
    smtpEncryption = tls
    smtpUser = your-gmail-account@gmail.com
    smtpPass = your-app-password
```
这样，邮件的 `From:` 头部将为 `Another.email@example.com`，而实际登录 Gmail SMTP 服务器进行发送的账户是 `your-gmail-account@gmail.com`；实际的 Git 提交（由 `user.email` 控制）可以继续是另一个地址。