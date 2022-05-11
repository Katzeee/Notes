# 2022.5.9

To increase the selection time, run commands as following:

> https://askubuntu.com/questions/711062/how-to-modify-waiting-time-of-the-dual-boot-menu

```bash
$ sudo vi /etc/default/grub
$ sudo update-grub
```

Change the `GRUB_TIMEOUT` to whatever you want
