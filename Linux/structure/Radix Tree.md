基数树，或称为压缩前缀树/压缩 trie ——  空间优化的前缀树 trie 结构。
策略：使用字符串而不是字符标记边，压缩节点链为单个字符串。
![[radix-tree-example.png|500]]

基数树优化：基数树会合并只有单个子节点的节点。

节点合并规则
- 只有一个子节点的节点会与其父节点合并
- 合并后边可以标记字符序列而不仅仅是单个字符
基数限制
- 每个内部节点的子节点数量最多为基数 r 
- 其中 $r = 2^x \quad (x \ge 1)$，常见的 r = 2, 4, 8, 16, 256 等


在 radix tree 中查找值和在 trie 中查找值一样。
- 不变量：每个节点只有一个以每个字母开头的出边（outgoing edge）
- 每个非双圈节点至少有 2 个孩子节点
定理：节点数最多为 2n ，其中 n 是集合的大小

插入和 Trie 中的插入类似，除了有时需要将一条边一分为二
如，插入 `"cabbie"`
![[radix-tree-insert.png|500]]

实现
为了在 radix tree 中导航，需要可以查找一个字符并且获取以此字符开头的出边。
因此，每个节点将其出边存为一个映射：
- key：标签的第一个字符
- value：A pair（标签剩余部分，目标节点）
除了这些，实现与 tries 相似，其他主要区别是：将边一分为二。

使用示例：
- 内核4.20 以前使用 Radix Tree 来存储所有的页缓存（后续采用 `xarray` 取代了Radix Tree）
- 内存管理：`struct address_space` 中包含了一个 Radix Tree 用于跟踪所有的映射页
- 中断管理：内核使用 Radix Tree 来映射硬件中断号到软件中断号
- Linux 内存管理单元用 `Radix Tree` 来快速查找脏页或者回写页等

`include/linux/radix-tree.h`：
```c
#ifndef _LINUX_RADIX_TREE_H
#define _LINUX_RADIX_TREE_H

#include <linux/bitops.h>
#include <linux/gfp_types.h>
#include <linux/list.h>
#include <linux/lockdep.h>
#include <linux/math.h>
#include <linux/percpu.h>
#include <linux/preempt.h>
#include <linux/rcupdate.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/xarray.h>
#include <linux/local_lock.h>

// 向后兼容，旧结构将被 xarray 和 xa_node 替换
#define radix_tree_root     xarray
#define radix_tree_node     xa_node

// 用于基数树节点预分配的结构：在插入新节点到 radix tree 时，内核会优先从 `radix_tree_preload` 结构体中获取已预分配好的节点，避免频繁的动态内存分配，提高效率，尤其适合在不能睡眠的上下文（如中断）中使用。
/*
@lock: 本地锁，用于保护该结构体中的数据，防止多核/多线程环境下的并发访问冲突
@nr: 记录当前预分配节点的数量
@nodes: 指向预分配的基数树节点链表的头部，使用 nodes->parent 字段把这些节点串成链表
*/
struct radix_tree_preload {
	local_lock_t lock;
	unsigned nr;
	/* nodes->parent 指向下一个预分配的 node */
	struct radix_tree_node *nodes;
};
// 为每个 CPU 分配一个独立的 struct radix_tree_preload 变量，变量名为 radix_tree_preloads
DECLARE_PER_CPU(struct radix_tree_preload, radix_tree_preloads);

/*
 槽位的最低两位决定了该槽位中其他位（剩余部分）的解释方式
 00 -- 数据指针（data pointer）
 10 -- 内部条目（internal entry），通常指向树的下一级节点、兄弟节点或一个特殊标记，表示该条目已被移动
 x1 -- 值条目（value entry），`x`可以是 0 或 1
 尽管 NULL 也符合 data pointer 的模式，但它有特殊含义，表示该索引在树中没有条目（无论在哪一层），因此存储 NULL 等价于删除。
*/
#define RADIX_TREE_ENTRY_MASK       3UL     // 用于提取槽的最低两位
#define RADIX_TREE_INTERNAL_NODE    2UL     // 用于标记 internal entry

static inline bool radix_tree_is_internal_node(void *ptr)
{
	return ((unsigned long)ptr & RADIX_TREE_ENTRY_MASK) == 
	           RADIX_TREE_INTERNAL_NODE;
}

// radix-tree API starts here
#define RADIX_TREE_MAP_SHIFT    XA_CHUNK_SHIFT
#define RADIX_TREE_MAP_SIZE (1UL << RADIX_TREE_MAP_SHIFT)
#define RADIX_TREE_MAP_MASK (RADIX_TREE_MAP_SIZE-1)

#define RADIX_TREE_MAX_TAGS XA_MAX_MARKS
#define RADIX_TREE_TAG_LONGS    XA_MARKS_LONGS

#define RADIX_TREE_INDEX_BITS (8 /* CHAR_BIT */ * sizeof (unsigned long))
#define RADIX_TREE_MAX_PATH (DIV_ROUND_UP(RADIX_TREE_INDEX_BITS,                            RADIX_TREE_MAP_SHIFT))

/* The IDR tag is stored in the low bits of xa_flags */
#define ROOT_IS_IDR ((__force gfp_t)4)
/* The top bits of xa_flags are used to store the root tags */
#define ROOT_TAG_SHIFT (__GFP_BITS_SHIFT)

#define RADIX_TREE_INIT(name, mask) XARRAY_INIT(name, mask)

#define RADIX_TREE(name, mask) \
    struct radix_tree_root name = RADIX_TREE_INIT(name, mask)

#define INIT_RADIX_TREE(root, mask) xa_init_flags(root, mask)

static inline bool radix_tree_empty(const struct radix_tree_root *root)
{
	return root->xa_head == NULL;
}

/**
 * struct radix_tree_iter - 基数树迭代器状态
 *
 * @index: 当前槽的索引（即当前遍历到的元素在树中的索引）
 * @next_index: 当前 chunk（块）中最后一个槽的下一个索引。用于确定本次遍历的范围
 * @tags: 用于“带标签遍历”时的位掩码，表示当前 chunk 中哪些槽有特定的 tag 
 * @node: 指向当前包含这些槽的 radix tree 节点
 * 
 * 该 radix tree 迭代器以 "chunk" 为单位遍历。chunk 是 radix tree 叶子节点中一段连续槽位。通过 index 和 next_index 可以知道当前 chunk 的范围。如果是带标签的遍历，tags 字段会保存当前 chunk 的标签位图。
 */
struct radix_tree_iter {
	unsigned long index;
	unsigned long next_index;
	unsigned long tags;
	struct radix_tree_node *node;
};


```