`flatpak` —— 构建、安装和运行应用程序和运行时

描述
`flatpak` 是一个用于管理应用程序及其运行时的工具。在 `flatpak` 模型中，应用程序可以独立于其所依赖的主机系统进行构建和分发，并且在运行时与主机系统保持一定程度的隔离（“沙盒化”）

两种运行模式
- system-wide
	- 系统级数据（运行时、应用和配置）位于 `$prefix/var/lib/flatpak/` 目录下
	- > `$prefix` 手动编译时指定的，默认为空 
- per-user
	- 每用户级数据位于 `$HOME/.local/share/flatpak/` 目录下。
在这些目录中
- `repo/` 中有一个本地仓库
- `runtime/` 已安装的运行时
- `app/` 已安装的应用程序

`Flatpak` 使用 OSTree 来分发和部署数据。它使用的存储库是 OSTree 存储库，可以使用 `ostree` 实用程序操作。已安装的运行时和应用程序都是 OSTree 检出的。

常用命令：
```bash
flatpak [OPTION...] {COMMAND}
```

安装 `Flatpak`
```bash
sudo apt install flatpak

# 可选：将 Flatpak 集成到 GNOME 软件中心
sudo apt install gnome-software-plugin-flatpak
```

从文件安装（以 `org.gnome.gitlab.cheywood.Buffer.flatpakref` 为例）
```bash
# 安装到当前用户（默认，非 root 用户也能使用）
flatpak install --user org.gnome.gitlab.cheywood.Buffer.flatpakref

# 安装至系统（需要 root 权限）
sudo flatpak install --system org.gnome.gitlab.cheywood.Buffer.flatpakref
```

查看应用是用户级还是系统级
```bash
flatpak list --app --user
flatpak list --app --system
```

添加 `Flathub` 仓库
```
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo

# 或使用上海交大镜像源
flatpak remote-modify flathub --url=https://mirror.sjtu.edu.cn/flathub
```

查找应用
```bash
flatpak search appName
```

安装应用
```bash
flatpak install flathub appID
```
示例：`flatpak install flathub org.mozilla.firefox`

运行应用
```bash
flatpak run appID
```
示例：`flatpak run org.mozilla.firefox`

更新应用
```bash
flatpak update
```

卸载应用
```bash
flatpak uninstall appID
```
示例：`flatpak uninstall org.mozilla.firefox`

列出已安装应用
```bash
flatpak list
```

查看添加的仓库
```bash
flatpak remotes
```

常用选项：
- `-v, --verbose` 处理过程中显示调试信息，`-vv` 显示更多信息。

