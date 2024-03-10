#linux/arch #code-env/dual-system
## 2020.4.20

在y7000的笔记本windows系统下安装双系统，虽然现在wsl非常方便，但说不定什么时候还是需要用到，因此进行记录。

## windows下的设置

- 在计算机->管理->磁盘管理里面分出一块磁盘空间，我分了80g

## 准备u盘启动盘

- 下载ubuntu-20.04.4-desktop-amd64.iso
- 将u盘格式化为FAT32，将iso文件解压进去

## bios设置

- secure boot disable
- usb boot enable
- 设置u盘启动
- 显卡设置为混合模式dynamic graphic

## 开始安装

- 在安装类型里面选择其他选项，手动分盘，选择空闲分区双击即可开始分盘，建立逻辑分区
- `/` 20G ext4	

  根目录
- `/swap` 物理内存两倍 ext4

  交换空间：交换分区相当于Windows中的“虚拟内存”，如果内存低的话（1-4G），物理内存的两倍，高点的话（8-16G）要么等于物理内存，要么物理内存+2g左右

- `/boot` 1G左右 ext4	

  建议：应该大于400MB或1GB Linux的内核及引导系统程序所需要的文件，比如 vmlinuz initrd.img文件都位于这个目录中。在一般情况下，GRUB或LILO系统引导管理器也位于这个目录；启动撞在文件存放位置，如kernels，initrd，grub

- `/home` 剩余全部 ext4	

  用户工作目录；个人配置文件，如个人环境变量等；所有账号分配一个工作目录

- `/tmp` 5G左右	ext4 正常不需要

  系统的临时文件，一般系统重启不会被保存。（建立服务器需要？）

- 之后一直下一步即可


## 删除双系统

- 首先在磁盘管理里将所有分给ubuntu的分区全部删除
- **用DiskGenius找到SYSTEM_DRV(0)下的EFI分区，将里面的ubuntu文件夹删除即可**
  
  如果不做这一步将导致ubuntu启动项没有删除进不了系统，当然也可以靠重装ubuntu系统来解决进不了系统的问题
