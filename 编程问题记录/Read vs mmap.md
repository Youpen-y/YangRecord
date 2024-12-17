执行 10 次取平均值

![[mmap vs read with different buffer sizes.png]]

结论
- 对于较小的文件，与 `mmap` 方法相比，`read` 方法更快。大文件（> 128K）会改变这种情况。
- 对于较小的文件，read 最适合的缓冲区是 4096 B 或 8192 B
- 而对于较大的文件，read 最适合的缓冲区是 16KB

![[deviation.png]]
时间 deviation（方差） 随着文件的大小而变大