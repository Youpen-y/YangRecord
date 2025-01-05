`zstd` - 压缩或解压缩 `.zst` 文件
原理：
Zstandard 采用基于字典的压缩算法，可根据被压缩数据的特性动态调整其压缩参数。与 `gzip` 和 `bzip2` 等传统算法相比，这使其能够实现更好的性能和压缩比。该算法利用熵编码和快速解压缩方法等技术，使其适用于广泛的用途。
》在不牺牲速度的情况下提供高压缩比。
语法
```bash
zstd [OPTIONS] [-|INPUT-FILE] [-o OUTPUT-FILE]
```

选项：
`-z, --compress` ： 压缩，默认操作模式
`-d, --decompress, --uncompress`：解压
`-t, --test`：测试压缩文件的完整性，相当于 `--decompress --stdout > /dev/null` 
`-o file`：将结果保存到 file 中
`-r dir`：压缩 dir 目录下所有文件
`-q` ：抑制除错误之外的所有输出
`-num`：设置压缩级别 `ZSTD_CLEVEL` （`num` 从 1 到 22 ，其中 1 是最快的， 22 是最好的压缩(最高压缩比)）
- `--fast`：使用快速压缩设置，以较低的压缩比进行更快的处理
`--threads <N>` ：使用 N 个线程进行压缩，提高多核系统的性能
`-Tn`：设置 `ZSTD_NBTHREADS` 以使用多个线程，`-T0`使用所有核心，`-Tn` 使用 `n` 个核心

注意：`Zstd` 不支持使用多线程解压

其中:
`zstdmt` -- `zstd -T0`
`unzstd` -- `zstd -d`
`zstdcat` -- `zstd -dcf`

描述
`zstd` 是一种快速无损压缩算法和数据压缩工具。基于 `LZ77` 系列，并具有进一步的 `FSE` 和 `huff0` 熵级。 `zstd` 提供高度可配置的压缩速度，从每核 > 200 MB/s 的快速模式到具有出色压缩比的强模式。它还具有非常快的解码器，每个核心的速度为 > 500 MB/s，在所有压缩设置下都保持大致稳定。

`zstd` 命令行语法通常与 `gzip` 类似，但有以下区别：
- 默认情况下会保留源文件。可使用 `--rm` 命令自动删除它们。
- 压缩单个文件时，默认显示进度通知和结果摘要，使用 `-q` 将它们关闭。
- `zstd` 不接受来自控制台的输入，当 `stdin` 不是控制台时可接收其输入。
- `zstd` 不存储输入的文件名或属性，仅存储其内容。

示例：
1. 压缩单个文件
```bash
zstd file.txt
```

2. 解压缩文件
```bash
zstd -d file.txt.zst
```

3. 指定压缩后输出文件
```bash
zstd -o compressed.zst file.txt
```

4. 使用多个线程压缩文件
```bash
zstd --threads 4 file.txt
```

5. 使用 tar 压缩目录
```bash
tar --zstd -cvf dir.tar.zst dir

# or
tar caf dir.tar.zst dir

# or 更多参数
tar \
	--use-compress-program "zstd --threads=$NUM_THREADS -$COMPRESSION_LEVEL" \
	--create \
	--file myFolder.tar.zst myFolder
```
为什么需要使用 `tar` ？
`zstd` 是压缩器，而不是归档器。可以使用`zstd -r directory/`单独压缩目录中的每个文件。但如果你想压缩整个目录，你应该使用`tar`或`libarchive` 。

6. 解压目录
```bash
tar --zstd -xf dir.tar.zst
```