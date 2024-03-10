#csharp #windows 
## 2021.5.14

## 托盘程序创建

```cpp
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
