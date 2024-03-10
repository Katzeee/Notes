#code-env/vscode
## 2022.04.19

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