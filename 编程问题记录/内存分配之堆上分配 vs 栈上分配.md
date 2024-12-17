结果：
时间开销上，堆上分配 > 栈上分配
适用上，堆上适合分配大段内存，而栈上内存一般受限。

栈上分配/释放内存只需要递增/递减指针，该指针在大多数体系结构上通常具有专用寄存器。
如，在 8086 上，使用 `PUSH/POP` 来入栈和出栈
`SS` 堆栈段寄存器，通常用于存储关于 _存储当前执行程序的调用栈的 memory segment 的信息_
`SP` 堆栈寄存器，指向当前堆栈段顶部（通常情况下，堆栈在内存中向下增长，即，新值放在较低的内存地址）
由于Stack allocation（栈上分配）仅限于特定函数，是它们称为编译器优化的候选项，如预先计算栈上所需的总空间并为整个栈帧执行单个增量；另外，栈可以更好地保证数据局部性。堆栈顶部几乎总是保证位于 Cache line 中；此外，由于堆栈指针通常存储在寄存器中，在某些体系结构上，优化编译器甚至可以通过重用以前栈帧中的参数（这些参数作为参数传递给更深栈帧中的被调用函数）来完全消除栈上的分配。同样，栈上分配的变量通常也可以提升到寄存器，从而避免分配。

---
相比之下，堆上分配（free store）复杂的多。像 `glibc ptmalloc2` 中对空闲内存的组织将涉及多种不同的数据结构。
优点：
- 分配的区域全局可见，可跨线程传递

缺陷：
- 存在碎片管理，需要复杂的碎片管理机制



堆上提供的是一大块连续的内存区域，如何满足用户的高效分配/释放内存的需要 => 不同的内存池实现。
- [dlmalloc](https://gee.cs.oswego.edu/dl/html/malloc.html)) - Doug Lea's malloc
- [ptmalloc](https://www.gnu.org/software/libc/manual/html_node/The-GNU-Allocator.html) - Glibc malloc
- [phkmalloc](https://phk.freebsd.dk/pubs/malloc.pdf) - FreeBSD
- [tcmalloc](https://goog-perftools.sourceforge.net/doc/tcmalloc.html) - Google malloc
	- [How tcmalloc Works | James Golick](https://jamesgolick.com/2013/5/19/how-tcmalloc-works.html#footnote1)
	- 
- [jemalloc](https://people.freebsd.org/~jasone/jemalloc/bsdcan2006/jemalloc.pdf) - Jason Evan's malloc implementation for FreeBSD (successor of phkmalloc)


---
内存池设计选择（安全相关）
- 分组：基于大小的组/操作
	- 将相同大小的对象缓存到一起
- 就地元数据：metadata before/after or even inside
	- 将元数据放在释放的区域
- 基数（Cardinal）元数据：不编码，直接指针和大小
	- 链表使用原始指针

![[memory allocator.png]]

