数据一致性
JIAJIA 是基于 home 的软件DSM，每个共享页都有一个固定的 home 结点。每个处理机都要根据一致性协议的要求（如在一个同步区间结束时）把对 home 不在本地的页面的 diff 传播到 home 结点。在缺页时，缺页的处理机直接从缺失页的 home 结点取数。
- 在 JIAJIA 系统中，在释放锁时，将修改写回到页面的 Home 结点。
- 获取锁时不需要特殊操作
- 读取时，如果本地副本不是最新的，从 Home 结点请求最新版本


#### 其他分布式共享内存系统中的一致性模型
- `Munin` 系统（ERC, Eager Release Consistency）
    - 在释放锁时，立即将所有修改广播给其他所有处理器
    - 获取锁时不需要特殊操作
``` python
// 伪代码
def write(page, data):
    modify_local_copy(page, data)
    mark_page_as_dirty(page)

def release_lock():
    for page in dirty_pages:
        broadcast_updates(page)
    clear_dirty_pages()

def acquire_lock():
    # 直接获取锁，无需额外操作

def read(page):
    if page_is_local(page):
        return read_local_copy(page)
    else:
        request_page_from_owner(page)
        return read_local_copy(page)
```
- `TreadMarks` 系统（LRC， Lazy Release Consistency）
    - 释放锁时只创建写通知，不实际发送数据
    - 获取锁时接收写通知并使本地副本无效
    - 读取时，如果页面无效，才请求更新
``` python
def write(page, data):
    modify_local_copy(page, data)
    mark_page_as_dirty(page)

def release_lock():
    create_write_notice(dirty_pages)
    clear_dirty_pages()

def acquire_lock():
    receive_write_notices()
    for page in pages_with_notices:
        invalidate_local_copy(page)

def read(page):
    if page_is_invalid(page):
        request_updates_from_last_writer(page)
    return read_local_copy(page)
```

---
`rexec()` 函数通过网络发送未加密的密码 

---------
1. `init.c` 274 ~ 276 需要进一步修改





---
一个想法：使用处于同一无线网下的多台终端形成一个分布式系统。（手机，电脑(多用户)） 



什么样的场景适合用 分布式软件共享内存？？？



内存池，
当使用 mprotect 设置地址访问保护时，



Shared Memory Object
An object that represents memory that can be mapped concurrently into the address space of more than one process. Posix1-2024