#linux/tool
## 2023.11.19
    
> https://htr3n.github.io/2018/07/faster-zsh/

## Performance Analysis

### Overall execution time

Crunching some numbers on Zsh loading time would be definitely useful for further in depth analysis of its performance. A simple timing of Zsh startup time can be measured approximately using the `time` command that is available in most Unix/Linux/Mac systems.

```bash
$ /usr/bin/time /bin/zsh -i -c exit
        0.67 real         0.41 user         0.26 sys
```

The output of the command shows the execution time of Zsh breaking down to user-land and system. In order to get a better approximation, we can make a loop that invokes `time` for 10 or greater, if possible.

```bash
for i in $(seq 1 10); do /usr/bin/time /bin/zsh -i -c exit; done;
```

This timing method is very fast and handy in case you want to quickly see how your Zsh performs, especially to test some changes you have just made.

### Profiling

Zsh provides a built-in module [`zsh/zprof`](http://zsh.sourceforge.net/Doc/Release/Zsh-Modules.html#The-zsh_002fzprof-Module) that can be used to profile Zsh functions. At the beginning of `~/.zshrc`, we add `zmodload zsh/zprof`. After restart the shell, we can use the command `zprof` to show a very rich output on Zsh startup loading. An illustrative output of `zprof` is shown below.

```bash
$ zprof
num  calls                time                       self            name
------------------------------------------------------------------------------
 1)    1          51.31    51.31   22.68%     49.68    49.68   21.96%  zle-line-init
 2)    2          45.72    22.86   20.21%     45.72    22.86   20.21%  compaudit
 3)  195          34.71     0.18   15.34%     25.52     0.13   11.28%  _zsh_autosuggest_bind_widget
 ...
```