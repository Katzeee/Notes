#code-env/vscode #code-env/neovim #c-cpp/cmake #c-cpp/debug

> https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/debug-launch.md#select-a-launch-target  
> https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/cmake-settings.md#command-substitution  
> https://code.visualstudio.com/docs/cpp/launch-json-reference

## Simplest template of debugging c/c++

```json
{
  // Use IntelliSense to learn about possible attributes.
  // Hover to view descriptions of existing attributes.

  "configurations": [
    {
      // name of the launch
      "name": "(gdb) Launch", 
      // the debugger type, `cppvsdbg` for visual studio windows debugger, `cppdbg` for gdb or lldb
      "type": "cppdbg", 
      // launch or attach
      "request": "launch",
      // program path
      "program": "${workspaceFolder}/a.out",
      "args": [],
      "stopAtEntry": false,
      // working directory
      "cwd": "${fileDirname}",
      // environment varibles
      "environment": [{ "name": "config", "value": "Debug" }],
      // use external console rather vscode console
      "externalConsole": false,
      // gdb or lldb
      "MIMode": "gdb",
      "setupCommands": [
          {
              "description": "Enable pretty-printing for gdb",
              "text": "-enable-pretty-printing",
              "ignoreFailures": true
          },
          {
              "description": "Set Disassembly Flavor to Intel",
              "text": "-gdb-set disassembly-flavor intel",
              "ignoreFailures": true
          }
      ]
    } 
  ]
}
```

## Cmake project

## Single cpp file