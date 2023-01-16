2023.01.16
---

References:
> https://blog.csdn.net/qingdao666666/article/details/114132107
>
> https://www.cnblogs.com/morphling/p/3458546.html
>
> https://blog.csdn.net/bandaoyu/article/details/126627574

## What is net bridge

最初的网桥就是一个两口的交换机，经过网桥的数据包会原封不动的转发。与hub不同的是网桥会检查mac地址，不在当前端口的mac地址就过滤掉了。而hub是工作在物理层的设备，一个口收到的信号原封不动的发送到所有的接口。早期的交换机，可以看成是很多网桥的集成。简单来说，桥接就是把一台机器上的若干个网络接口“连接”起来。其结果是，其中一个网口收到的报文会被复制给其他网口并发送出去。以使得网口之间的报文能够互相转发。

Net bridge is a hub working on 


现在的Linux 网桥可以看做是（三层的）虚拟交换机，功能和物理交换机一样，最常用的功能是链接虚拟机和容器--为虚拟机和容器提供一个虚拟交换机。

Linux net bridge can be seen as a virtual switch connecting virtual machines to the real net.

A net card `eth0` or some other net devices can be attached to the bridge(such as `br0`), and is called the sub-device of `br0`. 

After being attached, `eth0` will lose its own ip and mac, it is seen as a network interface of `br0`.

The `br0` has its own ip and mac(the smallest mac of its sub-devices), all of its sub-devices just receive net pack and transfer it to `br0`. Then `br0` decide how to process the pack.

## Create a net bridge

- Create a new bridge and change its state to up:

  ```bash
  $ ip link add name br1 type bridge
  $ ip link set br1 up
  ```

- Attach interface to bridge

  ```bash
  # To add an interface (e.g. eth0) into the bridge, its state must be up:
  $ ip link set eth0 up
  # Adding the interface into the bridge is done by setting its master to br1:
  $ ip link set eth0 master br1
  ```

- Check the status of devices `ip a`

- Set ip for bridge, config its route

  ```bash
  $ ip addr add dev br1 192.168.31.66/24
  $ ip route show
  $ ip route add default via 192.168.31.1 dev br1
  ```

- Remove an interface from a bridge:

  ```bash
  $ ip link set eth0 nomaster
  ```

- The interface will still be up, so you may also want to bring it down:

  ```
  $ ip link set eth0 down
  ```

- To delete a bridge issue the following command:

  ```
  $ ip link delete br1 type bridge
  ```

  This will automatically remove all interfaces from the bridge. The slave interfaces will still be up, though, so you may also want to bring them down after.

