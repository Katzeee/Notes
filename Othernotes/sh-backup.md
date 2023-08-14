#windows 
## 2021.7.13

### 备份桌面某些脚本文件

```bash
# logitech.cmd
taskkill /im lghub.exe /f
taskkill /im lghub_agent.exe /f
taskkill /im lghub_updater.exe /f
start "" "C:\Program Files\LGHUB\lghub_agent.exe"
start "" "C:\Program Files\LGHUB\lghub_updater.exe"
start "" "C:\Program Files\LGHUB\lghub.exe"
```

```bash
# guitar pro.bat
@echo off
SET y=%date:~0,4%
SET m=%date:~5,2%
SET d=%date:~8,2%
ECHO %y%/%m%/%d%
DATE 2050/12/17
START "" "E:\Guitar Pro 7\GuitarPro7.exe"
timeout /t 5
DATE %y%/%m%/%d%&&
```





