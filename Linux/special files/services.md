services 文件的位置由 `<netdb.h>` 中的 `_PATH_SERVICES` 指定，通常设置为 `/etc/services`。
其中的每一行格式为：
```asci
service-name port/protocol  [aliases...]
```
`service-name` - 服务所采用的名称。区分大小写，通常，客户端程序以 `service-name` 命名。
`port` - 是用于此服务的端口号（十进制）。
`protocol` - 要使用的协议值。（典型包括：tcp 和 udp）
`aliases` - 可选名称

services 是一个纯 ASCII 文件，包含 Internet 网络服务列表，提供 网络服务名称 和它们底层分配的端口号和协议类型之间的映射。
每个网络程序都应该查看此文件以获取其服务的端口号（和协议）。C库函数 `getservent`，`getservbyname`，`getservbyport`，`setservent` 和  `endservent` 支持了在应用程序中查询此文件。

端口号由 `IANA`（Internet 编号分配机构）分配，他们当前的策略是在分配端口号时同时分配 TCP 和 UDP 协议。因此，大多数条目将有两个条目，即使对于仅限 TCP 的服务也是如此。