# 2022.4.20

为了使用usb摄像头，光下载usbipd将usb挂载到wsl下是不够的，这样仅能在lsusb中对摄像头进行显示，而不会有`/dev/video*`出现，cheese也无法检测到设备

想在wsl2中使用usb，需要自己进行内核的编译，打开usb/ip选项。

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
$ cd /usr/src
$ sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git /usr/src/4.19.43-microsoft-standard
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

根据你自己的需要选定不同的模块：

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
$ cd ..
$ cp arch/x86/boot/bzImage /mnt/c/Users/<user>/usbip-bzImage
```

Create a .wslconfig file on /mnt/c/Users/<user>/ and add a reference to the created image with the following.

```
[wsl2]
kernel=c:\\Users\\<user>\\usbip-bzImage
```

然后执行 wsl.exe --shutdown，重启 WSL2 即可加载新内核