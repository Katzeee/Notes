#game/ue5
## 2023.09.22

> https://forums.unrealengine.com/t/emacs-clangd-and-unreal-engine/246457/16  
> https://docs.unrealengine.com/5.3/en-US/build-configuration-for-unreal-engine/

**Change the build tool to clangd**

Change `<PROJECT_DIRECTORY>/Config/DefaultEngine.ini` `Compiler` flag under section `[/Script/WindowsTargetPlatform.WindowsTargetSettings]` to clangd.

**Generate the database**

```bash
$ PathToUnrealEngine\Engine\Build\BatchFiles\Build.bat -mode=GenerateClangDatabase -project="PathToYourProject\ProjectName.uproject" ProjectNameEditor Development Win64
```

!!! "ProjectNameEditor means concat your project name with "Editor""

**Copy the compile_commands.json to the project root dir**