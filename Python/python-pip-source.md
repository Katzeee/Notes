#code-env #python  
## 2023.08.12

Create a file with

```conf
[global]
index-url = http://mirrors.aliyun.com/pypi/simple/
[install]
trusted-host = mirrors.aliyun.com
```

### For windows

You can put it at `$HOME/pip/pip.ini` or `$HOME/AppData/Roaming/pip/pip.ini`

### For linux

You can put it at `$HOME/config/pip/pip.conf`.


## 2023.08.18

!!! Note Easier way

    Run command `pip config set global.index-url https://mirrors.aliyun.com/pypi/simple/`