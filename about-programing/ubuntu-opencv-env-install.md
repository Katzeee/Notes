# 2022.4.18

## **[ubuntu]**

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
$ sudo cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_GENERATE_PKGCONFIG=ON .. 
# OPENCV_GENERATE_PKGCONFIG=ON为了生成pkgconfig的配置文件，便于多版本opencv切换
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
$ sudo vi /etc/ld.so.conf.d/opencv.conf
# 在文件中最后加入include /usr/local/lib
$ sudo ldconfig
$ sudo vi /etc/profile
# 在文件中最后加入
# export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/lib/pkgconfig
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
$ source /etc/profile
```

## 检验是否安装成

```bash
$ pkg-config opencv --modversion
```

## 同时安装opencv4

更改安装路径
```bash
$ sudo cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local/opencv-4.5.5 -D OPENCV_GENERATE_PKGCONFIG=ON .. 
```

## 配置环境变量

```bash
$ sudo vi /etc/ld.so.conf.d/opencv.conf
# 在文件中最后加入include /usr/local/opencv-4.5.5/lib
$ sudo ldconfig
$ sudo vi /etc/profile
# 在文件中最后加入
# export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/usr/local/opencv-4.5.5/lib/pkgconfig
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/opencv-4.5.5/lib
$ source /etc/profile
```

## 检验是否安装成

```bash
$ pkg-config opencv4 --modversion
``` 

## 使用时的CMakeLists

```cmake
find_package(OpenCV 3.4.16 REQUIRED) # 使用opencv3
find_package(OpenCV 4.5.5 REQUIRED) # 使用opencv4
# set(${OpenCV_INCLUDE_DIRS} "/usr/local/opencv-4.5.5/include/opencv4/opencv2/") # 或许要加这两行
# set(${OpenCV_LIBS}  "/usr/local/opencv-4.5.5/lib/")
```

# 2022.5.8

You can install opencv4 just by `vcpkg install opencv4`, and install opencv3 by `vcpkg install opencv3`.