#linux/ubuntu #machine-learning #c-cpp #code-env/lib 
## 2022.4.24

运行pytorch程序提示3050显卡当前cuda版本不支持，因此需要更新cuda与torch

See [[ubuntu-vcpkg-install-cuda-cudnn]], easier than this.

## 安装cuda

在官网查看安装命令，选择deb(local)
runfile
> https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=20.04&target_type=runfile_local

```bash
$ wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/x86_64/cuda-ubuntu2004.pin
$ sudo mv cuda-ubuntu2004.pin /etc/apt/preferences.d/cuda-repository-pin-600
$ wget https://developer.download.nvidia.com/compute/cuda/11.6.2/local_installers/cuda-repo-ubuntu2004-11-6-local_11.6.2-510.47.03-1_amd64.deb
$ sudo dpkg -i cuda-repo-ubuntu2004-11-6-local_11.6.2-510.47.03-1_amd64.deb
$ sudo apt-key add /var/cuda-repo-ubuntu2004-11-6-local/7fa2af80.pub
$ sudo apt-get update
$ sudo apt-get -y install cuda
```

打开`~/.bashrc`

```bash
$ export LD_LABRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib64
$ export PATH=$PATH:/usr/local/cuda/bin
$ export CUDA_HOME=$CUDA_HOME:/usr/local/cuda
```

通过`nvcc -V`命令查看当前cuda版本

`/usr/local/cuda`是链接文件，当需要更换cuda版本时，只需要把`/usr/local/cuda`删除，再用`ln`建立新的链接文件

若不成功还需要用dpkg卸载原有cuda

## 安装cudnn

> https://developer.nvidia.com/cudnn

在官网下载对应cuda版本的cudnn(tar)，采用`tar -zvf`解压缩

- 可选（如不这么做可以在cmake文件里进行目录指定）

将cuda/include/cudnn.h文件复制到usr/local/cuda/include文件夹，将cuda/lib64/下所有文件复制到/usr/local/cuda/lib64文件夹中，并添加读取权限

```bash
$ sudo chmod a+r /usr/local/cuda/include/cudnn.h /usr/local/cuda/lib64/libcudnn*
```
