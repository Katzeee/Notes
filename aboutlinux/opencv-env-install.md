# 2022.4.18

opencv在linux环境下开发的安装配置过程

参考文章

> https://blog.csdn.net/qq_37268614/article/details/108704106

## 官网下载opencv-3.4.16

> https://opencv.org/releases/


## 安装依赖项

```bash
$ sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
$ sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev ffmpeg
```

## 开始安装

```bash
$ unzip opencv-3.4.16
$ cd opencv-3.4.16
$ mkdir build && cd build
$ export PKG_CONFIG_PATH=/usr/local/ffmpeg/lib/pkgconfig:$PKG_CONFIG_PATH # ffmpeg安装路径
$ sudo cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local .. 
```

做完这一步之后要确保在输出信息里ffmpeg为YES

```
--   Video I/O:
--     DC1394:                      NO
--     FFMPEG:                      YES
--       avcodec:                   YES (ver 58.101.101)
--       avformat:                  YES (ver 58.51.101)
--       avutil:                    YES (ver 56.58.100)
--       swscale:                   YES (ver 5.8.100)
--       avresample:                NO
--     GStreamer:                   
--       base:                      YES (ver 0.10.36)
--       video:                     YES (ver 0.10.36)
--       app:                       YES (ver 0.10.36)
--       riff:                      YES (ver 0.10.36)
--       pbutils:                   YES (ver 0.10.36)
--     libv4l/libv4l2:              0.9.0-test1 / 0.9.0-test1
--     v4l/v4l2:                    linux/videodev2.h
```

之后进行编译安装
```bash
$ sudo make -j8 
$ sudo make install
```

## 环境变量配置

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

## 检验是否安装成

```bash
$ pkg-config opencv --modversion
```
