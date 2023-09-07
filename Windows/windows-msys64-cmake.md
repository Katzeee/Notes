#c-cpp/cmake #code-env/vscode #code-env 
## 2022.5.4

## 下载msys2
> https://www.msys2.org/

## 安装

先安装，将`D:\msys64\mingw64\bin`添加到PATH

执行下列命令
```bash
$ pacman -Syyu
$ pacman -S --needed base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-make
```
---
Ignore!!

## 下载安装cmake

> https://cmake.org/download/

将`C:\Program Files\CMake\bin`添加到PATH

## vscode配置

在setting.json中加入

```json
 // camke
 "cmake.cmakePath": "D:\\cmake-3.23.1-windows-x86_64\\bin\\cmake.exe",
 "cmake.mingwSearchDirs": [
   "D:\\msys64\\mingw64\\bin"
],
"cmake.generator": "MinGW Makefiles",
```
