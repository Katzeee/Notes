# 2022.4.18

在vscode中为md文件配置自定义代码块

### 设置->snippets->markdown.json

- prefix :这个参数是使用代码段的快捷入口，比如这里的log在使用时输入Iog会有智能提示。
- body :这个是代码段的主体.需要设置的代码放在这里,字符串间换行的话使用\r\n换行符隔开.注意如果值里包含特殊字符需要进行转义。多行语句的以逗号隔开
- $1 :这个为光标的所在位置。
- $2 :使用这个参数后会光标的下一位置将会另起一行,按tab键可进行快速切换,还可以有$3,$4,$5.....
- description :代码段描述, 在使用时智能提示时的描述

### 配置markdown

到此刻还不行，要在settings.json中也就是首选项中最后加入

```
"[markdown]": {
        "editor.quickSuggestions": true
    },
```

到此刻才可以进行代码补全

# 2022.4.19

## hotkeys binding

设置->首选项->Keyboard Shortcuts
代码补全快捷键原本<kbd>ctrl+space</kbd>，搜索trigger suggestion将其改为<kbd>alt+x</kbd>即可

不想用<kbd>enter</kbd>补全代码，在acceptsuggestion有一个绑定了enter，将其删除

# 2022.5.9

Changed a lot in settings.json and keybindings.json