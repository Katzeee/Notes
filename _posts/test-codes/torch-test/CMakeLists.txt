cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(cmake_opencv_torch)

# set Torch_DIR to your libtorch
set(Torch_DIR /home/xac/local/libtorch/share/cmake/Torch)

find_package(OpenCV REQUIRED)



find_package(Torch REQUIRED)
include_directories(${Torch_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
#link_directories(/home/xac/local/libtorch/lib)

add_executable(main main.cpp)

#target_link_libraries(main PRIVATE "${TORCH_LIBRARIES}" ${OpenCV_LIBRARIES})
target_link_libraries(main ${TORCH_LIBRARIES} ${OpenCV_LIBRARIES})