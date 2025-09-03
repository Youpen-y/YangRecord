`dpkg` - Debian 的包管理器（Debian Package Manager）

语法：
```bash
dpkg [option...] action
```

描述：
`dpkg` 是安装、构建、移除和管理 Debian 包的中间层工具。基于 `dpkg` 的用户更友好的前端是 CLI 工具 `apt(8)` 和 TUI 工具 `aptitude(8)` 。
> 为什么说是中间层（medium-level）工具？因为 `dpkg` 也用作 `dpkg-deb(1)` 和 `dpkg-query(1)` 的前端。

`dpkg` 本身完全由命令行参数（一个 action，零个或多个 options）控制。参数 action 告诉 `dpkg` 干什么，而 options 控制执行时的行为。

---
包相关信息：
`dpkg` 维护了可用包的有用信息：`states`、`selection states` 和 `flags`。这些值主要使用 `dselect` 修改。

1. 包状态
	- `not-installed`：该包并未在系统上安装
	- `config-files`：系统中只存在配置文件或 postrm 脚本以及移除软件包需要删除的数据。
	- `half-installed`：该包的安装已经开始，但由于某些原因还未完成。
	- `unpacked`：已经解包了，但未配置。
	- `half-configured`：已经解包了，配置也开始了，但由于某些原因还未完成。
	- `triggers-awaited`：该包正在等待另一个包的触发处理。
	- `triggers-pending`：包已经被触发了。
	- `installed`：该包已正确解包并配置。

2. 包选择状态（Selection states）
	- `install`：安装，该软件包被选中安装。
	   当运行 `dpkg -i` 或 `apt install` 时，包会被安装（包括程序文件、文档、配置等）。
	- `hold`：保持，锁定当前版本，不会被自动升级、降级或删除，除非明确操作。
	   设置软件包为 `hold` 状态：`sudo apt-mark hold <package-name>` 。
	- `deinstall`：卸载（保留配置）。
	   等价于 `sudo apt remove <package>` 或 `sudo dpkg --remove <package>`
	- `purge`：完全清除（包括配置）。
	   等价于 `sudo apt purge <package>` 或 `sudo dpkg --purge <package>`
	- `unknown`：未知状态。
	   软件包的选择状态是未知的，表示 `dpkg` 数据库中对这个包没有明确记录。
3. 包状态标志（package flags）
	- `ok`：正常状态。包处于“良好”状态，结构上没有损坏。但不代表完全安装成功，可能仍需配置。
	- `reinstreq`：包是“损坏”的，比如缺少文件或依赖。不能正常使用，无法移除（除非强制）。
	   修复方案：`sudo apt install --reinstall <package-name>`
---
ACTIONS
- `-i/--install package-file...`：安装 Package 。如果指定了 `--recursive` 或 `-R` 选项，`package-file` 必须指向目录。安装包含以下步骤：
	1.  提取新包的控制文件
	2. 如果同一包的其他版本在此之前已安装了，执行旧包的 `prerm` 脚本
	3. 如果包提供的话，运行 `preinst` 脚本
	4. 解包新文件，同时备份旧文件，如果出错了，可以恢复
	5. 如果同一包的其他版本在此之前已安装了，执行旧包的 `postrm` 脚本。注意，该脚本在新包的 `preinst` 脚本后执行，因为新文件被写入同时，旧文件被移除
	6. 配置该包（Configure the package）
- `--unpack package-file...`：解压软件包，但不进行配置。如果指定了 `--recursive` 或 `-R` 选项，则 `package-file` 必须改为引用目录。
- `--configure package... |-a|--pending`：配置已被解压但还未被配置的包。如果使用 `-a` 或 `--pending` 选项而非 `package` ，所有解压但未配置的包被配置。使用 `dpkg-reconfigure` 重新配置已配置的包。
	> 配置包含如下步骤：
	> 1.  Unpack the conffiles（解包配置文件），同时备份旧的 conffiles，以便失败时恢复。
	> 2. 运行 `postinst` 脚本，如果包提供的话。
	> 将处理 triggers 除非指定了 `--no-triggers` 。
	
- `-r/--remove package...|-a|--pending`：移除已安装的包。移除除了 conffiles 和其他由 postrm 脚本清除的数据。
    >  移除包包含以下步骤：
    >  1. 运行 `prerm` 脚本
    >  2. 移除安装的文件
    >  3. 运行 `postrm` 脚本
    
- `-P/--purge package...|-a|--pending`：完全清除（Purge）一个已安装或已移除（Removed）包。
    > Purging 一个包包含以下步骤：
    > 1. 移除该包（Remove the Package），如果没完全移除
    > 2. 运行 `postrm` 脚本

示例：
1. 安装 `.deb` 软件包
```bash
sudo dpkg -i package_name.deb
```
如果提示依赖错误，可使用以下命令修复。
```bash
sudo apt-get install -f
```

2. 卸载已安装的软件包
```bash
# Remove（保留配置文件）
sudo dpkg -r package_name

# Purge（完全删除）
sudo dpkg -P package_name
```

3. 查询已安装包
```bash
# 查看某个包是否已安装
dpkg -l | grep package_name

# 查看安装的所有包
dpkg -l

# 查看特定包的详细信息
dpkg -s package_name

# 列出该包安装的文件列表
dpkg -L package_name
```


查看某个库的头文件
```bash
$ dpkg -L libname | grep .h
# 示例：libnotify-dev
$ dpkg -L libnotify-dev | grep .h
/usr/include/libnotify/notification.h
/usr/include/libnotify/notify-enum-types.h
/usr/include/libnotify/notify-features.h
/usr/include/libnotify/notify.h
/usr/share
/usr/share/doc
/usr/share/doc/libnotify-dev
/usr/share/doc/libnotify-dev/copyright
/usr/share/gir-1.0
/usr/share/gir-1.0/Notify-0.7.gir
/usr/share/doc/libnotify-dev/changelog.Debian.gz
```


查看头文件属于哪个包？
```bash
$ dpkg -S 'FILE.h'

# libc6-dev:amd64: /usr/include/x86_64-linux-gnu/bits/types/FILE.h
# libc6-dev:amd64: /usr/include/x86_64-linux-gnu/bits/types/__FILE.h
# libc6-dev:amd64: /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h
```