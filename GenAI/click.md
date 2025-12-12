`Click`（Command Line Interface Creation Kit） —— Python 包，只需以尽可能少的代码，便可以组合的方式创建漂亮的命令行界面。
特性：
- 任意层级的命令嵌套（子命令系统）
在 Click 中，可以定义一个主命令，再在其下定义任意多层子命令。
示例：
```python
import click

@click.group()
def main():
    pass

@main.group()
def cli():
    pass

@cli.command()
def start():
    click.echo("Starting server...")

@cli.command()
def stop():
    click.echo("Stopping server...)
    
if __name__ == "__main__":
    main()
```

- 自动生成帮助页面
Click 会自动解析函数的参数、选项、文档字符串，生成漂亮的命令行帮助说明。
比如上述示例（`click-example.py`）可以通过
```bash
$ python click-example.py --help
Usage: click-example.py [OPTIONS] COMMAND [ARGS]...

Options:
  --help  Show this message and exit.

Commands:
  cli
  
$ python click-example.py cli --help
Usage: click-example.py cli [OPTIONS] COMMAND [ARGS]...

Options:
  --help  Show this message and exit.

Commands:
  start
  stop
```

- 支持子命令的延迟加载
即，允许在程序运行时才动态加载子命令，而不是在程序启动时全部加载。

---
Click 通过装饰器（decorators）声明命令。在内部，有一个用于高级用例的非装饰器接口，但不建议用于高级使用。
通过 `command()` 对函数进行修饰，函数将成为 Click 命令行工具。最简单的是，只需用这个装饰器装饰一个函数，就会把它变成一个可调用的脚本：
```python
import click

@click.command()
def hello():
    click.echo("Hello World!")

@click.group()
def cli():
    pass
    
@click.command()
def initdb():
    click.echo("Initialized the database")
    
@click.command()
def dropdb():
    click.echo("Dorpped the database")

cli.add_command(initdb)
cli.add_command(dropdb)

if __name__ == "__main__":
    hello()
```
`command()`装饰器将函数转换为一个 Command，然后可以调用该命令。
使用 `group()` 装饰器创建一个 Group 对象，可给出多个可以用 `Group.add_command()` 附加的自命令。

对于简单的脚本，还可以使用 `Group.command()` 自动附加和创建命令装饰器。等价于：
```python
@click.group()
def cli():
    pass

@cli.command()
def initdb():
    click.echo("Initialized the database")
    
@cli.command()
def dropdb():
    click.echo("Dropped the database")
    
if __name__ == "__main__":
    cli()
```

#### 添加参数
为了添加参数，可使用 `option()` 和 `argument()` 装饰器。
```python
@click.command()
@click.option('--count', default=1. help='number of greetings')
@click.argument('name')
def hello(count, name):
    for x in range(count):
        click.echo(f"Hello {name}!")
```

执行
```python
$ python click-example2.py --help
Usage: click-example2.py [OPTIONS] NAME

Options:
  --count INTEGER  number of greetings
  --help           Show this message and exit.
```

更好的做法是将命令行实用程序编写为带有入口点的可安装包，而不是告诉用户运行 `python somefile.py`。

示例项目：
```bash
hello-project/
    src/
        hello/
            __init__.py
            hello.py
    pyproject.toml
```
`hello.py` 的内容：
```python
import click

@click.command()
def cli():
    """Prints a greeting."""
    click.echo("Hello, World!")
```
`pyproject.toml` 的内容：
```toml
[project]
name = "hello"
version = "1.0.0"
description = "Hello CLI"
require-python = ">=3.11"
dependencies = [
	"click>=8.1",
]

[project.scripts]
hello = "hello.hello:cli"   # "=" 之前是应生成的脚本的名称，之后是导入路径，后跟冒号，随后是要调用的函数（Click 命令）

[build-system]
require = ["flit_core<4"]
build-backend = "flit_core.buildapi"
```