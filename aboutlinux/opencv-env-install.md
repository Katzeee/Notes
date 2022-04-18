# 2022.4.18

opencv在linux环境下开发的安装配置过程

### 官网下载opencv-3.4.16

> https://opencv.org/releases/

### 开始安装

```bash
$ unzip opencv-3.4.16
$ cd opencv-3.4.16
$ mkdir build && cd build
$ sudo cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
$ sudo make -j8 
$ sudo make install
```

### 环境变量配置

```bash
$ sudo vi /etc/ld.so.conf
# 在文件中最后加入include /usr/loacal/lib
$ sudo ldconfig
$ sudo vi /etc/profile
# 在文件中最后加入
# PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
# export PKG_CONFIG_PATH
$ source /etc/profile
```

### 检验是否安装成

```bash
$ pkg-config opencv --modversion
```
