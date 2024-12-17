### 语法
```bash
[variable]="[username]:[password]@[proxy_address]:[port_number]"
```

如果代理不需要用户名和密码，请忽略。

`Ubuntu` 使用以下环境变量来设置这些参数：
- `http_proxy` - HTTP 连接的代理服务器
- `https_proxy` - HTTPS 连接的代理服务器
- `ftp_proxy` - FTP 连接的代理服务器
- `no_proxy` - 应绕过代理的以逗号分隔的地址和域列表（对于本地地址或 intranet 有用）

注意：旧系统使用相同的大写格式变量名称。为涵盖所有可能情况，请同时使用大写和小写变量的名称。

### 为单个用户设置临时代理：
系统重新启动后，临时设置的代理会被重置（`export`命令为当前用户设置临时代理）。
如：
```bash
export http_proxy="192.168.1.100:8080"
export https_proxy="192.168.1.100:8080"
export no_proxy="localhost,127.0.0.1,::1"
```

### 为单个用户设置永久代理
要对单个用户进行永久更改，修改 `.bashrc` 文件
1. 使用文本编辑器打开文件
```bash
sudo nano ~/.bashrc
```

2. 在文件末尾底部添加 `export` 命令
```bash
export http_proxy="192.168.1.100:8080"
export https_proxy="192.168.1.100:8080"
export no_proxy="localhost,127.0.0.1,::1"
```

3. 保存退出，并应用
```bash
source ~/.bashrc
```

### 为所有用户设置永久代理
要为所有用户永久设置代理，编辑 `/etc/environment` 文件
1. 在文本编辑器中打开文件
2. 在文本末尾添加 `export` 命令
3. 保存退出，更改将在下次登录时应用

### 为 APT 设置代理
`apt` 命令在某些系统上由于不使用系统环境变量需要单独的代理配置文件。要设置 `apt` 的代理：
1. 在 `/etc/apt` 目录中创建或编辑（如果已存在）名为 `apt.conf` 的文件
2. 添加以下格式的数据
```bash
Acquire::http::Proxy "http://[proxy_address]:[port_number]/";
Acquire::https::Proxy "https://[proxy_address]:[port_number]/";
```
如果代理需要身份验证，在 `proxy_address` 前添加 `[username]:[password]@` 。
3. 保存文件并退出，该配置将在重新启动后应用

### 为 `wget` 设置代理
1. 打开或创建 `wget` 命令的配置文件
```bash
sudo nano ~/.wgetrc
```

2. 按以下格式添加代理设置
```bash
use_proxy = on
http_proxy = "http://[proxy_address]:[port_number]/"
https_proxy = "https://[proxy_address]:[port_number]/"
ftp_proxy = "ftp://[proxy_address]:[port_number]"
```
如果代理需要身份验证，在 `proxy_address` 前添加 `[username]:[password]@` 。
3. 保存退出，设置立即应用

### 为 git 设置代理
1. 在终端中使用以下命令配置代理服务器
```bash
git config --global http.proxy http://[proxy_address]:[port_number]
git config --global https.proxy https://[proxy_address]:[port_number]
```
如果代理需要身份验证，在 `proxy_address` 前添加 `[username]:[password]@` 。
2. 检查设置是否已应用
```bash
git config --global --get http.proxy
git config --global --get https.proxy
```


### 检查 Ubuntu 代理是否工作
- 环境变量
```bash
echo $http_proxy
echo $https_proxy
```

- 连接性（`curl` 测试）
```bash
curl -I http://example.com
```

- `APT` 代理测试
```bash
sudo apt update
```
