iSCSI （Internet Small Computer Systems Interface，互联网小型计算机系统接口）定义为一种传输协议，提供在基于 TCP 和 IP 的网络上承载的 SCSI 块协议。见：RFC 7143 和 RFC 7144 

iSCSI 与光纤通道一样，可用于创建存储区域网络（SAN）。iSCSI 流量可以在共享网络或专用于存储的网络上运行。但 iSCSI 不支持文件访问 NAS（网络附加存储）或对象存储访问（它们使用不同的传输协议）

有多种传输可用于 iSCSI。最常见的是基于以太网的 TCP/IP，但 RDMA （远程直接内存访问）也可以于 iSER （RDMA 的 iSCSI 扩展）一起使用。如果使用 iSER，传输层是 `RoCE`（底层是Ethernet）或 InfiniBand（底层是 InfiniBand）

![[iscsi.png]]

`iSCSI` 还可以通过使用 iSCSI 硬件卸载和/或 TCP 卸载引擎（TOE）的网络适配器来加速。
- 前者，硬件适配器（HBA）从服务器 CPU 卸载 iSCSI initiator 功能。
- 后者，适配器从服务器内核和 CPU 卸载 TCP 处理。