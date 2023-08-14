# 2022.4.14

## **[ubuntu]**

ffmpeg在ubuntu环境下开发的安装配置过程

## 依赖包安装
```bash
$ sudo apt-get install -y nasm yasm cmake mercurial
$ sudo apt-get install -y libxcb-xfixes0-dev pkg-config texinfo wget zlib1g-dev
$ sudo apt-get install -y libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev 
$ sudo apt-get install -y libfreetype6-dev libsdl2-dev libtheora-dev
$ sudo apt-get install -y autoconf automake build-essential git libass-dev
$ sudo apt-get install libx264-dev # H.264视频编码器
$ sudo apt-get install libx265-dev libnuma-dev # H.265/HEVA视频编码器
$ sudo apt-get install libvpx-dev # VP8/VP9视频编码器
$ sudo apt-get install libfdk-aac-dev # AAC音频编码器
$ sudo apt-get install libmp3lame-dev # mp3音频编码器
$ sudo apt-get install libopus-dev # Opus音频解码器和编码器
```

如果yasm和nasm没有安装成功，采取以下的方法
- yasm安装
  ```bash
  $ wget http://www.tortall.net/projects/yasm/releases/yasm-1.3.0.tar.gz
  $ tar -zxvf yasm-1.3.0.tar.gz
  $ cd yasm-1.3.0
  $ ./configure
  $ make && sudo make install
  ```
- nasm安装
  ```bash
  $ curl -O -L http://www.nasm.us/pub/nasm/releasebuilds/2.14.02/nasm-2.14.02.tar.bz2 
  $ tar -zxvf nasm-2.14.02.tar.bz2
  $ cd nasm-2.14.02
  $ ./configure
  $ make && sudo make install
  ```
如果libx264没有安装成功，采取以下的方法
- libx264安装
  ```bash
  $ git clone https://code.videolan.org/videolan/x264
  $ cd x264
  $ ./configure --enable-static --disable-opencl
  $ make && sudo make install
  ```

## 安装ffmpeg

```bash
$ wget http://www.ffmpeg.org/releases/ffmpeg-5.0.tar.gz
$ tar -zxvf ffmpeg-5.0.tar.gz
$ cd ffmpeg-5.0
$ ./configure --prefix="/usr/local/ffmpeg" \
  --enable-gpl \
  --enable-libass \
  --enable-libfdk-aac \
  --enable-libmp3lame \
  --enable-libopus \
  --enable-libfreetype \
  --enable-libtheora \
  --enable-libvorbis \
  --enable-libvpx \
  --enable-libx264 \
  --enable-libx265 \
  --enable-nonfree \
  --enable-shared
$ make && sudo make install
```

## 配置ffmpeg环境变量
```bash
$ sudo vi /etc/profile  # /etc/profile对所有用户起效，.bashrc对当前用户起效
```
在末尾加入
```
export FFMPEG_HOME=/usr/local/ffmpeg
export PATH=$FFMPEG_HOME/bin:$PATH
```
刷新，立即生效
```bash
$ soruce /etc/profile
```

### 查看ffmpeg版本
```bash
$ ffmpeg -version
```
如果报错libavdevice.so.58，执行下面的操作
```bash
$ sudo vi /etc/ld.so.conf.d/ffmpeg.conf
```
在最后一行加入
```
# /usr/local/ffmpeg/lib
```
执行下面的语句
```bash
$ sudo ldconfig
```