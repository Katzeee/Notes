# 2021.3.29

### git命令

```bash
git status #查看文件状态
git add . #添加所有文件到暂存区
git commit -m "message" #提交暂存区内容到本地仓库
git log #查看当前版本之前提交记录
git reflog #查看所有提交记录
git reset --hard 版本号 #回退到某个版本
git branch -v #查看分支
git branch 分支名 #创建分支
git checkout 分支名 #切换分支
git branch -a #查看所有分支
git merge 分支名 #将该分支合并到当前工作分支
git mv 原文件名 现文件名/文件夹名 #重命名文件，或移动文件位置
```

### vim命令

```bash
进入编辑模式，按i进行编辑
编辑结束，按ESC键 跳到命令模式，然后输入退出命令：
:w保存文件但不退出vi 编辑
:w! 强制保存，不退出vi 编辑
:w file将修改另存到file中，不退出vi 编辑 
:wq保存文件并退出vi 编辑
:wq!强制保存文件并退出vi 编辑
q:不保存文件并退出vi 编辑
:q!不保存文件并强制退出vi 编辑
:e!放弃所有修改，从上次保存文件开始在编辑
```

### linux命令

```bash
cd .. #返回上级目录，注意有空格
touch <文件名> #创建文件
mkdir <文件夹名> #创建文件夹
```

### git一个新项目

```shell
# 需要先创建bare文件夹
$ git remote add origin xjf@106.15.43.98:/data/gitrepos/notes/notes.git
$ git push -u origin master #提交
```

# 2021.5.13

### spy++消息查看

<p align="left">
  <img src="images/201708221503403013143025.jpg">
</p>

- 第一条：<00001> 00130042 S WM_SETCURSOR hwnd:00130042 nHittest:HTCLIENT wMouseMsg:WM_MOUSEMOVE

<00001>：是SPY++捕获到的消息序号，主要是它记录顺序用。

00130042：这是目标窗口句柄的16进制。

S：这里第一行是S，第二行是R，第三行是P。一起说了。S 是指使用 SendMessage 发送的消息，R 是使用SendMessage发送消息后得到的返回值。P 是使用PostMessage 发送的消息。

WM_SETCURSOR：发送的消息值，也就是 SendMessage 的第二个参数（PostMessage 参数与SendMessage的一样）。

从这里往后，就分别是 SendMessage 的第三个参数和第四个参数的值了，但是它们不是固定对应的，是根据具体消息值不同而不同的。

第一条在消息值 WM_SETCURSOR 之后，是 hwnd:00130042，可以看出这是一个句柄，也就是目标窗口的句柄。同时这里是 SendMessage 的第三个参数。

然后在 hwnd:00130042 的后面有两个数据，而我们的SendMessage只剩最后一个参数了，所以后面的两个数据都是SendMessage的最后一个参数。一个低位，一个高位。

SPY++捕获的第一条消息，要使用代码发送实现一样的数据的话，就应该是：

```c++
::SendMessage(00130042, WM_SETCURSOR, 0x00130042, MAKELPARAM(HTCLIENT, WM_MOUSEMOVE));
```

- 第二条：<00002>00130042 R WM_SETCURSOR fHaltProcessing:False

这条不需要我们操作，我们使用SendMessage发送了之后，就会有类似的返回值回来。

- 第三条：<00003> 00130042 P WM_MOUSEMOVE fwKeys:0000 xPos:1088 yPos:27

我们直接说 WM_MOUSEMOVE 之后的东西就行了，WM_MOUSEMOVE包含之前的看上面的第一条。

WM_MOUSEMOVE 之后是 fwKeys:0000 ，因为WM_MOUSEMOVE是PostMessage的第二个参数（为什么是PostMessage而不是SendMessage?上面有说到）

所以这个fwKeys:0000就是PostMessage的第三个参数了，而且值是 0 ，我们不用看前面的 fwKeys ,只需要看 : 后面，它的值就行了。

fwKeys:0000 是第三个参数了，那它之后的就是第四个参数。

同样，还有两个数据，低位和高低。

最后，这条捕获的消息用代码实现就是：

```c++
::PostMessage(0x00130042, WM_MOUSEMOVE, 0, ::MAKELPARAM(1088, 27);
```

> VS2005 我在MFC单文档程序里，想获取View窗体的上下左右四个坐标值，于是使用GetClientRect(**LPRECT** lp)来获取，可是出了问题，问题如下：
> **LPRECT** lp; GetClientRect(lp);
> long viewLength = lp->right-lp->left; long viewHeight = lp->bottom-lp->top;
> 首先，我看GetClientRect方法里带的参数是**LPRECT** lp指针，于是想当然的在前面定义了一个，编译的时候并没有报错，可是在运行的时候报错Run-Time Check Failure #3 - The variable 'lp' is being used without being defined.
> CRect rc; GetClientRect(&rc);
> long viewLength = rc.right-rc.left; long viewHeight = rc.bottom-rc.top;
> 这是后来修改用的方法 定义一个CRect rc类 把rc的地址作为GetClientRect方法的参数，这样就可以正确的运行了。
> 可是一直不明白 为什么不能直接用开始时定义的**LPRECT** lp来作为GetClientRect方法的参数呢？望知道的告诉下小弟 不胜感激

**LPRECT** lp只是定义了一个DWORD的值，这个值你可以叫指针，也可以叫数值。但其实把这个DWORD当做地址来用时它并不是一个有效地址。这种指针又叫野指针，它不指向任何内存块。所以运行一定会错。

# 2021.5.14

## 托盘程序创建

```c++
不建窗口无法实现托盘消息的处理，随便建个窗口，隐藏了不就行了？
#define WM_NOTIFYICON (WM_USER+1)
#define IDM_SHOWMAIN 8577//托盘右键菜单的打开主界面选项
#define IDM_EXIT 24876//托盘右键菜单的退出选项
HMENU hpopupmenu;
NOTIFYICONDATA notifyicondata;
hpopupmenu=CreatePopupMenu();//创建一个弹出式菜单
//增加菜单项“关闭”，点击则发送消息WM_DESTROY给主窗口（已隐藏），将程序结束。
AppendMenu(hpopupmenu,MF_STRING,IDM_SHOWMAIN,TEXT("打开主界面"));
AppendMenu(hpopupmenu,MF_STRING,IDM_EXIT,TEXT("关闭"));
notifyicondata.cbSize=sizeof(NOTIFYICONDATA);
notifyicondata.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;//旗标NIF_ICON使hIcon成员有效，NIF_MESSAGE使uCallbackMessage成员有效，NIF_TIP使szTip成员有效
notifyicondata.hIcon=LoadIcon(hInst,MAKEINTRESOURCE(IDI_ICON));//拖盘图标
notifyicondata.hWnd=hwnd;//接收通知消息的窗口
strcpy(notifyicondata.szTip,TEXT("鼠标悬停在托盘图标上的提示信息"));
notifyicondata.uCallbackMessage=WM_NOTIFYICON;//自定义通知消息的ID
notifyicondata.uID=104;//任务栏图标的ID,作为通知消息的wParam发送
Shell_NotifyIcon(NIM_ADD,&ifyicondata);
```

vcpkg

https://blog.csdn.net/cjmqas/article/details/79282847

https://github.com/microsoft/vcpkg

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

# 2021.5.16

### 本地上传至github

```bash
$ git init
$ git add .
$ git remote add origin <url>
$ git pull
$ git push -u origin master -f #强制覆盖
```

