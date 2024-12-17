### 使用并行的原因：
- 解决更大的问题
- 更快的解决问题
- 架构需要
    - Multi/Many-core
    - GPUs
- 能耗
- 投资最大化
### 并行的类型
程序 = 算法 + 数据结构
- 数据 - 并行对不同数据集应用相同的算法
- 函数/任务 - 对数据应用算法的不同部分
- 流水线（混合上述两者）- 对不同数据集应用算法的不同部分

### 串行到并行的经典方法
1. 开发串行（顺序）代码
2. 分析代码以确定最耗费时间的部分
3. 从顺序迁移到并行
简单但并不是最好
- 并行最好的实现是重新设计

---
OpenMP（最常使用） 方法
- 使用通用语言（如 C 语言）编写一个顺序代码
- 添加指令以并行化部分代码
- 得到一个计算出相同结果的并行程序（串行到并行等价）
- 程序是顺序和并行部分的混合
    - 并行部分由指令定义
OpenMP 是共享内存并行编程的标准（在多核机器上）
- 基于：编译器指令，库函数和环境变量
- 支持 C/C++ 和 Fortran 编程语言
执行模型基于并行执行的 `fork-join` 模型
![[fork-join 模型.png]]
- 并行通过指令指定，由程序员添加到代码
    - 编译器实现并行
OpenMP 考虑：
- 程序员负责确保并行程序的正确性和效率。
    - OpenMP 本身不解决：数据竞争、饥饿、死锁或糟糕的负载均衡
    - 但，提供函数（routines）以解决负载均衡或内存一致性
- 线程的创建/管理委托给编译器和 OpenMP 运行时
    - 容易并行化程序
    - 对线程行为少些操控
- 默认地，并行活动的数量根据现有资源在运行时定义
    - 如，2 cores => 2 threads
    - HT（超线程）算作额外的 cores

OpenMP 编程模型
- OpenMP 程序从单个线程开始（主线程，master thread）
- 并行部分（Parallel regions）创建一组并行活动
- 共享（Work-sharing）结构为并行活动生成任务
- 数据共享（Data sharing）子句指定如何在并行区域中共享变量
![[OpenMP language extensions.png]]
#### OpenMP 结构概述
- 针对C/C++应用的OpenMP指令格式
```
#pragma omp directive-name [clause[[,] clause]...] new-
block of code // group of statements separated by semicolons, enclosed in braces
```
- Parallel Constructs
    - `#pragma omp parallel`      创建一组线程
- Work-sharing Constructs
    - `#pragma omp for`              将循环 iterations 分配给线程
    - `#pragma omp sections`     将代码块（sections）分配给线程
    - `#pragma omp single`         限制一个代码块在单个线程上执行
- Tasking Constructs
    - `#pragma omp task`             创建被线程执行的任务池（pool of tasks）
- Master & Synchronization Constructs
    - `#pragma omp master`         限制一个代码块只能在主线程中执行
    - `#pragma omp critical`     限制一个代码块每次在单个线程上执行
    - `#pragma omp barrier`       使组中所有线程等待剩余的
    - `#pragma omp taskwait`     等待当前任务子任务的完成
    - `#pragma omp atomic`         确保以原子方式管理特定的存储位置
    - `#pragma omp flush`           使线程的临时内存视图与内存一致
    - `#pragma omp ordered`       指定循环区域的代码块将按循环迭代的顺序执行

当一个线程遇到一个 `parallel construct` ，一组线程被创建（`FORK`）。
```c
// Parallel Region Syntax
#pragma omp parallel [clauses]
{
    code_block
}

// clauses 可为
// if(scalar-expression)
// num_threads(integer-expression)
// private(list)
// firstprivate(list)
// shared(list)
// reduction(operator: list)

```
遇到并行区域（parallel region）的线程称为新线程组的主线程（`master`）；
组中所有线程（包括 master）执行该区域；
在并行区域的末尾，所有线程同步，并加入主线程（`JOIN`）

嵌套并行区域（Nested Parallel Region）
开启/关闭 => `omp_set_nested(x);` ，当开启时，执行并行区域的组中的一个线程遇到另一个并行指令，将创建一个新组，并称为该组的主线程。
![[Parallel Region level nested.png]]

Loop Construct（循环结构）
```c
#pragma omp for[clauses]
{
    code_block
}

// clause 可为
// private(list)
// firstprivate(list)
// lastprivate(list)
// reduction(operator: list)
// schedult(kind[, chunk_size])
// collapse(n)
// ordered
// nowait
```
- `for` 循环迭代分配个组中线程
    - 分配基于（chunk_size, 默认为 1），（schedule，默认为 static）
- Loop schedule
    - static - 迭代被分成 `chunk_size` 大小的块，以轮回的方式分配给组中的线程
    - dynamic - 块按线程对它们的请求分配给组中的线程
    - guided - 类似于 dynamic 但块大小在执行时减少
    - auto - 调度策略的选择委托给 OpenMP实现
- 静态调度 VS 动态调度
    - 静态开销低
    - 动态具有更好的负载平衡方法
    - 增加动态调度中块大小可以（减少调度开销；增加负载平衡问题的可能性）
- Parallel for with ordered clause
```c
#include <stdio.h>
#include <omp.h>

#define N 10
int main() {
    #pragma omp parallel
    {
        #pragma omp for schedule(static) ordered
        for (int i = 0; i < N; ++i) {
            // do something here (in parallel)
            #pragma omp ordered
            {
                printf("test() iteration %d by thread %d\n", i, omp_get_thread_num());
            }
        }
    }
    return 0;
}
/* 输出
test() iteration 0 by thread 0
test() iteration 1 by thread 0
test() iteration 2 by thread 0
test() iteration 3 by thread 0
test() iteration 4 by thread 0
test() iteration 5 by thread 1
test() iteration 6 by thread 1
test() iteration 7 by thread 1
test() iteration 8 by thread 1
test() iteration 9 by thread 1
*/
```

代码段（code sections）的并行执行
```c
// 支持异构 tasks
#pragma omp parallel
{
    #pragma omp sections
    {
        #pragma omp section
        {
            taskA();
        }
        #pragma omp section
        {
            taskB();
        }
        #pragma omp section
        {
            taskC();
        }
    }
}
```

Task Constructor 
```c
#pragma omp task [clauses]

#pragma omp taskwait
```
当一个线程遇到 task constructor 时，产生一个 task 。线程可以立即执行 task ，或可以被组中的其他线程随后执行。
taskwait 指令确保产生的任务在 return 语句之前返回

Synchronization Constructs
- 临界区域（互斥执行）
    - `#pragma omp critical [name]`
    - 将相关结构块执行限制为单个线程一次
    - 跨组工作（如，全局锁）
- 原子操作（细粒度同步）
```
#pragma omp atomic
 A[i] += x;
```
指定的内存将被原子更新。不需要使整个语句是原子的；仅内存更新是原子的。

Avoid/reduce synchronization
- Reduction of multiple values (in parallel)
```c
sum = 0;
#pragma omp parallel for reduction(+:sum)
for(int i = 0; i < 100; i++) {
    sum += array[i];
}
```
- 跨并行区域的线程重用
```c
#pragma omp parallel {
#pragma omp for
for (int i = 0; i < 100; i++)
    ...
#pragma omp for
for(int j = 0; j < 100; j++)
    ...
}
```

数据共享
并行区域中的变量会发生什么？
- 声明为 `inside` 的变量对线程而言是局部的
- 声明为 `outside` 的变量是共享的
数据共享语句
- `private(varlist)`  => `varlist` 中的每个变量对每个线程是私有的，未指定初始值。
- `firstprivate(varlist)` => 和 private 类似，但变量使用区域外的值初始化
- `lastprivate(varlist)` => 和 private 类似，但最终值是最后一个循环迭代的值
- `reduction(op:var)` => 和 `lastprivate` 类似，但最终值是使用 operator `op` 对私有值 reduction 的结果
数据共享指令
```c
#pragma omp threadlocal  // 每个线程获得值得一个本地拷贝
```
`copyin` 子句从主线程复制值到其他线程。


环境变量
`OMP_SCHEDULE` {static, dynamic, guided, auto}
`OMP_NUM_THREADS` 
`OMP_DYNAMIC`
`OMP_NESTED`
`OMP_STACKSIZE`
`OMP_WAIT_POLICY`
`OMP_MAX_ACTIVE_LEVELS
`OMP_THREAD_LIMIT`


OpenMP 讨论
- 优势
    - 可移植
    - 简单（相比于 MPI）
    - 增量并行
    - 顺序语义
    - 对GPGPU（有限的）支持
- 劣势
    - 需要支持 OpenMP 的编译器
    - 可能的数据竞争（其他线程同步问题）
    - 可扩展性
        - 仅限于共享内存
        - 不支持分布式内存
    - 复杂并行需要明确的并行代码