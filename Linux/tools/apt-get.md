


#### `apt-get source` 或 `apt source`
`apt-get source` —— 用于获取源代码，在当前目录下创建源代码目录。
典型目录结构如下：
```
./package-name-version/    # 主源代码目录
./package-name_version.orig.tar.gz   # 原始上游代码
./package-name_version-revision.dsc  # 描述文件
./package-name_version-revision.debian.tar.xz   # Debian/Ubuntu 补丁
```
基本用法：
```shell
apt-get source <包名>
```

工作原理：
执行 `apt-get source` 命令时，系统会：
1. 查找源代码：在 `/etc/apt/sources.list` 和 `/etc/apt/sources.list.d/` 目录中查找启用了 `deb-src` 条目的软件源
2. 下载文件
	- 原始源代码归档（`.orig.tar.gz`）
	- Debian/Ubuntu 特定补丁（`.diff.gz` 或 `.debian.tar.xz`）
	- 元数据描述文件（`.dsc`）
3. 解压处理：自动解压源代码并应用发行版特定的补丁。

前提条件：
1. 启用源代码仓库：确保 `/etc/apt/sources.list` 中包含 `deb-src` 开头的条目，如
```
deb-src https://archive.ubuntu.com/ubuntu/ jammy main restricted
```
2. 更新软件源信息：执行命令前先运行 `apt-get update`

常用选项：
- `-d, --download-only`：仅下载源代码包，不解压
- `--compile`：下载、解压并编译源代码
- `--build`：下载并构建二进制包
