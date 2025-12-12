`pip` （Package Installer for Python） 是 Python 的官方包管理工具，用于安装、升级、卸载和管理 Python 包或模块。
使用 `pip` ，可以从 `Python Package Index`[PyPI](https://pypi.org/)

### 更新 `pip`
```bash
python -m pip install --upgrade pip
```

### 安装模块/包
```bash
pip install package_name
```

### 安装特定版本
```bash
pip install package_name=VERSION
```

### 升级 `package_name`
```bash
pip install --upgrade package_name
# pip install --upgrade pip # 升级 pip
```

### 查看已安装的模块
```bash
pip list
```

### 卸载模块
```bash
pip uninstall package_name
```

### 查看模块详细信息
```bash
pip show package_name

# 或 python -m pip show package_name
pip show pypdf
Name: pypdf
Version: 5.5.0
Summary: A pure-python PDF library capable of splitting, merging, cropping, and transforming PDF files
Home-page: 
Author: 
Author-email: Mathieu Fenniak <biziqe@mathieu.fenniak.net>
License: 
Location: /home/yongy/.pyenv/versions/3.13.3/lib/python3.13/site-packages
Requires: 
Required-by: 
```

### 查找模块 
```bash
pip search package_name
```

### 输出当前 Python 环境中已安装的所有包及其版本号
```bash
pip freeze > requirements.txt
```

### 从 `requirements.txt` 安装依赖
```bash
pip install -r requirements.txt
```

## 使用国内镜像加速
### 临时使用
```bash
pip install package_name -i https://mirrors.aliyun.com/pypi/simple
```

### 永久配置
#### 查看 `pip` 配置文件路径
```bash
pip config debug
env_var:
env:
global:
  /etc/xdg/xdg-ubuntu/pip/pip.conf, exists: False
  /etc/xdg/pip/pip.conf, exists: False
  /etc/pip.conf, exists: True
    global.index-url: https://pypi.tuna.tsinghua.edu.cn/simple
    global.extra-index-url: https://pypi.org/simple
site:
  /home/yongy/.pyenv/versions/3.13.3/pip.conf, exists: False
user:
  /home/yongy/.pip/pip.conf, exists: False
  /home/yongy/.config/pip/pip.conf, exists: False
```
可以看到全局配置 `global` 下 `/etc/pip.conf` 为 `True`，其中设置了两个源。在 Linux 上，我们通常可以使用
```bash
/etc/pip.conf       # 全局
~/.pip/pip.conf     # 用户级
```

- 直接修改
```conf
[global]
index-url = https://pypi.tuna.tsinghua.edu.cn/simple
extra-index-url = https://pypi.org/simple
```

- 终端设置
```bash
# 阿里源
pip config set global.index-url https://mirrors.aliyun.com/pypi/simple

# 清华源
pip config set global.index-url https://mirrors.tuna.tsinghua.edu.cn/pypi/web/simple
Writing to /home/yongy/.config/pip/pip.conf
```
