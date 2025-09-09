构建 Hugo 静态网站
Hugo 是使用 Go 语言构造的快速、灵活的静态网站生成器。

语法：
```bash
hugo [flags]
hugo [command]
hugo command --help # 显示命令用法
```

可用命令：

| command      | purpose                                               |
| ------------ | ----------------------------------------------------- |
| `build`      | 构造网站                                                  |
| `completion` | 为指定的 `shell` 生成自动完成脚本                                 |
| `config`     | 显示网站配置                                                |
| `convert`    | 将前言转变为另一种格式                                           |
| `deploy`     | 在云供应商上部署网站                                            |
| `env`        | 展示版本和环境信息                                             |
| `gen`        | 生成文档和语法高亮样式                                           |
| `help`       | 查看其他任何命令的用法：`hugo help COMMAND`                       |
| `import`     | 从另一个系统导入一个网站                                          |
| `list`       | 列出撰写的内容：`all`，`drafts`，`expired`，`future`，`published` |
| `mod`        | 管理模块                                                  |
| `new`        | 创建新内容                                                 |
| `server`     | 打开嵌入的网络服务器                                            |
| `version`    | 展示版本                                                  |

用法：
1. 创建新网站
```bash
hugo new site AnotherSite
Congratulations! Your new Hugo site was created in /home/yongy/Projects/blog/AnotherSite.

Just a few more steps...

1. Change the current directory to /home/yongy/Projects/blog/AnotherSite.
2. Create or install a theme:
   - Create a new theme with the command "hugo new theme <THEMENAME>"
   - Or, install a theme from https://themes.gohugo.io/
3. Edit hugo.toml, setting the "theme" property to the theme name.
4. Create new content with the command "hugo new content <SECTIONNAME>/<FILENAME>.<FORMAT>".
5. Start the embedded web server with the command "hugo server --buildDrafts".

See documentation at https://gohugo.io/.
```

2. 安装并配置主题
```bash
cd AnotherSite && git init
git submodule add https://github.com/theNewDynamic/gohugo-theme-ananke.git themes/ananke
echo "theme = 'ananke'" >> hugo.toml
```

3. 启动
```bash
hugo server
```

4. 添加内容
```bash
hugo new content content/posts/my-first-post.md
```
以下是文件的 front matter：
```markdown
+++
title = 'My First Post'
date = 2025-09-06
draft = true
+++
```
上述使用 `draft = true` ，意味着使用 `hugo server` 构建时
不会发布该草稿。可使用 `hugo server -D/--buildDrafts` 查看草稿情况，满意则设置 `draft = false`。

