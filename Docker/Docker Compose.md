`Docker Compose` —— 用于定义和运行多容器（multi-container）应用的工具，使用 Compose 文件格式定义。Compose 文件用于定义如何配置组成应用程序的一个或多个容器。拥有了 Compose 文件后，可以使用单个命令创建并启动应用程序：`docker compose up` 。

`Compose` 简化了对整个应用程序堆栈的控制，使您可以在单个 YAML 配置文件中轻松管理服务、网络和卷。然后，使用一个命令，从配置文件中创建并启动所有服务。

`Compose` 可以在所有环境中工作；包括生产环境、预发布（staging）、开发（development）、测试（testing）以及持续集成（CI）工作流。它还提供用于管理应用整个生命周期的命令：

| **生命周期阶段** | 相关命令                        | **说明**        |
| ---------- | --------------------------- | ------------- |
| 构建镜像       | `docker compose build`      | 构建服务使用的镜像     |
| 启动服务       | `docker compose up`         | 创建并运行容器       |
| 停止服务       | `docker compose down`       | 停止并清理容器、网络、卷等 |
| 查看日志       | `docker compose logs`       | 查看服务日志        |
| 执行命令       | `docker compose exec`       | 在运行中的容器内执行命令  |
| 扩容服务       | `docker compose up --scale` | 同一服务运行多个实例    |
| 查看日志       | `docker compose ps`         | 查看日志          |

Docker Compose 使用步骤
1. 使用 `Dockerfile` 定义应用的环境，以便可以在任何地方复制
2. 在 `compose.yaml` 中定义组成应用的服务，以便它们可以在隔离的环境中一起运行
3. 运行 `docker compose up` ，Compose 将启动并运行整个应用程序

#### Compose Specification
Compose Specification 允许定义一个与平台无关的基于容器的应用程序。这样的应用程序被设计为一组容器，它们在足够的共享资源和通信信道下一起运行。

应用程序的计算组件被定义为 `services` 。服务是一个抽象的概念，通过在平台上一次或多次运行相同的容器映像和配置实现。Services 通过网络相互通信。

示例：
考虑将应用程序拆分为前端 Web 应用程序和后端服务。
- 前端在运行时配置有由基础设施管理的 HTTP 配置文件，提供外部域名和由平台的安全秘密存储注入的 HTTPS 服务器证书。
- 后端将数据存储在持久卷中
这两个服务在一个隔离的后台网络上相互通信，而前端也连接到一个前台网络，并暴露 443 供外部使用。
![[服务示例.png]]
示例应用程序由以下部分组成：
- 2 个服务，由 Docker 镜像支持：`webapp` 和 `database`
- 1 个secret（HTTPS 证书），注入前端
- 1 个配置（HTTP），注入前端
- 1 个持久卷，连接到后端
- 2 个网络
```yaml
services:
    frontend:
        image: example/webapp
        ports:
          - "443:8043"
        networks:
          - front-tier
          - back-tier
        configs:
          - httpd-config
        secrets:
          - server-certificate

    backend:
        image: example/database
        volumes:
          - db-data:/etc/data
        networks:
          - back-tier

volumes:
    db-data:
        driver: flocker
        driver_opts:
            size: "10GiB"

configs:
    httpd-config:
        external: true

secrets:
    server-certificate:
        external: true

networks:
    # The presence of these objects is sufficient to define them
    front-tier: {}
    back-tier: {}
```
三种挂载类型
- `volumes`：存储数据（如数据库文件、上传文件等）
- `configs`：存储配置文件（如 app config, Nginx 配置等）
- `secrets`：存储敏感信息（如密码、API 密钥、TLS 证书等）
虽然这三者都**以挂载文件或目录的形式**暴露给容器——即，容器内部看起来像是普通文件或目录——但它们底层的用途和特性是不同的。`volumes`可以读写；`configs`和`secrets`声明为 `external`，不属于容器生命周期的一部分，只能读取，不能写入。

Compose 的核心要素
- services：要运行的每个容器
- volumes：挂载的持久化数据
- networks：服务之间通信的虚拟网络
- configs/secrets：可选，用于敏感信息或配置管理

完整示例（常用字段）
```yaml
services:
    app:
        build: .
        container_name: myapp
        ports:
         - "3000:3000"
        environment:
         - NODE_ENV=production
        volumes:
         - ./src:/app/src
        depends_on:
         - db

    db:
        image: postgres:15
        environment:
          POSTGRES_PASSWORD: example
        volumes:
         - db-data:/var/lib/postgresql/data

volumes:
    db-data:
```
- `servives` —— 每个 service 是一个容器配置

|字段名|作用|
|---|---|
|`image`|使用的镜像|
|`build`|构建镜像（可配 Dockerfile）|
|`container_name`|指定容器名|
|`ports`|主机:容器 端口映射|
|`volumes`|数据卷挂载|
|`environment`|环境变量|
|`depends_on`|服务依赖（不会控制启动顺序细节）|
|`command`|覆盖容器默认命令|
|`restart`|设置重启策略（如 always / unless-stopped）|
- `volumes` —— 定义并持久化数据，如数据库
```yaml
volumes:
    db-data:
        driver: local
```
- `networks`
多个服务在自定义网络中通过名称互联互通
```yaml
networks:
    mynet:
        driver: bridge
```
- `secrets` 和 `cofigs` —— 用于安全地传递敏感信息或配置文件

推荐结构
```bash
project/
|
|--- docker-compose.yml
|--- .env
|--- Dockerfile
|--- src/
|     |--- app.js
```