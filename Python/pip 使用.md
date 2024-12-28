`pip` （Package Installer for Python） 是 Python 的官方包管理工具，用于安装、升级、卸载和管理 Python 包或模块。
使用 `pip` ，可以从 `Python Package Index`[PyPI](https://pypi.org/)

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
```bash
# 阿里源
pip config set global.index-url https://mirrors.aliyun.com/pypi/simple

# 清华源
pip config set global.index-url https://mirrors.tuna.tsinghua.edu.cn/pypi/web/simple
Writing to /home/yongy/.config/pip/pip.conf
```
