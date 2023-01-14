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

Boot your device and just press continue, maybe set your dns server address to `8.8.8.8` instead of your gateway address will be more robust.

Then you can login pve system from another pc's browser at `https://<your-ip-addr>:8006`, **https** not ~~http~~

### Config

Change mirrors

```bash
# /etc/apt/sources.list 
deb https://mirrors.aliyun.com/debian buster main contrib non-free
deb https://mirrors.aliyun.com/debian buster-updates main contrib non-free
deb https://mirrors.aliyun.com/debian-security buster/updates main contrib non-free # proxmox source
deb https://mirrors.ustc.edu.cn/proxmox/debian/pve buster pve-no-subscription
```

Annotate enterprise source

```bash
# /etc/apt/sources.list.d/pve-enterprise.list
# deb https://enterprise.proxmox.com/debian/pve buster pve-enterprise
```

> *Try to use pve-tools*

```bash
$ apt update && apt -y install git && git clone https://github.com/ivanhao/pvetools.git
```

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

## Install Arch

### Download iso file

Download iso file from 163 mirror into `local` storeage.

### Create new virtual machine

Set every options as defualt, except those about quantities(mem, disk, cpu cores) and select to use qemu agent.

Choose the iso file as cd/rom, then boot the machine.

### Start install

Change source mirror, then directly start install via one command:

```bash
$ reflector --country 'China' --age 12 --protocol https --sort rate --save /etc/pacman.d/mirrorlist
$ archinstall
```

Set all language related options to English(or there may some font problems). You can change it in `/etc/locale.gen` later.

Additional package: neovim ranger net-tools

Network: copy from iso

Additional repo: mutilib

### Rest configure

Static IP:

```bash
# /etc/systemd/network/**
Address: 192.168.x.x/24
Gateway: 192.168.x.x
DNS: 
```

## LXC container

### Install LXC container

Create CT, deselect `unprivilieged container`, about 64G disk, 2048 mem and 2048 swap, static ip. DNS domain is the smae as gateway, DNS servers set as blank.

### Setup gpu share

In pve: `vi /etc/lxc/<CT_ID>.conf`

Add following(Get args by `ls -l /dev/dri`):
```
lxc.cgroup2.devices.allow: c 226:0 rwm
lxc.cgroup2.devices.allow: c 226:128 rwm
lxc.cgroup2.devices.allow: c 29:0 rwm
lxc.mount.entry: /dev/dri dev/dri none bind,optional,create=dir
lxc.mount.entry: /dev/fb0 dev/fb0 none bind,optional,create=file
lxc.apparmor.profile: unconfined
lxc.cgroup.devices.allow: a
lxc.cap.drop:
```

Then start CT, you will see gpu by running `ls /dev/dri`.

### Change apt source

### Mount NAS

Or just use sftp which need to add a new user to connect to this LXC via ssh.

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

## Install windows and config GPU pass-through



### Install windows

System: q35, UEFI
Disk: SCSI
Network: virtIO

Change boot sequential, then install windows as usual

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

Edit `/etc/pve/qemu-server/<vm-id>.conf`, **I don't know which are unnecessary**, but it works.

```
args: -cpu 'host,-hypervisor,+kvm_pv_unhalt,+kvm_pv_eoi,hv_spinlocks=0x1fff,hv_vapic,hv_time,hv_reset,hv_vpindex,hv_runtime,hv_relaxed,kvm=off,hv_vendor_id=null'
```
