用于数据压缩的程序和库。
- [Introduction](#introduction)
- [Usage](#usage)

### Introduction
`bzip2` 使用 Burrows-Wheeler 块排序文本压缩算法和 Huffman 编码来压缩文件。其压缩效果通常显著优于更传统的基于 LZ77/LZ78 的压缩器，并且接近 PPM 系列统计压缩器的性能。

> Burrows-Wheeler Transform （伯罗斯-惠勒变换）将字符串重新排列成一串相似的字符，这种排列方式可以通过逆向变换恢复原始字符串。
> 变换步骤：示例文本 T（`mississippi`）
> 1. 在文本 T 后加 `$` 形成操作文本。`mississippi$`
> 2. 通过循环移动形成矩阵 M<sub>T</sub>![[BWT图示.png]]
>3. 符号：F = `$iiiimppssss`；T<sup>bwt</sup> = `ipssm$pissii`
>```
># C[]: 数组长度为字符集数目（有多少不同的字母）；C[c] = 按字典序小于 c 的字母总数
># Occ(c, q): 在 T<sup>bwt</sup>[1, q] 前缀中字符 c 出现的次数。
># LF(i) = C[T<sup>bwt</sup>[i]] + Occ(T<sup>bwt</sup>[i], i)
>```
>![[BWT2.png]]
>4. Last to Front Mapping（末列与首列映射）—— 字符 T<sup>bwt</sup>[i] 位于首列 F 的 `LF[i]` 位置。
>如：`LF(10) = C[s] + Occ(s, 10) = 12` ；T<sup>bwt</sup>[10] 和 F[12] 对应的 `s` 均为示例文本的第一个 `s` 。
>5. `LF()` 映射允许从后向前搜索文本 T 。算法如下：
>```c
>// 输入：模式串 `P[1..p]`，长度为 p 。
>i = p,  c = P[p], First = C[c] + 1, Last = C[c+1];     // 从后向前，处理字符 P[p] ，其在首列中出现的位置为 [First, Last] 。
>while (( First ≤ Last ) and i ≥ 2 ) {  // 一步步向模式串左侧走
>   c = P[i-1];
>   First = C[c] + Occ(c, First-1) + 1;
>   Last = C[c] + Occ(c, Last);
>   i = i - 1;
>   }
> if ( Last < First ) then  // 区间不合法，模式串 P 在文本中不存在
>   return "No occurrence";
> else  // [First, Last] 是匹配位置在 BWT 行中的区间
>   return ( First, Last );
>```


`bzip2` 构建在 `libbzip2` 上，以 `bzip2` 格式处理压缩数据的灵活库。


### Usage
- `bzip2, bunzip2` - 块排序文件压缩器。
- `bzcat` - 解压查看内容（不解包）。
- `bzip2recover` - 从损坏的 `bzip2` 文件中恢复数据。

常见使用示例：
1. 压缩/解压缩单个文件（原有文件被压缩为 `filename.txt.bz2`）
```bash
bzip2 filename.txt
bunzip2 filename.txt.bz2    # 等价于 bzip2 -d 
```

2. 保留原文件（使用 `-k`）
```bash
bzip2 -k filename.txt
```

3. 压缩多个文件
```bash
bzip2 file1.txt file2.log
```

4. 设置压缩等级（1-9，默认是 9，等级越高压缩越慢）
```bash
bzip2 -5 largefile.iso
```
`-i` 指定了压缩时块大小为 100k, 200k, ..., 900k。由于块大小存储在压缩文件中，该标志解压时无效。

5. 解压查看内容（不解包)
```bash
bzcat filename.txt.bz2
```

6. 恢复损坏的 `.bz2` 文件
```bash
bzip2recover brokenfile.bz2
```

注意：及时压缩文件比原始文件略大，压缩也始终会执行。小于一百字节的文件往往会变得更大，因为压缩机制的开销约为 50 字节。随机数据（包括大多数文件压缩器的输出）的编码密度约为每字节 8.05 位，因此文件大小会膨胀约 0.5% 。

bzip2 使用 32 位 CRC 来确保文件的解压版本与原始数据。这可以防止压缩数据损坏，以及 `bzip2` 未检测到的 bugs 。 

压缩的排序阶段将文件中相似的字符串聚集在一起，因此，包含非常长的重复的符号，如 `aabaabaabaab...`（重复几百次）的压缩速度可能比正常情况慢。解压速度不受这些现象的影响。

具体库的介绍：
[Programming with libbzip2](https://sourceware.org/bzip2/manual/manual.html#:~:text=3.-,Programming%20with%20libbzip2,-Table%20of%20Contents)