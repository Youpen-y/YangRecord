```c
/*
* Keep mostly read-only and often accessed (especially for
* the RCU path lookup and 'stat' data) fields at the beginning
* of the 'struct inode'
* 将最常只读和访问的（尤其是RCU 路径查询和 stat 数据）域放在
* struct inode 结构的起始
*/
struct inode {
	umode_t i_mode;    // 类型 + 权限 + 特殊位
	unsigned short i_opflags;
	kuid_t i_uid;
	kgid_t i_gid;
	unsigned int i_flags;

#ifdef CONFIG_FS_POSIX_ACL
	struct posix_acl *i_acl;
	struct posix_acl *i_default_acl;
#endif

	const struct inode_operations *i_op;
	struct super_block *i_sb;
	struct address_space *i_mapping;

#ifdef CONFIG_SECURITY
	void *i_security;
#endif

/* Stat data, not accessed from path walking */
	unsigned long i_ino;
/*
* Filesystems may only read i_nlink directly. They shall use the
* following functions for modification:
*
* (set|clear|inc|drop)_nlink
* inode_(inc|dec)_link_count
*/

	union {
		const unsigned int i_nlink;
		unsigned int __i_nlink;
	};

	dev_t i_rdev;
	loff_t i_size;
	time64_t i_atime_sec;
	time64_t i_mtime_sec;
	time64_t i_ctime_sec;
	u32 i_atime_nsec;
	u32 i_mtime_nsec;
	u32 i_ctime_nsec;
	u32 i_generation;
	spinlock_t i_lock; /* i_blocks, i_bytes, maybe i_size */
	unsigned short i_bytes;

	u8 i_blkbits;
	enum rw_hint i_write_hint;
	blkcnt_t i_blocks;
  
#ifdef __NEED_I_SIZE_ORDERED
	seqcount_t i_size_seqcount;
#endif

	/* Misc */
	u32 i_state;
	/* 32-bit hole */
	struct rw_semaphore i_rwsem;
	
	unsigned long dirtied_when; /* jiffies of first dirtying */
	unsigned long dirtied_time_when;
	struct hlist_node i_hash;
	struct list_head i_io_list; /* backing dev IO list */
	
#ifdef CONFIG_CGROUP_WRITEBACK
	struct bdi_writeback *i_wb; /* the associated cgroup wb */
	/* foreign inode detection, see wbc_detach_inode() */
	int i_wb_frn_winner;
	u16 i_wb_frn_avg_time;
	u16 i_wb_frn_history;
#endif

	struct list_head i_lru; /* inode LRU list */
	struct list_head i_sb_list;
	struct list_head i_wb_list; /* backing dev writeback list */
	union {
		struct hlist_head i_dentry;
		struct rcu_head i_rcu;
	};

	atomic64_t i_version;
	atomic64_t i_sequence; /* see futex */
	atomic_t i_count;
	atomic_t i_dio_count;
	atomic_t i_writecount;

#if defined(CONFIG_IMA) || defined(CONFIG_FILE_LOCKING)
	atomic_t i_readcount; /* struct files open RO */
#endif

	union {
		const struct file_operations *i_fop; /* former ->i_op->default_file_ops */
		void (*free_inode)(struct inode *);
	};

	struct file_lock_context *i_flctx;
	struct address_space i_data;
	struct list_head i_devices;
	union {
		struct pipe_inode_info *i_pipe;
		struct cdev *i_cdev;
		char *i_link;
		unsigned i_dir_seq;
	};

#ifdef CONFIG_FSNOTIFY
	__u32 i_fsnotify_mask; /* all events this inode cares about */
	/* 32-bit hole reserved for expanding i_fsnotify_mask */
	struct fsnotify_mark_connector __rcu *i_fsnotify_marks;
#endif

#ifdef CONFIG_FS_ENCRYPTION
	struct fscrypt_inode_info *i_crypt_info;
#endif 

#ifdef CONFIG_FS_VERITY
	struct fsverity_info *i_verity_info;
#endif

void *i_private; /* fs or device private pointer */

} __randomize_layout;
```

解析：
#### 首部注释
```c
/*
* Keep mostly read-only and often accessed (especially for
* the RCU path lookup and 'stat' data) fields at the beginning
* of the 'struct inode'
* 将最常只读和访问的（尤其是RCU 路径查询和 stat 数据）域放在
* struct inode 结构的起始。
*/
```
1. `inode` 中的一些字段，一旦文件被创建或特定属性被设置后，就不再经常修改。如，文件创建时间、文件类型（普通文件、目录、符号链接等）通常是只读的，除非文件被删除或属性被特殊修改。
2. 还有一些字段在文件系统操作中被频繁读取。如
	- RCU（Read-Copy Update）path lookup（路径查找）：当内核需要解析一个文件路径（如`/home/user/document.txt`）时，会逐级查找目录对应的 `inode` 。在这个过程中，会频繁访问 `inode` 的目录项相关信息（如 `i_nlink` 连接数，`i_ino` inode 号，`i_mode` 权限和类型）。RCU 是一种同步机制，用于在读写并发场景下提供高效的读取操作。
	- `stat` 数据：应用程序经常会调用 `stat()` 、`fstat()` 、`lstat()` 等系统调用来获取文件的元数据，如文件大小（`st_size`）、访问时间（`st_atime`）、修改时间（`st_mtime`）、inode 号（`st_ino`）、设备 ID（`st_dev`）、权限（`st_mode`）等。这些数据通常直接从 `inode` 结构中读取。
将最常访问且大部分只读的字段放在结构体的开头，好处如下：
- 缓存局部性：现代 CPU 都有多级缓存（L1, L2, L3）。当 CPU 访问内存时，它通常会一次性将一块数据（称为“缓存行”或“缓存块”，通常是 64 字节）从主内存加载到缓存中。如果经常访问的字段都紧密地排列在结构体的开头，那么当一个字段被访问时，其他也经常访问的字段很可能已经被预取到同一个缓存行中。这样可以大大减少缓存未命中（cache miss）的概率，从而提高数据访问速度。
- 减少缓存行抖动/争用：如果可写字段（即经常被修改的字段）与只读字段在同一个缓存行中，那么当这个可写字段被修改时，即使只读字段没有变化，整个缓存行也会被标记为“脏”（dirty），需要被写回主内存或者在多个 CPU 核心之间进行同步（如果其他核心也有这个缓存行的副本）。这会导致所谓的“缓存行抖动”（cache line bouncing）或“伪共享”（false sharing），从而降低性能。将只读字段与可写字段分开，尤其将只读字段放在前面，可以使得只读字段所在的缓存行更稳定，减少不必要的缓存同步开销。
---
#### `i_mode` —— 决定文件类型与访问权限
`i_mode` 是 `inode` 结构中一个 16 位（有些系统是 32 位）字段，主要用于存储
1. 权限位（`rwx` 权限）
2. 特殊位（如 `SUID`，`SGID`，Sticky 位）
3. 文件类型（如普通文件、目录、符号链接等）

前 9 位`[0-8]`描述了：其他、组和所有者的权限
```mathematica
bits:  |   0   1   2   |       |   3   4   5   |       |   6   7   8   |
others:|   x   w   r   | group:|   x   w   r   | owner:|   x   w   r   |
```

中间 3 位 `[9-11]` 是特殊位
```mathematica
bits:  |         9           10          11         |
       |    Sticky bits     Set GID     Set UID     |
```

后面 4 位 `[12-15]` 指定文件类型
```
0001    FIFO（管道）
0010    Character device（字符设备）
0100    Directory（目录）
0110    Block device（块设备）
1000    Regular file（普通文件）
1010    Symbolic link（符号链接）
1110    Socket（套接字）
```

定义：
```c
#define S_IFMT  00170000        // file format
#define S_IFSOCK 0140000        // socket
#define S_IFLNK	 0120000        // symbolic link
#define S_IFREG  0100000        // regualar file
#define S_IFBLK  0060000        // block device
#define S_IFDIR  0040000        // directory
#define S_IFCHR  0020000        // character device
#define S_IFIFO  0010000        // fifo

// special bits
#define S_ISUID  0004000        // set UID
#define S_ISGID  0002000        // set GID
#define S_ISVTX  0001000        // sticky bits

#define S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)

// user
#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

// group
#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

// other
#define S_IRWXO 00007
#define S_IROTH 00004       // others may read
#define S_IWOTH 00002       // others may write
#define S_IXOTH 00001       // others may execute
```