`update-alternatives` - 维护决定了默认命令的符号链接。

语法：
```bash
update-alternatives [option...] command
```

描述：
`update-alternatives` 创建、删除、维护和显示组成了备选系统（alternatives system）的符号链接信息。

可以同时在单个系统上安装多个具有相同或类似功能的程序。如同时安装几个文本编辑器。这给了系统用户选择权，允许他们根据需要使用不同的编辑器，但如果用户未指定特定的首选项，则程序很难选择调用那个编辑器。
=> `alternatives system` 旨在解决该问题。在文件系统中有一个通用名称（generic name），由提供可互换功能的所有文件共享。`alternatives system` 和系统管理员共同决定该 generic name 引用的实际文件。

例如：安装两个文本编辑器 `ed` 和 `nvi` 。`alternatives system` 将导致 generic name `/usr/bin/editor` 默认引用 `/usr/bin/nvi` 。不过系统管理员可以覆盖此设置，并使其引用 `/usr/bin/ed` 。`alternatives system` 不会更改此设置，除非明确要求这样做。

generic name 不是指向所选候选者的直接符号链接。相反，它先指向 alternatives 目录下的名称，该名称是指向实际引用文件的符号链接。
示例：系统上安装了多个 gcc 编译器
```bash
$ list /usr/bin/gcc
lrwxrwxrwx -  9 Aug 17:03 /usr/bin/gcc -> /etc/alternatives/gcc

$ list /etc/alternatives/gcc
lrwxrwxrwx -  9 Aug 16:47 /etc/alternatives/gcc -> /opt/gcc-15/bin/gcc
```
使用 `update-alternatives` 修改 `/etc/alternatives/gcc` 指向，就能切换 `/usr/bin/editor` 最终执行的程序，而不必动 `/usr/bin/gcc` 。
> 这种设计使得系统管理员的修改都只发生在 `/etc` 目录里。
> - `/etc` 是存放系统配置文件的地方，管理员可以自由修改而不影响系统软件包本身。    
> - 如果改动散落在 `/usr/bin` 等目录里，系统升级或包更新可能会覆盖这些改动。

有时，我们希望多个替代项能够同步切换，成组一起变化。
示例：
- 系统中安装了多个版本的 `gcc`
- `/usr/bin/gcc` 指向希望的 `gcc` 版本
- 而 `/usr/share/man/man1/gcc.1` （`gcc` 的手册页）也应该指向和该版本 `gcc` 相对应的文档。
`update-alternatives` 通过主链接（master link）和从链接（slave link）机制实现这种同步切换。一个主链接和它的所有从链接组成一个链接组。链接组的切换是原子操作，切换主链接，从链接会一起切换。
- master link（主链接）是主要的通用名称，例如 `/usr/bin/vi`。
- slave link（从链接）是附属的通用名称，例如 `/usr/share/man/man1/vi.1`。
- 当**主链接**切换到另一版本时，所有关联的**从链接**也会自动切换到对应版本。

链接组（link group在任）意时刻只有两种工作模式：
1. 自动模式（automatic）
    - 当处于自动模式时，`update-alternatives` 会在软件包安装或卸载时**自动选择**并更新链接指向。
    - 系统会根据优先级（priority）来判断应该指向哪个候选版本=>最高优先级。
2. 手动模式（manual）
    - 当处于手动模式时，`update-alternatives` 会**保留管理员的选择**，不自动改动链接。
    - 只有在极特殊的情况下（例如所选版本被移除）才会更改指向。

当一个链接组（link group）第一次被系统识别或创建时，它默认处于自动模式。如果系统管理员对自动模式下的设置做了修改（如手动指定某个版本），那么下次 `update-alternatives` 操作这个链接组时，会自动切换到手动模式，并保留管理员的选择。

示例：使用 `--config` 选项查看可选版本，`*` 标记了当前使用的版本。
```bash
$ update-alternatives --config gcc
There is 1 choice for the alternative gcc (providing /usr/bin/gcc).

  Selection    Path                 Priority   Status
------------------------------------------------------------
* 0            /opt/gcc-15/bin/gcc   200       auto mode
  1            /opt/gcc-15/bin/gcc   200       manual mode

Press <enter> to keep the current choice[*], or type selection number:
```
尽管系统上存在其他 `Gcc` 版本，但不会自动出现在上述列表，除非已经被 `update-alternatives` 注册为该链接组的一个 `alternatvie`。
手动注册：
```bash
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 130

# /usr/bin/gcc - 通用名 `generic name`
# gcc - 链接组名
# /usr/bin/gcc-13 - 实际二进制路径
# 130 优先级
```
再次查看可以看到 `/usr/bin/gcc-13` 出现在候选列表中。

Linux 下管理多个可选软件版本的工具，主要用于维护多个软件包之间的符号链接，使系统可以方便地在不同版本的可执行文件、库、文档查看器等之间切换。
例如，一个系统可能同时安装了多个版本的 Java、Python 或编辑器，`update-alternatives` 可以帮助选择默认版本。

原理：
该工具通过管理 `/etc/alternatives/` 目录下的符号链接，实现软件版本的切换。例如：
- `/usr/bin/java` 是 `/etc/alternatives/java` 的符号链接
- `/etc/alternatives/java` 是指向 `/usr/lib/jvm/java-11-openjdk-amd64/bin/java`的符号链接
常用命令：
1. 查看已管理的替代项
```bash
update-alternatives --query <name>
```

2. 手动切换默认版本
```bash
sudo update-alternatives --config <name>        # 交互式
sudo update-alternatives --set <name> <path>    # 非交互式
```

`--set name path`：把某个替代项组（link group）里的主链接（`name`）直接设置为指定的实际程序路径（`path`，需要是提前注册到该链接组的候选项）。效果等价于使用 `--config` 手动选择该路径，但不需要交互，因此可以在脚本中使用。


3. 添加新的替代项
```bash
sudo update-alternatives --install <link> <name> <path> <priority> [--slave <link> <name> <path>]

# sudo update-alternatives --install /usr/bin/java java /usr/lib/jvm/java-8-openjdk-amd64/bin/java 1000
```
其中 `name` 是在 `alternatvie` 目录下符号链接名。


4. 移除替代项
```bash
sudo update-alternatives --remove <name> <path>     # 单个移除
sudo update-alternatives --remove-all <name>        # 移除所有
# sudo update-alternatives --remove java /usr/lib/jvm/java-8-openjdk-amd64/bin/java
```

5. 设置自动模式（自动跟随优先级变化）
```bash
sudo update-alternatives --auto <name>
# sudo update-alternatives --auto java
```

6. 检查当前使用的版本
```bash
update-alternatives --display <name>
# update-alternatives --display java
```


7. 依次配置每个备选方案（即对每个备选调用 `--config`）
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
