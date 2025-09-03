#### 背景
默认情况下，大多数现代系统都使用 UTF-8 编码。UTF-8 可以表示超过 110,000 个字符。支持全球多种书写系统。然而 `grep` 通常用于搜索 ASCII 编码的文件，ASCII 仅包含 128 个字符。

由于 UTF-8 需要更复杂的处理，使用默认区域设置的搜索可能会比较慢。切换到 `C` 区域设置（默认为 ASCII）后， `grep` 可以更高效地运行，从而减少处理开销并提升性能。

性能比较：
```bash
time grep "search_term" large_file.txt
time LC_ALL=C grep "search_term" large_file.txt
```