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

# Docker commands

```bash
--name=container_name
docker start <id> or <name> # continue to run a container
```