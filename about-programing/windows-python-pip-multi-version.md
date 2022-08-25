2022.08.25
---
Manage multi-version of python on Windows11.

I met a problem when I was using `xlwings` to deal with some `xlsx` files which is a module missing called `win32api`.

After searching, I knew that I should install `pywin32` by `pip`, but something went wrong when I was installing it.

First, I got this error:

```bash
pip install pywin32 
Defaulting to user installation because normal site-packages is not writeable
ERROR: Could not find a version that satisfies the requirement pywin32 (from versions: none)
ERROR: No matching distribution found for pywin32
```

This is because I'm using python installed by `mingw`, and got this solution:

> There are no binaries released for mingw, you need to build it from source

Then I found to build it from source I need `VS` and `Win 10 SDK` which is time-consuming, so I choose to install another version of python.

I installed python3.9 from official website, then I got three versions of python on my computer:

Notice: All of them should be added in `%PATH%`.

```bash
$ where python
D:\msys64\mingw64\bin\python.exe # python3.10
C:\Users\Xac\AppData\Local\Microsoft\WindowsApps\python.exe
D:\Python\Python39\python.exe # python3.9
```

Then I can call them by:

```bash
$ py -3.9
Python 3.9.11 (tags/v3.9.11:2de452f, Mar 16 2022, 14:33:45) [MSC v.1929 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>> 
```

and

```bash
$ python
Python 3.10.5 (main, Jun 18 2022, 01:16:30)  [GCC 12.1.0 64 bit (AMD64)] on win32
Type "help", "copyright", "credits" or "license" for more information.
>>>
```

To install modules for a specified python:

```bash
py -3.9 -m pip install pywin32
```

BTW, I'm using VSC coding python, so I can directly change the path in python extension to quick run python code with a specified version.