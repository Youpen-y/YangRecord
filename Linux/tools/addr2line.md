`addr2line` - 将地址转换为文件名和行号
addr2line [-b bfdname | --target=bfdname]   [-C | --demangle[=style]]  [-e filename | --exe=filename]   [-f|--functions]   [-s|--basename]  [-i|--inline]  [-j|--section=name]  [-H|--help]  [-V|--version]  [addr addr ...]

描述
`addr2line` 将地址转换为文件名和行号。给定可执行文件中的地址或可重定位对象的某个部分中的偏移量，它使用调试信息来找出与其关联的文件名和行号。
`addr2line` 有两种操作模式：
- 第一种，在命令行上指定十六进制地址， `addr2line` 显示每个地址的文件名和行号。
- 第二种， `addr2line` 从标准输入读取十六进制地址，并在标准输出上打印每个地址的文件名和行号。在此模式下， `addr2line` 可以在管道中使用来转换动态选择的地址。
输出的格式为**FILENAME:LINENO** 。每个地址的文件名和行号打印在单独的行上。如果使用-f  选项，则每个 **FILENAME:LINENO** 行前面都有一个 **FUNCTIONNAME** 行，它是包含该地址的函数的名称。
如果无法确定文件名或函数名， **addr2line**将在其位置打印两个问号。如果无法确定行号， **addr2line**将打印 0。

---
选项

**-b** _bfdname_
**--target=**_bfdname_
指定目标文件的目标代码格式为_bfdname_

**-e** _filename_
**--exe=**_filename_
指定应为其转换地址的可执行文件的名称。默认文件是_a.out_

**-f**
**--functions**
显示函数名称以及文件和行号信息

**-s**
**--basenames**
仅显示每个文件名的基数

**-i**
**--inlines**
如果该地址属于内联函数，则还将打印返回第一个非内联函数的所有封闭范围的源信息。例如，如果 "main" 内联 "callee1" ，"callee1" 内联 "callee2" ，地址来自 "callee2" ，源信息为 "callee1" 和 "main" 也将被打印。

**-j**
**--section**
读取相对于指定节的偏移量，而不是绝对地址

**@**_file_
从 `file` 中读取命令行选项。读取的选项将插入到原始 @_file_ 选项的位置。如果 `file` 不存在，或者无法读取，则该选项将按字面意思处理，而不是删除。
`file`中的选项由空格分隔。通过用单引号或双引号将整个选项括起来，可以将空白字符包含在选项中。任何字符（包括反斜杠）都可以通过在要包含的字符前加上反斜杠来包含。`file`本身可能包含附加的 @*file* 选项；任何此类选项都将被递归处理。

---
Usage

``` bash
$ addr2line -f -e a.out -j .text 0xbddc
```