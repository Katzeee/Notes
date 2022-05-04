# 2021.3.29

## vim命令

```
进入编辑模式，按i进行编辑
编辑结束，按ESC键 跳到命令模式
:w保存文件但不退出vi 编辑
:w! 强制保存，不退出vi 编辑
:w file将修改另存到file中，不退出vi 编辑 
:wq保存文件并退出vi 编辑
:wq!强制保存文件并退出vi 编辑
q:不保存文件并退出vi 编辑
:q!不保存文件并强制退出vi 编辑
:e!放弃所有修改，从上次保存文件开始在编辑
```

# 2022.1.17

## vim 跨文件复制
```
:sp或:vsp分割出两个窗口
:e .打开另一个文件
^W再按w切换两个窗口
yy可以复制一行
```
现在就可以复制了

## vim tab页

```
:tabnew 创建新tab
:tabp前一个
:tabn后一个
:tabc关闭当前的tab
:tabo关闭所有其他的tab
:tabs查看所有打开的tab
gt和gT可以直接在tab件切换
```
## vim中执行shell命令

```
:! command
```


# 2022.4.19

vim 快捷键
```
/：向下搜索
?：向上搜索
n：跳到下一个搜索值
N：跳到上一个搜索值
*：向下搜索到同一个单词处
#：向上搜索到同一个单词处
```

vim easymotion
```
<leader><leader> f <char>	Find character forwards
<leader><leader> F <char>	Find character backwards
<leader><leader> s <char>  Search character 
<leader><leader> / <char>... <CR>   Search n-character 
<leader><leader> (2s|2f|2F|2t|2T) <char><char>
```

vim Input Method change
可以在换模式的时候自动切换输入法
- 在https://github.com/daipeihust/im-select下载windows版64位im-select.exe
- 将其放在英文命名的文件夹下，并将文件夹添加进path
- 在文件夹下打开cmd分别在中英文输入法的情况下运行im-select，记录中文数字2052和英文数字1033
- 在settings.json中加入以下
  ```
    "vim.autoSwitchInputMethod.enable": true,
    "vim.autoSwitchInputMethod.defaultIM": "1033",
    "vim.autoSwitchInputMethod.obtainIMCmd": "D:\\im-select\\im-select.exe", 
    "vim.autoSwitchInputMethod.switchIMCmd": "D:\\im-select\\im-select.exe {im}", # im不用换 
  ```

VS Code has a lot of nifty tricks and we try to preserve some of them:
```
gd - jump to definition.
gq - on a visual selection reflow and wordwrap blocks of text, preserving commenting style. Great for formatting documentation comments.
gb - adds another cursor on the next word it finds which is the same as the word under the cursor.
af - visual mode command which selects increasingly large blocks of text. For example, if you had "blah (foo [bar 'ba|z'])" then it would select 'baz' first. If you pressed af again, it'd then select [bar 'baz'], and if you did it a third time it would select "(foo [bar 'baz'])".
gh - equivalent to hovering your mouse over wherever the cursor is. Handy for seeing types and error messages without reaching for the mouse!
```