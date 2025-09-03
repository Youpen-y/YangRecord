`id` - 打印 real 和 effective 用户和组 IDs 。

语法：
```bash
id [OPTION]... [USER]...
```

描述：
打印每个指定用户 `USER` 的用户和组信息；当省略 `USER` 时，打印当前进程的用户和组信息。

选项：
- `-a`：可忽略，为了与其他版本兼容
- `-Z/--context`：仅打印进程的安全上下文
- `-g/--group`：仅打印 Effective group ID
- `-G/--groups`：打印所有的组 IDs 
- `-n/--name`：打印 Name 而非数字
- `-r/--real`：打印 Real ID 而非 Effective ID
- `-u/--user`：打印 Effective user ID
- `-z/--zero`：使用 `NUL` 字符分隔条目，而不是空格

示例：
1. 查看当前用户的 UID，GID 和组
```bash
$ id
uid=1000(yongy) gid=1000(yongy) groups=1000(yongy),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),100(users),114(lpadmin),984(docker),993(kvm)
```

2. 查看指定用户的信息
```bash
$ id root
uid=0(root) gid=0(root) groups=0(root)
```

3. 只显示用户 ID（*重要*）
```bash
$ id -u
1000
```

4. 判断当前用户是否为 root 
```bash
if [ "$(id -u)" -ne 0 ]; then
    echo "Please run as root"
    exit 1
fi
```