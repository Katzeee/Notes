2022.12.16
---

## Bios setting(important for pass-through)

IOMMU: enable

SVM mode

VT-D(intel), AMD-V(AMD)

SCM disable(UEFI boot)

## Install pve

### Install

Download pve-iso from [pve-download](https://proxmox.com/en/downloads/category/iso-images-pve).

Use `rufus` to create a bootable USB drive.(GPT+DD)

Boot from your usb device and just press continue, maybe setting your dns server address to `8.8.8.8` instead of your gateway address will be more robust.

Then you can login pve system from another pc's browser at `https://<your-ip-addr>:8006`, mind **https** not ~~http~~

### Tools Config

Change mirror sources

```bash
# /etc/apt/sources.list 
deb https://mirrors.aliyun.com/debian buster main contrib non-free
deb https://mirrors.aliyun.com/debian buster-updates main contrib non-free
deb https://mirrors.aliyun.com/debian-security buster/updates main contrib non-free # proxmox source
deb https://mirrors.ustc.edu.cn/proxmox/debian/pve buster pve-no-subscription
```

Comment enterprise source

```bash
# /etc/apt/sources.list.d/pve-enterprise.list
# deb https://enterprise.proxmox.com/debian/pve buster pve-enterprise
```

> *Try to use pve-tools*

```bash
$ apt update && apt -y install git && git clone https://github.com/ivanhao/pvetools.git
```

### IOMMU and hardware pass-through

`vi /etc/default/grub`
```diff
- GRUB_CMDLINE_LINUX_DEFAULT="quiet"

# For AMD users
+ GRUB_CMDLINE_LINUX_DEFAULT="quiet intel_pstate=disable amd_iommu=on pcie_acs_override=downstream,multifunction video=vesafb:off video=efifb:off"

# all of them may not necessary, but I added `intel_pstate=disable pcie_acs_override=downstream,multifunction`
# PS:
# efifb:off disable efi-booting display devices
# vesafb:off disable vesa-booting display devices
```

then `update-grub`

`vi /etc/modules`

```diff
+ vfio
+ vfio_iommu_type1
+ vfio_pci
+ vfio_virqfd
```

then reboot

### Enable ipv6

Append these to `/etc/sysctl.conf`, reboot.

```bash
net.ipv6.conf.all.accept_ra=2
net.ipv6.conf.default.accept_ra=2
net.ipv6.conf.vmbr0.accept_ra=2
net.ipv6.conf.all.autoconf=1
net.ipv6.conf.default.autoconf=1
net.ipv6.conf.vmbr0.autoconf=1
```

### To change ip-address and examples

`/etc/network/interfaces`
```
auto lo
iface lo inet loopback

iface enp34s0 inet manual

auto vmbr0
iface vmbr0 inet static
    address 192.168.2.200/24
    gateway 192.168.2.2
    bridge-ports enp34s0
    bridge-stp off
    bridge-fd 0
```

`/etc/hosts` 
```
127.0.0.1 localhost.localdomain localhost
192.168.2.200 xac.pve xac

# IPv6

::1     ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
ff02::3 ip6-allhosts
```

`/etc/issue` for welcome page

`/etc/resolv.conf` for dns server

### DDNS

[ddns-go](https://github.com/jeessy2/ddns-go)
wget https://github.com/jeessy2/ddns-go/releases/download/v4.5.3/ddns-go_4.5.3_linux_x86_64.tar.gz
```bash
mkdir ddns-go && cd ddns-go
tar -zxvf ../ddns-**
./ddns-go -s install
# ./ddns-go -s uninstall
```

port: 9876

create API, set up API and domain

### ssh

`/etc/ssh/sshd_config`
```
PermitRootLogin yes
PasswordAuthentication yes
```

Use `WindTerm` as my ssh terminal

Generate ssh key in both host and client, then copy `<client>/.ssh/id_rsa.pub` to host as `<host>/.ssh/authorized_keys`, then client can ssh to host without password

In WindTerm, also set the sessions' authentication identity file as `<client>/.ssh/id_rsa` such that windterm to login host without password

## Install Openwrt

### Download iso file

https://github.com/coolsnowwolf/lede/releases

### Create a virtual machine

q35, no media, 2G ram, **delete disk**

Run command `find / -name openwrt*` to get the path of our img file, then import a new disk to openwrt machine by excuting `qm importdisk <vm-id> <img-path> local-lvm`.

Add the new disk in hardware tab, then select the boot device in device tab, start openwrt

### Pre-config

- Update source

`System->software package->configuration`, use origin source instead of local source

```
src/gz openwrt_koolshare_mod_core http://downloads.openwrt.org/snapshots/targets/x86/64/packages
src/gz openwrt_koolshare_mod_base http://downloads.openwrt.org/snapshots/packages/x86_64/base
src/gz openwrt_koolshare_mod_luci http://downloads.openwrt.org/snapshots/packages/x86_64/luci
src/gz openwrt_koolshare_mod_packages http://downloads.openwrt.org/snapshots/packages/x86_64/packages
src/gz openwrt_koolshare_mod_routing http://downloads.openwrt.org/snapshots/packages/x86_64/routing
src/gz openwrt_koolshare_mod_telephony http://downloads.openwrt.org/snapshots/packages/x86_64/telephony
```

Then run:

```bash
$ opkg update
$ opkg install qemu-ga
$ reboot
```
### Config

- One-armed router(router on a stick)

  Reserve only one lan interface, set its gateway as main router, then set every single devices manually route to openwrt. You can use either main router DHCP or openwrt DHCP. Do not use both of them!

  ```
  $ sudo route add default gw <openwrt-ip>
  $ sudo route del default
  ```

## Install Arch

### Download iso file

Download iso file from 163 mirror into `local` storeage.

### Create new virtual machine

Set every options as defualt(UEFI), except those about quantities(mem, disk, cpu cores) and select to use qemu agent.

Choose the iso file as cd/rom, then boot the machine.

### Start install

Change source mirror, then directly start install via one command:

```bash
$ reflector --country 'China' --age 12 --protocol https --sort rate --save /etc/pacman.d/mirrorlist
$ https://mirrors.ustc.edu.cn/archlinux
$ archinstall
```

Set all language related options to English(or there may some font problems). You can change it in `/etc/locale.gen` later.

```
Profile: minimal

Additional package: neovim net-tools qemu-guest-agent git openssh man-db

Network: copy from iso

Additional repo: mutilib
```

### Rest configure

Static IP:

```bash
# /etc/systemd/network/**
Address: 192.168.x.x/24
Gateway: 192.168.x.x
DNS: 
```

### git clone fail

Fail with `kex_exchange_identification: Connection closed by remote host`

`nvim ~/.ssh/config`

```
Host github.com
    HostName ssh.github.com
    User git
    Port 443
```

## Install windows and config GPU pass-through

### Install windows

System: q35, UEFI
Disk: SCSI
Network: virtIO

Change boot sequential, then install windows as usual, **need install virtio driver from iso file.**

### setup Remote desktop

- static ip

- RDP

  right click my computer->属性->远程桌面

  win+r -> secpol.msc -> 本地策略 -> 安全选项，在右侧选中帐户: 使用空白密码的本地帐户只允许进行控制台登录

  网络设置->防火墙->高级设置->入站设置（最下）->TCP-WSS-IN启用即可

### GPU pass-through

- Add PCIE device: select all options except `Primary GPU`.

- Install GPU driver, check whether GPU is working

- Change Display to `none`, add select `Primary GPU` option

### Hide vm from guest

Edit `/etc/pve/qemu-server/<vm-id>.conf` in host, **I don't know which options are unnecessary**, but it works.

```
args: -cpu 'host,-hypervisor,+kvm_pv_unhalt,+kvm_pv_eoi,hv_spinlocks=0x1fff,hv_vapic,hv_time,hv_reset,hv_vpindex,hv_runtime,hv_relaxed,kvm=off,hv_vendor_id=null'
```

## Install TrueNAS Scale(Recommended)

### Reference

> https://www.truenasscale.com/

### Create VM

Let everything default except 16G disk and 8192 mem, choose iso then start VM

### sata controller pass-through

choose `400 Series Chipset SATA Controller` (without All Functions)

### Change Web port(For accessing from internet)

System Settings -> General -> GUI

change http port from 80 to 8000

### Add Catalog(need VPN)

Apps -> Manage Catalogs -> Add Catalog

Name: truecharts

Repo: https://github.com/truecharts/catalog

Trains: stable dependency

To avoid TLS fail, use `git config --global http.sslVerify false`

## Install TrueNAS Core(Not Recommended)

### Install TrueNAS

32G disk SATA
8192 Mem

install then reboot

### Disk pass-through

Use pvetools to pass through disks (qm set), only choose the whole sata, not sata1, sata2 or etc.

Or use SATA controller PCI pass through(may not work)

### Config 

- Network

  Network->interface
  
  De-select DHCP, select autoconfig IPV6
  
  type in the IPV4 address
  
  TEST then confirm

- Users

  Account->Users->Add

- General

  System->General->Time zone


### Create Pool and dataset

Storage->Pool->Create new pool->type in a name and choose which disks to compose to a what kind of pool->press create

Click the threes dots at right of the pool->add dataset

### SMB share

Sharing->SMB

Choose the folder then submit

### guest-agent

`cd /usr/local/etc/pkg/repos/`, change `local.conf` `enabled=yes` to `no`, change `FreeBSD.conf` `enabled=no`to `yes`.

then run command:

```bash
$ pkg install qemu-guest-agent
# Modify your `/etc/rc.conf` by adding these settings

qemu_guest_agent_enable="YES"
qemu_guest_agent_flags="-d -v -l /var/log/qemu-ga.log"

and run
# service qemu-guest-agent start
```

## LXC container

### Install LXC container

Create CT, **deselect** `unprivilieged container`, about 20G(or 64G for not mount NAS folder) disk, 2048 mem and 2048 swap, static ip. DNS domain is the smae as gateway, DNS servers set as blank.

network ipv6 SLAAC

### Setup intel gpu share(optional)

In pve: `vi /etc/pve/lxc/<CT_ID>.conf`

Add following(Get args by `ls -l /dev/dri`):

Mandatory!!!

```
lxc.apparmor.profile: unconfined
lxc.cgroup.devices.allow: a
lxc.cap.drop:
```

Optional
```
lxc.cgroup2.devices.allow: c 226:0 rwm
lxc.cgroup2.devices.allow: c 226:128 rwm
lxc.cgroup2.devices.allow: c 29:0 rwm
lxc.mount.entry: /dev/dri dev/dri none bind,optional,create=dir
lxc.mount.entry: /dev/fb0 dev/fb0 none bind,optional,create=file
```

Then start CT, you will see gpu by running `ls /dev/dri`.

### Change apt source

ubuntu20.04, `vi /etc/apt/source.list`

```
deb http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-proposed main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ bionic-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-security main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ bionic-updates main restricted universe
```

### localization

`sudo dpkg-reconfigure locales`

<Space> to check, <Tab> then <Enter> to <Ok>

### Mount NAS

Or just use sftp which need to add a new user to connect to this LXC via ssh.

```bash
$ apt install cifs-utils
$ mkdir /mnt/<folder-name>
$ vi ~/.smbcredentials # password
```

type in things like 

```bash
username=<username>
password=<password>
```

Then `vi /etc/fstab`

```bash
//$smb_server/share /mnt/nas_share cifs credentials=/root/.smbcredentials,iocharset=utf8 0 0
```

Then reboot, you will find your smb folder

### Install docker

- Docker

  ```bash
  $ apt install curl -y
  $ curl -sSL https://get.daocloud.io/docker | sh
  ```

- Portainer

  ```bash
  $ docker volume create portainer_data
  $ docker run -d -p 8000:8000 -p 9000:9000 --name=portainer --restart=always -v /var/run/docker.sock:/var/run/docker.sock -v portainer_data:/data portainer/portainer-ce
  ```

Then go to <docker-CT-IP>:9000 to set up portainer.

### Install docker containers

- Docker compose

local->Stack->Add stack

- Docker cmd line

local->Container->Add container 

## Install homeassitant

### Install

One script in pve shell can help you install it

```bash
$ bash -c "$(wget -qLO - https://github.com/tteck/Proxmox/raw/main/vm/haos-vm-v4.sh)"
```

### Set up homeassitant

- Init

  you can access the manage page at `<ha-ip>:8123`
  
  Set your username or something, enable `Advanced mode` by click your avatar at the bottom-left of your screen.

- Install two add-ons in `settings->add-ons->add-on store`, which are `ssh` and `samba`

  If there is no add-ons, add repo `https://github.com/hassio-addons/repository`.

- Install `HACS`(add-on store)

  - via samba(tbc)
  
    config samba and start samba, open `\\<ha-ip>:8123` in your explore

  - via ssh

    start ssh

    `https://github.com/hacs-china`

    ```
    $ wget -q -O - https://install.hacs.xyz | bash -
    $ wget -O - https://hacs.vip/get | bash - # china
    ```

    restart HA, then add `HACS` integration in `settings->devices and services`, authorize on github follow the instruction.

- integrate MI devices

  click `HACS->integrations->Explore & Download repos`, choose `Xiaomi MIoT`, restart HA

  then add `Xiaomi MIoT` integration, now you can see all your devices, rename the devices at web 

- integrate to HomeKit

  add `HomeKit` integration, scan the QR code at notification panel to finish setting
