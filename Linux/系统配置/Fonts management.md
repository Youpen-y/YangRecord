Linux 上
- `/usr/share/fonts/` - 系统范围字体存放目录
- `/usr/local/share/fonts/` - 用户添加的系统范围字体
- `~/.fonts/` - 仅当前用户字体存放目录

`fc-cache` - 构建字体信息缓存文件
```bash
$ fc-cache -fv		# 更新字体缓存
```
- `fc-cache` 扫描系统上的字体目录，并为使用 `fontconfig` 进行字体处理的应用程序构建字体信息缓存文件。
- 如果未给出目录参数，将使用当前字体配置中的每个目录。扫描每个目录以查找 `FreeType` 可读的字体文件。创建一个缓存，其中包含每种字体的属性和关联的文件名。此缓存用于在使用 `fontconfig` 库时加快应用程序启动速度。
- `fc-cache` 必须在每个体系结构中执行一次，以生成为该体系结构定制的字体信息。

`fc-list` - 列出可用字体
```bash
$ fc-list
```
`fc-list` 列出系统上使用 `fontconfig` 的应用程序可用的字体和样式。


> `fontconfig` 用户指南，HTML 格式位置： `/usr/share/doc/fontconfig/fontconfig-user.html`