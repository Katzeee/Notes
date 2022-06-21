# 2021.5.14

- tutorial

> https://blog.csdn.net/cjmqas/article/details/79282847

- official github repo

> https://github.com/microsoft/vcpkg

# 2022.5.5

## using vcpkg on ubuntu

- cd to the folder where you want to install vcpkg

```bash
$ cd /usr/local
```

- download the src

```bash
$ git clone https://github.com/microsoft/vcpkg.git
```

- install

```bash
$ ./vcpkg/bootstrap-vcpkg.sh
```

- change the file owner and group(I don't know whether it is necessary.)

```bash
$ sudo chown -R xac:xac /usr/local/vcpkg
```

- use vcpkg with vscode

adding the following to the workspace `setting.json`

```json
{
  "cmake.configureSettings": {
    "CMAKE_TOOLCHAIN_FILE": "[vcpkg root]/scripts/buildsystems/vcpkg.cmake"
  }
}
```

adding the following to the `c_cpp_properties.json`

```json
"includePath":["${vcpkgRoot}/x64-linux/include/**"]
```

## ways to solve 99% problem

- **DELETE THE CMAKECHACHE AND BUILD AGAIN!!**
- **REOPEN VSCODE AND BUILD AGAIN!!**