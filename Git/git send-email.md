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
> 直接以修订列表形式发送含义：直接通过指定 Git 修订版本范围来让 `git send-email` 内部生成并发送。`git send-email` 能否理解并支持 `git format-patch` 的所有 `<revision range>` 和选项。
> 如：
> ```bash
> git send-email -1 --to="recipient@example.com"  --cc="cc@example.com"
> ```
> `-1` 是 `git format-patch` 的一个常用选项，表示只生成最近一次提交的补丁。
> `--to="recipient@example.com`：指定邮件的发送人
> `--cc="cc@example.com`：指定邮件的抄送人
> 幕后：`git send-email` 会在内部调用 `git format-patch -1` 来生成关于你最近一次提交的补丁，然后用这个补丁来构建并发送一封电子邮件。邮件的主题和正文会根据提交信息自动生成。



