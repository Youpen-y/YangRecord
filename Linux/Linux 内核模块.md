Linux 模块（`module`）可以在内核运行时动态加载（或卸载）的代码，常用于添加设备驱动、文件系统支持等功能，而无需重启系统。


#### 常见操作
1. 查看已加载模块
```bash
lsmod

# modinfo <module_name>.ko      # 获取模块详细信息
```

2. 加载模块
```bash
insmod <module_name>.ko         # 需要 .ko 后缀

# modprobe <module_name>        # 不需要 .ko 后缀
```

3. 卸载模块
```bash
sudo rmmod <module_name>

# sudo modprobe -r <module_name>
```

4. 模拟加载/卸载
```bash
sudo modprobe -n <module_name>
sudo modprobe -n -r <module_name>
```
`modprobe` 能够自动处理模块之间的依赖关系，使得模块管理变得更加方便和安全。