2023.05.06
---

# change mirror source

```
$ vi /etc/docker/daemon.json
{
  "registry-mirrors":["https://hub-mirror.c.163.com"]
}
$ systemctl daemon-reload
$ systemctl restart docker
```

# Cannot autolaunch D-Bus without X11 $DISPLAY

```
$ sudo apt install gnupg2 pass 
```