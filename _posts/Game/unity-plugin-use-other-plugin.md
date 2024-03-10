#game/unity
## 2024.02.19

## Can't include other plugins' namespace in my own plugin's scripts

There was an error when I tried `using UnityEditor.Formats.Fbx.Exporter;` in my scripts even I had already install `FBX Exporter` in package manager, but it worked when I put it in a `monobehavior` script.

Finally, I found if you want to use another plugin in your plugin, you should add it as a reference in `.asmdef` file.

![alt text](../.images/unity-plugin-asmdef-reference.png)