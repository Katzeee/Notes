#linux
## 2022.5.9

> https://askubuntu.com/questions/711062/how-to-modify-waiting-time-of-the-dual-boot-menu

To increase the selection time, run commands as following:

```bash
$ sudo vi /etc/default/grub
$ sudo update-grub
```

Change the `GRUB_TIMEOUT` to whatever you want
