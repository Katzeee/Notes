#cmake 
## 2022.5.17

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

using `file(GLOB)` to collect the files in the current cmake directory with the specified file name extension

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

Writing by the first way is common, as if user set `Debug` manually, we will not cover his settings.

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
[[cmake-add-library]]

- object libraries

```cmake
add_library(<name> OBJECT [<source>...])
```

Create an object library. An object library compiles source files but does not archive or link their object files into a library.

Using object Library can avoid the cross-platform error, so it's nice to always use object libraries in your project instead of static libraries.

For static libraries, the compiler will auto-remove the symbols that do not be use, which causes some problem.

And using shared libraries on Windows is a disagreeable thing, because on Windows we cannot set `-rpath`. Windows search for `.dll` file only in `PATH` or current directory. So we should ensure that the output path of shared libraries is the same as the executable program.

If you MUST use shared libraries on Windows:

```cpp
// mylib.cpp
#include <cstdio>
#ifdef _MSC_VER
__declspec(dllexport)
#endif

// mlib.h
#pragma once
#ifdef _MSC_VER
__declspec(dllimport)
#endif
```

In default, cmake build libraries as static libraries. `BUILD_SHARED_LIBS` is a global variable to control the default type of building the libraries, and is default set to `OFF`. If it is set to `ON`, cmake will build SHARED libraries as default. 

**TIPS: SHARED LIBRARIES CANNOT LINK TO STATIC LIBRARIES DIRECTLY**

- SOLUTION FOR TIPS: for global

    ```cmake
    set (CMAKE_POSITION_INDEPENDENT_CODE ON)
    ```

- Or just set it for one library:

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

## 2022.09.17

## Instruction of using CMake

- Old CMake
    ```bash
    $ mkdir -p build && cd build
    $ cmake [-GNinja] -DCMAKE_BUILD_TYPE=Release ..
    $ make -j4
    $ make install
    ```

- Modern CMake

    ```bash
    $ cmake -B build [-GNinja] -DCMAKE_BUILD_TYPE=Release # construct cmake, create a `build` folder and Makefile 
    $ cmake --build build --parallel 4 # using make or ninja or whatever compiler to complie the source code
    $ cmake --build build --target install # install the code
    ```

## `target_sources()`

```cmake
target_sources(<target>
<INTERFACE|PUBLIC|PRIVATE> [items1...]
[<INTERFACE|PUBLIC|PRIVATE> [items2...] ...])
```

Use `target_sources()` to append source files to the library or executable.

```cmake
aux_source_directory(. SRC)
add_excutable(main)
target_sources(main PUBLIC ${SRC})
```

Even you can use `target_sources` to append files to the object in sub-directories.

## `find_packages()`

For some libraries, you must specify the components you used or you will get a cmake failure.

```cmake
find_packages(Qr5 COMPONENTS Widgets Gui REQUIRED)
target_link_libraries(main PUBLIC Qt5::Widgets Qt5::Gui)
```

Sometimes you should specify the install path of your libraries, there are some approaches.

- `set` CMAKE_MODULE_PATH

    ```cmake
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} C:/Qt/Qt5.14.2/msvc2019_64/lib/cmake)

    find_packages(Qr5 COMPONENTS Widgets Gui REQUIRED)
    target_link_libraries(main PUBLIC Qt5::Widgets Qt5::Gui)
    ```

- `set` Qt5_DIR

    ```cmake
    set(Qt5_DIR C:/Qt/Qt5.14.2/msvc2019_64/lib/cmake)

    find_packages(Qr5 COMPONENTS Widgets Gui REQUIRED)
    target_link_libraries(main PUBLIC Qt5::Widgets Qt5::Gui)
    ```

- specify the path in command line

    ```bash
        cmake -B build -DQt5_DIR="C:/Qt/Qt5.14.2/msvc2019_64/lib/cmake"
    ```

- set the environment variable

    ```bash
    export Qt5_DIR="/opt/Qt5.14.2/lib/cmake"
    cmake -B build
    ```

When not use `REQUIRED` in `find_package`, cmake will set `TBB_FOUND` to be `FALSE`.

```cmake
find_package(TBB)
if (TBB_FOUND)
# or
# if (TARGET TBB::tbb)
# if (NOT TARGET TBB::tbb AND TARGET Eigen3::eigen)
    message(STATUS "TBB found at: ${TBB_DIR}")
    target_link_libraries(main PUBLIC TBB::tbb)
    target_compile_definitions(main PUBLIC WITH_TBB) # define `WITH_TBB` marco in cpp files
else()
    message(WARNING "TBB not found!")
endif()
```

```cpp
#ifdef WITH_TBB // defined in cmake file
do_something();
#endif
```

## `message()`

```cmake
message(STATUS <str>)
message(WARNING <str>)
message(AUTHOR_WARNING <str>)
message(FATAL_ERROR <str>) # shutdown
message(SEND_ERROR <str>) # not shutdown
```

## update cache file

Use -D parameter to update the variables in `CMakeCache.txt`

```bash
$ cmake -B build -Dmyvar=world
```

Use `ccmake` on Linux or `cmake-gui` on Windows.

```bash
$ ccmake -B build
```

## set cmake cache varibles

```cmake
set(WITH_TBB OFF CACHE BOOL "set to ON to enable TBB") # define a bool varible WITH_TBB and decribe it then set to OFF
# equivalent to 
option(WITH_TBB "set to ON to enable TBB" OFF) 
```

This setting can be overwritten by user when call cmake in command line:

```bash
$ cmake -B build -DWITH_TBB:BOOL=ON
```

## Determine the operating system type

- `CMAKE_SYSTEM_NAME`
    ```cmake
    if (CMAKE_SYSTEM_NAME MATCHES "Windows")
    elesif (CMAKE_SYSTEM_NAME MATCHES "Linux")
    elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    endif()
    ```

- built-in variables

    ```cmake
    if (WIN32)
    elseif (UNIX AND NOT APPLE) # FreeBSD Linux Android MacOS iOS
    elseif (APPLE) # MacOS iOS
    endif()
    ```

- generator

    syntax: $<$<TYPE:VALUE>:WHEN TRUE EXPR>

    ```cmake
    target_compile_definitions(main PUBLIC
        $<$<PLATFORM_ID:Windows>:MY_NAME="Bill">
        $<$<PLATFORM_ID:Linux>:MY_NAME="Linus">
        $<$<PLATFORM_ID:Darwin>:MY_NAME="Steve">
    )
    $<$<CXX_COMPILER_ID:GNU,Clang>:MY_NAME="Open-source"> # multi-VALUE
    $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<PLATFORM_ID:Linux,FreeBSD>>:MY_NAME="Open"> # complex judgement
    ```

## `if`

Call cmake variables with `${}` except if

```cmake
set(Hello world) # ${Hello}=world
set(MYVAR Hello) # ${MYVAR}=Hello
if (${MYVAR} MATCHES "Hello") # ${MYVAR} will be expend to Hello then Hello will be expand to world
    message("MYVAR is Hello")
else()
    message("MYVAR is not Hello") # this will be execute
endif()
```

## variable inherit

The variable only can be inherited to the sub-directories by default.

Sub-module can modify parent's variable using `PARENT_SCOPE`:

```cmake
set(MYVAR ON PARENT_SCOPE)
```


