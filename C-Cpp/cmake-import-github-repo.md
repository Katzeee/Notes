#cmake 
## 2022.10.09

## `FetchContent()`

Use `FetchContent` to fetch source code from github while cmake time and `ExternalProject` while build time

```cmake
include(FetchContent)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.11.0)

FetchContent_MakeAvailable(googletest)

add_executable(main main.cpp)
target_link_libraries(main gtest_main)
```
