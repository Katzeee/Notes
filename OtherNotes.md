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

# 2021.6.3

### 油猴脚本备份

```javascript
// ==UserScript==
// @name         百度搜索去广告+百度页面美化+支持百度黑色主题【代码精简高效】
// @description  代码精简，无垃圾推广！快速去除百度结果页的顽固广告和右边栏，适当放大了界面，采用居中布局，美化了字体等样式，页面显示更加美观，支持随时开启/关闭黑色主题；为了精简代码以及提高性能，没有加任何定时器脚本，没有任何购物推荐啥的，最大程度减少系统资源消耗。
// @icon         http://baidu.com/favicon.ico
// @namespace    https://greasyfork.org/zh-CN/users/393603-tsing
// @version      3.8
// @author       Tsing
// @run-at       document-start
// @include      *://www.baidu.com/s?*
// @include      *://ipv6.baidu.com/s?*
// @include      *://www.baidu.com/baidu?*
// @include      *://ipv6.baidu.com/baidu?*
// @include      *://www.baidu.com/
// @include      *://ipv6.baidu.com/
// @require      https://cdn.bootcss.com/jquery/3.4.1/jquery.min.js
// @grant        GM_setValue
// @grant        GM_getValue
// @grant        GM_registerMenuCommand
// @copyright    该脚本完全由 Tsing@greasyfork 原创，谢绝抄袭部分或全部代码！如有借鉴代码，请声明并标注脚本链接。如发现有人抄袭，欢迎举报，谢谢。
// ==/UserScript==

(function() {
    /* global $ */
    'use strict';
    var style_tag = document.createElement('style');
    var body_zoom_ratio = 1.15;
    var body_zoom_original = GM_getValue("body_zoom_original"); // 用户可以指定采用原来100%页面缩放，字体变小，内容变窄。
    if(body_zoom_original){body_zoom_ratio = 1.0;}else{body_zoom_ratio = 1.15;}
    var white_style = 'body{zoom:' + body_zoom_ratio +';} #s_main, .s-top-nav, .s-hotsearch-wrapper, #content_right{display:none;} #content_left{padding-left:0 !important; width:1000px !important; margin:0 auto; float:none;} #rs{padding:0 !important; margin:10px auto !important; width:900px !important;} #rs table{width:640px !important;} #page .page-inner{padding-left:0 !important; display:block; width:900px !important; margin:0 auto !important;} #page a{margin-right:20px !important;} .wrapper_new #foot #help{display:block; width:900px; margin:0 auto !important; float:none !important; padding-left:unset !important;} #content_left a, #rs a, .c-title-text, .c-showurl{color: #3C50B4; text-decoration: none !important; } .b2b-universal-card .official-site{color: #3C50B4 !important;} .se_st_footer a{color:#008000;} .m{color:#666666 !important;} em{color: #FA3232 !important; text-decoration: none !important;} .t a, .c-title-text{font-size: 18px !important;} .jy-course-pc-title .c-title-text{color: #3C50B4 !important;} h3.c-title.c-font-medium.c-color-link.c-line-clamp2 span.c-title-text{font-size:14px !important;} .slowmsg{left:300px !important; top:90px !important; box-shadow:none !important; border:none !important; background:none !important;} a.c-text{color:#ffffff !important; font-size:0.8em !important;} #kw{font-size: 1.5em !important;} .search_tool_conter, .nums{width:900px !important; margin:0 auto !important;} #rs_top_new, .hit_top_new{width:900px !important; margin:0 auto !important;} .c-result-content article{width: 100% !important; padding: 0 !important; box-shadow: none;} .c-result-content article:hover{box-shadow:none;} .c-border{box-shadow:none !important; width:880px;} .op-img-portrait-menu .op-img-portrait-text-public{color:#ffffff !important;} .head_wrapper{width: 1196px; margin: 0 auto; position: relative;} #container{box-sizing: border-box; width: 1000px; margin: 0 auto;} .c-border.ec-pl-container {width: 900px; margin: 0 auto; margin-bottom: 15px; padding: 15px; border-radius: 15px; box-shadow: 0 0 4px #eeeeff !important; border: none; display:none;} .op-img-address-link-type{margin-right:10px;} .op-img-address-pbline{margin-top:10px !important;} .c-span18{width:760px !important;} .c-span24{width:890px !important;} #s_tab.s_tab{padding-left:0 !important;} @media screen and (min-width: 1921px){#s_tab.s_tab{margin-left:-96px;}} #s_tab.s_tab .s_tab_inner{display: block; box-sizing: border-box; padding: 0; width: 900px; margin: 0 auto; padding-left:0 !important;} .op-img-address-link-type a{margin-right:10px !important;} .op-img-portrait-item-con{padding:5px;} .c-border .c-span6{margin-bottom:10px;} .c-border .c-span-last{margin-right:10px;} .op-img-portrait-pic-more{text-align:left !important;} .op_exactqa_tag_item{color:#3C50B4 !important;} span.op_exactqa_tag_item.op_exactqa_tag_selected.OP_LOG_BTN{color:#ffffff !important;} .wenda-abstract-wrap{margin-bottom:0 !important; border:none !important;} .hint_common_restop{width:900px !important; margin:0 auto !important;} .wenda-abstract-img-wrap{display:none;} #content_left .c-group{width:900px !important; margin-bottom: 15px !important; padding: 10px 15px 15px 15px !important; border: none !important;} .op-short-video-pc-img-group{max-height:none !important;} .result.new-pmd, #content_left>.c-container.result, #content_left .result-op{ width: 900px !important; word-break: break-all; word-wrap: break-word; box-shadow: 0 0 6px #eeeeff; padding: 10px 15px 15px 15px; margin:0 auto; margin-bottom:15px; border-radius: 10px; transition:box-shadow 0.5s, border-radius 0.5s, margin-bottom 0.6s, padding-bottom 0.6s;} .result:hover, #content_left .result-op:hover{box-shadow:1px 1px 10px #cccccc; border-radius:0;} .sftip_com.sftip_blue{width:900px; margin:0 auto; margin-bottom:15px; border-radius:10px; border:none; background-color:#ffeeee;} .sftip_com span{text-indent:0 !important;} #header_top_bar, .tab-wrapper, #gotoPage, p#page{width:900px !important; margin:0 auto; padding-left:0 !important;} #header_top_bar .nums{width:700px !important;} #gotoPage{padding-bottom:0;} p#page{padding:0;} form.fm{font-size:11px;} .op-gk-topic-header-imgc, .op-gk-topic-banners{display:none !important;} div[class$="op_rs"]{width:900px !important; margin:0 auto; margin-bottom:15px;} div[class$="op_rs_left"]{width:auto;} table.result-op{display: block; padding: 15px !important; margin-bottom: 15px !important;} #op_wiseapp{box-shadow:none !important;} a.c-tip-icon{display:none;} .c-border .c-span18{width:735px !important;} #container.sam_newgrid{margin:0 auto !important; padding-left:0 !important;} .new-pmd .c-span9{width:756px;} .new-pmd .c-span12{width:unset;}';
    var dark_style = 'body{zoom:' + body_zoom_ratio +'; color:#eeeeee; background-color:#1a1a1a !important;} #rs{background-color:#1a1a1a !important;} .wrapper_new #head, .foot-inner, .wrapper_new .container_new~#page{background-color:#222222;} #s_main, .s-top-nav, .s-hotsearch-wrapper, #content_right{display:none;} #content_left{padding-left:0 !important; width:1000px !important; margin:0 auto; float:none;} #rs{padding:0 !important; margin:10px auto !important; width:900px !important;} #rs table{width:640px !important;} #page .page-inner{padding-left:0 !important; display:block; width:900px !important; margin:0 auto !important;} #page a{margin-right:20px !important; background-color:#333333 !important;} .wrapper_new #foot #help, #foot #help{display:block; width:900px; margin:0 auto !important; float:none !important; padding-left:0 !important;} #content_left h3 a, .subLink_answer a, #rs a, .b2b-universal-card a, .c-title-text{color: #eeeeee !important; text-decoration: none !important; } .se_st_footer a{color:#008000;} .m{color:#aaaaaa !important;} .c-container{background:none !important;} .c-container em, .result em, .result-op em{color: #eeeeee !important; text-decoration: none !important;} .t a{font-size: 18px !important;} .c-border.ec-pl-container {width: 900px; margin: 0 auto; margin-bottom: 15px; padding: 15px; border-radius: 15px; border: 1px solid #222222 !important; display:none;} a.c-text{color:#ffffff !important; font-size:0.8em !important;} #kw{font-size: 1.5em !important; color:#ffffff;} .bdsug.bdsug-new, .bdsug.bdsug-new.bdsugbg{background-color:#222222 !important;} .wrapper_new #form .bdsug-new ul li{color:#cccccc;} .wrapper_new #form .bdsug-new ul li b{color:#cccccc;} .wrapper_new #form .bdsug-new ul{border-top:2px solid #333333;} .soutu-env-result #form .s_ipt_wr{border-color:#444444;} .wrapper_new .s_ipt_wr{overflow:hidden !important; border-color:#444444;} .wrapper_new #u>a{color:#eeeeee;} .c-table td{border-bottom:1px solid #222222;} .c-table th{border-color:#222222; background:none;} .head_nums_cont_inner{margin-top:5px;} .search_tool_conter, .nums{width:900px !important; margin:0 auto !important;} .wrapper_new #s_tab a{color:#cccccc;} .wrapper_new #s_tab .s-tab-item:hover, .wrapper_new #s_tab .s-tab-item:hover:before{color:#ffffff;} .wrapper_new #s_tab .cur-tab, .wrapper_new #s_tab .cur-tab:before{color:#ffffff;} .wrapper_new #page{background-color:#1a1a1a;} .c-container a{color:#cccccc; text-decoration:none;} #rs_top_new, .hit_top_new{width:900px !important; margin:0 auto !important;} .c-result-content article{width: 100% !important; padding: 0 !important; box-shadow: none;} .c-result-content article:hover{box-shadow:none;} .c-border{box-shadow:none !important; border:none; width:880px;} .wrapper_new .container_new~#page a, .wrapper_new #page a{color:#5599ff; background-color:#282828;} #foot, #foot #help{background-color:#222222 !important; border-top:none;} .wa-zp-exact-new-current{background-color:#444444 !important;} .wa-zp-exact-new-border{border:1px solid #444444 !important;} .wa-zp-exact-new-topline{border-top:1px solid #444444 !important;} .wa-zp-exact-new-table{background:none !important;} .wa-zp-exact-new-bline{border:none !important;} .wa-zp-exact-new-gray-a, .wa-zp-exact-new-color{color:#eeeeee !important;} .wa-zp-exact-new-aurl{color:#8888ff !important;} .search_tool:hover, .search_tool_conter span:hover{color:#cccccc !important;} .op_dict3_inlineblock, .car-pc-series-color, .car-pc-link, .car-pc-link-diff, .car-pc-series-more{color:#cccccc !important;} .car-pc-series-table{background:none !important;} .car-pc-list{border-color:#444444 !important;} .car-pc-series-bline{border-bottom-color:#444444 !important;} .op_dict3_else_items a, span.op_dict_text2, .op_dict_text2 a, .op_dict_table td{color:#cccccc !important;} .op-img-portrait-menu .op-img-portrait-text-public{color:#ffffff !important;} .head_wrapper{width: 1196px; margin: 0 auto; position: relative;} #container{box-sizing: border-box; width: 1000px; margin: 0 auto;} .op-img-address-link-type{margin-right:10px;} .op-img-address-pbline{margin-top:10px !important;} .c-span18{width:760px !important;} .c-span24{width:890px !important;} #s_tab.s_tab{padding-left:0 !important;} @media screen and (min-width: 1921px){#s_tab.s_tab{margin-left:-96px;}} #s_tab{background-color:#222222;} #s_tab.s_tab .s_tab_inner{display: block; box-sizing: border-box; padding-left:0 !important; width: 900px; margin: 0 auto; margin-top:10px;} @media screen and (min-width: 1921px){#s_tab.s_tab .tab-wrapper{margin-top:10px !important;}} .c-showurl{color:#88ff88;} .nvl-bookstore-wrap .chapter-list{border-top:solid 1px #444444 !important;} .nvl-bookstore-wrap .chapter-item{border-bottom:solid 1px #444444 !important;} .nvl-bookstore-wrap .chapter-title, .nvl-bookstore-wrap .bookinfo-text{color:#cccccc !important; text-decoration:none !important;} .op-img-address-link-type a{margin-right:10px !important;} .op-img-portrait-item-con{padding:5px;} .c-border .c-span6{margin-bottom:10px;} .OP_LOG_BTN{background-color:#1a1a1a !important; color:#cccccc !important;} .op-stockdynamic-moretab-map-tip, .op-stockdynamic-moretab-map, .op-stockdynamic-moretab-map-tag{background-color:#222222 !important;} .op-stockdynamic-moretab-map-tip{border:none !important;} .op-stockdynamic-moretab-tabs-nav .op-stockdynamic-moretab-tabs-nav-selected{border:1px solid #444444 !important;} .op-stockdynamic-moretab-tabs-nav li{border-left:1px solid #444444 !important;} .c-tabs-nav .c-tabs-nav-selected{border-color:#444444 !important;} .c-tabs-nav{background-color:#1a1a1a !important; border:none !important;} .c-tabs-nav li{border-width:2px !important;} .c-tabs-nav .c-tabs-nav-sep{border:none !important;} .op-video-vast-ul li.op-video-vast-smallicon-li, .op_caipiao_idea_content ul li{border-bottom:1px solid #222222 !important;} .op_caipiao_idea_Title a{color:#eeeeee !important; font-weight:bold;} .op-video-vast-border{border:none !important;} .op-video-vast-ul li.op-video-vast-smallicon-li a span i{color:#eeeeee !important;} .op-img-portrait-pic-more{text-align:left !important;} .wenda-abstract-wrap{margin-bottom:0 !important; border:none !important;} #bottom_layer, .s-top-wrap{background:none !important;} .c-color-gray2, .c-color-t{color:#cccccc !important;} #head_wrapper #kw{background-color:#222222 !important; border-color:#444444; color:#eeeeee;} #head_wrapper #form .bdsug-new ul li, #head_wrapper #form .bdsug-new ul li b{color:#cccccc !important;} #head_wrapper #form .bdsug-new ul{border-top:2px solid #444444;} #head_wrapper .soutu-btn{background-color:#222222;} .op_dict3_extra{border-top:1px solid #222222 !important;} .op-mapcity-gray{background-color:#222222; color:#cccccc !important; border-color:#444444;} .c-input:hover{box-shadow:none;} .f13 a, .c, .c-row span, .g, a.g, .c-showurl, .c-text-info, .b2b-universal-card .official-site, .op_exactqa_s_abstract_more a{color:#eeeeee !important;} .op_exactqa_title, .op-trade-purchase-content-title, .op-img-portrait-menu a{color:#cccccc !important;} .op-trade-purchase-content li, .op-trade-purchase-table{border-bottom-color:#222222 !important;} .opui-page-more, .opui-page-number-now{color:#cccccc !important;} #_mask{opacity:0 !important;} .wenda-abstract-img-wrap{display:none;} .wenda-abstract-short-answer h3{color:#cccccc !important;} #head_wrapper .under-tips .links-link{color:#cccccc;} .op-timeliness-abs, .op_sp_realtime_subabs{color:#cccccc !important;} .c-divider .thin{background-color:#444444 !important;} .op-documents-tr-first{border-color:#444444 !important;} #content_left .c-group{width:900px !important; margin-bottom: 15px !important; padding: 10px 15px 15px 15px !important; border: 1px solid #222222 !important;} .op_sp_realtime_subabs em{color:#cccccc !important;} .op-short-video-pc-img-group{max-height:none !important;} .result.new-pmd, #content_left>.c-container.result, #content_left .result-op{ width: 900px !important; background:none !important; word-break: break-all; word-wrap: break-word; border:1px solid #333333; padding: 10px 15px 15px 15px; margin:0 auto; margin-bottom:15px; border-radius: 10px; transition:margin-bottom 0.6s, padding-bottom 0.6s;} .result:hover, #content_left .result-op:hover{border:1px solid #444444; border-radius:0; transition:border-radius 0.5s, border-color 0.5s ease-in-out;} table.result-op{display: block; padding: 15px !important; margin-bottom: 15px !important;} #op_wiseapp{box-shadow:none !important; border-color:#444444 !important;} #header_top_bar, .tab-wrapper, #gotoPage, p#page{width:900px !important; margin:0 auto; padding-left:0 !important;} .tab-wrapper{margin-top:10px;} #gotoPage{padding-bottom:0;} p#page{padding:0;} form.fm{font-size:11px;} .wrapper_new .sam_newgrid~#page, .s_ipt_wr{background-color:#222222;} .fm .s_ipt_wr{border-color:#444444 !important;} .soutu-env-index span.bg.s_ipt_wr.quickdelete-wrap{border:none !important;} .c-summary.c-row{color:#eeeeee;} #head{background-color:#222222; border-bottom:none;} .bdsug{width:539px; background-color:#222222; border:1px solid #444444; box-shadow:none; background-image:none !important;} .bdsug li{color:#cccccc;} .bdsug li.bdsug-s{background-color:#333333; width:518px;} .s_btn:hover, .s_btn.btnhover{box-shadow:none;} #u>a{color:#cccccc;} #s_tab a{color:#888888;} #s_tab a:hover, #s_tab b{color:#eeeeee;} #gotoPS{color:#cccccc;} .PS-word{color:#cccccc;} #page a, #page strong{background:none;} #page .pc{background-color:#333333; border:1px solid #444444; color:#cccccc;} #head.s_down{box-shadow: 0 0 5px #1a1a1a;} #page strong .pc, #page .n:hover, #page a:hover .pc{background:none;} #page .n{border:none; color:#cccccc;} .nors .tip_head, .nors li, .content_none .nors p{color:#cccccc;} .nors p em{color:#ff5555;} .nors li a{color:#5555ff;} .op-gk-topic-body-border{background:none !important; border:none !important;} .op-gk-topic-title{color:#eeeeee !important;} .op-gk-topic-bigdata-item .title, .op-gk-topic-news-item .content{color:#cccccc !important;} .op-gk-topic-header-imgc, .op-gk-topic-banners{display:none !important;} .tab-nav-name{background:none !important; border-bottom:2px solid #444444 !important;} .tabs-split{border-color: transparent !important;} div[class$="op_rs"]{width:900px !important; margin:0 auto; margin-bottom:15px;} div[class$="op_rs_left"]{width:auto;} div[class$="op_rs"] a{color:#cccccc;} .c-recommend{border-color:#222222;} img.nor-src-icon{border:none;} p.c-gap-top-small{color:#cccccc;} .op_jingyan_list_hide, .op_jingyan_list_showmore{border-top-color:#222222 !important;} .c-tabs-item{background:none !important;} .c-tabs-nav .c-tabs-nav-selected{background-color:#1a1a1a !important; color:#eeeeee;} .c-btn{background:none !important; border-color:#444444;} .c-btn, .c-btn:visited{color:#cccccc !important;} #container.sam_newgrid .result .c-tools .c-icon, #container.sam_newgrid .result-op .c-tools .c-icon{color:#444444;} a.c-tip-icon{display:none;} .c-text-public, .tab-tvshow-link-newest{background-color:#444444 !important;} .tabs-li > li.tabs-li-active{background:none !important; border:2px solid #444444 !important; color:#cccccc !important;} .op-calendar-pc-table-rest{background-color:#604444 !important;} .c-dropdown2 .c-dropdown2-btn-icon, .op-calendar-pc-prev-month, .op-calendar-pc-next-month{background-color:unset !important;} .dropdown-btn-group{color:#cccccc;} .dropdown-menu{background-color:#1a1a1a !important;} .dropdown-menu-inner-ul > li:hover, .dropdown-menu-inner-ul > li.active{background-color:#444444 !important;} .op-calendar-pc-daynumber{color:unset !important;} .c-border .c-span18{width:745px !important;} .sftip_com.sftip_blue{width:900px; margin:0 auto; margin-bottom:15px; border-radius:10px; border:none; background-color:#222222;} .sftip_com span, .sftip_com span a, .sftip_com td strong{color:#cccccc !important; text-indent:0 !important;} .op_caipiao_idea, .med-qa .c-line-bottom{border-bottom-color:#444444 !important;} div#3001, div#3002, div#6001{display:none !important;} .slowmsg{left:300px !important; top:90px !important; box-shadow:none !important; border:none !important; background:none !important;} .jy_course_border__HotjI{background:none !important; border:none !important;} .jy_course_border__HotjI .tabs-bar{background:none !important; border-bottom:1px solid #444444 !important;} .jy_course_border__HotjI .tabs-active{color:#cccccc !important; border:2px solid #444444 !important; background:none !important;} #container.sam_newgrid{margin:0 auto !important; padding-left:0 !important;} .new-pmd .c-abstract, .new-pmd.c-container, .new-pmd .c-color-text{color:#eeeeee;} .new-pmd .c-span9{width:756px;} .new-pmd .c-span12{width:unset;} #container.sam_newgrid .hint_common_restop{width:900px; margin:5px auto; background-color:#884444; color:#eeeeee; padding:2px 10px; border-radius:30px;} #header_top_bar .nums{width:700px !important;}';
    style_tag.innerHTML = white_style;
    document.head.appendChild(style_tag);
    document.addEventListener ("DOMContentLoaded", kill_baidu_ad);
    function kill_baidu_ad () {
        unsafeWindow.$(document).ajaxSuccess(function(e, xhr, opt) { // Microsoft Edge 和 Safari 不支持 $(document).ajaxSuccess()
            document.head.appendChild(style_tag);
            $('#content_left>div').has('span:contains("广告")').remove();
            setTimeout(function () { $('.c-container').has('.f13>span:contains("广告")').remove(); }, 2100); // 去除顽固性的延迟加载广告，一般延迟2秒左右。例如搜索“淘宝”，当页面加载完毕之后在搜索结果最前或最后会再插入一个广告。
            try{
                $(".product-item.c-span6.first-line-product").attr("class", "product-item c-span6"); // 全网热卖商品展示区布局错误，例如搜索：iPhone
                if(document.getElementsByClassName("op-img-portrait-con").length){
                    $(".c-span6.c-span-last.op-img-portrait-item-con").removeClass("c-span-last"); // 百度图片的重新排列，例如搜索：头像
                    var arr = $(".op-img-portrait-con .c-row.c-gap-top"); var html = [];
                    for(var i=0;i<arr.length;i++){
                        html[i] = arr[i].innerHTML; if(i > 0){ arr[i].remove(); }
                    }
                    arr[0].innerHTML = html.join(" ");
                }
            }catch(e){}
        });
        if(body_zoom_ratio == 1.15){
            $("#s-usersetting-top").mouseover(function(){ // 由于body放大，导致首页上的设置弹出框位置错乱，百度的UI设计能不能走点心。。。
                var offset_right = $(document).width()/1.15 - $("#s-usersetting-top").offset().left - $("#s-usersetting-top").width()/2 - $("#s-user-setting-menu").width()/2; // 计算公式参考百度官方代码：https://ss1.bdstatic.com/5eN1bjq8AAUYm2zgoY3K/r/www/cache/static/protocol/https/home/js/nu_instant_search_7422ef2.js
                setTimeout(function () { $("#s-user-setting-menu").css("right", offset_right); }, 0);
            });
        }
        $("#head_wrapper").addClass("s-ps-islite"); // 登录了账号之后首页有可能出现资讯信息流，以下强制关闭。
        if(document.getElementsByClassName("hide-feed").length){ $("#s-user-setting-menu .show-feed:first").css("display", "none"); setTimeout(function () { document.getElementsByClassName("hide-feed")[0].click(); }, 1000); } // 不允许开启资讯信息流，原生click()才可以实现
        if(document.getElementsByClassName("set-hide").length){ setTimeout(function () { document.getElementsByClassName("set-hide")[0].click(); $("#s-user-setting-menu .set-show:first").css("display", "none");}, 1000); } // 关闭热榜
    }
    var dark_mode = GM_getValue("dark_mode_on"); // 百度黑色主题
    if(dark_mode){ style_tag.innerHTML = dark_style; }else{ style_tag.innerHTML = white_style; }
    GM_registerMenuCommand ("点击切换：明亮模式/黑色模式", dark_func, "");
    function dark_func () { if(dark_mode){ GM_setValue("dark_mode_on", false); location.reload(); }else{ GM_setValue("dark_mode_on", true); location.reload(); } }
    GM_registerMenuCommand ("点击切换：页面缩放大小", change_zoom, "");
    function change_zoom () { if(body_zoom_original){ GM_setValue("body_zoom_original", false); location.reload(); }else{ GM_setValue("body_zoom_original", true); location.reload(); } }
    GM_registerMenuCommand ("打赏支持脚本开发者", menu_func, "");
    function menu_func () { window.open("https://greasyfork.org/zh-CN/users/393603-tsing"); }

})();
```

# 2021.6.11

### cpp关键字using作为别名

```c++
typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS;
using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;
```

```c++
typedef void (*FP) (int, const std::string&);
using FP = void (*) (int, const std::string&);
```

```c++
typedef std::string (Foo::* fooMemFnPtr) (const std::string&);
using fooMemFnPtr = std::string (Foo::*) (const std::string&);
```

```c++
template <typename T>
using Vec = MyVector<T, MyAlloc<T>>;
// usage
Vec<int> vec;

//typedef做不到
```

# 2021.7.13

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

# 2021.9.2

### 报文，数据报，分片，分组，帧的区别

>**报文（message）**：传输协议交给ip的数据称为报文；典型的报文包含一个传输层首部和应用程序数据；
>
>**数据报（datagram）**：在报文首部加上ip首部形成一个数据报；
>
>**分片（fragment）**：如果在选定网络中，数据报的长度太大，ip就会把数据报分裂成几个分片，每个分片含有它自己的ip首部和一段原来数据报的数据；
>
>**分组（packet）：**提交给数据链路层进行传送时，一个ip分片或者一个很小的无需分片的ip数据报成为分组；
>
>**帧（frame）：**数据链路层在分组前面加上它自己的首部形成帧，并发送该帧；

<p align="left">
  <img src="images/2020040109441427.png">
</p>
