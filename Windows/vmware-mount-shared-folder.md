#windows #code-env 
## 2022.08.29

Mounting VMware shared folders in Arch linux via command line:

No need to install vmware tools via cdrom i.e. `VM->install VMware tools` in VMware workstation.

```bash
$ sudo pacman -S open-vm-tools
$ sudo mkdir /mnt/hgfs
$ sudo vmhgfs-fuse .host:/ /mnt/hgfs -o subtype=vmhgfs-fuse,allow_other
# or 
# $ sudo vmhgfs-fuse .host:/ /mnt/hgfs/ -o allow_other -o uid=1000
```