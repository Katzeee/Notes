# 2021.5.15

## 如何在ubuntu中获取管理员权限

>1.Ubuntu中如何切换到超级用户（root）
>
>我们知道在其他linux操作系统中（如红帽操作系统）普通权限的用户想切换到root下，只需输入su命令，系统会提示输入root用户的密码，只要我们正确输入即可。
>
>然而在Ubuntu中我们并不清楚root的密码是多少（还曾以为自己安装时没记住）。其实在默认的情况下，Ubuntu是没有超级用户(superuser, root)的。
>
>但有些系统操作必须有超级用户的权限才能进行，那么该如何做呢？
>
>在Ubuntu 下切换到超级用户需要使用其他方法，主要有两种：
>
>1.    利用sudo -i命令  输入sudo -i ,提示输入当前用户密码，输入后即可获得root权限，这并不需要root的密码。但是它也有很多限制，在默认的情况下，只能在 5 分钟之内使用 root 权限。
>
>2.    如果想一直使用 root 权限，可使用sudo passwd root 命令重新设置一下root的密码。之后就可以自由使用su命令自由的切换了。
>
>2. 在VMware虚拟机中Linux在虚拟终端之间切换的方法
>
>在linux操作系统中当我们想在虚拟终端之间切换时，经常的做法是ctrl+Alt+F1~F6，然而当我们在VMware中安装安装Linux后，想在X Windows与虚拟终端之间切换时，却发现Ctrl+Alt+Fx不起作用，那么我们如何解决这个问题呢？
>
>总结起来有两种方法：
>
>方法1：我们只需要将VMware或者其他虚拟机软件的控制鼠标切换的热键修改一下，不再使用Ctrl+Alt即可，如改为Ctrl+Alt+Shift，VMware具体方法如下：中文版——选择编辑——参数——热键，选择 Ctrl+Alt+Shift即可。英文版选择edit-preferences-hotkeys选择 Ctrl+Alt+Shift即可。
>
>方法二
>
>也可以不修改热键，只要在切换时使用Ctrl+shift+Alt+Fx即可，如在x windows下打开的终端窗口中按住ctrl+shift+alt+F1即可打开tty1虚拟终端窗口。而后在个虚拟终端之间的切换只要使用Alt+FX即可。如按Alt+F7即可在从刚才的虚拟终端tty1中切换到刚才的x windows下。
# 2022.4.14
### 查找依赖so在哪个包里
apt-file
```bash
$ apt-file search libXmu.so.6
```
