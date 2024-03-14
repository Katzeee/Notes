---
title: Network Message Categories
date: 2021.09.02
categories: [Othernotes]
tags: [network]
---


### 报文，数据报，分片，分组，帧的区别

>**报文（message）**：传输协议交给ip的数据称为报文；典型的报文包含一个传输层首部和应用程序数据；
>
>**数据报（datagram）**：在报文首部加上ip首部形成一个数据报；
>
>**分片（fragment）**：如果在选定网络中，数据报的长度太大，ip就会把数据报分裂成几个分片，每个分片含有它自己的ip首部和一段原来数据报的数据；
>
>**分组（packet）：**提交给数据链路层进行传送时，一个ip分片或者一个很小的无需分片的ip数据报成为分组；
>
>**帧（frame）：**数据链路层在分组前面加上它自己的首部形成帧，并发送该帧；

<!-- <p align="left"> -->
  <!-- <img src="/assets/images/2020040109441427.png"> -->
<!-- </p> -->