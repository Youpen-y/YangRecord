基数树/压缩前缀树
- V4.20 以前使用 Radix Tree 来存储所有的页缓存（后续采用 `xarray` 取代了Radix Tree）
- 内存管理：`struct address_space` 中包含了一个 Radix Tree 用于跟踪所有的映射页
- 中断管理：内核使用 Radix Tree 来映射硬件中断号到软件中断号
- Linux 内存管理单元用 `Radix Tree` 来快速查找脏页或者回写页等

