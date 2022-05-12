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

## Modern Cmake

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