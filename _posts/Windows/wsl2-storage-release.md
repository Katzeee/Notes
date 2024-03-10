#windows/wsl #code-env 
## 2022.4.24

WSL2使用虚拟硬盘(VHD)存储linux下的文件，随着Linux下文件越来越多，占用空间也会不断增长，不过有个最大限制256G。但是，在Linux中减少文件占用，WSL却没有相应的减少硬盘空间的占用。所以为了避免碰到256G的限制，或者硬盘空间告警，在删除掉linux下的文件后，我们需要手动释放这部分空间。

- 要找到wsl的vhd文件: ext4.vhdx(%PROFILE%\AppData\Local\Packages\CanonicalGroupLimited.UbuntuonWindows_79rhkp1fndgsc\LocalState\).

- 打开PowerShell, 输入命令：

```bash
$ wsl --shutdown
$ diskpart
# open window Diskpart
$ select vdisk file="C:\…\ext4.vhdx"
$ attach vdisk readonly
$ compact vdisk
$ detach vdisk
$ exit
```