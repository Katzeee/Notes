# 2022.5.16

References:
> [Arch Linux + Windows 双系统安装教程](https://blog.linioi.com/posts/18/)
>
> [gentoo linux配置intel和nvidia双显卡电脑，使用prime方案](https://www.jianshu.com/p/515a1f39b438)

## Check the boot mode

Set `Secure Boot` option to `DISABLE` in your BIOS.

Press <kbd>win+r</kbd> and input `msinfo32` to check whether `bios mode` is `UEFI`

This article only intends for `UEFI` boot mode

## Prepare for a bootable usb

Download [Rufus](http://rufus.ie/en/)

Select the Device that you want to burn program in, the iso file at boot selection. Remain default at `Persistent partition size`.

Select `GPT` for `Partition scheme`, `UEFI` for `Target system`. Remain default for Format Options, then press START. Select the recommended option `ISO` and press OK.

## Prepare for free space

Open `Disk management panel` in `Computer management` which can be opened by right click on `This Computer`->`Manage`.

Right click on the disk in which you want to install arch, select `Compress` and separate free space from it. **At least 30G.**

## Set timezone shift on Windows

To avoid some wierd problem when you switch system from linux to windows,  run the following command with Admin priority.
```bash
reg add "HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\TimeZoneInformation" /v RealTimeIsUniversal /d 1 /t REG_DWORD /f
```

## Start installation

First, boot your computer from the usb, select the first option

<p align="center">
  <img src="../images/boot-select.png">
</p>

Connect to network and synchronize time.

- Unblock to the network card
  ```
  $ rfkill unblock all
  ```
- Connect to the network
  ```bash
  $ iwctl # enter iwd shell
  $ device list
  $ station <device> get-networks # list all detected networks
  $ station <device> connect <SSID> # connect
  $ exit # exit iwd shell
  $ ping www.baidu.com # check the connection
  ```
- Synchronize the time
  ```bash
  $ hwclock --systohc --utc
  $ timedatectl set-ntp true
  ```

Format and mount the disks

- Check the disks
    ```bash
    $ fdisk -l
    ```
- Partition the disks
  ```bash
  $ cfdisk /dev/nvme1n1 # select the whole disk, not a partition like nvme1n1p1
  ```
  Divide the free space into two part, like 100G for `/mnt` and 300G for `/mnt/home`, **NOW THE SYSTEM IS BOOTED BY USB, SO THE DISKS ARE MOUNTED ON `/mnt`**

  `UEFI` do not need a `/boot` partition, we just need to mount `/boot` on `WINDOWS EFI`

  Then select `[Write]` and `[Quit]`, you are supposed to input `yes` to confirm.

- Mount the disks
  ```bash
  $ fdisk -l # Check the partition
  $ mkfs.ext4 /dev/nvme1n1p1 # Format /mnt
  $ mkfs.ext4 /dev/nvme1n1p2 # Format /mnt/home
  $ mount /dev/nvme1n1p1 /mnt # Mount /mnt
  $ mkdir /mnt/home
  $ mount /dev/nvme1n1p2 /mnt/home # Mount /mnt/home
  $ mkdir /mnt/boot
  $ mount /dev/nvme0n1p2 /mnt/boot # Mount /mnt/boot
  $ fdisk -l # Check the partition again
  ```

Install the system

- Change the mirrors
  ```bash
  $ reflector --country 'China' --age 12 --protocol https --sort rate --save /etc/pacman.d/mirrorlist
  $ pacman -Syy # refresh the mirrors
  ```
- Install kernel
  ```bash
  $ pacstrap /mnt base base-devel linux linux-firmware vim e2fsprogs ntfs-3g 
  # 在上述命令中，base 是元软件包（基本系统所需依赖，你可以不知道它是啥但是必须安装），base-devel 是基础软件包组，linux 是内核，linux-firmware 是固件包，nano、vim 是常用编辑器，e2fsprogs 是 ext4 文件系统所需工具，如果你是按照上面教程使用 ext4 文件系统则必须安装该软件包。ntfs-3g 是与 Windows 的 NTFS 有关的软件包，如果是单系统可以不安装。
  ```
- Config file system table
  ```bash
  $ genfstab -U -p /mnt >> /mnt/etc/fstab
  $ ls /mnt/etc/fstab # check
  ```

## Install the new system

- Enter the new system by running `arch-chroot /mnt /bin/bash`.
- Set root password by running `passwd`.
- Create a user by running `useradd -m -U -s /bin/bash <username>`.
- Set the user password by `passwd <username>`.
- Change the user priority
  ```bash
  $ vim /etc/sudoers
  # add
  %<username> ALL=(ALL) ALL # your group name is same as your user name because the `-U` option 
  ```
- Set the name of your computer by `echo <computername> > /etc/hostname`
- Set up hosts
  ```bash
  $ vim /etc/hosts
  # add
  127.0.0.1	localhost
  ::1		localhost
  127.0.1.1	<computername>.localdomain	<computername>
  ```
- Install microcode
  ```bash
  $ pacman -S intel-ucode # intel
  $ pacman -S amd-ucode # amd
  ```
- Set dual-system instruction
  ```bash
  $ pacman -S grub efibootmgr os-prober # install softwares
  $ grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=grub --recheck # install to disks
  $ echo "GRUB_DISABLE_OS_PROBER=false" >> /etc/default/grub # enable os-prober
  $ grub-mkconfig -o /boot/grub/grub.cfg # generate config file
  # You would see "Found Windows" or something
  ```
- Set up language and timezone
  ```bash
  $ vim /etc/locale.gen
  # remove the front '#' of
  en_US.UTF-8 UTF-8
  zh_CN
  zh_TW
  ```
  Then run `locale-gen` to refresh, `echo LANG=en_US.UTF-8 > /etc/locale.conf` to set system language to English, `ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime` to change timezone.
- Set up network
  ```bash
  $ pacman -S networkmanager
  $ systemctl enable NetworkManager # auto-startup when boot
  ```
- Localize
  ```bash
  $ pacman -S noto-fonts noto-fonts-cjk noto-fonts-emoji adobe-source-han-sans-otc-fonts wqy-microhei wqy-zenhei # install fonts
  ```
- Add archlinuxcn source and install yay
  ```bash
  $ vim /etc/pacman.conf
  # add
  [archlinuxcn]
  Server = https://mirrors.tuna.tsinghua.edu.cn/archlinuxcn/$arch
  $ pacman -Syy # refresh
  $ pacman -S archlinuxcn-keyring # install keyring
  $ pacman -S yay
  ```
- Solve the Windows-encryption
  ```bash
  $ yay -S dislocker
  $ sudo frub-mkconfig -o /boot/grub/grub.cfg
  ```

## Other configuration

- Grub theme

  [Grub theme](https://www.gnome-look.org/browse?cat=109)

  [My theme](https://github.com/vandalsoul/darkmatter-grub2-theme/)

- Xorg

  [Xorg](https://wiki.archlinux.org/title/Xorg)

  [Xrandr](https://wiki.archlinux.org/title/Multihead)
  
  Display server, for GUI applications. `xrandr` is to custom your display like changing resolution, setting up dual monitor etc.
  
  ```bash
  $ yay -S xorg-server xorg-init xorg-xrandr
  ```

- Dwm

  awesomebar: to hide and showe clients

  cfacts: to resize the clients' window vertically

  hide_vacant_tags: to hide vacant tags

  layoutscroll: to scroll over all layouts

  pertag: to set different layouts in per tag

  viewontag: show all clients' title on bar

  cool-autostart: autostart scripts and close it when quit dwm

  sendmon_keepfocus: send client to other monitors and keep focusing on it
  
- Nvidia driver

  [Nvidia](https://wiki.archlinux.org/title/NVIDIA)

  [Nvidia prime](https://wiki.archlinux.org/title/PRIME)

  `nvidia` is Nvidia driver for Nvidia graphic cards, `nvidia-prime` which can run a specific application rendering by nvidia card is only needed by hybird graphic cards like intel+nvidia, amd+nvidia.
  
  ```bash
  $ yay -S nvidia nvidia-prime 
  ```

  Here are my [xorg.conf](https://github.com/Katzeee/dot-files/tree/master/xorg) files.
  
- Sound

  [Alsa](https://wiki.archlinux.org/title/Advanced_Linux_Sound_Architecture)
  
  In order to drive the sound card

  ```bash
  yay -S alsa-utils
  ```

- VSCode sync

  ```bash
  yay -S visual-studio-code-bin  
  ```
  
  For sync to your github account, you need to install following packages:

  > [settings sync not working on arch linux #113157](https://github.com/microsoft/vscode/issues/113157)
  
  ```bash
  yay -S xdg-utils gnome-keyring libsecret
  ```