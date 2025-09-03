`tar` - 存档（archiving）工具

GNU tar 是一个存档程序，旨在将多个文件存储在单个文件（an archive）中，并操作此类存档（从存档中恢复一个个文件）。该文件（the archive）可以是常规文件或设备（如磁带驱动器，tape archiver），可以位于本地或远程计算机上。

语法：
```bash
tar [OPTION...] [FILE]...
```

常用选项
- `-c/--create` - 创建存档（creating the archive）
- `-v` - 请求详细操作
- `-f/--file=ARCHIVE` - 采用一个参数来设置要操作的存档的名称
如下指令，将 `/etc` 中所有文件存储到存档文件 `etc.tar` 中。（选项前的 `-` 是可选的）。
```bash
tar [-]cf etc.tar /etc
tar [-]cfv etc.tar /etc
```
- `-x` - 从存档中提取文件。
```bash
tar xf path/to/source.tar[.gz|.bz2|.xz] --directory=path/to/directory
```

- `-z/--gzip, --gunzip, --ungzip`  -  通过 `gzip` 过滤归档文件，对归档文件进行压缩或解压缩（取决于是在打包还是解包）
```bash
tar -czf archive.tar.gz folder/     # 打包，生成 gzip 压缩过的 .tar.gz 文件
tar -xzf archive.tar.gz             # 解包，自动通过 gzip 解压 .gz 文件，然后提取 .tar 内容
```

- `-j` - 通过 `bzip2` 过滤归档文件
```bash
tar -cjf archive.tar.bz2 folder/
tar -xjf archive.tar.bz2 -C DIR/
```

- `-J` - 通过 `xz` 过滤归档文件
```bash
tar -cJf archive.tar.xz folder/
tar -xJf archive.tar.xz -C DIR/
```

- `--zstd` - 通过 `zstd` 过滤归档文件
```bash
tar --zstd -cf archive.tar.zst folder/
tar --zstd -xf archive.tar.zst -C DIR/
```

- `-t/--list` - 列举 archive 中的内容
```bash
tar [-]tvf archive.tar  # 详细列举 archive.tar 中的文件
```

注意：`tar` 会根据扩展名自动判断压缩方式，如看到 `.tar.gz` 就自动使用 `gzip` 。


