`update-alternatives` - Linux 下管理多个可选软件版本的工具，主要用于维护多个软件包之间的符号链接，使系统可以方便地在不同版本的可执行文件、库、文档查看器等之间切换。
例如，一个系统可能同时安装了多个版本的 Java、Python 或编辑器，`update-alternatives` 可以帮助选择默认版本。

原理：
该工具通过管理 `/etc/alternatives/` 目录下的符号链接，实现软件版本的切换。例如：

- `/usr/bin/java` 可能是 `/etc/alternatives/java` 的符号链接
    
- `/etc/alternatives/java` 再指向 `/usr/lib/jvm/java-11-openjdk-amd64/bin/java`

常用命令：
1. 查看已管理的替代项
```bash
update-alternatives --query <name>
```

2. 手动切换默认版本
```bash
sudo update-alternatives --config <name>
```

3. 添加新的替代项
```bash
sudo update-alternatives --install <link> <name> <path> <priority>

# sudo update-alternatives --install /usr/bin/java java /usr/lib/jvm/java-8-openjdk-amd64/bin/java 1000
```

4. 移除替代项
```bash
sudo update-alternatives --remove <name> <path>
# sudo update-alternatives --remove java /usr/lib/jvm/java-8-openjdk-amd64/bin/java
```

5. 设置自动模式
```bash
sudo update-alternatives --auto <name>
# sudo update-alternatives --auto java
```

6. 检查当前使用的版本
```bash
update-alternatives --display <name>
# update-alternatives --display java
```

示例：
管理多个 Java 版本
```bash
sudo update-alternatives --install /usr/bin/java java /usr/lib/jvm/java-8-openjdk-amd64/bin/java 1000
sudo update-alternatives --install /usr/bin/java java /usr/lib/jvm/java-11-openjdk-amd64/bin/java 1100
sudo update-alternatives --install /usr/bin/java java /usr/lib/jvm/java-17-openjdk-amd64/bin/java 1200
```

使用：
```bash
sudo update-alternatives --config java
```
设置默认 Java 版本
