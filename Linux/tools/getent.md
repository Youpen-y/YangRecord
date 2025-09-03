`getent` - 从系统数据库中获取条目

语法：
```bash
getent [OPTION...] database [key...]
```

`getent` （get entries 的缩写），可以查询 NSS（Name Service Switch）支持的任何数据库。如：

| 数据库名         | 描述/用途                           | 对应文件 / 来源              |
| ------------ | ------------------------------- | ---------------------- |
| `passwd`     | 用户账户信息，包括 UID、主目录、shell 等       | `/etc/passwd`、LDAP 等   |
| `shadow`     | 用户密码哈希、过期策略（仅 root 可用）          | `/etc/shadow`          |
| `group`      | 组信息，含 GID 和组成员列表                | `/etc/group`           |
| `gshadow`    | 组密码和管理信息（通常与 `group` 配套）        | `/etc/gshadow`         |
| `initgroups` | 显示用户所属的所有组（类似 `id`）             | 动态生成（非文件）              |
| `hosts`      | 主机名和 IP 地址映射（支持 DNS 和本地）        | `/etc/hosts`、DNS       |
| `ahosts`     | IPv4/IPv6 主机地址列表，包括 socket 类型信息 | `/etc/hosts`、DNS       |
| `ahostsv4`   | 仅 IPv4 的 `ahosts` 版本            | 同上                     |
| `ahostsv6`   | 仅 IPv6 的 `ahosts` 版本            | 同上                     |
| `networks`   | 网络名与网络地址映射                      | `/etc/networks`        |
| `protocols`  | 协议号与协议名称映射（如 tcp=6, udp=17）     | `/etc/protocols`       |
| `services`   | 服务名与端口/协议映射（如 ssh=22/tcp）       | `/etc/services`        |
| `rpc`        | RPC 程序号与服务名映射（远程过程调用）           | `/etc/rpc`             |
| `aliases`    | 邮件别名信息（邮件服务用，现代系统少用）            | `/etc/aliases`         |
| `ethers`     | 以太网地址（MAC）与主机名映射                | `/etc/ethers`          |
| `netgroup`   | 网络组定义（常用于 NIS/LDAP 的主机/用户访问控制）  | `/etc/netgroup`、LDAP 等 |

使用示例：
1. 查询用户信息（等同于 `cat /etc/passwd | grep username` ）
```bash
$ getent passwd yongy
yongy:x:1000:1000:yongy:/home/yongy:/bin/bash
```

2. 查询所有本地和远程用户
```bash
$ getent passwd
```

3. 查询组信息
```bash
$ getent group sudo
sudo:x:27:yongy
```

4. 查询主机名与 IP 映射（DNS + `/etc/hosts`）
```bash
$ getent hosts google.com
59.24.3.174     google.com
```

5. 查询服务端口
```bash
$ getent services ssh
ssh                   22/tcp
```

注意：查询大多数数据库不需要 sudo 权限（如 `passwd` ， `group` ，`hosts`）；但查询 `shadow` 数据库通常需要 root 权限。

6. 查询用户的密码哈希信息及密码相关策略
```bash
$ sudo getent shadow ${USER}
yongy:$6$Uax********AFLI$8Q//3EY2nZW67TMd2.1.e2vKuJgi6/ust35IXuGjkppRZp1C5HV3oCFOorefz2cwGRqyozx5tZKkuctIgnCe4/:20221:0:99999:7:::
```
密码总共有 9 个字段，分别为：

| 字段位置 | 含义                     | 示例值      |
| ---- | ---------------------- | -------- |
| 1    | 用户名                    | `yongy`  |
| 2    | 密码哈希（或特殊标志）            | `$6$...` |
| 3    | 上次修改密码的日期（从 1970 起的天数） | `20221`  |
| 4    | 最短改密周期（天）              | `0`      |
| 5    | 最长改密周期（天）              | `99999`  |
| 6    | 密码过期前的警告期（天）           | `7`      |
| 7    | 密码过期后的不活跃天数            | 空（表示无限）  |
| 8    | 账号过期时间（从 1970 起的天数）    | 空（未设置）   |
| 9    | 保留（未使用）                | 空        |

子示例：
- 检查用户密码是否已锁定
```bash
$ getent shadow yongy | cut -d: -f2
# 如果是 ！或 *，说明被锁定
```

- 检查密码过期策略
```bash
$ getent shadow yongy | cut -d: -f5
```