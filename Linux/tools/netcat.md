`nc` - 使用 TCP 或 UDP 跨网络连接读取和写入数据。
> 能否写一个使用 RDMA 跨网络连接读取和写入数据？？？

`nc` 几乎可用于任何涉及 TCP、UDP 或 UNIX-domain 套接字的食物。可以打开 TCP 连接、发送 UDP 数据包、侦听任意 TCP 和 UDP 端口、可以进行 端口扫描，文件传输。

#### 用法
```bash
nc [options] [destination][port]
```
#### 常见用例：
- Simple TCP proxies
- Shell-script based HTTP clients and servers
- Network daemon testing
- A SOCKS or HTTP ProxyCommand for ssh

#### 常用选项
`-4` - 仅使用 IPv4 地址
`-6` - 仅使用 IPv6 地址
`-b` - 允许广播
`-I length` - 指定 TCP 接收缓冲区的大小
`-O length` - 指定 TCP 发送缓冲区的大小
`-l` - 侦听传入连接而不是启动与远程主机的连接。要侦听的 `destination` 和 `port` 可以指定为非可选参数，也可以分别使用 `-s` 和 `-p` 指定。
`-u` - 使用 UDP 而非 TCP，不能与 `-x` 一起使用
`-v` - 产生更详细的输出
`-w timeout` - 在 `timeout` 秒后无法建立连接
`-x proxy_address[:port]` - 使用 `proxy_address` 和 `port` 处的代理连接到目标
`-z` - 仅扫描监听守护进程，而不向它们发送任何数据。不能与 `-l` 一起使用
`-n` 

#### 端口扫描
获悉目标计算机上哪些端口是开放的并正在运行服务。可以扫描单个或多个或一系列 open ports.
```bash
nc -v -w 2 -z host 22    # scan a single port
nc -v -w 2 -z host 22 80 # scan multiple ports
nc -v -w 2 -z host 20-25 # scan range of ports
```

#### 客户端/服务器模型
在第一个控制台上，启动 `nc` 侦听特定端口上的连接。
如：
```bash
nc -l 1234
```
`nc` 现在正在侦听端口 `1234` 上的连接。在第二个控制台上，连接到正在侦听的计算机和端口：
```bash
nc -N 127.0.0.1 1234
```

#### 命令行 Chat 服务器
```bash
nc -l -vv -p 5000   # command 1
```
在一台机器上，运行上述命令以创建侦听端口 5000 的聊天服务器
```bash
nc host 5000        # command 2
```
在另一台机器上，运行 Command 2 以启动与运行 server 的计算机的聊天会话

#### 查找在 port 上运行的服务
```bash
nc -v -n host port
```

#### 示例

```bash
nc host port
```
创建连接到 `host`  指定 `port` 的 TCP 连接，之后的标准输入将发送到 `host`，通过连接返回的任何内容都会发送到标准输出。这会无限期持续，直到连接断开。

```bash
nc -p 31337 -w 5 host.example.com 42
```
打开到 `host.example.com` 端口为 42 的 TCP 连接，使用端口 31337 作为源端口，超时时间为 5 s .

```bash
nc -u host.example.com 53
```
打卡与 `host.example.com` 端口为 53 的 UDP 连接。

```bash
nc -s 10.1.2.3 host.example.com 42
```
使用 `10.1.2.3` 作为连接本地端的IP，打开到 `host.example.com` 端口 42 的 TCP 连接。
