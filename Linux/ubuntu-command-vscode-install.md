#linux/ubuntu #windows/wsl #code-env/vscode 

## 使用一下命来添加源
```bash
$ curl https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > microsoft.gpg
$ sudo mv microsoft.gpg /etc/apt/trusted.gpg.d/microsoft.gpg
$ sudo sh -c 'echo "deb [arch=amd64] https://packages.microsoft.com/repos/vscode stable main" > /etc/apt/sources.list.d/vscode.list'
```

## 更新包缓存，然后安装code
```bash
$ sudo apt-get update
$ sudo apt-get install code
```

本来想在wsl中使用，结果安装好之后wsl提示我说只要在windows上安装了vsc，下载extension Remote-WSL就可以了，白忙一场
