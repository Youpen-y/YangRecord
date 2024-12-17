环形缓冲区实现队列
### 一、数组 + 两个索引
使用数组作为后备存储，加上 read 和 write 索引。
read 和 write 索引范围 `[0..(capacity - 1)]` 。

该实现的限制：
- 数组 size 需为 2 的幂次方
- 需要浪费数组的一个元素来指示数组已满
```c
uint32 read;
uint32 write;

// 实现索引的环回，条件是 capacity 为 2 的幂次方
int mask(val) {
	return val & (array.capacity - 1);
}

int inc(index) {
	return mask(index + 1);
}

// 从尾部插入数据
void push(val) {
	assert(!full());
	array[write] = val;
	write = inc(write);
}

// 读取头部值
int pop() {
	assert(!empty());
	ret = array[read];
	read = inc(read);
	return ret;
}

bool empty() {
	return read == write;
}

bool full() {
	return inc(write) == read;
}

int size() {
	return mask(write -read);
}
```

### 二、数组 + 索引（read） + 长度（length）
```c
// wirte 位置可通过 read + length 计算得到
write_pos = mask(read + length)
```
优点：高效内存使用；O(1) 插入和删除操作；无需移动数据；通过位操作实现快速索引计算；自动处理环形缓冲区的回环。
缺陷：
容量必须是 2 的幂；需正确处理空和满的状态；
整数溢出问题：
```c
// 由于 read 一直递增，read + length 可能溢出
uint32_t write_pos = mask(read + length); 

// 解决方案1：使用64位计算中间结果
static uint32_t get_write_index(RingBuffer_t *rb) {
	return ((uint64_t)rb->read + rb->length) & rb->mask;
}

// 解决方案2：周期性重置 read 指针
static void maybe_reset_read(RingBuffer_t *rb) {
	if (rb->length == 0) {
		rb->read = 0;
	}
}

// 解决方案3：使用取模运算而不是位掩码
static uint32_t get_index(RingBuffer_t *rb, uint32_t pos) {
	return pos % rb->capacity;
}
```
Ring Buffer 最常见用途是作为并发读取器和写入器之间的中介（无论是两个线程、共享内存的进程，还是与硬件通信的软件进程）。reader 和 writer 都将写入 length 字段，不利于缓存。read index 和 length 需要始终以原子方式读取和更新。
```c
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef struct RingBuffer {
	int *array;				// 数据数组
	uint32_t capacity;	// 缓冲区容量
	uint32_t read;			// 读指针位置
	uint32_t length;		// 当前存储的元素个数
} RingBuffer_t;

// 初始化缓冲区
RingBuffer_t *rb_create(uint32_t capacity) {
	/*确保容量是 2 的幂*/
	assert((capacity & (capacity - 1)) == 0);
	RingBuffer_t *rb = malloc(sizeof(RingBuffer_t));
	rb->array = malloc(capacity * sizeof(int));
	rb->read = 0;
	rb->length = 0;
	return rb;
}

// 掩码操作
static int mask(RingBuffer_t *rb, int val) {
	return val & (rb->capacity - 1);
}

// 递增索引
static uint32_t inc(RingBuffer_t *rb, uint32_t index) {
	return mask(rb, index + 1);
}

// 压入元素
void rb_push(RingBuffer_t *rb, int val) {
	assert(!rb_full(rb));
	array[mask(rb, rb->read + rb->length)] = val;
	rb->length++;
}

// 弹出元素
int rb_shift(RingBuffer_t *rb) {
	assert(!(rb_empty(rb));
	int ret = rb->array[rb->read];
	rb->read = inc(rb, rb->read);
	rb->length--;
	return ret;
}

// 判断为空
bool rb_empty(RingBuffer_t *rb) {
	return rb->length == 0;
}

// 判断已满
bool rb_full(RingBuffer_t *rb) {
	return rb->length == rb.capacity;
}

// 当前大小
uint32_t size(RingBuffer_t *rb) {
	return rb->length;
}

// 清空缓冲区
void rb_clear(RingBuffer_t *rb) {
	rb->read = 0;
	rb->length = 0;
}

// 销毁缓冲区
void rb_destroy(RingBuffer_t *rb) {
	free(rb->array);
	free(rb);
}
```
### 三、数组 + 两个未掩码的索引
相比于第一个方案的调整：当索引递增时，不用将索引压缩到正确的范围内，仅在它们用于索引数组时调整。让这些索引无限增长，最终在无符号整数溢出时回绕到零。
- 避免浪费 slot 。

![[Circular buffer.png]]
```c
uint32 read;
uint32 write;

int mask(uint32 val) {
	return val & (array.capacity - 1);
}

void push(val) {
	assert(!full());
	array[mask(write++)] = val;
}

int shift() {
	assert(!empty());
	return array[mask(read++)];
}

// 检查缓冲区状态
bool empty() {
	return read == write;
}

bool full() {
	return size() == array.capacity;
}

uint32 size() {
	return write - read;
}
```

限制：
- 实现语言（如 C）需要支持无符号整数（unsigned）溢出环绕。
- 容量必须始终是 2 的幂。

> [!NOTE]
> - 最大容量只能是索引（index）数据类型范围的一半。（因此当使用 32 位无符号整数为 $2^{31} -1$）。某种程度上，可以解释为窃取索引的最高位以用作 flag 。
> 这里有些疑惑？为什么会有这样一个限制？



