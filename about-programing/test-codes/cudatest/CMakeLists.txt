cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cmake_and_cudnn)

#enable_language(CUDA)
set(CMAKE_PREFIX_PATH /home/xac/Downloads/libtorch)
find_package(Torch REQUIRED)
find_package(CUDNN REQUIRED)
find_package(CUDA REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${CUDNN_INCLUDE_DIR})

add_executable(main main.cpp)
#target_include_directories(main PRIVATE ${CUDNN_INCLUDE_DIRS})
target_link_libraries(main ${CUDNN_LIBRARIES} ${CUDA_LIBRARIES})

#target_link_libraries(main CuDNN::CuDNN)