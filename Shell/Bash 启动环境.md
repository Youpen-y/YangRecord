### Session
用户每次使用 Shell ，都会开启一个与 Shell 的 Session （会话）。
两种类型 Session：
- 登录 Session （login shell）
- 非登录 Session （non-login shell）
#### 登录 Session
登录 Session 是用户登录系统以后，系统为用户开启的原始 Session，通常需要用户输入用户名和密码进行登录。

登录 Session 一般进行整个系统环境的初始化，启动的初始化脚本依次如下。
- `/etc/profile`：所有用户的全局配置脚本。
- `/etc/profile.d`目录里面所有`.sh`文件
- `~/.bash_profile`：用户的个人配置脚本。如果该脚本存在，则执行完就不再往下执行。
- `~/.bash_login`：如果`~/.bash_profile`没找到，则尝试执行这个脚本（C shell 的初始化脚本）。如果该脚本存在，则执行完就不再往下执行。
- `~/.profile`：如果`~/.bash_profile`和`~/.bash_login`都没找到，则尝试读取这个脚本（Bourne shell 和 Korn shell 的初始化脚本）。

Linux 发行版更新的时候，会更新 `/etc` 里的文件，如 `/etc/profile` ，因此不要直接修改这个文件。如果想修改所有用户的登录环境，在 `/etc/profile.d` 目录下新建 `.sh` 脚本。

如果想修改用户的登录环境，修改 `~/.bash_profile` 文件。

`bash` 命令的 `--login` 参数，会强制执行登录 Session 会执行的脚本。
`bash` 命令的 `--noprofile` 参数，会跳过上面这些 Profile 脚本。

#### 非登录 Session
非登录 Session 是用户进入系统以后，手动新建的 Session，这时不会进行环境初始化。比如，在命令行执行`bash`命令，就会新建一个非登录 Session。

非登录 Session 的初始化脚本依次如下。
- `/etc/bash.bashrc`：对全体用户有效。
- `~/.bashrc`：仅对当前用户有效。

对用户来说，`~/.bashrc`通常是最重要的脚本。非登录 Session 默认会执行它，而登录 Session 一般也会通过调用执行它。每次新建一个 Bash 窗口，就相当于新建一个非登录 Session，所以`~/.bashrc`每次都会执行。注意，执行脚本相当于新建一个非互动的 Bash 环境，但是这种情况不会调用`~/.bashrc`。

`bash`命令的`--norc`参数，可以禁止在非登录 Session 执行`~/.bashrc`脚本。
```
$ bash --norc
```

`bash`命令的`--rcfile`参数，指定另一个脚本代替`.bashrc`。
```
$ bash --rcfile testrc
```

#### `.bash_logout`
`~/.bash_logout` 脚本在每次退出 Session 时执行，通常用来做一些清理工作和记录工作，如删除临时文件，记录用户在本次 Session 花费的时间。
如果没有退出时要执行的命令，这个文件也可以不存在。