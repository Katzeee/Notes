# 2022.4.22

Cannot find the higher version of libraries.

`WARNING: pip is being invoked by an old script wrapper. This will fail in a future version of pip`

Rebuild a symbol link. Or install `python-pip3`

```bash
$ sudo rm /usr/bin/pip
$ sudo ln -s /usr/bin/pip3 /usr/bin/pip
```