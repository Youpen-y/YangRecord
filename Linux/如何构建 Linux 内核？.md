1. 下载源代码
```c
git clone https://mirrors.tuna.tsinghua.edu.cn/git/linux.git
```

2. 安装必要软件包
```c
sudo apt-get install git fakeroot build-essential ncurses-dev xz-utils libssl-dev bc flex libelf-dev bison
```

| 包                    | 描述                                  |
| -------------------- | ----------------------------------- |
| fakeroot             | 创建伪根（fake root）环境                   |
| build-essential      | 安装 C、C++、gcc 和 g++ 等开发工具            |
| ncurses-dev          | 为基于文本的终端提供 API                      |
| xz-utils             | 提供快速的文件压缩和文件解压缩                     |
| libssl-dev           | 支持 SSL 和 TSL，可加密数据并确保 Internet 连接安全 |
| bc（Basic Calculator） | 支持语句的交互式执行                          |
| flex（快速词法分析器生成器）     | 生成将字符转换为标记的词法分析器                    |
| libelf-dev           | 发布用于管理 ELF 文件（可执行文件、核心转储和目标代码）的共享库  |
| bison                | 将语法描述转换为 C 程序                       |
3. 配置内核
Linux 内核使用 `.config` 文件来配置编译选项，比如启用哪些模块、编译成模块还是内联等。
- 使用已有默认配置
```bash
make defconfig
```
生成一个通用的 `.config` ，适用于大多数情况。

- 使用当前系统的配置
```bash
cp -v /boot/config-$(uname -r) .config
```

- 菜单配置（图形界面）
```bash
make menuconfig
```
打开一个基于 `ncurses` 的菜单界面以选择内核功能

4. 构建内核
```bash
make
# make -j8
# bear -- make -j8 LLVM=1       # 生成 compile_commands.json
# bear -- make drivers/net/ethernet/intel/e1000/    # 编译感兴趣的子模块 `.o`
```

如果在 Ubuntu 上编译内核，可能会收到以下错误：
```
No rule to make target 'debian/canonical-certs.pem'
```
可通过执行以下两个命令来禁用冲突的安全证书：
```
scripts/config --disable SYSTEM_TRUSTED_KEYS
scripts/config --disable SYSTEM_REVOCATION_KEYS
```

5. 安装所需模块
```bash
sudo make modules_install
```

6. 安装内核
```bash
sudo make install
```
