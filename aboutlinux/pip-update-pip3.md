# 2022.4.22

今天用pip安装库，说搜不到那个版本，我想是不是因为pip是python2的不是python3的

报`WARNING: pip is being invoked by an old script wrapper. This will fail in a future version of pip`

重新建立软链接即可

```bash
$ sudo rm /usr/bin/pip
$ sudo ln -s /usr/bin/pip3 /usr/bin/pip
```