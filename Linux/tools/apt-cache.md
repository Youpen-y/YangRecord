`apt-cache` - 查询 APT 缓存

描述
`apt-cache` 对 APT 的包缓存执行各种操作，`apt-cache` 不会操作系统的状态，但提供从包元数据搜索和生成有趣输出的操作。

示例：
1. 搜索软件包
```bash
sudo apt-cache search <package-name>
```

2. 查看软件包信息
```bash
sudo apt-cache show <package-name>
```

3. 查看软件包依赖关系
```bash
sudo apt-cache depends <package-name>
```

4. 查看哪些软件包依赖于指定的软件包
```bash
sudo apt-cache rdepends <package-name>
```

5. 查看软件包的反向依赖关系
```bash
sudo apt-cache rdepends --installed <package-name>
```

6. 查看软件包是否已安装
```bash
sudo apt-cache policy <package-name>
```

7. 查看软件包的说明文件
```bash
sudo apt-cache showpkg <package-name>
```

8. 显示可以清理的不再需要的软件包
```bash
sudo apt-cache autoclean
```

9. 查看软件包的差异信息
```bash
sudo apt-cache diff <package-name>/<version>
```




