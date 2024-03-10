#code-env/dual-system 
## 2022.06.07

After I disabled the bitlocker, my grub cannot find the Windows booter.

Solution:

```bash
$ sudo os-prober # check whether there is a Windows boot file
# If there actually has, but didn't be recognized, you could do following steps
$ yay -S update-grub
$ sudo update-grub
```