2022.4.20
---

# Webcam drivers in wsl2

## Problem

It is not enough to use usb webcam if you only use usbipd connect the usb port to wsl.

You will only find it by `lsusb`, but there will be no `/dev/video*` appear.

```bash
xac-lenovo@Xac-lenovo:~$ lsusb
Bus 002 Device 001: ID 1d6b:0003 Linux Foundation 3.0 root hub
Bus 001 Device 003: ID 1b3f:2247 Generalplus Technology Inc. GENERAL WEBCAM
Bus 001 Device 002: ID 174f:2459 Syntek Integrated Camera
Bus 001 Device 001: ID 1d6b:0002 Linux Foundation 2.0 root hub

xac-lenovo@Xac-lenovo:~$ ls /dev/video*
ls: cannot access '/dev/video*': No such file or directory
```

In order to use webcam, you should compile your own kernel with webcam supported.

Following are the solution.

## References

> https://dowww.spencerwoo.com/4-advanced/4-4-usb.html#usbip-win

> https://github.com/dorssel/usbipd-win/issues/352

> https://github.com/dorssel/usbipd-win/issues/257

## complie kernel in linux

- Download the tools and libs for compiling linux kernel

  ```bash
  $ sudo apt install build-essential flex bison libssl-dev libelf-dev libncurses-dev autoconf libudev-dev libtool
  ```

- Get current linux kernel name:

  ```bash
  $ uname -r
  5.10.102.1-microsoft-standard-WSL2
  ```

- Download kernel source codes:

  ```bash
  $ cd /usr/src # 源码一般放在此目录下
  $ sudo git clone https://github.com/microsoft/WSL2-Linux-Kernel.git 
  $ cd /usr/src/4.19.43-microsoft-standard
  $ sudo git checkout linux-msft-wsl-5.10.102.1 # checkout到我们的内核版本分支
  ```

- copy all files in:

  ```bash
  $ sudo cp /proc/config.gz config.gz
  $ sudo gunzip config.gz
  $ sudo mv config .config
  ```

- run `menuconfig` to select the modules we want to add to kernel:

  ```bash
  $ sudo make menuconfig
  ```

- Select some needed modules(press space to select):

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

  Above are only for USB, and webcam also need the followings. These are enough for me, but maybe you need more.
  
  ```bash
  Device Drivers->Multimedia support[M]
  Device Drivers->Multimedia support[M]->Media core support->Video4Linux core[M]
  Device Drivers->Multimedia support[M]->Media core support->Media Controller API[*]
  Device Drivers->Multimedia support[M]->Media drivers->Media USB Adapters[*]
  Device Drivers->Multimedia support[M]->Media drivers->Media USB Adapters[*]->USB Video Class(UVC)[M]
  Device Drivers->Multimedia support[M]->Media drivers->Media USB Adapters[*]->UVC input evnets device support[*]
  ```

- Compile kernel:

  ```bash
  $ sudo make -j 12 && sudo make modules_install -j 12 && sudo make install -j 12
  ```

  Error: `BTF: .tmp_vmlinux.btf: pahole (pahole) is not available`

  ```bash
  $ sudo apt install dwarves
  ```

- Compile usbip

  ```bash
  $ cd tools/usb/usbip
  $ sudo ./autogen.sh
  $ sudo ./configure
  $ sudo make install -j 12
  ```

- Copy usbip libs to system folder

  ```bash
  $ sudo cp libsrc/.libs/libusbip.so.0 /lib/libusbip.so.0
  ```

  cd into the origin folder and copy image into Windows user dir

  ```bash
  $ cd /usr/src/4.19.43-microsoft-standard
  $ cp arch/x86/boot/bzImage /mnt/c/Users/<user>/usbip-bzImage
  ```

  Create `.wslconfig` file at `/mnt/c/Users/<user>/` and add a reference to the created image with the following.

  ```
  [wsl2]
  kernel=c:\\Users\\<user>\\usbip-bzImage
  ```

  Then execute `wsl --shutdown`，reboot wsl2 to reload new kernel mod，but you should manually enable them


## Setup script

  Execute `vi $HOME/startusb.sh`

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

  # These are no use for me
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
  # ifconfig
  ```

  Run `sudo chmod +x ./startusb.sh`, then`./startusb.sh`. 
  Notice that you should run this script every booting wsl, also you can write this in `.bashrc`.

## Config usbipd on Windows

Download msi app at release page then add it to env path

> https://github.com/cezanne/usbip-win/releases

## Use usbipd to connect devives to wsl in windows terminal

```bash
$ usbipd wsl list # 列出所有usb设备与attach情况
$ usbipd wsl attach -b <busid> # 将usb设备接入linux
# 若出错则先执行下面这句
# If an error occurs, execute the following command first
$ usbipd bind --force -b <busid>
```

## Use usbipd to connect devices to wsl in wsl terminal

Run `vi $HOME/attach-cam.sh`and`sudo chmod +x $HOME/attach-cam.sh`

```bash
#!/bin/bash
sudo usbip list --remote=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}')
sudo usbip attach --remote=$(cat /etc/resolv.conf | grep nameserver | awk '{print $2}') --busid=2-6  # My cam number is 2-6
```

You can choose either of the above two ways. If you want to cancel the connection, you can use `detach`, or you can directly unplug the usb.

Now you can see there is `/dev/video0`, use `ffplay -f video4linux2 -input_format mjpeg -framerate 30 -video_size 640*480 /dev/video0` to test the cam.

Or you can test it with `opencv`, but notice to change the permission of the cam.(use `sudo chmod 777 /dev/video0`)

PS: for me `cheese` cannot access the cam, maybe it's the problem of video format, I didn't look into it.
