Docker 可以通过读取 `Dockerfile` 中的指令自动构建镜像。
##### Dockerfile 支持的指令：

| **指令**        | **描述**                |
| ------------- | --------------------- |
| `ADD`         | 添加本地或远程文件或目录          |
| `ARG`         | 使用构建时变量               |
| `CMD`         | 指定默认命令                |
| `COPY`        | 复制文件和目录               |
| `ENTRYPOINT`  | 指定默认可执行文件             |
| `ENV`         | 设置环境变量                |
| `EXPOSE`      | 描述应用程序正在监听哪些端口        |
| `FROM`        | 从 base 镜像创建新的构建 stage |
| `HEALTHCHECK` | 启动时检查容器的健康状况          |
| `LABEL`       | 对镜像添加元数据              |
| `MAINTAINER`  | 指定镜像的作者               |
| `ONBUILD`     | 在构建时使用该镜像时指定的命令       |
| `RUN`         | 执行构建命令                |
| `SHELL`       | 设置镜像的默认 shell         |
| `STOPSIGNAL`  | 指定退出容器的系统调用信号         |
| `USER`        | 设置用户和组 ID             |
| `VOLUME`      | 创建卷挂载                 |
| `WORKDIR`     | 更改工作目录                |

##### Dockerfile 格式：
```Dockerfile
# Comment line，在执行 Dockerfile 指令之前，注释行会被删除
INSTRUCTION arguments
```
`INSTRUCTION` 不区分大小写；但为了更容易与 `arguments` 区分开来，通常采用大写方式。
Docker 按顺序执行 Dockerfile 中的指令。Dockerfile 必须以 `FROM` 指令开头，该指令可以在解析器指令（parser directives）、注释和全局作用域的 `ARGs`之后声明。
- `FROM` 指令指定了要构建的 base image 。
- `FROM` 指令前只能有一个或多个 `ARG` 指令，这些指令声明了 Dockerfile 中 `FROM` 行中使用的参数。

##### 解析器指令（parser directives）
解析器指令以 `# directive=value` 形式编写，是一种特殊的注释。解析器指令可选，将影响 Dockerfile 中后续行的处理方式，单个指令只能使用一次。解析器指令不会为构建添加层，也不会显示为构建步骤。
支持以下解析器指令：
- `syntax`—— 声明用于构建的 Dockerfile 语法版本；未指定的话，使用 Dockerfile 前端的捆绑版本。
```
# syntax=docker/dockerfile:1
```
`docker/dockerfile:1` 导致 BuildKit 在构建之前提取 Dockerfile 语法的最新稳定版本。
- `escape` —— 设置 Dockerfile 中用于转义字符的字符。未指定，则默认转义字符为 `\` 。
```
# escape=\
或者 # escape=`
```
- `check`（自 Dockerfile v1.8.0 起）—— 用于配置如何评估 build checks 。默认情况下，所有检查都会运行，失败将被视为警告。可以使用 `#check=skip=<check-name>` 禁用特定检查。要指定多个跳过，使用逗号进行分隔。
```
# check=skip=JSONArgsRecommended,StageNameCasing
或 # check=skip=all 禁用所有检查
```
默认情况下，即使构建过程中出现了检查警告（build checks），构建依然会成功（退出码为 0）。如果希望这些警告导致构建失败（退出码非 0），设置`#check=error=true`
组合 `skip` 和 `error` 选项，使用分号分隔它们：
```
# check=skip=JSONArgsRecommended;error=true
```

##### 环境替换
通过 `ENV` 指令声明的环境变量，可以在某些 Dockerfile 指令中作为变量使用，由 Dockerfile 在构建时解析替换。
示例：
```dockerfile
ENV APP_HOME=/usr/src/app

WORKDIR $APP_HOME
```
环境变量在 Dockerfile 中以 `$variable_name` 或 `${variable_name}` 标记。`${variable_name}` 语法还支持一些标准 `bash` 修饰符如下所述：
- `${variable:-word}` 表示如果设置了 `variable` ，则结果为该值。如果未设置 `variable`，则结果为 `word`。
- `${variable:+word}` 表示如果设置了 `variable` ，则结果为 `word`；否则结果为空字符串。

示例：
```dockerfile
FROM busybox
ENV FOO=/bar
WORKDIR ${FOO}      # WORKDIR /bar
ADD . $FOO          # ADD . /bar
COPY \$FOO /quux    # COPY $FOO /quux
```
- `FROM busybox` —— 使用 `busybox` 作为基础镜像
- `ENV FOO=/bar` —— 定义环境变量 `FOO` ，值为 `/bar` 
- `WORKDIR ${FOO}` —— 使用环境变量 `FOO` 的值 `/bar` 作为工作目录，如果目录不存在，会在镜像中创建它
- `ADD . $FOO` —— 将当前上下文（`.`）的内容复制到容器内的 `/bar` 目录
- `COPY \$FOO /quux` —— `\$FOO` 中的反斜杠 `\` 是一个转义符，防止 Dockerfile 解析 `$FOO` 为变量

##### `.dockerignore` 文件
可以使用 `.dockerignore` 文件来**排除**某些文件和目录，使它们**不会被包含在 Docker 构建上下文中**。
>[!NOTE]
>构建上下文：当运行 `docker build .` 时，其中的 `.` 表示“当前目录”是构建上下文。Docker 会将该目录（所有文件和子目录）打包发送给 Docker 守护进程，用来执行 Dockerfile 中的构建指令。


##### Shell 和 exec 形式
`RUN`、`CMD` 和 `ENTRYPOINT` 指令都有两种可能的形式：
- `INSTRUCTION ["executable", "param1", "param2"` —— 执行形式
```dockerfile
ENTRYPOINT ["/bin/bash", "-c", "echo hello"]
```
执行形式不会自动调用命令 Shell ，意味着 shell 处理（如变量替换）不会发生。
如果需要 shell 处理，可直接使用 exec 形式执行 shell ：
```dockerfile
RUN ["sh", "-c", "echo $HOME"]
```
- `INSTRUCTION command param1 param2` —— shell 形式
```dockerfile
 RUN source $HOME/.bashrc && \
 echo $HOME
```
可以使用 `SHELL` 命令更改默认 shell 。如：
```dockerfile
SHELL ["/bin/bash", "-c"]
RUN echo hello
```

##### `FROM` 指令
`FROM` 指令初始化一个新的构建阶段并设置后续指令的基础镜像。
语法：
```dockerfile
FROM [--platform=<platform>] <image> [AS <name>]
或 FROM [--platform=<platform>] <image>[:<tag>] [AS <name>]
或 FROM [--platform=<platform>] <image>[@<digest>] [AS <name>]
```
在一个 Dockerfile 中， `FROM` 指令可以出现多次，以：
- 用来构建多个镜像
- 或用于多阶段构建：一个构建阶段可以用另一个阶段的结果作为依赖

`[AS <name>]` 是给构建阶段的命名（可选），该名称可在后续的 `FROM <name>`、`COPY --from=<name>` 和 `RUN --mount=type=bind,from=<name>` 指令中来引用此阶段构建的镜像。

`tag` 或 `digest` 是可选的。省略将默认使用  `latest` 标签。

如果 `FROM` 引用了多平台镜像，则可以使用可选的 `--platform` 标志来指定镜像的平台。如 `linux/amd64`，`linux/arm64`，`windows/amd64` 。默认情况下使用构建请求的目标平台。

>[!NOTE]
>`FROM` 前的 `ARG` —— 全局 `ARG` ，只能用于第一个 `FROM`
>`FROM` 后的 `ARG` —— 当前构建阶段的本地变量，可用于 `RUN` ，`ENV` 等指令
>要让变量能在 `RUN` 使用，需要在 `FROM` 后重新写一次 `ARG`
>```
>ARG BASE_IMAGE=alpine
>FROM ${BASE_IMAGE}
>ARG BASE_IMAGE
># 可在后续 RUN、ENV、LABEL 等指令中继续用 $BASE_IMAGE
>```

除了选择现有镜像为基础镜像外，Docker 还存在一个特殊的镜像，名为 `scratch` 。该镜像是虚拟的概念，实际并不存在，表示一个空白的镜像。
```dockerfile
FROM scratch
...
```

##### `RUN` 指令

`RUN` 指令将在当前镜像之上创建一个新层以执行命令。新层将在 Dockerfile 中的下一步中使用（累积）。形式：
- Shell 形式（默认）
```dockerfile
RUN [options] <command> ...
```
如：`RUN echo "Hello World"` 等价于 `RUN /bin/sh -c "echo Hello World`

- Exec 形式
```dockerfile
RUN [options] ["<command>", ...]
```
该形式使用 JSON 数组语法，没有 shell ，直接调用命令和参数，更安全，不易受到 shell 注入影响。

好的 `Dockerfile` 编写实践：
- 使用单个 `RUN` 完成一个目的，而不是拆分为多个 `RUN`
```dockerfile
FROM debian:stretch

RUN set -x; buildDeps='gcc libc6-dev make wget' \
    && apt-get update \
    && apt-get install -y $buildDeps \
    && wget -O redis.tar.gz "http://download.redis.io/releases/redis-5.0.3.tar.gz" \
    && mkdir -p /usr/src/redis \
    && tar -xzf redis.tar.gz -C /usr/src/redis --strip-components=1 \
    && make -C /usr/src/redis \
    && make -C /usr/src/redis install \
    && rm -rf /var/lib/apt/lists/* \
    && rm redis.tar.gz \
    && rm -r /usr/src/redis \
    && apt-get purge -y --auto-remove $buildDeps
```

注意：Docker 有构建缓存用以加速构建过程，避免重复执行相同指令；RUN 指令默认使用缓存（如果命令没变，不会重新执行）。如 `RUN apt-get dist-upgrade -y` 在下次构建时默认使用缓存结果，而不会再次运行升级。
解决方案：
- RUN 前的 ADD/COPY 会导致缓存失效而重新执行
- 构建镜像加选项 `--no-cache`
```bash
docker build --no-cache .
```
- 小技巧：强制失效某些命令
```dockerfile
RUN apt-get update && apt-get install -y curl && echo "Force run at $(date)"
# 每次构建时 $(date) 变化，Docker 识别为新命令，从而不使用缓存
```

可用 `[options]`：

| Option       | 最低 Dockerfile 版本 |
| ------------ | ---------------- |
| `--device`   | 1.14-labs        |
| `--mount`    | 1.2              |
| `--network`  | 1.3              |
| `--security` | 1.1.2-labs       |
###### `RUN --device`（BuildKit 特性）
尚无稳定语法，使用 `# syntax=docker/dockerfile:1-labs` 以及 `BuildKit 0.20.0` 及以后版本。
```dockerfile
RUN --device=name,[required]
```
让构建步骤访问特定的 CDI 设备（Container Device Interface devices），使得构建阶段不仅仅是文件复制与软件安装，还可以直接利用底层硬件资源，如 GPU、加速器等。
>[!NOTE]
>CDI （Container Device Interface）是一种规范，定义了如何将设备描述为 JSON 文件，使得容器运行时（如 Docker，Kubernetes，containerd）可以使用这些设备。

示例：CUDA-Powered LLaMA 推理（使用 `--device` 标志通过 CDI 使用 NVIDIA GPU 设备运行 `llama.cpp` 推理）
```dockerfile
# syntax=docker/dockerfile:1-labs

# syntax=docker/dockerfile:1-labs

FROM scratch AS model
ADD https://huggingface.co/bartowski/Llama-3.2-1B-Instruct-GGUF/resolve/main/Llama-3.2-1B-Instruct-Q4_K_M.gguf /model.gguf

FROM scratch AS prompt
COPY <<EOF prompt.txt
Q: Generate  a list of 10 unique biggest countries by population in JSON with their estimated poulation in 1900 and 2024. Answer only newline formatted JSON with keys "country", "population_1900", "population_2024" with 10 items.
A:
[
    {

EOF

FROM ghcr.io/ggml-org/llama.cpp:full-cuda-b5124
RUN --device=nvidia.com/gpu=all \
    --mount=from=model,target=/models \
    --mount=from=prompt,target=/tmp \
    ./llama-cli -m /models/model.gguf -no-cnv -ngl 99 -f /tmp/prompt.txt
```

###### `RUN --mount`
`RUN --mount` 是 Docker BuildKit 的一个强大功能，用于在 `RUN` 指令中挂载临时卷、缓存、秘密文件、绑定目录等资源，仅在该构建步骤中生效。
格式：
```dockerfile
RUN --mount=[type=TYPE][,option=<value>[,option=<value>]...]
```
挂载类型：

| Type        | Description                        |
| ----------- | ---------------------------------- |
| `bind` （默认） | Bind-mount context directories（只读） |
| `cache`     | 挂载一个临时目录来缓存编译器和包管理器的目录             |
| `tmpfs`     | 在构建容器中挂载 `tmpfs`                   |
| `secret`    | 允许构建容器访问安全文件（如私钥），而无需将其构建到镜像或构建缓存中 |
| `ssh`       | 用于 SSH 代理转发，通常用于 Git 拉取            |

- `RUN --mount=type=bind` 允许将文件或目录绑定到构建容器。

| Option                         | Description                                                                                    |
| ------------------------------ | ---------------------------------------------------------------------------------------------- |
| `target`, `dst`, `destination` | Mount path.                                                                                    |
| `source`                       | Source path in the `from`. Defaults to the root of the `from`.                                 |
| `from`                         | Build stage, context, or image name for the root of the source. Defaults to the build context. |
| `rw`,`readwrite`               | Allow writes on the mount. Written data will be discarded.                                     |
示例：
```dockerfile
# 本地构建时绑定一个文件夹
RUN --mount=type=bind,target=/src,source=./mycode ls /src
```

- `RUN --mount=type=cache` 允许构建容器为编译器和包管理器缓存目录。

| Option                         | Description                                                                                                                                                                                                                                                                |
| ------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `id`                           | Optional ID to identify separate/different caches. Defaults to value of `target`.                                                                                                                                                                                          |
| `target`, `dst`, `destination` | Mount path.                                                                                                                                                                                                                                                                |
| `ro`,`readonly`                | Read-only if set.                                                                                                                                                                                                                                                          |
| `sharing`                      | One of `shared`, `private`, or `locked`. Defaults to `shared`. A `shared` cache mount can be used concurrently by multiple writers. `private` creates a new mount if there are multiple writers. `locked` pauses the second writer until the first one releases the mount. |
| `from`                         | Build stage, context, or image name to use as a base of the cache mount. Defaults to empty directory.                                                                                                                                                                      |
| `source`                       | Subpath in the `from` to mount. Defaults to the root of the `from`.                                                                                                                                                                                                        |
| `mode`                         | File mode for new cache directory in octal. Default `0755`.                                                                                                                                                                                                                |
| `uid`                          | User ID for new cache directory. Default `0`.                                                                                                                                                                                                                              |
| `gid`                          | Group ID for new cache directory. Default `0`.                                                                                                                                                                                                                             |
示例：
```dockerfile
# syntax=docker/dockerfile:1
FROM ubuntu
RUN rm -f /etc/apt/apt.conf.d/docker-clean; echo 'Binary::apt::APT::Keep-Downloaded-Packages "true";' > /etc/apt/apt.conf.d/keep-cache
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
  --mount=type=cache,target=/var/lib/apt,sharing=locked \
  apt update && apt-get --no-install-recommends install -y gcc
```
Apt 需要独占访问其数据，因此缓存使用选项 `sharing=locked` ，这将确保使用同一缓存挂载（cache mount）的多个并行构建将相互等待，并且不会同时访问相同的缓存文件。如果您希望在这种情况下每个构建都创建另一个缓存目录，也可以使用 `sharing=private` 。

- `RUN --mount=type=tmpfs` 允许在构建容器中挂载 `tmpfs` 

| Option                     | Description  |
| -------------------------- | ------------ |
| `target, dst, destination` | 挂载路径         |
| `size`                     | 指定该文件系统大小的上限 |

- `RUN --mount=type=secret` 此安装类型允许构建容器访问秘密值，例如令牌或私钥，而无需将它们烘焙到镜像中。默认情况下，secret 会被挂载为文件。也可以通过设置 `env` 选项将 `secret` 挂载为环境变量

| Option                         | Description                                                                                                     |
| ------------------------------ | --------------------------------------------------------------------------------------------------------------- |
| `id`                           | ID of the secret. Defaults to basename of the target path.                                                      |
| `target`, `dst`, `destination` | Mount the secret to the specified path. Defaults to `/run/secrets/` + `id` if unset and if `env` is also unset. |
| `env`                          | Mount the secret to an environment variable instead of a file, or both. (since Dockerfile v1.10.0)              |
| `required`                     | If set to `true`, the instruction errors out when the secret is unavailable. Defaults to `false`.               |
| `mode`                         | File mode for secret file in octal. Default `0400`.                                                             |
| `uid`                          | User ID for secret file. Default `0`.                                                                           |
| `gid`                          | Group ID for secret file. Default `0`.                                                                          |

- `RUN --mount=type=ssh` 此挂载类型允许构建容器通过 SSH 代理访问 SSH 密钥，并支持密码。

| Option                         | Description                                                                                    |
| ------------------------------ | ---------------------------------------------------------------------------------------------- |
| `id`                           | ID of SSH agent socket or key. Defaults to "default".                                          |
| `target`, `dst`, `destination` | SSH agent socket path. Defaults to `/run/buildkit/ssh_agent.${N}`.                             |
| `required`                     | If set to `true`, the instruction errors out when the key is unavailable. Defaults to `false`. |
| `mode`                         | File mode for socket in octal. Default `0600`.                                                 |
| `uid`                          | User ID for socket. Default `0`.                                                               |
| `gid`                          | Group ID for socket. Default `0`.                                                              |
示例：访问 GitLab
```dockerfile
# syntax=docker/dockerfile:1
FROM alpine
RUN apk add --no-cache openssh-client
RUN mkdir -p -m 0700 ~/.ssh && ssh-keyscan gitlab.com >> ~/.ssh/known_hosts
RUN --mount=type=ssh \
  ssh -q -T git@gitlab.com 2>&1 | tee /hello
# "Welcome to GitLab, @GITLAB_USERNAME_ASSOCIATED_WITH_SSHKEY" should be printed here
# with the type of build progress is defined as `plain`.
```

```bash
$ eval $(ssh-agent)
$ ssh-add ~/.ssh/id_rsa
(Input your passphrase here)
$ docker buildx build --ssh default=$SSH_AUTH_SOCK .
```

- `RUN --network=TYPE` 允许控制在哪个网络环境中运行命令

| Type          | Description |
| ------------- | ----------- |
| `default`（默认） | 在默认网络中运行    |
| `none`        | 无网络运行       |
| `host`        | 在宿主机网络环境中运行 |

##### `CMD`
`CMD` 指令设置从镜像运行容器时要执行的命令。
- Shell 形式
```dockerfile
CMD command param1 param2
```
- Exec 形式
```dockerfile
CMD ["executable", "param1", "param2"]
```
Dockerfile 中只能有一个 `CMD` 指令。如果列出多个 `CMD` ，只有最后一个指令生效。如果在 `docker run` 后显式提供了命令，`CMD` 就会被覆盖。
`CMD` 的目的是为正在执行的容器提供默认值。这些默认值可以包含可执行文件，也可以省略可执行文件，不过在这种情况下，还必须指定 `ENTRYPOINT` 指令。
`CMD ["param1", "param2"]` 作为 `ENTRYPOINT` 的默认参数。
示例：
```dockerfile
ENTRYPOINT ["python", "app.py"]
CMD ["--port=8000"]
# 最终执行的是 python app.py --port=8000
```

##### `LABEL`
`LABEL` 指令用于向镜像添加元数据。一个 `LABEL` 是一个键值对。
```dockerfile
LABEL <key>=<value> [<key>=<value>...]
```

示例：
```dockerfile
LABEL "com.example.vendor"="ACME Incorporated"
LABEL com.example.label-with-value="foo"
LABEL version="1.0"
LABEL description="This text illustrates \
that label-values can span multiple lines."
```
基础镜像（ `FROM` 行中的镜像）中包含的标签会被您的镜像继承。如果标签已存在但值不同，则最近应用的值将覆盖任何先前设置的值。

##### `EXPOSE`
`EXPOSE` 用于声明容器运行时会监听的端口。作用主要是文档化和协作提示，并不会真正打开端口或做端口映射。
语法：
```dockerfile
EXPOSE <port> [<port>/<protocol>...]
```
- 默认协议是 TCP
- 也可以指定 UDP，如 `EXPOSE 53/udp`
>[!NOTE]
>`EXPOSE` 不做端口映射；即不会把容器端口暴露给主机，必须手动或自动执行端口映射。
>```dockerfile
>EXPOSE 8080
>```
>运行时仍需手动映射
>```bash
>docker run -p 8080:8080 my-image
>```

##### `ENV`
`ENV` 指令用于定义环境变量，这些变量在构建时和运行时都可用，可用于简化配置、控制行为和提高可维护性。
语法：
```dockerfile
ENV <key>=<value> ...
```
与 `ARG` 的区别

| 项目 | `ENV` | `ARG` |
|---|---|---|
| 可用于运行时 | 是 | 否 |
| 可在镜像中持久 | 是 | 否 |
| 默认值设置 | 支持 | 支持 |

##### `ADD` 
`ADD` 指令将本地文件/目录或远程 URL 文件添加到镜像中的指令。
语法：
```dockerfile
ADD <源路径>,,,<目标路径>
```
- <源路径>可以是：
	- 构建上下文中的文件或目录
	- `.tar`，`.tar.gz`，`.tar.bz2`，`.tar.xz` 等压缩文件（会自动解压，如果只希望复制压缩文件，请使用 `COPY`）
	- 远程 URL（会下载，下载后的文件权限自动设置为 `600` ，如果不是想要的权限，需要增加额外一层 `RUN` 进行权限调整）
- <目标路径> 是容器镜像中的目标位置
```dockerfile
ADD hello.txt /app/hello.txt
ADD my-archive.tar.gz /app/       # 自动解压
ADD https://example.com/file.txt /app/file.txt  # 下载远程文件
```
##### `COPY`
`COPY` 有两种形式。路径包含空格时必须使用后一种形式。
```dockerfile
COPY [OPTIONS] <src> ... <dest>
COPY [OPTIONS] ["<src>", ... "<dest>"]
```
可用的 `OPTIONS` 有：

| Option      | Minimum Dockerfile version |
| ----------- | -------------------------- |
| `--from`    |                            |
| `--chown`   |                            |
| `--chmod`   | 1.2                        |
| `--link`    | 1.4                        |
| `--parents` | 1.7-labs                   |
| `--exclude` | 1.7-labs                   |
如果源路径为文件夹，复制的时候不是直接复制该文件夹，而是将文件夹中的内容复制到目标路径。

##### `ENTRYPOINT`
`ENTRYPOINT` 允许配置将作为可执行文件运行的容器。
形式：
- `Exec` 形式
```dockerfile
ENTRYPOINT ["executable", "param1", "param2"]
```
- `Shell` 形式
```dockerfile
ENTRYPOINT command param1 param2
```

`ENTRYPOINT` 的作用
- 定义容器启动时**必须执行的命令**；
- 即使用户通过 `docker run` 提供了其他命令参数，`ENTRYPOINT` 也会被执行；
- 可以配合 `CMD` 提供默认参数；
- 常用于创建“固定行为”的容器，例如启动数据库、运行脚本等
示例：
```dockerfile
FROM python:3.11
COPY app.py /app/
WORKDIR /app
ENTRYPOINT ["python", "app.py"]
```
运行容器时：
```bash
docker run my-python-app       # ➜ 执行 python app.py
docker run my-python-app foo   # ➜ 执行 python app.py foo
```

###### `CMD` 和 `ENTRYPOINT` 如何交互？
`CMD` 和 `ENTRYPOINT` 指令都定义了容器运行时执行的命令。下面是它们之间的配合规则：
1. Dockerfile 应该至少指定一个 `CMD` 或 `ENTRYPOINT` 命令。
2. 当使用容器作为可执行文件时应该定义 `ENTRYPOINT`。
3. `CMD` 应该用作定义 `ENTRYPOINT` 命令的默认参数或在容器中执行临时命令的一种方式。
4. 当使用替代参数运行容器时，`CMD` 将被覆盖。

##### `VOLUME`
Dockerfile 中，`VOLUME` 指令用于声明容器中的某个目录为挂载点，即数据卷。
- 用于持久化数据
- 与主机目录或其他容器共享数据
- 防止数据随容器删除而丢失
语法：
```dockerfile
VOLUME ["/path/in/container"]
```
可以多个路径
```dockerfile
VOLUME ["/data", "/logs"]
```

##### `USER`
`USER` 指令用于**指定后续命令执行时的用户和用户组身份**，包括容器启动后的默认用户。
```dockerfile
USER <user>[:<group>]
```
或
```dockerfile
USER UID[:GID]
```

##### `WORKDIR`
`WORKDIR` 指令用于**设置工作目录（当前目录）**，即后续所有相关指令（如 `RUN`、`COPY`、`CMD` 等）执行时所处的默认目录。

语法：
```dockerfile
WORKDIR /path/to/directory
```
- 如果路径不存在，会自动创建
- 可以使用相对路径或绝对路径
- 多个 `WORKDIR` 会逐层叠加
```dockerfile
FROM node:20

WORKDIR /app
COPY package.json .
RUN npm install

COPY . .
CMD ["node", "index.js"]
```
上面示例中，所有操作都发生在 `/app` 目录下，无需手动使用 `cd` 。

###### 多个 `WORKDIR` 示例（路径叠加）
```dockerfile
WORKDIR /app
WORKDIR src
```
效果等同于
```dockerfile
WORKDIR /app/src
```

##### `ARG`

```docerfile
ARG <name>[=<default value>] [<name>[=<default value]...]
```

`ARG` 指令定义了一个变量，用户可以在构建时使用 `docker build` 命令使用 `--build-arg <varname>=<value>` 标志将该变量传递给构建器。
`ARG` 指令可以选择包含默认值——如果 `ARG` 指令具有默认值，并且在构建时没有传递值，则构建器将使用默认值。
```dockerfile
FROM busybox
ARG user1=someuser
ARG buildno=1
```

示例：Dockerfile
```dockerfile
FROM ubuntu
ARG CONT_IMG_VER
ENV CONT_IMG_VER=v1.0.0     # ENV CONT_IMG_VER=${CONT_IMG_VER}
RUN echo $CONT_IMG_VER
```
构建镜像命令
```bash
docker build --build-arg CONT_IMG_VER=v2.0.1 .
```
- `ARG CONT_IMG_VER`：声明了一个构建参数；
- `--build-arg CONT_IMG_VER=v2.0.1`：构建时传入值 `v2.0.1`；
- `ENV CONT_IMG_VER=v1.0.0`：将环境变量 `CONT_IMG_VER` 设置为 `v1.0.0`，**但此处会覆盖上面的 ARG 值**；
- `RUN echo $CONT_IMG_VER`：这个命令会在镜像构建时执行，**使用的是 `ENV` 中的值**。

预定义 `ARG` 变量
- `HTTP_PROXY`
- `http_proxy`
- `HTTPS_PROXY`
- `https_proxy`
- `FTP_PROXY`
- `ftp_proxy`
- `NO_PROXY`
- `no_proxy`
- `ALL_PROXY`
- `all_proxy`

##### `ONBUILD`
`ONBUILD` 指令用于**定义一个“延迟执行”的触发指令**：这个指令不会在当前镜像构建时执行，而会在**以该镜像为基础构建其他镜像时**自动执行。
语法：
```dockerfile
ONBUILD <INSTRUCTION>
```
其中 `<INSTRUCTION>` 可以是合法的 Dockerfile 指令，比如：`COPY`、`RUN`、`ADD`、`CMD` 等（但不能是 `FROM`、`ONBUILD` 本身等）。

##### `STOPSIGNAL`
```dockerfile
STOPSIGNAL signal
```
`STOPSIGNAL` 指令设置将发送给容器以退出的系统调用信号。信号格式 `SIG<NAME>` 或对应数字值，未定义默认为 `SIGTERM` 。
该镜像的默认 stopsignal 可以被容器覆盖，需要 `docker run` 和 `docker create` 启用 `--stop-signal` 标志。

##### `HEALTHCHECK`
两种形式：
-  `HEALTHCHECK [OPTIONS] CMD command` （通过在容器内运行命令来检查容器的健康状况）
- `HEALTHCHECK NONE` （禁用从基础镜像继承的任何健康检查）
`HEALTHCHECK`指令告诉 Docker 如何测试容器是否仍在运行。这可以检测出如 Web 服务器陷入无限循环、无法处理新连接等情况，即使服务器进程仍在运行。
`CMD` 前的选项有：
- `--interval=DURATION` （默认值：`30s`），健康检查每隔几秒运行一次
- `--timeout=DURATION` （默认值：`30s`），单次检查所用时间超过 `timeout` ，则视为失败
- `--start-period=DURATION` （默认值：`0s`），`start period` 为需要时间进行引导的容器提供初始化时间。在此期间检测到失败将不会计入最大重试次数。但，如果在 `start period` 内健康检查成功，则容器被视为已启动，所有以后的失败都将会计入最大重试次数
- `--start-interval=DURATION` （默认值：`5s`），启动期间健康检查的间隔时间
- `--retries=N` （默认值：3），容器重试 `retries` 次健康检查连续失败才能视为 `unhealthy`


##### `SHELL`
```dockerfile
SHELL ["executable", "param1", "param2", ...]
```
`SHELL` 指令用于**指定默认的 shell 程序及其参数**，该 shell 会被用来执行后续的 `RUN` 命令（除非显式指定了 `exec` 格式）。Linux 上的默认 shell 为 `["/bin/sh", "-c"]`；Windows 上的默认 shell 为 `["cmd", "/S", "/C"]` 。`SHELL` 指令必须以 JSON 格式写入 Dockerfile 。

示例：Linux 容器中更换为 Bash
```dockerfile
FROM ubuntu

# 指定使用 bash 替代默认 sh
SHELL ["/bin/bash", "-c"]

RUN echo $BASH_VERSION
RUN for i in {1..3} do echo $i; done
```

示例：Windows 更换为 `powershell`
```dockerfile
FROM mcr.microsoft.com/windows/servercore:ltsc2022

# 使用 PowerShell 作为 shell
SHELL ["powershell", "-Command"]

RUN Write-Output "Hello from PowerShell"
```