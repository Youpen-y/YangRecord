Async I/O 是一种单线程、单进程的技术，采用协作式多处理（cooperative multitasking）。尽管 Async I/O 在单进程中使用单线程，但却给人一种并发的感觉。协程（coroutines，coro）是 Async I/O 的核心特性，可以并发调度，但它们本身并非并发的。

协程（coroutine）是一个可以暂停执行并稍后恢复的对象。同时，它可以将控制权交给事件循环（event loop），事件循环可以执行另一个协程。

协程对象通过调用协程函数（也称为异步函数）生成的。`async def` 语法构造引入了协程函数或异步生成器（asynchronous generator）.
```python
async def a_func():
    ...
```

`async with` 和 `async for` 语法结构分别引入异步 `with statements` 和 `for loops`.

`await` 关键字暂停当前协程的执行并将控制权交还给事件循环。
```python
import asyncio

async def f():
    print("f start running")
    await asyncio.sleep(2)
    print("f running over")
    return 42
    
async def g():
    print("g start running")
    result = await f()  # g 在这里暂停，等 f 完成
    print("g resume running, get the result:", result)
    
asyncio.run(g())
"""
g start running
f start running
f running over
g resume running, get the result: 42
"""
```

同步 VS 异步示例
```python
"""
同步示例：Cost 6.00 seconds
"""
import time

def count():
    print("One")
    time.sleep(1)
    print("Two")
    time.sleep(1)
    
def main():
    for _ in range(3):
        count()
        
if __name__ == "__main__":
    start = time.perf_counter()
    main()
    elapsed = time.perf_counter() - start
    print(f"{__file__} executed in {elapsed:0.2f} seconds.")
```
```python
"""
异步：Cost 2.00 seconds
"""
import asyncio

async def count():
    print("One")
    await asyncio.sleep(1)
    print("Two")
    await asyncio.sleep(1)
    
async def main():
    await asyncio.gather(count(), count(), count())
    
if __name__ == "__main__":
    import time
    
    start = time.perf_counter()
    asyncio.run(main())
    elapsed = time.perf_counter() - start
    print(f"{__file__} executed in {elapsed:0.2f} seconds.")
```