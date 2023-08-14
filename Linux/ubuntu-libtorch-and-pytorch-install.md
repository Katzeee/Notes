#linux/ubuntu #code-env/lib  #machine-learning #c-cpp 
## 2022.5.7

Install libtorch in ubuntu with a small project for testing it.

Make sure you have installed cuda and cudnn already, or the test code may do not work for you.

You can install cuda/cudnn by the instruction here [[ubuntu-vcpkg-install-cuda-cudnn]] or [[ubuntu-cuda11-install]]

## Download and install libtorch


Get the zip file on official website, choose the version that satisfy your demand. I chose the libtorch with cuda-11.3 though my cuda version is 11.6

> https://pytorch.org/get-started/locally/

```bash
$ cd ~/Downloads/compressed
$ wget https://download.pytorch.org/libtorch/cu113/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcu113.zip
```

Then unzip the file and put wherever you want, I choose to put it at `~/local/libtorch`

```bash
$ cd ~/local
$ unzip ~/Downloads/compressed/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcu113.zip
```

## Install pytorch

Also, on the above website can you see how to install pytorch.

Choose the SAME VERSION as the libtorch(I don't know whether it will affect the following steps if you don't choose the same version as libtorch, but I recommend choosing the same version of cuda and torch.)

```bash
$ pip3 install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu113
```

If the program got "KILLED" when you almost finish installation, you can try 

```bash
$ pip3 install torch torchvision torchaudio --extra-index-url https://download.pytorch.org/whl/cu113 --no-cache-dir
```

## Test 

First, we test pytorch,

```bash
$ python
Python 3.8.10 (default, Mar 15 2022, 12:22:08) 
[GCC 9.4.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import torch
>>> torch.__version__
'1.11.0+cu113'
```

Then, test libtorch, the whole project is in [torch-test](../test-codes/torch-test)

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
set(CMAKE_PREFIX_PATH /home/xac/local/libtorch) # set the path to libtorch
find_package(OpenCV REQUIRED)
find_package(Torch REQUIRED)
include_directories(${Torch_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
add_executable(main main.cpp)
target_link_libraries(main PRIVATE "${TORCH_LIBRARIES}" ${OpenCV_LIBRARIES})
```

**WARNING:MUST IMPORT OpenCV BEFORE Torch**

If I put `find_package(Torch REQUIRED)` before `find_package(OpenCV REQUIRED)`, I will get the following error, I don't know why. I think it is casued by vcpkg, because I didn't meet the error when I was trying in another computer which I installed the opencv without vcpkg.


```
[cmake] CMake Error at /usr/local/vcpkg/installed/x64-linux/share/protobuf/protobuf-targets.cmake:37 (message):
[cmake]   Some (but not all) targets in this export set were already defined.
[cmake] 
[cmake]   Targets Defined: protobuf::libprotobuf
[cmake] 
[cmake]   Targets not yet defined:
[cmake]   protobuf::libprotobuf-lite;protobuf::libprotoc;protobuf::protoc
```

## Tips for installation

When I was configuring my libtorch, I append my `$LD_LIBRARY_PATH` to `libtorch/lib`. That makes my pytorch cannot be imported, I will get a segmentation fault when I was trying to `import torch` like this:

```bash
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/local/libtorch/lib
$ python
Python 3.8.10 (default, Mar 15 2022, 12:22:08) 
[GCC 9.4.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import torch
Segmentation fault (core dumped)
```

I find it is caused by `libtorch_python.so` in libtorch.

```bash
$ gdb python
(gdb) r -c "import torch"
Starting program: /usr/bin/python -c "import torch"
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
[Detaching after fork from child process 7815]

Program received signal SIGSEGV, Segmentation fault.
0x00000000005a45f4 in ?? ()
(gdb) bt
#0  0x00000000005a45f4 in ?? ()
#1  0x00000000005020d9 in ?? ()
#2  0x00000000005a630e in ?? ()
#3  0x00000000005bf8d2 in PyObject_GetAttr ()
#4  0x00000000005c01c0 in PyObject_GetAttrString ()
#5  0x00007ffff555e13d in pybind11::detail::accessor<pybind11::detail::accessor_policies::str_attr>::get_cache() const ()
   from /home/xac/local/libtorch/lib/libtorch_python.so
#6  0x00007ffff5563497 in pybind11::cpp_function::initialize_generic(std::unique_ptr<pybind11::detail::function_record, pybind11::cpp_function::InitializingFunctionRecordDeleter>&&, char const*, std::type_info const* const*, unsigned long)
    () from /home/xac/local/libtorch/lib/libtorch_python.so
#7  0x00007ffff5b5253f in pybind11::enum_<onnx_torch::TensorProto_DataType>::enum_<>(pybind11::handle const&, char const*) ()
   from /home/xac/local/libtorch/lib/libtorch_python.so
#8  0x00007ffff5b4fe25 in torch::onnx::initONNXBindings(_object*) ()
   from /home/xac/local/libtorch/lib/libtorch_python.so
#9  0x00007ffff57e9229 in initModule ()
   from /home/xac/local/libtorch/lib/libtorch_python.so
#10 0x000000000068430e in _PyImport_LoadDynamicModuleWithSpec ()
#11 0x0000000000684e61 in ?? ()
#12 0x00000000005c4a40 in ?? ()
#13 0x00000000005f2f5d in PyVectorcall_Call ()
--Type <RET> for more, q to quit, c to continue without paging--
```

Normally, pytorch choose to link `.local/lib/python3.8/site-packages/torch/lib/libtorch_python.so` rather `~/libtorch/lib/libtorch_python.so`. 

And I think it will also happen if you put libtorch in the system default link path.(But I didn't try it.)

Thus, if you get the same problem, try to find if it's caused by libtorch. Also, if you have some other solution, you can create an issue to tell me.