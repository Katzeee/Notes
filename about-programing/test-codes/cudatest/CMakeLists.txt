cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(cmake_and_cudnn)

find_package(CUDNN REQUIRED)

find_package(CUDA REQUIRED)

find_package(OpenCV REQUIRED)
include_directories(${CUDNN_INCLUDE_DIR})

add_executable(main main.cpp)

target_link_libraries(main ${CUDNN_LIBRARIES} ${CUDA_LIBRARIES})
