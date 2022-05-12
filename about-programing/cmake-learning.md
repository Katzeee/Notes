# 2022.4.14

### cmake官方文档

> https://cmake.org/cmake/help/latest/manual/cmake-commands.7.html

## `add_library`

```cmake
add_library(<name> [STATIC | SHARED | MODULE]
            [EXCLUDE_FROM_ALL]
            [<source>...])
```

### Normal Libraries

- 添加一个从`<source>`列表列出文件构建而来的库，名为`<name>`全局唯一
- `<source>`可以直接指定，也可以后续再用`target_sources()`
- STATIC（静态库）（`<lib>name.a`），SHARED（动态库），MODULE（模块库）
- MODULE是在函数中进行调用动态库
  > MODULE libraries are plugins that are not linked into other targets but may be loaded dynamically at runtime using dlopen-like functionality. 

示例代码
```cmake
ADD_LIBRARY(hellolib STATIC ./src/hello.cpp)
```

### Object Libraries

### Interface Libraries

### Imported Libraries

```cmake
add_library(<name> <type> IMPORTED [GLOBAL])
```

- 用来导入已经存在的库，CMake也不会添加任何编译规则给它。
- 此类库的标志就是有IMPORT属性，导入的库的作用域为创建它的目录及更下级目录。但是如果有GLOBAL属性，则作用域被拓展到全工程。
- 导入的库的类型必须是STATIC, SHARED, MODULE, UNKNOWN中的一种
对于UNKNOW类型，不需要知道类型就可使用的
从工程外部引入一个库，使用IMPORTED_LOCATION属性确定库文件的在磁盘上的完整路径。

示例代码

我们在调用别的非官方SDK时，需要链接动态库，此时就需要用到该命令

```cmake
ADD_LIBRARY(avutil SHARED IMPORTED) # 设置为导入共享库
SET_TARGET_PROPERTIES(avutil PROPERTIES IMPORTED_LOCATION /usr/local/ffmpeg/lib) # 第三方库的路径
TARGET_LINK_LIBRARIES(a.out avutil) # 链接第三方库
```


### Alias Libraries

```cmake
add_library(<name> ALIAS <target>)
```
为给定library添加一个别名，后续可使用`<name>`来替代`<target>`。

- `<target>`不能是ALIAS

- 可用于判断`<target>`是否存在、链接。

- ALIAS的library不能修改属性，不能调用`set_property()`, `set_target_properties()`和`target_link_libraries()`等方法

- 不能用于`install()`

# 2022.5.17

## Adding source files

### `aux_source_directory()`

```cmake
aux_source_directory(<dir> <variable>)
```

Collect all source files in `<dir>`, excluding `.h`

```cmake
# example, collect all source files store the list in ${SOURCE}
aux_source_directory(. SOURCES)
```


### `file(GLOB)`

```cmake
file({GLOB | GLOB_RECURSE} <out-var> [...] [<globbing-expr>...])
```

using `file(GLOB)` to collect the files in the current cmake directory with the file name extension you set

```cmake
# example, collect all ".h", ".cpp" files and store in ${SOURCES}
file(GLOB SOURCE *.h *.cpp)
# NOTICE: when you add new files in directiory, this command will not add them in the ${SOURCE} variable, so using `CONFIGURE_DEPENDS` to update ${SOURCE} in every time rebuilding the project
file(GLOB SOURCE CONFIGURE_DEPENDS *.h *.cpp)
```

to find recursively in the directory, use `file(GLOB_RECURSE)`

```cmake
# example, recursively collect the files
file(GLOB_RECURSE SOURCE *.h *.cpp)
# NOTICE: if your CMakeLists.txt is in the project directory, this command will add some cache files in `build/` directory, the solution is to put all source files in `src/` directory
```

## Config variables

### `CMAKE_BUILD_TYPE`

Typically, values include:

- `Debug`: `-O0 -g`(default)
- `Release`: `-O3 -DNDEBUG`
- `MinSizeRel`: `-Os -DNDEBUG`
- `RelWithDebInfo`: `-O2 -g -DNDEBUG`

TIPS: `-DNDEBUG` set `assert()` as NULL MARCO

We can set `CMAKE_BUILD_TYPE` as `Release` by this way:

```cmake
# correct
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Release)
endif()
# wrong
set (CMAKE_BUILD_TYPE Release)
```

Writing by the first way is normal, as if user set `Debug` manually, we will not cover his settings.

### `project()`

```cmake
project(<PROJECT-NAME> [<language-name>...])
project(<PROJECT-NAME>
        [VERSION <major>[.<minor>[.<patch>[.<tweak>]]]]
        [DESCRIPTION <project-description-string>]
        [HOMEPAGE_URL <url-string>]
        [LANGUAGES <language-name>...])
```

`project()` will set some variables:

- `PROJECT_SOURCE_DIR`, `<PROJECT-NAME>_SOURCE_DIR`

  Absolute path to the source directory for the project.


- `PROJECT_BINARY_DIR`, `<PROJECT-NAME>_BINARY_DIR`

  Absolute path to the binary directory for the project.

### `PROJECT_<X>_DIR` vs `CMAKE_CURRENT_<X>_DIR` in sub-direcetories

`PROJECT_SOURCE_DIR` means the absolute path to the directory which is closest to the current directory with `project()` called by `CMakeLists.txt` in it. So `PROJECT_SOURCE_DIR` can be used to get the root directory i.e. the top-level directory of your own project.

`CMAKE_CURRENT_SOURCE_DIR` means the absolute path to the source directory where the current `CMakeLists.txt` is located.

**USING `CMAKE_SOURCE_DIR` IS NOT RECOMMENDED, WHICH MAKES YOUR PROJECT CANNOT BE USED AS SUB-MODULE OF OTHER PROJECT**

```cmake
PROJETC_SOURCE_DIR: ~/cmakelearning
PROJECT_BINARY_DIR: ~/cmakelearning/build
CMAKE_CURRENT_SOURCE_DIR: ~/cmakelearning/src
CMAKE_CURRENT_BINARY_DIR: ~/cmakelearning/build/src
```

### `PROJECT_NAME` vs `CMAKE_PROJECT_NAME`

They represent the current project name and the top-level project name, respectively.

### `CMAKE_<LANG>_STANDARD`, `CMAKE_<LANG>_STANDARD_REQUIRED`, `CMAKE_CXX_EXTENSIONS`

- `CMAKE_CXX_STANDARD` represents the cpp standard.
- `CMAKE_CXX_STANDARD_REQUIRED` can be set to `ON` or `OFF`. `ON` means if your compiler doesn't support the above standard you set, the cmake will raise an error and shut down the current work. But if it is set to `OFF`, the compiler will continue work.
- `CMAKE_CXX_EXTENSIONS` specifies whether compiler specific extensions should be used. Such as the `GCC` vs `MSVC`

**TIPS: USING `CMAKE_CXX_STANDARD` BEFORE `project()`**

**WARNING: DO NOT SET `CMAKE_CXX_FLAGS` TO CHANGE CPP STANDARD**

```cmake
set_property(TARGET main PROPERTIY CXX_STANDARD 17) # correct
target_compile_options(main PUBLIC "-std=c++17") # wrong
set_property(TARGET main PROPERTIY CUDA_ARCHITECTURES 75) # correct
target_compile_options(main PUBLIC "-arch=sm_75") # wrong
```

### `cmake_minimum_required()`

```cmake
cmake_minimum_required(VERSION <min>[...<policy_max>] [FATAL_ERROR])
```

set the minimum version of cmake

```cmake
# example
cmake_minimum_required(VERSION 3.16)
```

## Link libraries

### `add_library()`

- object libraries

```cmake
add_library(<name> OBJECT [<source>...])
```

Create an object library. An object library compiles source files but does not archive or link their object files into a library.

Using object Library can avoid the cross-platform error, so it's nice to always use object libraries in your project instead of static libraries.

For static libraries, the compiler will auto-remove the symbols that do not be use, which causes some problem.

And using shared libraries on Windows is a disagreeable thing, because on Windows we cannot set `-rpath`.

In default, cmake build libraries as static libraries. The variable `BUILD_SHARED_LIBS` can be set to `ON`, for always building shared libraries.

**TIPS: SHARED LIBRARIES CANNOT LINK TO STATIC LIBRARIES DIRECTLY**

SOLUTION FOR TIPS: for global
```cmake
set (CMAKE_POSITION_INDEPENDENT_CODE ON)
```

Or just set it for one library:

```cmake
add_library(mylib STATIC mylib.cpp)
set_property(TARGET mylib PROPERTY POSITION_INDEPENDENT_CODE ON)
```

### `set_property()`, `set_target_properties()`

Use `set_property()` to set target properties one by one:
```cmake
# c++17 standard(default 11 in cmake 3.16)
set_property(TARGET main PROPERTY CMAKE_CXX_STANDARD 17)
# rasie an error when compiler doesn't support c++17(default OFF)
set_property(TARGET main PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
# when running on Windows, no console, only GUI(default OFF)
set_property(TARGET main PROPERTY WIN32_EXECUTUABLE ON)
# don't delete symbols which are not refered(default OFF)
set_property(TARGET main PROPERTY LINK_WHAT_YOU_USE ON)
# set shared libraries output path(default ${CMAKE_SOURCE_DIR})
set_property(TARGET main PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
# set static libraries output path(default ${CMAKE_SOURCE_DIR})
set_property(TARGET main PROPERTY ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
# set binary output path(default ${CMAKE_SOURCE_DIR})
set_property(TARGET main PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
```

Use `set_target_properties()` to set target properties all in one:

```cmake
set_target_properties(main PROPERTY 
    CMAKE_CXX_STANDARD 17
    CMAKE_CXX_STANDARD_REQUIRED ON
    WIN32_EXECUTUABLE ON
    LINK_WHAT_YOU_USE ON
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    )
```

Use `set()` to set global variables to make all target built after have the properties:

```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(WIN32_EXECUTUABLE ON)
set(LINK_WHAT_YOU_USE ON)
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
```