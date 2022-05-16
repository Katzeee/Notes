# 2021.3.29
### linux命令

```bash
cd .. #返回上级目录，注意有空格
touch <文件名> #创建文件
mkdir <文件夹名> #创建文件夹
```
# 2021.5.15

## 如何在ubuntu中获取管理员权限

> 1.Ubuntu中如何切换到超级用户（root）
>
> 我们知道在其他linux操作系统中（如红帽操作系统）普通权限的用户想切换到root下，只需输入su命令，系统会提示输入root用户的密码，只要我们正确输入即可。
>
> 然而在Ubuntu中我们并不清楚root的密码是多少（还曾以为自己安装时没记住）。其实在默认的情况下，Ubuntu是没有超级用户(superuser, root)的。
>
>但有些系统操作必须有超级用户的权限才能进行，那么该如何做呢？
>
>在Ubuntu 下切换到超级用户需要使用其他方法，主要有两种：
>
>1.    利用sudo -i命令  输入sudo -i ,提示输入当前用户密码，输入后即可获得root权限，这并不需要root的密码。但是它也有很多限制，在默认的情况下，只能在 5 分钟之内使用 root 权限。
>
>2.    如果想一直使用 root 权限，可使用sudo passwd root 命令重新设置一下root的密码。之后就可以自由使用su命令自由的切换了。

# 2021.12.28

### Linux运行文件时报错：bash: $'\r': command not found

```
# 使用vim打开文件
vim aaa.sh
# 转换格式
:set ff=unix
# 保存文件
:wq
```
# 2022.4.14

### 查找依赖so在哪个包里

[ubuntu]

在使用apt install安装库的时候，经常会出现找不到依赖包的情况，这时候就可以使用apt-file去查找所需要的依赖包在哪个lib里，再进行下载安装

```bash
$ sudo apt-get install apt-file
$ sudo apt-file update
$ apt-file search libXmu.so.6
```
