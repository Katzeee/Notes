2022.10.15
---

## install scoop

```bash
# 安装scoop
$ Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
$ iwr -useb get.scoop.sh | iex
$ .\install.ps1 -ScoopDir 'D:\Scoop' -ScoopGlobalDir 'D:\Scoop\Global'

# 开始安装软件
$ scoop config proxy 127.0.0.1:8889
$ scoop install aria2
$ scoop install sudo
# scoop install <软件名>
# global目录下安装：scoop install -g <软件名>
```

## install oh-my-posh

```bash
$ scoop install oh-my-posh
$ code $Profile
```

Put these into 

$ oh-my-posh init pwsh --config "$(scoop prefix oh-my-posh)\themes\amro.omp.json" | Invoke-Expression

