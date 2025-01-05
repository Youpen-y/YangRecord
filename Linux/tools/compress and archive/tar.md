`tar` - 存档（archiving）工具

GNU tar 是一个存档程序，旨在将多个文件存储在单个文件（an archive）中，并操作此类存档。该文件（the archive）可以是常规文件或设备（如磁带驱动器，tape archiver），可以位于本地或远程计算机上。


常用选项
- `c` - 创建存档（creating the archive）
- `v` - 请求详细操作
- `f` - 采用一个参数来设置要操作的存档的名称
如下指令，将 `/etc` 中所有文件存储到存档文件 `etc.tar` 中。
```bash
tar cf etc.tar /etc
tar cfv etc.tar /etc
```

- `z`  - 

- `t` - 

- `x` - 从存档中提取文件。
```bash
tar xf path/to/source.tar[.gz|.bz2|.xz] --directory=path/to/directory
```





