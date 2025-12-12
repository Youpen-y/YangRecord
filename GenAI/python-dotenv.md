`python-dotenv` 从 `.env` 文件中读取键值对，并可以将它们设置为环境变量。有助于开发符合[[The Twelve-factor APP]] 的应用。

要求：在项目根目录添加一个 `.env` 文件，`.env` 文件的语法类似于 Bash 。
```.env
# Environment variable settings
DOMAIN=example.org
ADMIN_EMAIL=admin@${DOMAIN}
ROOT_URL=${DOMAIN}/app
```
如果在值中使用变量，确保使用 `{` 和 `}` 括起来，如 `${DOMAIN}`，因为裸变量（`$DOMAIN`）不会被扩展。
示例1：使用 `load_dotenv()`
```python
from dotenv import load_dotenv

load_dotenv()   # 从 .env 文件中读取变量并在 os.environ 中设置

# ...           # 使用环境变量的代码
```
默认情况下：
- 在与 Python 脚本相同的目录中查找 `.env` 文件（或者是目录树更高的位置）
- 读取每个键值对并将其添加到 `os.environ`
- 除非显式传递 `override=True` ，否则不会覆盖已设置的环境变量

示例2：`dotenv_values()` 在不改变环境的情况下加载配置
```python
from dotenv import dotenv_values

config = dotenv_values(".env")  # 返回一个字典，其中包含 .env 文件中的键值对
```

高级配置管理
```python
import os
from dotenv import dotenv_values

config = {
	**dotenv_values(".env.shared"),    # 装载共享环境变量
	**dotenv_values(".env.secret"),    # 装载敏感变量
	**os.environ,      # 使用环境变量覆盖已装载的变量
}
```

从流（stream）中解析配置
无论是 `load_dotenv()` 还是 `dotenv_values()` 都可通过 `stream` 参数接受流。这样，可以将配置源从文件扩展到其他源（如网络）。
```python
from io import StringIO
from dotenv import load_dotenv

config = StringIO("USER=foo\nEMAIL=foo@example.org")
load_dotenv(stream=config)
```

注意：
1. `.env` 中的变量可以无值（`VAR`），`load_dotenv()` 将忽略无值变量，`dotenv_values()` 将无值变量与 `None` 关联起来。注意与 `VAR=` 的区分，`VAR=` 是变量与空字符串相关联。
2. 使用 `load_dotenv(override=True)` 或 `dotenv_values()` 时，变量的值是以下列表中定义的值中的第一个：
	1. `.env` 文件中该变量的值
	2. 该变量在环境中的值
	3. 默认值（如果提供）
	4. 空字符串
3. 如果使用 `load_dotenv(override=False)`，变量的值是以下列表中定义的第一个值：
	1. 该变量在环境中的值
	2. `.env` 文件中该变量的值
	3. 默认值
	4. 空字符串

