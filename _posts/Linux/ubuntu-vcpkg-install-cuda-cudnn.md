#c-cpp #linux/ubuntu #machine-learning #code-env/vcpkg  #code-env/lib 
## 2022.5.6

I looked up many websites trying to config cuda and libtorch via vcpkg.

> https://stackoverflow.com/questions/56431461/how-to-remove-cuda-completely-from-ubuntu

## Remove old-version cuda

If you don't have an old-version cuda, no need to do this.

To remove cuda toolkit:

```bash
$ sudo apt-get --purge remove "*cublas*" "cuda*" "nsight*"
```

To remove Nvidia drivers:

```bash
sudo apt-get --purge remove "*nvidia*"
```

If you have installed via source files, then remove it using:

```bash
sudo rm -rf /usr/local/cuda*
```

From cuda 11.4 onwards, an uninstaller script has been provided. Use it for the uninstallation instead:

```bash
# To uninstall cuda
$ sudo /usr/local/cuda/bin/cuda-uninstaller 
# To uninstall nvidia
$ sudo /usr/bin/nvidia-uninstall
```

## Install driver

```bash
$ nvidia-detector
$ sudo apt install nvidia-driver-<num>
```

## Download and install cuda

Check which version is suitable for your graphic card.

Download the sh file from following website, choose `runfile(local)`

> https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=20.04&target_type=runfile_local

run like this

```bash
$ wget https://developer.download.nvidia.com/compute/cuda/11.6.2/local_installers/cuda_11.6.2_510.47.03_linux.run
$ sudo sh cuda_11.6.2_510.47.03_linux.run
```

Deselect the `driver`

Once finisher, you should see a summary like this:

```
===========
= Summary =
===========

Driver:   Not Selected
Toolkit:  Installed in /usr/local/cuda-11.0/
Samples:  Installed in /home/michael/, but missing recommended libraries

Please make sure that
 -   PATH includes /usr/local/cuda-11.0/bin
 -   LD_LIBRARY_PATH includes /usr/local/cuda-11.0/lib64, or, add /usr/local/cuda-11.0/lib64 to /etc/ld.so.conf and run ldconfig as root

To uninstall the CUDA Toolkit, run cuda-uninstaller in /usr/local/cuda-11.0/bin

Please see CUDA_Installation_Guide_Linux.pdf in /usr/local/cuda-11.0/doc/pdf for detailed information on setting up CUDA.
***WARNING: Incomplete installation! This installation did not install the CUDA Driver. A driver of version at least .00 is required for CUDA 11.0 functionality to work.
To install the driver using this installer, run the following command, replacing <CudaInstaller> with the name of this run file:
    sudo <CudaInstaller>.run --silent --driver
```

Do as instructions say, adding following lines to `.bashrc` or whatever shell you are using:

```bash
export PATH=$PATH:/usr/local/cuda/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda/lib64
```

You can see your cuda version by ruuning `nvcc -V`, and you will get the outputs like this:

```bash
$ nvcc -V
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2022 NVIDIA Corporation
Built on Tue_Mar__8_18:18:20_PST_2022
Cuda compilation tools, release 11.6, V11.6.124
Build cuda_11.6.r11.6/compiler.31057947_0
```

Then run `vcpkg install cuda`, you can see following

```
Computing installation plan...
The following packages will be built and installed:
    cuda[core]:x64-linux -> 10.1#10
Detecting compiler hash for triplet x64-linux...
Restored 0 packages from /home/xac/.cache/vcpkg/archives in 322.3 us. Use --debug to see more details.
Starting package 1/1: cuda:x64-linux
Building package cuda[core]:x64-linux...
-- Found CUDA V11.6.124
-- Performing post-build validation
-- Performing post-build validation done
Stored binary cache: /home/xac/.cache/vcpkg/archives/c0/c0e51a9513a1c58b81f9d2d77e1bc51e557d2708457e299b87c8a3c06fa68bec.zip
Installing package cuda[core]:x64-linux...
Elapsed time for package cuda:x64-linux: 30.28 ms

Total elapsed time: 459.6 ms
```

If you failed, you can try to config the environment variables, but the author said it is fixed.

> https://github.com/microsoft/vcpkg/issues/8247

```bash
$ export CUDA_PATH=/usr/local/cuda;
$ export CUDA_BIN_PATH=/usr/local/cuda/bin;
```

## Download and install cudnn

Follow the instructions to download the tar file`Local Installer for Linux x86_64 (Tar)`, then untar the file and merge it to `$CUDA_HOME/include` and `$CUDA_HOME/lib64`

```bash
$ sudo tar -xvf cudnn-linux-x86_64-8.4.0.27_cuda11.6-archive.tar.xz
$ cd cudnn-linux-x86_64-8.4.0.27_cuda11.6-archive
$ sudo cp ./include/cudnn*.h /usr/local/cuda/include
$ sudo cp ./lib64/libcudnn* /usr/local/cuda/lib64
# change the mode bits of these files (optional)
$ sudo chmod a+r /usr/local/cuda/include/cudnn*.h /usr/local/cuda/lib64/libcudnn*
```

Then run `vcpkg install cudnn`, you can see following

```
Computing installation plan...
The following packages will be built and installed:
    cudnn[core]:x64-linux -> 7.6.5#5
Detecting compiler hash for triplet x64-linux...
Restored 0 packages from /home/xac/.cache/vcpkg/archives in 5.795 us. Use --debug to see more details.
Starting package 1/1: cudnn:x64-linux
Building package cudnn[core]:x64-linux...
-- Note: cudnn only supports dynamic library linkage. Building dynamic library.
-- Found CUDA V11.6.124
-- CUDNN_INCLUDE_DIR: /usr/local/cuda/include
-- CUDNN_LIBRARY: /usr/local/cuda/lib64/libcudnn.so
-- Found CUDNN 8.4.0 located on system: (include /usr/local/cuda/include lib: /usr/local/cuda/lib64/libcudnn.so)
-- Installing: /usr/local/vcpkg/packages/cudnn_x64-linux/share/cudnn/FindCUDNN.cmake
-- Installing: /usr/local/vcpkg/packages/cudnn_x64-linux/share/cudnn/usage
-- Installing: /usr/local/vcpkg/packages/cudnn_x64-linux/share/cudnn/vcpkg-cmake-wrapper.cmake
-- Performing post-build validation
-- Performing post-build validation done
Stored binary cache: /home/xac/.cache/vcpkg/archives/ab/ab548ca4133d45b934379ea0b0df319d21f1b9fd074031f1f20bbd699bcab0b5.zip
Installing package cudnn[core]:x64-linux...
Elapsed time for package cudnn:x64-linux: 20.1 ms

Total elapsed time: 303.2 ms

The package cudnn provides CMake variables:

    find_package(CUDNN REQUIRED)
    target_link_libraries(main PRIVATE ${CUDNN_LIBRARIES})
    target_include_directories(main PRIVATE ${CUDNN_INCLUDE_DIRS})

Or the following CMake target:

    find_package(CUDNN REQUIRED)
    target_link_libraries(main PRIVATE CuDNN::CuDNN)
```


## Test code

The whole project is at [cudatest](https://github.com/Katzeee/Notes/tree/master/about-programing/test-codes/cuda-test)

```cpp
// main.cpp
#include <iostream>
#include <cuda_runtime.h>
#include <cudnn.h>

/**
 * Minimal example to apply sigmoid activation on a tensor 
 * using cuDNN.
 **/
int main(int argc, char** argv)
{    
    int numGPUs;
    cudaGetDeviceCount(&numGPUs);
    std::cout << "Found " << numGPUs << " GPUs." << std::endl;
    cudaSetDevice(0); // use GPU0
    int device; 
    struct cudaDeviceProp devProp;
    cudaGetDevice(&device);
    cudaGetDeviceProperties(&devProp, device);
    std::cout << "Compute capability:" << devProp.major << "." << devProp.minor << std::endl;

    cudnnHandle_t handle_;
    cudnnCreate(&handle_);
    std::cout << "Created cuDNN handle" << std::endl;

    // create the tensor descriptor
    cudnnDataType_t dtype = CUDNN_DATA_FLOAT;
    cudnnTensorFormat_t format = CUDNN_TENSOR_NCHW;
    int n = 1, c = 1, h = 1, w = 10;
    int NUM_ELEMENTS = n*c*h*w;
    cudnnTensorDescriptor_t x_desc;
    cudnnCreateTensorDescriptor(&x_desc);
    cudnnSetTensor4dDescriptor(x_desc, format, dtype, n, c, h, w);

    // create the tensor
    float *x;
    cudaMallocManaged(&x, NUM_ELEMENTS * sizeof(float));
    for(int i=0;i<NUM_ELEMENTS;i++) x[i] = i * 1.00f;
    std::cout << "Original array: "; 
    for(int i=0;i<NUM_ELEMENTS;i++) std::cout << x[i] << " ";

    // create activation function descriptor
    float alpha[1] = {1};
    float beta[1] = {0.0};
    cudnnActivationDescriptor_t sigmoid_activation;
    cudnnActivationMode_t mode = CUDNN_ACTIVATION_SIGMOID;
    cudnnNanPropagation_t prop = CUDNN_NOT_PROPAGATE_NAN;
    cudnnCreateActivationDescriptor(&sigmoid_activation);
    cudnnSetActivationDescriptor(sigmoid_activation, mode, prop, 0.0f);

    cudnnActivationForward(
        handle_,
        sigmoid_activation,
        alpha,
        x_desc,
        x,
        beta,
        x_desc,
        x
    );

    cudnnDestroy(handle_);
    std::cout << std::endl << "Destroyed cuDNN handle." << std::endl;
    std::cout << "New array: ";
    for(int i=0;i<NUM_ELEMENTS;i++) std::cout << x[i] << " ";
    std::cout << std::endl;
    cudaFree(x);
    return 0;
}
```

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cmake_and_cudnn)

find_package(CUDNN REQUIRED)
find_package(CUDA REQUIRED)
include_directories(${CUDNN_INCLUDE_DIR})

add_executable(main main.cpp)
target_link_libraries(main ${CUDNN_LIBRARIES} ${CUDA_LIBRARIES})
```

run `tree . -L 1`, you will see like this:

make sure that `./build` is empty

```bash
$ tree . -L 1
.
├── build
├── CMakeLists.txt
└── main.cpp
```

`cd` into `./build` and run following:

```bash
$ cd build
$ cmake -B . -S .. -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
$ cmake --build .
$ make
```
