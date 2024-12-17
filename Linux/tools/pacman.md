安装/升级软件包
```bash
pacman -S package_name
```

删除软件包
```bash
pacman -R package_name
```

#### 1. 搜索软件包
```c
pacman -Ss search_term
```

#### 2. 查看包的详细信息
```c
pacman -Si package_name
```

#### 3. 根据文件名查找包
```c
pacman -Qo /path/to/file
```

#### 4. 搜索已安装的包
```c
pacman -Qs search_term
```

#### 5. 列出所有已安装的包
```c
pacman -Q
```

#### 6. 列出所有可用的包
使用 `-Qm` 选项列出所有从官方仓库安装的包
```c
pacman -Qm
```

#### 7. 列出所有孤儿包
使用 `-Qdt` 选项列出所有孤儿包，这些包不再被任何其他包依赖：
```c
pacman -Qdt
```

#### 8. 清理缓存
使用 `-Sc` 选项清理不再需要的包缓存
```c
pacman -Sc
```

#### 9. 更新系统（滚动升级系统，不推荐仅在必要时使用）
```c
pacman -Syu
```

#### 10. 安装软件包
```c
pacman -S package_name1 [package_name2]
```

#### 11. 删除软件包
```c
pacman -Rs package_name
```

#### 12. 删除特定软件及其所有依赖
```c
pacman -R package_name
```

#### 13. 下载但不安装软件包
```c
pacman -Sw package_name
```

#### 14. 安装本地包文件
```c
pacman -U /path/to/package.pkg.tar.xz
```

#### 15. 列出已安装软件包的文件
```c
pacman -Ql package_name
```