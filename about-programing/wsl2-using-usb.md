# 2022.4.20

为了使用usb摄像头，光下载usbipd将usb挂载到wsl下是不够的，这样仅能在lsusb中对摄像头进行显示，而不会有`/dev/video*`出现。

```bash
xac-lenovo@Xac-lenovo:~$ lsusb
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 003: ID 1b3f:2247 Generalplus Technology Inc. GENERAL WEBCAM
Bus 001 Device 002: ID 174f:2459 Syntek Integrated Camera
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub

xac-lenovo@Xac-lenovo:~$ ls /dev/video*
ls: cannot access '/dev/video*': No such file or directory
```

想在wsl2中使用usb，需要自己进行内核的编译，并加载摄像头相应驱动。

在网上查阅很多资料都没有办法按照步骤达到我的效果，因此自己琢磨了一天写出下面的解决办法

## 参考资料

> https://dowww.spencerwoo.com/4-advanced/4-4-usb.html#usbip-win

> https://github.com/dorssel/usbipd-win/issues/352

> https://github.com/dorssel/usbipd-win/issues/257

## 在linux中编译内核

在 WSL 中，下载安装编译 Linux 内核所需要的库和工具

```bash
$ sudo apt install build-essential flex bison libssl-dev libelf-dev libncurses-dev autoconf libudev-dev libtool
```

找到我们当前所使用的 Linux 内核名称：

```bash
$ uname -r
5.10.102.1-microsoft-standard-WSL2
```

下载内核源码

```bash
$ cd /usr/src # 源码一般放在此目录下
$ sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git 
$ cd /usr/src/4.19.43-microsoft-standard
$ sudo git checkout linux-msft-wsl-5.10.102.1 # checkout到我们的内核版本分支
```

将我们内核文件复制进去

```bash
$ sudo cp /proc/config.gz config.gz
$ sudo gunzip config.gz
$ sudo mv config .config
```

运行 menuconfig 来选择我们想要添加的内核模块：

```bash
$ sudo make menuconfig
```

根据你自己的需要选定不同的模块：（按空格选择或取消选择）

```
Device Drivers->USB support[*]
Device Drivers->USB support->Support for Host-side USB[M]
Device Drivers->USB support->Enable USB persist by default[*]
Device Drivers->USB support->USB Modem (CDC ACM) support[M]
Device Drivers->USB support->USB Mass Storage support[M]
Device Drivers->USB support->USB/IP support[M]
Device Drivers->USB support->VHCI hcd[M]
Device Drivers->USB support->VHCI hcd->Number of ports per USB/IP virtual host controller(8)
Device Drivers->USB support->Number of USB/IP virtual host controllers(1)
Device Drivers->USB support->USB Serial Converter support[M]
Device Drivers->USB support->USB Serial Converter support->USB FTDI Single Port Serial Driver[M]
Device Drivers->USB support->USB Physical Layer drivers->NOP USB Transceiver Driver[M]
Device Drivers->Network device support->USB Network Adapters[M]
Device Drivers->Network device support->USB Network Adapters->[Deselect everything you don't care about]
Device Drivers->Network device support->USB Network Adapters->Multi-purpose USB Networking Framework[M]
Device Drivers->Network device support->USB Network Adapters->CDC Ethernet support (smart devices such as cable modems)[M]
Device Drivers->Network device support->USB Network Adapters->Multi-purpose USB Networking Framework->Host for RNDIS and ActiveSync devices[M]
```

以上为USB所需要的，为了使用usb摄像头还需要以下这些，或许还有其他的摄像头驱动需要安装，但这些对我来说已经够了

```bash
Device Drivers->Multimedia support[M]
Device Drivers->Multimedia support[M]->Media core support->Video4Linux core[M]
Device Drivers->Multimedia support[M]->Media core support->Media Controller API[*]
Device Drivers->Multimedia support[M]->Media drivers->Media USB Adapters[*]
Device Drivers->Multimedia support[M]->Media drivers->Media USB Adapters[*]->USB Video Class(UVC)[M]
Device Drivers->Multimedia support[M]->Media drivers->Media USB Adapters[*]->UVC input evnets device support[*]
```

编译内核

```bash
$ sudo make -j 12 && sudo make modules_install -j 12 && sudo make install -j 12
```

出错BTF: .tmp_vmlinux.btf: pahole (pahole) is not available

```bash
$ sudo apt install dwarves
```

编译usbip

```bash
$ cd tools/usb/usbip
$ sudo ./autogen.sh
$ sudo ./configure
$ sudo make install -j 12
```

将 USBIP 工具库复制到 USBIP 可以访问到的位置：

```bash
$ sudo cp libsrc/.libs/libusbip.so.0 /lib/libusbip.so.0
```

回退到初始的make目录下将镜像拷贝到windows目录下

```bash
$ cd /usr/src/4.19.43-microsoft-standard
$ cp arch/x86/boot/bzImage /mnt/c/Users/<user>/usbip-bzImage
```

创建一个.wslconfig文件在/mnt/c/Users/<user>/ and add a reference to the created image with the following.

```
[wsl2]
kernel=c:\\Users\\<user>\\usbip-bzImage
```

然后执行`wsl --shutdown`，重启 WSL2 即可加载新内核，但新模块需要手动添加


执行`vi $HOME/startusb.sh`在其中写入

```
#!/bin/bash
sudo modprobe usbcore
sudo modprobe usb-common
sudo modprobe hid-generic
sudo modprobe hid
sudo modprobe usbnet
sudo modprobe cdc_ether
sudo modprobe rndis_host
sudo modprobe usbserial
sudo modprobe usb-storage
sudo modprobe cdc-acm
sudo modprobe ftdi_sio
sudo modprobe usbip-core
sudo modprobe vhci-hcd
sudo modprobe dvb-core
sudo modprobe videodev
sudo modprobe v4l2-dv-timings
sudo modprobe videobuf2-common
sudo modprobe videobuf2-memops
sudo modprobe videobuf2-v4l2
sudo modprobe videobuf2-vmalloc
sudo modprobe mc
sudo modprobe uvcvideo
sudo modprobe mii
sudo modprobe cdc_ncm
sudo modprobe phy-generic
sudo modprobe ch341
sudo modprobe cp210x
# 以下为我测试后删除的模组
# sudo modprobe videobuf2-dma-contig
# sudo modprobe gspca_sn9c20x
# sudo modprobe aspeed-video
# sudo modprobe cdns-csi2rx
# sudo modprobe cdns-csi2tx
# sudo modprobe gspca_main
# sudo modprobe gspca_stk014
# sudo modprobe gspca_stk1135
# sudo modprobe pwc
# sudo modprobe stkwebcam
# sudo modprobe v4l2-fwnode
# 将给出本机ip地址
echo $(cat /etc/resolv.conf | grep nameserver | awk '{print $2}')
```

执行`sudo chmod +x ./startusb.sh`赋予执行权限，之后执行`./startusb.sh`，注意每次重启wsl都需要进行加载，当然也可以将其写入.bashrc

## 在windows下配置usbipd

在release页面下下载msi应用进行安装并添加至环境变量path

> https://github.com/cezanne/usbip-win/releases

## 在windwos terminal中使用usbipd接入设备

```bash
$ usbipd wsl list # 列出所有usb设备与attach情况
$ usbipd wsl attach -b <busid> # 将usb设备接入linux
# 若出错则先执行下面这句
$ usbipd bind --force -b <busid>
```

## 在linux下使用usbipd接入设备

通过`vi $HOME/attach-cam.sh`和`sudo chmod +x $HOME/attach-cam.sh`

```bash
#!/bin/bash
sudo usbip list --remote=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}')
sudo usbip attach --remote=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}') --busid=2-6  # 我的摄像头是2-6
```

上面两种方式选择一种接入即可，想要取消接入可以使用detach，也可以把usb拔了重插

此刻可以发现已经出现`/dev/video0`，可以用`ffplay -f video4linux2 -input_format mjpeg -framerate 30 -video_size 640*480 /dev/video0`进行测试

或者也可以使用opencv进行测试，注意摄像头权限可能没有开，使用`sudo chmod 777 /dev/video0`赋予访问权限

注意`cheese`无法访问摄像头，原因可能是因为视频格式的问题，我没有细查，我的目的是在opencv中使用
