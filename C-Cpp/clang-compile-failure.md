#c-cpp #compile 
## 2023.07.25

## Problem

I got an error when I compile a project using `clang` like this:

```bash
 $> clang++ -stdlib=libc++ main.cpp /usr/bin/ld: cannot find -lc++abi
```

or 

```bash
 $> clang++ -stdlib=libc++ main.cpp /usr/bin/ld: cannot find -lstdc++
```

or

```bash
main.cpp:5:10: fatal error: 'fstream' file not found
#include <fstream>
         ^~~~~~~~~
1 error generated.
```

## Solution

First try to install packages `libc++-dev`

If not work, check all folders in `/usr/lib/gcc/aarch64-linux-gnu/clang++`, ensure that there is a `libstdc++.a` in the folder with largest number.

