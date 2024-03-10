#c-cpp/cmake 
## 2023.08.08

## Manipulate file path

### `get_filename_component`

> https://cmake.org/cmake/help/latest/command/get_filename_component.html

Use `get_filename_component` to retrieve elements in file path

```cmake
get_filename_component(<var> <FileName> <mode> [CACHE])
# example
get_filename_component(EXE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
```

```
<mode>
DIRECTORY = Directory without file name
NAME      = File name without directory
EXT       = File name longest extension (.b.c from d/a.b.c)
NAME_WE   = File name with neither the directory nor the longest extension
LAST_EXT  = File name last extension (.c from d/a.b.c)
NAME_WLE  = File name with neither the directory nor the last extension
PATH      = Legacy alias for DIRECTORY (use for CMake <= 2.8.11)
ABSOLUTE  = Full path to file
REALPATH  = Full path to existing file with symlinks resolved
```


### `cmake_path()` (After version 3.20)

> https://cmake.org/cmake/help/latest/command/cmake_path.html#command:cmake_path

