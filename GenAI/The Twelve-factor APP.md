The Twelve-factor APP（十二因素应用程序）

### Introduction
在现代，软件通常作为服务提供：称为 *Web Apps*， 或 *Software-As-A-Service*（SAAS）。十二因素应用程序是一种构建 SAAS 应用程序的方法。

### 1. Codebase
十二因素应用始终在版本控制系统（Version Control System, VCS）进行跟踪，例如 `Git`、`Mercurial`、`JJ` 或 `Subversion`。修订跟踪数据库的副本称为代码存储库（Code Repository），通常称为 code repo 或 repo 。
![[代码库.png]]
### 2. Dependencies
> 显式声明和隔离依赖项

大多数编程语言都提供了一个打包系统来分发所支持的库。

| 语言         | 包管理系统            | 命令工具                           | 包仓库             |
| ---------- | ---------------- | ------------------------------ | --------------- |
| Python     | pip / setuptools | `pip install xxx`              | PyPI            |
| JavaScript | npm / yarn       | `npm install xxx`              | NPM Registry    |
| Java       | Maven / Gradle   | `mvn install` / `gradle build` | Maven Central   |
| Ruby       | Bundler          | `gem install xxx`              | RubyGems        |
| Rust       | Cargo            | `cargo add xxx`                | crates.io       |
| Go         | go modules       | `go get xxx`                   | Go module proxy |
| C#         | NuGet            | `nuget install xxx`            | NuGet Gallery   |
| Perl       | cpan / cpanm     | `cpan install xxx`             | CPAN            |
C/C++语言没有官方包管理系统，其库通常来自：
- `Linux` 系统包管理器 `apt`，`yum`，`pacman`，`brew`等安装 `.so` 或 `.a` 库
- 或手动下载源码  + 编译
Bash 世界里的“库”更像是命令行工具集合，不是语言级模块。

✅ **Explicitly declare 显式声明依赖**
指：
- 不能让依赖“隐式存在”或靠系统默认环境
- 必须用文件/配置把依赖写清楚
如：

| 技术栈        | 声明依赖的方式                                            |
| ---------- | -------------------------------------------------- |
| Python     | `requirements.txt` / `pyproject.toml`              |
| JavaScript | `package.json` + `package-lock.json` / `yarn.lock` |
| Java       | `pom.xml` (Maven) / `build.gradle` (Gradle)        |
| Ruby       | `Gemfile` + `Gemfile.lock`                         |
| Rust       | `Cargo.toml` + `Cargo.lock`                        |
| Go         | `go.mod` + `go.sum`                                |
| C#         | `.csproj` / `packages.config`                      |
| Perl       | `cpanfile`                                         |

✅ **Isolate 分离依赖**
不同项目隔离彼此的依赖，避免冲突。通过包仓库安装的库可以在系统范围内安装，也可以限定到包含应用的目录。
典型方法：
- Python `virtualenv`
- Node.js 项目独立 `node_modules`
- Java Maven/Gradle 隔离 JAR 包
- Docker 容器隔离运行环境
- Kubernetes Pod 隔离服务依赖

总结：十二因素应用不应依赖系统已有的东西，而应声明所有依赖，隔离运行环境，保证开发与生产环境一致。

### 3. Config
> 将配置存储在环境中，实现**配置与代码严格分离**

应用程序的配置（config）是部署（暂存、生产、开发人员环境等）之间可能有所不同的所有内容。包括：
- 数据库、Memcached 和其他后备服务的资源句柄（Resource handles）
- 外部服务（如 Amazon S3 或 Twitter）的凭证
- 每个部署的值，如部署的规范主机名

12-Factor 将配置存储在环境变量中，通常采用 `.env` （开发用）+ CI/CD 注入（生产用）：
示例：
- `.env`（本地开发，不会提交）
```ini
DB_HOST=localhost
API_KEY=dev123
```
- 生产部署（CI/CD注入）
```bash
export DB_HOST=prod-db
export API_KEY=prod-secret
```

### 4. Backing services
> 将 Backing services 视为附加资源

Backing services（支撑服务）是应用在正常运行中通过网络使用的任何外部服务。可以是本地管理的传统服务，也可以是第三方云服务或 API。
示例：

| **类型**                      | **服务示例**                            | **功能**    |
| --------------------------- | ----------------------------------- | --------- |
| 数据存储（datastore）             | MySQL, CouchDB, PostgreSQL, MongoDB | 保存应用数据    |
| 消息/队列系统（Messaging/Queueing） | RabbitMQ, Beanstalkd, Kafka         | 异步任务、事件传递 |
| 邮件服务（SMTP）                  | Postfix, SendGrid, Amazon SES       | 发送邮件      |
| 缓存系统（Caching）               | Memcached, Redis                    | 提高数据访问速度  |
12-Factor 应用的代码不应区分服务来源，部署时可以把本地 MySQL 换成第三方数据库（如 Amazon RDS），而不改代码。
![[附加资源.png]]
在 12-Factor 应用中，每个独立的支撑服务都是一个资源，应用把它们当作附加资源访问，从而实现松耦合和可替换性。

### 5. Build, release, run
> 严格分离构建（build）和运行（run）阶段

12-Factor 应用的部署流程：
1. Build Stage（构建阶段）：将代码存储库转换为可执行的捆绑包。构建阶段使用部署过程中指定的提交版本（commit），获取供应商依赖项并编译二进制文件和资产。
2. Release Stage（发布阶段）：发布阶段采用构建阶段生成的构建，并将其与部署的当前配置相结合。生成的 release 包含构建和配置，并准备好在执行环境中立即执行。
3. Run Stage（运行阶段，也称 runtime）：通过针对所选版本启动某些应用进程，在执行环境中运行应用。

![[构建-发布-运行.png]]
12-Factor 应用在构建、发布和运行阶段之间使用严格的分离。
部署工具通过管理 release 目录和符号链接，让应用每次部署都是看可逆的，一旦新版本出现问题，可以快速回滚而不改代码。每个版本都应始终具有唯一的发布 ID ，如发布的时间戳或递增编号（`v100`）。
示例：以 `Capistrano` 为例
```bash
/app
    /releases
        20251101-1234/
        20251101-2345/
    current -> /app/releases/20251101-2345
```
其中 `releases/` 存放每次构建的 `release`，而 `current` 是执行当前正在运行的 `release` 的符号链接，这样，切换 `release` 只需修改符号链接指向新的 `release` 。

### 6. Processes
> 将应用作为一个或多个无状态进程执行

12-Factor 应用进程是无状态的、share-nothing。任何需要持久化的数据都必须存储在有状态的支撑服务中（通常是数据库/缓存）。
- 无状态：进程本身不保存用户数据或业务状态，每次请求处理都不依赖进程内存中存储的数据。
- Share-nothing：进程之间不共享内存或本地磁盘数据，每个进程独立运行，数据共享需要通过外部服务（如数据库、缓存系统）完成。
进程的内存或本地文件系统可以用作短期缓存，通常用于单次事务。因为即便只有一个进程，（代码部署、配置变更或执行环境把进程迁移到不同的机器或容器）重启也会清空本地状态。

一些 Web 系统依赖于 `sticky sessions`——即将用户会话数据缓存在应用程序进程的内存中，并期望来自同一访问者的未来请求被路由到同一进程。粘性会话违反了 12-Factor 应用原则，不应该使用。相反，正确的做法是把会话状态存储在支持过期的外部服务中（如 Memcached，Redis）。

### 7. Port binding
> 通过端口绑定导出服务

传统方式：Web 应用常依赖 Web 服务器容器执行，如 PHP 程序作为 Apache 模块运行，而 Java 程序部署到 Tomcat 或 Jetty 容器。这种方式应用和 Web 服务器过于耦合，部署环境依赖服务器配置。

12-Factor 应用不依赖外部服务器注入，而是自包含 HTTP 服务，通过端口监听请求，本地和生产环境一致，生产环境由路由层分发请求。
注意：
应用通过依赖声明（dependency declaration）引入 Web 服务器库，这完全发生在用户空间即应用代码中：
- Python （Tornado, Flask, FastAPI）
- Ruby（Thin, Puma）
- Java/JVM （Jetty, Netty）
此外，端口绑定不仅限于 HTTP ，任何服务都可以通过端口提供：
- `ejabberd` => XMPP 协议
- Redis => Redis 协议
- gRPC, WebSocket 等服务
端口绑定也使得应用也能作为支撑服务。即将应用自身作为其他应用的资源使用。

### 8. Concurrency
>通过进程模型横向扩展

12-Factor 应用的进程模型：进程是应用的核心单位，每种类型的工作由不同进程类型处理（如 Web 请求由 web 进程，后台任务由 worker 进程）。
此外，12-Factor 应用允许单进程内部多路复用（垂直扩展：如，进程内创建多个线程处理并发任务；异步/事件驱动通过事件循环处理大量请求）。但是单进程或单虚拟机有资源上限，因此应用应支持水平扩展，即可跨多进程、多机器扩展。

进程管理原则：
- 禁止守护进程化
- 禁止自行写 PID 文件
- 依赖操作系统或平台管理进程（systemd, upstart等）
### 9. Disposability
> 通过快速启动和正常关机最大限度地提高稳健性（robustness）

12-Factor App 进程要能快速启动、随时被杀、优雅退出、崩溃自动恢复，并确保任务可重试，一切为了弹性、快速部署和高可用。

### 10. Dev/prod parity
> 保持开发、暂存和生产阶段尽可能相似

开发人员有时会发现在本地环境中使用轻量级后备服务非常有吸引力，而在生产环境中倾向于使用更严肃、更强大的后备服务。例如，在本地使用 SQLite，在生产中使用 PostgreSQL；或用于开发中缓存和生产中 Memcached 的本地进程内存。
12-Factor 应用旨在通过保持开发和生产之间的较小差距来实现持续部署。

|                      | 传统应用      | 12-Factor 应用           |
| -------------------- | --------- | ---------------------- |
| 部署之间的时间              | Weeks     | Hours                  |
| 代码作者 vs 代码部署者        | Different | Same                   |
| Dev vs Production 环境 | Divergent | As similar as possible |

后端服务（数据库、消息队列、缓存等）是开发环境和生产环境必须保持一致的重要部分。
许多语言提供库来统一访问后端服务，并支持切换不同服务类型。如

| 语言      | 统一访问库        |
| ------- | ------------ |
| Python  | SQLAlchemy   |
| Ruby    | ActiveRecord |
| Java    | JDBC         |
| Node.js | ORM/clients  |
| Go      | database/sql |
推荐做法：
- 本地 `docker-compose` 跑 PostgreSQL / Redis / RabbitMQ
- 配置环境变量：`DATABASE_URL`, `REDIS_URL`
- 框架 ORM/driver 统一访问接口
- CI/CD 自动创建测试数据库环境

### 11. Logs
> 将日志视为事件流

日志（logs）提供对正在运行的应用行为的可见性。在基于服务器的环境中，它们通常写入磁盘上的文件（“logfile”）；但这只是一种输出格式。
日志是从所有正在运行的进程和支持服务的输出流中收集的按时间顺序排列的聚合事件流 。原始形式的日志通常是一种文本格式，每行一个事件（尽管异常的回溯可能跨越多行）。日志没有固定的开始或结束，但只要应用程序运行，日志就会持续流动。

12-Factor 应用不关心其输出流的路由或存储。不应尝试写入或管理日志文件。相反，每个正在运行的进程都会将其事件流（无缓冲的）写入 `stdout`。在本地开发期间，开发人员可通过在终端查看此流，以查看应用的行为。
在暂存或生产部署中，每个进程的流将被执行环境捕获，与应用程序中的所有其他流一起整理，并路由到一个或多个最终目的地以供查看和长期存档。这些存档目标对应用程序不可见或不可配置，而是完全由执行环境管理。开源日志路由器（如 Logplex 和 Fluentd）即用于此目的。这些系统为监察应用程序随时间变化的行为提供了强大的功能和灵活性。

日志的进一步处理：
日志可以路由到文件，也可以通过终端中的实时尾部进行监视。最重要的是，流可以发送到日志索引和分析系统（如 Splunk）或通用数据仓库系统（如 Hadoop/Hive）。
### 12. Admin processes
> 将管理/管理任务作为一次性流程运行

12-Factor 应用推荐把一次性管理任务作为与正常服务一样的进程，使用同样的代码、同样依赖、同样环境，只是运行一次并退出。