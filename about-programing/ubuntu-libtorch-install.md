# 2022.5.7

Install libtorch in ubuntu, and provided a small project for it.

## Download and install libtorch

Get the zip file on official website, choose the version that satisfy your demand. I chose the libtorch with cuda-11.3 though my cuda version is 11.6

> https://pytorch.org/get-started/locally/

then unzip the file

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
