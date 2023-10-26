#c-cpp/cmake 
## 2022.10.09

> https://cmake.org/cmake/help/latest/module/FetchContent.html

!!! Warning Only the most commonly used methods have been summaried, refer to the official documentation for detailed usage.

## `FetchContent`

Use `FetchContent` to fetch source code from github or some other source at cmake time and `ExternalProject` while build time


### `FetchContent_Declare`
```cmake
include(FetchContent)

# Fetch from github use tag
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.11.0
)

# Fetch from github use tag commit hash
# it is advisable to use a hash for GIT_TAG rather than a branch or tag name
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
)

# Fetch compressed file
FetchContent_Declare(
  myCompanyIcons
  URL      https://intranet.mycompany.com/assets/iconset_1.12.tar.gz
  URL_HASH MD5=5588a7b18261c20068beabfb4f530b87
)

# Fetch from SVN
FetchContent_Declare(
  myCompanyCertificates
  SVN_REPOSITORY svn+ssh://svn.mycompany.com/srv/svn/trunk/certs
  SVN_REVISION   -r12345
)
```

### `FetchContent_MakeAvailable`

After version 3.24:

- ~~If `dependency provider` is set...~~ (to be done, look up documents)

- If permitted, `find_package` will be called

Otherwise:

1. If the dependency has already been populated, set the `<lowercaseName>_POPULATED`, `<lowercaseName>_SOURCE_DIR` and `<lowercaseName>_BINARY_DIR` variables in the same way as a call to `FetchContent_GetProperties()`, then skip the remaining steps below and move on to the next dependency in the list.

2. Call `FetchContent_Populate()` to populate the dependency 

3. If the top directory of the populated content contains a `CMakeLists.txt` file, call `add_subdirectory()` to add it to the main build. Otherwise do nothing.

!!! Note You are supposed to declare all modules then call `FetchContent_MakeAvailable` once

```cmake
FetchContent_MakeAvailable(googletest myCompanyIcons)
```

### `FetchContent_Populate`

Saved details in `FetchContent_Declare` are used to construct a call to `ExternalProject_Add()` in a private sub-build to perform the content population immediately.

Set the `<lowercaseName>_POPULATED`, `<lowercaseName>_SOURCE_DIR` and `<lowercaseName>_BINARY_DIR`.

!!! Note Where possible, prefer to use `FetchContent_MakeAvailable()` instead of implementing population manually with this command.

### `FetchContent_GetProperties`

Set the same variables as a call to `FetchContent_MakeAvailable(name)` or `FetchContent_Populate(name)`. 

!!! Note This command is rarely needed when using `FetchContent_MakeAvailable()`. 

It is more commonly used as part of implementing the following pattern with FetchContent_Populate()

```cmake
# Check if population has already been performed
FetchContent_GetProperties(depname)
if(NOT depname_POPULATED)
  # Fetch the content using previously declared details
  FetchContent_Populate(depname)

  # Set custom variables, policies, etc.
  # ...

  # Bring the populated content into the build
  add_subdirectory(${depname_SOURCE_DIR} ${depname_BINARY_DIR})
endif()
```

