GitHub Actions 是打包的脚本，用于直接在存储库中自动化、自定义和执行软件开发工作流程。可以配置 GitHub Actions 以触发满足需要的复杂工作流。

GitHub Actions 是遵循 yml 数据格式的脚本。每个存储库有一个 `Actions` 标签页来快速设置第一个脚本。如果看到一个可作为起点的 workflow ，只需点击 `Configure` 按钮以添加脚本并开始编辑源 yml .
除了 `Actions` tab 上的 GitHub Actions 外，还可以
- 在 [GitHub Marketplace](https://github.com/marketplace?type=actions) 中搜索 GitHub Actions. GitHub Marketplace 允许发现和购买扩展工作流的工具。
- 搜索开源项目。如 [GitHub Actions](https://github.com/actions)组织有许多流行的开源存储库，其中包含可使用的 GitHub Actions.
- 从头开始编写自己的 GitHub Actions.

GitHub Actions 的类型
1. 容器动作（container actions）
对于容器动作，环境是动作代码的一部分。这些动作只能在 GitHub 托管的 Linux 环境中运行。容器动作支持多种不同的语言。
2. JavaScript 动作
JavaScript 动作不在代码中包含环境。但你必须指定执行这些动作的环境。可以在云或本地的 VM 中运行这些动作。JavaScript 动作支持 Linux, macOS 和 Windows 环境。
3. 复合动作（composite actions）
复合动作允许你把多个工作流步骤组合到一个动作里面。比如，你可以把多个 run 命令打包成一个动作，然后在工作流里，只需要写一个步骤来调用这个动作，就能一次性执行这些打包好的命令。”

### GitHub Actions 组件
![](https://learn.microsoft.com/en-us/training/github/github-actions-automate-tasks/media/github-actions-workflow-components.png)
#### 工作流
工作流（`Workflows`）是一种可配置的自动化流程，可运行一个或多个作业（至少一个）。它使用存储库中的 YAML 文件创建，并在事件触发时运行。工作流可以手动触发或按照定义的时间表触发。

工作流由仓库的 `.github/workflows` 目录下的 `.yml` 文件定义。
示例：
```yml
name: A workflow for my Hello World file
on: push
jobs:
    build:
        name: Hello world action
        runs-on: ubuntu-latest
        steps:
        - uses: actions/checkout@v1
        - uses: ./action-a
          with:
            MY_NAME: "Mona"
```
`on:`属性指定何时运行该工作流的触发器。可以指定单个事件如`on: push`，或事件数组 `on: [push, pull_request]`，或计划工作流或者限制在特定文件/tags/分支改变时执行工作流的事件配置映射。
工作流至少有一个 `job`. `job` 是与 `runner` 关联的工作流的一部分。`runner` 可以是 GitHub 托管的、也可以是自托管的，而且作业可以在计算机或容器中运行。可使用 `runs-on:` 属性指定 `runner`。

f在工作流中引用 actions ：在  GitHub Actions 中创建工作流时，可以引用来自各个源的 actions。 这些 actions 可用于自动执行工作流中的任务。以下是可以引用的 actions 的主要来源：
- Docker Hub 上发布的 Docker 容器镜像
示例：
```yml
steps:
    - name: Run a Docker action
      uses: docker://<docker-image-name>:<tag>
```
- 任何公共存储库
示例：
```yml
steps:
    - name: Use a public action
      uses: actions/checkout@v3
```
>[!NOTE]
>为了安全，在引用 action 时使用完全提交 `SHA`，而不仅仅是像 `@v3` 这样的标签。
>如：`uses:actions/checkout@c2c1744e079e0dd11c8e0af4a96064ca4f6a2e9e`

- 与工作流文件相同的存储库（采用相对路径）
```yml
steps:
    - name: Use a local action
      uses: ./path-to-action
```

- 企业市场（面向组织） 

 ### Steps（步骤）
 > A step is an individual task that can run commands in a job.
 
 ### Actions （动作）
 动作是工作流中执行的独立命令。这些独立命令可以引用 GitHub actions 如自己定制的 actions ，或社区中的 actions.

#### 事件
事件（Events）是存储库中的特定活动，可触发或运行 GitHub 存储库中的工作流程。例如，当您将代码推送到存储库时，它会触发 `push` 事件。当您创建新问题时也会发生同样的情况 - 它会触发 `issues` 事件。当有人在您的存储库中发出拉取请求时，它会触发 `pull_request` 事件。示例：
1. Push
2. Pull Request
3. Release
4. Label
5. Issues
6. Milestone

#### 作业（Jobs）
默认情况下，GitHub Action 工作（Jobs）并行运行。GitHub Action 工作流程运行一个或多个作业，每个作业包含一组执行命令或动作的步骤。以下是示例：
```yaml
# .github/workflows/demo.yml
name: Demo Workflows

on:
	push:

# A workflow run is made up of one or more jobs that can
# can run sequentially or in parallel

jobs:

jobs:
```

#### 运行器
**运行器**（Runners）是触发后执行工作流程的服务器。每个运行器一次只能处理一项作业。
运行器可以是 GitHub 托管的或自托管的
1. GitHub 托管的：`runs-on: {operating system-version}`，这种方式更快、更简单。
2. 自托管的：需要应用自托管标签、操作系统和系统体系结构。如：`runs-on: [self-hosted, linux, ARM32]`
GitHub 提供适用于 Ubuntu Linux、Microsoft Windows 和 MacOS 的运行器来运行您的工作流程。要定义运行器，请在 `runs-on` 选项中指定运行器值。您可以将其作为**单个字符串**或**字符串数​​组**提供。
```yaml
# .github/workflows/demo.yml

# String
runs-on: ubuntu-latest
# Array of string
runs-on: [ ubuntu-latest, windows-latest, macos-latest ]
```


示例：定时触发工作流
依赖下图所示 `cron` 语法，包含五个 `*` 字段，每个字段代表一个时间单位。

![[posix cron.png]]
如：每15分钟运行一次工作流。
```yml
on:
  schedule:
    - cron: '*/15 * * * *'
```
每周日凌晨 3:00 运行工作流
```yml
on:
  schedule:
    - cron: '0 3 * * SUN'
```

示例：为手动事件配置工作流
借助 `workflow_dispatch` 事件，由用户手动启动，使用 `workflow_dispatch`，可以选择在哪个分支上运行工作流，并设置 GitHub 在 UI 中作为表单元素呈现的可选 `inputs`。
两种方式：
- GitHub UI：在仓库的 Actions 页面里，选择某个 `workflow`，然后点击 `Run workflow` 按钮。
```yml
on:
  workflow_dispatch:
    inputs:
      logLevel:
        description: 'Log level'
        required: true
        default: 'warning'
      tags:
        description: 'Test scenario tags'
```
- GitHub REST API：通过 GitHub API 请求触发名为 `repository_dispatch` 的 webhook 事件。
```bash
curl \
    -X POST \
    -H "Accept: application/vnd.github.v3+json" \
    https://api.github.com/repos/OWNER/REPO/dispatches \
    -d '{"event_type":"deploy-app", "client_payload": {"env": "staging"}}'
```
```yml
# 监听自定义事件"deploy-app"
on:
  repository_dispatch:
    types: [deploy-app]
```

[GitHub Action 教程](https://www.freecodecamp.org/news/learn-to-use-github-actions-step-by-step-guide/#heading-how-to-create-a-github-action-in-your-repository)