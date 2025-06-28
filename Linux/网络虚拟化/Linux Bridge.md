物理网络中，交换机用于连接多台主机，组成局域网。而在 Linux 网络虚拟化技术中，提供了物理交换机的虚拟实现，即 Linux Bridge 。
Linux Bridge （Linux 网桥，虚拟交换机）用以将多个网络接口（如物理网卡 eth0、虚拟接口 veth 、tap 等）桥接为一个统一的网络。网桥根据“以太网地址”（MAC地址）来转发数据帧，而不是像路由器那样根据 IP 地址来转发。当数据帧进入 Linux Bridge 时，系统根据数据帧的类型和目的 MAC 地址执行以下处理：
- 广播帧：转发到所有桥接到该 Linux Bridge 的设备
- 单播帧：查找 FDB（Forwarding Database，地址转发表）中 MAC 地址与设备网络接口的映射记录：
	- 若未找到记录，执行“洪泛”（Flooding），将数据帧发送到所有接口，并根据响应将设备的网络接口与 MAC 地址记录到 FDB 表中；
	- 若找到记录，则直接将数据帧转发到对应设备的网络接口。
>[!NOTE]
>桥接（Bridging）工作在第二层（数据链路层），与上层通信协议无关；路由工作在第三层（网络层）。
>

| 特性       | 桥接（Bridge）       | 路由（Router）         |
| -------- | ---------------- | ------------------ |
| 工作层级     | 第二层（数据链路层，MAC地址） | 第三层（网络层，IP地址）      |
| 是否属于同一子网 | 是                | 否                  |
| IP地址要求   | 可以在同一个网段         | 必须在不同网段            |
| 隔离性      | 无隔离，完全打通         | 有隔离，可以控制通信         |
| 应用场景     | 扩展同一局域网、无线桥接     | 不同网络间互联、设置防火墙/访问策略 |

#### 操作
Linux 中，使用 `brctl` 或 `ip` 完成网桥管理：
- `brctl`（传统工具，来自 `bridge-utils` 包）
- `ip` 命令（来自`iproute2`包）
##### 创建网桥接口 `br0`
```bash
sudo ip link add br0 type bridge
# sudo brctl addbr br0  # 创建网桥
```
##### 将接口加入网桥
```bash
sudo ip link set eth0 master br0
sudo ip link set eth1 master br0
# sudo brctl addif br0 eth0
# sudo brctl addif br0 eth1
```
##### 启动接口
```bash
sudo ip link set dev br0 up
sudo ip link set dev eth0 up
sudo ip link set dev eth1 up
```
##### 给网桥分配 IP 地址（可选）
```bash
sudo ip addr add 192.168.1.100/24 dev br0
```
##### 查看网桥状态
```bash
ip link show type bridge
bridge link     # 查看网桥关联的端口
bridge vlan     # 查看 VLAN 配置
```
##### 删除网桥（清理）
```bash
sudo ip link set eth0 nomaster      # 移除接口
sudo ip link set eth1 nomasetr
sudo ip link set br0 down
sudo ip link del br0 type bridge
```

