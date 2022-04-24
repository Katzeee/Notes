# 2022.4.24

运行pytorch程序提示3050显卡当前cuda版本不支持，因此需要更新cuda与torch

## 重新安装nvcc

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
$ export LD_LABRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda-11.6/lib64
$ export PATH=$PATH:/usr/local/cuda-11.6/bin
$ export CUDA_HOME=$CUDA_HOME:/usr/local/cuda-11.6
```

通过`nvcc -V`命令查看当前cuda版本

若不成功还需要用dpkg卸载原有cuda

## 重新安装torch

在官网查看安装命令

> https://pytorch.org/get-started/locally/

```bash
$ pip3 install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu113
```

在安装过程中出现最后一秒`killed`的情况，带上安装选项`--no-cache-dir`


