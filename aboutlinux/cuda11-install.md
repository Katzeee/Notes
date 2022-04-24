# 2022.4.24

运行pytorch程序提示3050显卡当前cuda版本不支持，因此需要更新cuda

## nvcc

通过`nvcc -V`命令查看当前cuda版本

## 重新安装

在官网查看安装命令

> https://pytorch.org/get-started/locally/

```bash
$ pip3 install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu113
```

在安装过程中出现最后一秒`killed`的情况，带上安装选项`--no-cache-dir`


