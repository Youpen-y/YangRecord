`veth` —— 虚拟网络设备（Virtual Ethernet Device）

![[veth-structure.png]]

---
`veth` 设备是虚拟以太网设备。 总是成对出现，也常被称为 `veth pair` 。简单而言，`veth pair` 是两个虚拟的网卡接口（不是真实网卡，而是内核中模拟的一种网卡对），像一根虚拟网线两端连接的网卡。
创建 `veth pair` 语法：
```bash
ip link add <p1-name> type veth peer name <p2-name>
```
`p1-name` 和 `p2-name` 是两个相连的端点。一个设备上发送的数据包会立即被另一个设备接收。当其中一个设备发生故障时，链路对的链路状态也变为故障。
用途：
- 可以用于连接不同的网络命名空间，实现不同命名空间的通信。
```bash
# 创建命名空间 ns1, ns2 和 ns3
ip netns add ns1
ip netns add ns2

# 将 veth pair 的 vp1 放入命名空间 ns1，vp2 放入命名空间 ns2
ip link add vp1 netns ns1 type veth peer vp2 netns ns2

# 为两个网卡设置 IP 地址，使其位于同一个子网 192.168.1.0/24 下。
ip -n ns1 addr add 192.168.1.1/24 dev vp1
ip -n ns2 addr add 192.168.1.2/24 dev vp2

# 使用 ip link 命令设置两张虚拟网卡状态为 up
ip -n ns1 link set vp1 up
ip -n ns2 link set vp2 up

# 进入不同的网络命名空间并执行命令
# 语法：ip netns exec <ns_name> <COMMAND> 
# 等价于 "ip netns exec <ns_name> bash
# <COMMAND>"
# 在 ns1 命名空间中查看其网络接口配置
ip netns exec ns1 ip addr show
1: lo: <LOOPBACK> mtu 65536 qdisc noop state DOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: vp1@if2: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether 3e:29:5f:7c:52:82 brd ff:ff:ff:ff:ff:ff link-netns ns2
    inet 192.168.1.1/24 scope global vp1
       valid_lft forever preferred_lft forever
    inet6 fe80::3c29:5fff:fe7c:5282/64 scope link proto kernel_ll 
       valid_lft forever preferred_lft forever

# ns2 查看
ip netns exec ns2 ip addr show
1: lo: <LOOPBACK> mtu 65536 qdisc noop state DOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
2: vp2@if2: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
    link/ether 1e:13:5e:bf:90:f7 brd ff:ff:ff:ff:ff:ff link-netns ns1
    inet 192.168.1.2/24 scope global vp2
       valid_lft forever preferred_lft forever
    inet6 fe80::1c13:5eff:febf:90f7/64 scope link proto kernel_ll 
       valid_lft forever preferred_lft forever

# 从 ns1 ping ns2 
ip netns exec ns1 ping 192.168.1.2
```
- 可以用于桥接到真实网卡，让虚拟环境访问真实网络。
>[!NOTE]
>网络命名空间是 Linux 的一种隔离机制，使得每个命名空间拥有自己的网络栈（接口、IP、路由等）。允许一个命名空间中的进程有自己的网络堆栈，包括接口、路由表和防火墙规则。
>将 `veth pair` 的一端放入一个命名空间，另一端放入另一个命名空间，就能让这两个隔离的网络环境通信。
>示例：Docker 中容器（使用独立的网络命名空间），如何通过宿主机的物理网卡访问互联网？
>解决方案：将 `veth` 的一端放在容器的命名空间中，另一端放在主机的命名空间，并将其桥接到宿主机的物理网卡或虚拟网桥（bridge）如 `docker0` 上，这样容器就可以通过虚拟网桥访问真实网络。
![[Docker-network-internals.png]]
