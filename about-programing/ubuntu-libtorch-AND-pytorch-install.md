# 2022.5.7

Install libtorch in ubuntu with a small project for testing it.

Make sure you have installed cuda and cudnn already, or the test code may do not work for you.

You can install cuda/cudnn by the instruction here [via vcpkg](./ubuntu-cuda-install-vcpkg.md) or [without vcpkg](./ubuntu-cuda11-torch-install.md)

## Download and install libtorch


Get the zip file on official website, choose the version that satisfy your demand. I chose the libtorch with cuda-11.3 though my cuda version is 11.6

> https://pytorch.org/get-started/locally/

```bash
$ cd ~/Downloads/compressed
$ wget https://download.pytorch.org/libtorch/cu113/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcu113.zip
```

Then unzip the file and put wherever you want, I choose to put it at `/home/xac/local/libtorch`

```bash
$ cd /home/xac/local
$ unzip ~/Downloads/compressed/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcu113.zip
```

## Install pytorch

Also, on the above website can you see how to install pytorch.

Choose the SAME VERSION as the libtorch(I don't know whether it will affect the following steps, but I recommend choosing the same version of cuda and torch.)

```bash
$ pip3 install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu113
```

If the program got "KILLED" when you almost finish installation, you can try 

```bash
$ pip3 install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu113 --no-cache-dir
```
## Using libtorch and opencv in cpp with cmake

```cpp
// main.cpp
// for gpu version test

#include <opencv2/opencv.hpp>
#include <torch/script.h>
#include <torch/torch.h>
#include <iostream>

int main()
{
    std::cout << torch::cuda::is_available() << std::endl;
    torch::DeviceType device_type = at::kCUDA;
    std::cout << device_type << std::endl;
    torch::Tensor tensor = torch::rand({5, 3}).cuda();
    std::cout << tensor << std::endl;
    cv::Mat img = cv::imread("../image.jpg");
    cv::imshow("img", img);
    cv::waitKey(0);
    return 0;
}
```

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(cmake_opencv_torch)
set(CMAKE_PREFIX_PATH /home/xac/Downloads/libtorch) # set the path to libtorch
find_package(OpenCV REQUIRED)
find_package(Torch REQUIRED)
include_directories(${Torch_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
add_executable(main main.cpp)
target_link_libraries(main PRIVATE "${TORCH_LIBRARIES}" ${OpenCV_LIBRARIES})
```

**WARNING:MUST IMPORT OpenCV BEFORE Torch**
