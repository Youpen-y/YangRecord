`logging` - Python 的日志记录工具

源代码：`lib/logging/__init__.py`

该模块定义了函数和类，这些函数和类为应用和库实现了一个灵活的事件记录系统。

Python 标准库提供了统一的 `logging` API，最大的好处是：你自己写的代码和你用到的第三方库可以共用同一套日志系统，从而让应用的日志信息统一、集中，更易于管理和分析。

---
### 教程
1. 创建一个 `logger`
```python
logger = getLogger(__name__)
```

2. 调用 `logger` 的方法（`debug()`, `info()`, `warning()`, `error()`, `critical()`）


| 任务                                  | 最佳工具                                                                |
| ----------------------------------- | ------------------------------------------------------------------- |
| 命令行脚本或程序普通使用的控制台输出                  | `print()`                                                           |
| 报告程序正常运行期间发生的事件（如，用于状态监控或故障调查）      | `logger` 的 `info()` {或 `debug()` 方法获取非常详细的输出以进行诊断                   |
| 发出有关特定运行事件的警告                       | 库代码中的`warnings.warn()`如果问题是可以避免的;而且应该修改客户端应用程序以消除警告                 |
| 报告有关特定运行时事件的错误                      | 引发异常                                                                |
| 报告错误抑制而不引发异常（如，长时间运行的服务器进程中的错误处理程序） | `logger` 的 `error()`, `exception()`或 `critical()` 方法（适用于特定错误和应用程序域） |

`logger` 方法以其用于跟踪的事件的级别或严重性命名。标准级别及其适用性如下所述（按严重性升序排列）：

| Level    | 什么时候使用                               |
| -------- | ------------------------------------ |
| DEBUG    | 详细信息，通常仅在诊断问题时才感兴趣                   |
| INFO     | 确认一切按预期进行                            |
| WARNING  | 表明发生了意外情况，或表明在不久的将来会出现某些问题。软件仍在按预期工作 |
| ERROR    | 由于一个更严重的问题，该软件无法执行某些功能               |
| CRITICAL | 严重错误，表明程序本身可能无法继续运行                  |

默认级别为 WARNING，这意味着将仅跟踪此严重性及更高级别的事件，除非日志记录包配置为做其他操作。
跟踪事件的处理方式：
- 打印到控制台
- 写入磁盘文件

#### 简单示例
```python
import logging
logging.warning('Watch out!')   # will print a message to the console
logging.info('I told you so')   # will not print anything
```
运行时将打印 `WARNING:root:Watch out!`，而 `INFO` 消息未显示，因为默认级别为 `WARNING`。
可以看到，该示例直接在日志模块上使用函数模块，而不是创建一个 `logger` 并在其上调用函数。这些函数在 root logger 上运行，如果尚未调用，它们将为您调用 `basicConfig()` 。
但在较大的程序中，通常需要显式控制日志记录配置 - 因此，出于这个原因以及其他原因，最好创建 `logger` 并调用它们的方法。

#### 记录到文件
```python
import logging
logger = logging.getLogger(__name__)
logging.basicConfig(filename='example.log', encoding='utf-8', level=logging.DEBUG)
logger.debug('This message should go to the log file')
logger.info('So should this')
logger.warning('And this, too')
logger.error('And non-ASCII stuff, too, like resund and Malmo')
```

对 `basicConfig()` 的调用应该在对 `logger` 方法（`debug()`, `info()`）调用之前进行。否则，可能无法以所需的方式处理该日志记录事件。

```python
def basicConfig(**kwargs):
    """
    完成日志系统的基本配置

    如果根日志记录器（root logger）已配置处理程序（handlers），除非将关键字参数 *force* 设置为 True 。这是一个便捷的方法，旨在供简单脚本使用，用于对日志包（logging package）进行一次性配置。默认行为是创建一个 StreamHandler，用于写入 sys.stderr ，使用 BASIC_FORMAT 格式字符串，并将处理程序添加到根日志记录器。
    可以指定多个可选关键字参数，这些参数可以更改默认行为。

    filename 指定日志输出到的文件名。如果设置了这个参数，会创建一个 FileHandler（文件处理器），否则默认输出到标准错误（stderr）。
    filemode 文件打开模式，只有在指定 filename 时有效，默认是 `a`（追加模式）
    format   为该 handler 使用日志输出的格式字符串
    datefmt  使用指定的 date/time 格式
    style    格式化风格，支持 `%`, `{`, `$`, 分别对应不同的格式化方式，默认是 `%`
    level    设置 root logger level 为指定级别
    stream   指定日志输出的流（如 sys.stdout )，和 filename 互斥，不能同时指定
    handlers 已创建好的 handler 列表，会被添加到 root logger 。和 filename/stream 互斥
    force    如果为 True ，会移除并关闭 root logger 上已有的所有 handler，然后重新配置
    encoding 指定文件编码（仅 filename 有效）
    errors   指定文件打开时的错误处理方式（仅 filename 有效，默认为 `backslashreplace`）

    注意，你可以使用 open(filename, mode) 来指定打开的文件流，传给 `StreamHandler` ，而不是直接传文件名给 `FileHandler` 。
    ```python
    f = open("mylog.txt", "w")
    handler = logging.StreamHandler(f)
    # `StreamHandler` 不会在 `handler.close()` 时关闭 `f`；必须手动关闭 `f`
    ```
    ```python
    handler = logging.FileHandler("mylog.txt", mode="w")
    # `FileHandler` 会内部调用 `open(...)`
    # 在 `handler.close()` 时，文件会被自动关闭
    ```
```


### 高级日志教程
日志库采用模块化方法，并提供几类组件：`loggers`, `handlers`, `filters`, `formatters`
- `loggers` 公开应用程序代码可直接使用的接口
- `handlers` 将日志记录（由 `loggers` 创建）发送到相应的目标
- `filters` 为确定要输出的日志记录提供更精细的工具
- `formatters` 指定最终输出中日志记录的布局

