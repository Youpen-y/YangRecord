软链接（符号链接，symbolic link），一个指向现有文件或目录的特殊的文件（存储现有文件或目录的路径）。

```bash
ln -s [target] [symlink]
```
`-s`: 指示 `ln` 创建符号链接。如果未提供，将创建硬链接。
`[target]`: 链接指向的文件
`[symlink]`: 符号链接位置。如果省略该元素，将放置在当前工作目录中。

> 注意：如果 `target` 不使用绝对路径的话，可能无法正常通过 `symlink` 操纵 `target` 文件内容。
>比如，想要为 `/home/yongy/test.sh` 在 `/home/yongy/desktop` 目录下创建一个软链接。
>错误创建：在 `target` 目录下：`ln -s test.sh ./desktop/test.sh` ，错误原因：*Too many levels of symbolic links*
>
>正确创建：
>1. 任意位置：`ln -s /home/yongy/test.sh /home/yongy/desktop/test.sh` 
>2. 符号链接所在位置：`ln -s ../test.sh test.sh` （`target` 是相对于 `symlink` 的路径）


示例：
- 创建软链接
```bash
yongy@Surge:~$ ln -s /home/yongy/test.sh ~/desktop/test.sh
```
- 查看软链接（1. 最前方的 `l` 标识是一个链接文件，2. 默认授予了链接权限`0777`，尽管如此，并不能通过 `./test.sh` 执行文件，因为链接到的常规文件没有执行权限）
```bash
yongy@Surge:~/desktop$ ls -l test.sh 
lrwxrwxrwx 1 yongy yongy 19 12月 16 16:02 test.sh -> /home/yongy/test.sh
```
使用 `realpath` 和 `readlink` 查看引用文件的相对路径和绝对路径。
```bash
yongy@Surge:~/desktop$ ln -s ../test.sh test.sh
yongy@Surge:~/desktop$ realpath test.sh 
/home/yongy/test.sh
yongy@Surge:~/desktop$ readlink test.sh 
../test.sh
```

- 对比常规文件与链接文件属性（1. 不同的大小和 `inode`，2. `Links` 未更改）
```bash
yongy@Surge:~$ stat test.sh 
  File: test.sh
  Size: 126       	Blocks: 8          IO Block: 4096   regular file
Device: 8,3	Inode: 1058106     Links: 1
Access: (0664/-rw-rw-r--)  Uid: ( 1000/   yongy)   Gid: ( 1000/   yongy)

yongy@Surge:~/desktop$ stat test.sh 
  File: test.sh -> /home/yongy/test.sh
  Size: 19        	Blocks: 0          IO Block: 4096   symbolic link
Device: 8,3	Inode: 1062575     Links: 1
Access: (0777/lrwxrwxrwx)  Uid: ( 1000/   yongy)   Gid: ( 1000/   yongy)
```
- 修改符号链接文件，修改 `desktop/test.sh` 符号链接文件内容，修改反映到常规文件，而符号链接文件属性不变。
- 移动或删除常规文件，符号链接文件成为 `dangling soft link`将无法再使用，重新在原始位置创建同名文件后，链接恢复。
```bash
# 在 directory 下找不起作用的链接
find [directory] -type l ! -exec test -e {} \; -print
```

其他特性：
- 跨多个文件系统
- 如果系统连接到另一台计算机（远程服务器），可以使用符号链接引用这些远程系统上的资源。
---

硬链接，Linux 上每个文件都以单个硬链接开始，这个链接是文件系统上的文件名和实际数据之间的链接。

创建硬链接意味着
- 创建一个新文件名，但指向与旧文件名完全相同的数据。（不同文件名，指向相同的数据）
```bash
yongy@Surge:~/desktop$ ls -al ../test.sh
-rwxrwxr-x 1 yongy yongy 146 12月 16 16:14 ../test.sh
yongy@Surge:~/desktop$ ln ../test.sh test.sh
yongy@Surge:~/desktop$ ls -al ../test.sh test.sh 
-rwxrwxr-x 2 yongy yongy 146 12月 16 16:14 ../test.sh
-rwxrwxr-x 2 yongy yongy 146 12月 16 16:14 test.sh
```
- 查看属性（`inode` 相同）
```bash
yongy@Surge:~/desktop$ stat ../test.sh 
  File: ../test.sh
  Size: 146       	Blocks: 8          IO Block: 4096   regular file
Device: 8,3	Inode: 1058106     Links: 2
Access: (0775/-rwxrwxr-x)  Uid: ( 1000/   yongy)   Gid: ( 1000/   yongy)

yongy@Surge:~/desktop$ stat test.sh 
  File: test.sh
  Size: 146       	Blocks: 8          IO Block: 4096   regular file
Device: 8,3	Inode: 1058106     Links: 2
Access: (0775/-rwxrwxr-x)  Uid: ( 1000/   yongy)   Gid: ( 1000/   yongy)

```
- 对一个文件的修改可以为另一个文件感知

其他特性
- 只能为常规文件（不包括目录或特殊文件）创建硬链接
- 不能跨越多个文件系统（不能跨分区或驱动器）

为什么不能硬链接目录？
1. 硬链接目录会破坏文件系统的树形结构。文件系统组织结构通常是一个树形层次结构，目录树的每个节点（文件/目录）都有唯一的父目录；如果允许对目录使用硬链接，可能在目录树中创建循环结构，如一个目录指向它的父目录，形成闭环。
2. 文件系统一致性变得复杂。目录硬链接会导致多条路径指向同一个目录，这样文件系统需要解决：如何维护多个父目录？如何更新引用计数？删除目录时如何处理多个路径引用的问题？
3. 文件权限和安全问题。如果对目录支持硬链接，用户可能会意外或恶意地为敏感系统目录（如 `/etc` 或 `/home`）创建硬链接，导致权限滥用或安全问题。例如，普通用户可以创建指向其他用户目录的硬链接，从而访问不属于自己的文件。
4. 文件系统回收机制失效。硬链接计数是文件系统用来判断文件或目录是否可以安全删除的依据。对文件系统中的文件，只有当硬链接计数为 0 时，其数据才会被清理。如果目录有多个硬链接，判断是否可以删除目录将变得复杂，不同路径之间的依赖关系可能导致文件系统回收机制失效。

![[软链接-硬链接.png]]