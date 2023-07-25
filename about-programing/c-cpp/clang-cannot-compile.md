2022.07.25
---

## Problem

I got an error when I compile a project using `clang` like this:

```
 $> clang++ -stdlib=libc++ main.cpp /usr/bin/ld: cannot find -lc++abi
```

or 

```
 $> clang++ -stdlib=libc++ main.cpp /usr/bin/ld: cannot find -lstdc++
```

## Solution

First try to install packages `libc++-dev`

If not work, check all folders in `/usr/lib/gcc/aarch64-linux-gnu/clang++`, ensure that there is a `libstdc++.a` in the last folder

