#c-cpp #compile 
## 2022.4.14

平时学习c的时候只写过很简单的程序，那么用gcc编译就只需要一句话即可，有时候都不用-o指定编译出来的文件
```bash
$ gcc main.c -o main
```
除此之外，还有-g生成调试文件，

其他对我来说都是知道没用过，如-Ox编译优化，-D定义宏，-I指定查找头文件目录等

包括以前使用vscode出现红色波浪线，只要编译能通过我一律手动忽视。今天开始学习cmake，调用第三方库才开始研究这些。

- `I`
  
  `-I`指定搜索头文件的目录，不加会找不到头文件报错，默认从/usr/include和/usr/local/include里面找
  ```bash
  $ gcc main.c -I./include -o main
  ```

- `l`和`L`
  
  `-l`指定链接的库的名字，去掉lib和.so，比如动态链接库叫libavutil.so，写`-l`参数时写成`-lavutil` or `-l:libavutil.so`，不加-l参数但是用到了该库中的函数会导致链接失败
  
  `-L`指定动态链接库去哪里找，默认从/usr/lib和/usr/local/lib找，只加`-l`，不加`-L`没法使用不在前两个目录下的库，这种情况下即使你加了-`l`也会链接失败
  ```bash
  $ gcc mian.c -L/usr/local/ffmpeg/lib -lavutil -o main
  ```
  
## 2022.5.8
  
## `LIBRARY_PATH` and `LD_LIBRARY_PATH`

开发时，设置LIBRARY_PATH，以便gcc能够找到编译时需要的动态链接库。

发布时，设置LD_LIBRARY_PATH，以便程序加载运行时能够自动找到需要的动态链接库。

When building a project, `LIBRARY_PATH` for `gcc` finding the dynamic link library or you will get a build error.

When running a program, `LD_LIBRARY_PATH` for loading the dynamic link library.

Another way to solve runtime error `fatal: libgcc_s.so.1: open failed: No such file or directory` is to use `-Wl,-rpath,${lib/folder}` when you are building the project to set runtime path.

```bash
$ g++ -std=c++20 -g -Wall -Wextra main.cpp -Iinclude -Lbin/Linux -l:libclang.so.12 -Wl,-rpath,bin/Linux
```
