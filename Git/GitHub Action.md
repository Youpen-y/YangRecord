使用 GitHub Actions 直接在存储库中自动化、自定义和执行软件开发工作流程。可以发现、创建和共享操作来执行想要的任何工作（包括 CI/CD），并将操作组合成完全自定义的工作流程。

#### 工作流
工作流（`Workflows`）是一种可配置的自动化流程，可运行一个或多个作业。它使用存储库中的 YAML 文件创建，并在事件触发时运行。工作流可以手动触发或按照定义的时间表触发。

工作流在仓库的 `.github/workflows` 目录中定义。在仓库中，可以创建多个执行不同任务的工作流。如
1. 构建和测试拉取请求
2. 在云上部署您的应用程序
3. 对每个拉取请求运行测试

#### 事件
事件（Events）是存储库中的特定活动，可触发或运行 GitHub 存储库中的工作流程。例如，当您将代码推送到存储库时，它会触发 `push` 事件。当您创建新问题时也会发生同样的情况 - 它会触发 `issues` 事件。当有人在您的存储库中发出拉取请求时，它会触发 `pull_request` 事件。示例：
1. Push
2. Pull Request
3. Release
4. Label
5. Issues
6. Milestone

#### 工作
默认情况下，GitHub Action 工作（Jobs）并行运行。GitHub Action 工作流程运行一个或多个作业，每个作业包含一组执行命令或操作的步骤。以下是示例：
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
**运行器**（Runners）是触发后执行工作流程的服务器。每个运行器一次只能处理一项作业。GitHub 提供适用于 Ubuntu Linux、Microsoft Windows 和 MacOS 的运行器来运行您的工作流程。要定义运行器，请在 `runs-on` 选项中指定运行器值。您可以将其作为**单个字符串**或**字符串数​​组**提供。
```yaml
# .github/workflows/demo.yml

# String
runs-on: ubuntu-latest
# Array of string
runs-on: [ ubuntu-latest, windows-latest, macos-latest ]
```


[GitHub Action 教程](https://www.freecodecamp.org/news/learn-to-use-github-actions-step-by-step-guide/#heading-how-to-create-a-github-action-in-your-repository)