# 2022.4.24

运行pytorch程序提示3050显卡当前cuda版本不支持，因此需要更新cuda与torch

## 重新安装nvcc

在官网查看安装命令，选择runfile

> https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=20.04&target_type=runfile_local

```bash
$ wget https://developer.download.nvidia.com/compute/cuda/11.6.2/local_installers/cuda_11.6.2_510.47.03_linux.run
$ sudo sh cuda_11.6.2_510.47.03_linux.run
```

通过`nvcc -V`命令查看当前cuda版本

## 重新安装torch

在官网查看安装命令

> https://pytorch.org/get-started/locally/

```bash
$ pip3 install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu113
```

在安装过程中出现最后一秒`killed`的情况，带上安装选项`--no-cache-dir`


