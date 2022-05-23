# 2022.5.23

It is hard to debug dwm, because it is a window manager which is the fundamental of the GUI programs.

To debug dwm, we need use `Xephyr` which can launch a small built-in Xserver in current Xserver.

So we can do like this:

```bash
$ Xephyr -ac -screen 1920x1080 -br -reset -terminate 2> /dev/null :1 &
```

Then we can launch a terminal by:

```bash
$ DISPLAY=:1 st&
```

In the terminal can we debug dwm:

```bash
$ gdb ./dwm
```

BUT there are some problems. 

- The dwm won't exit when it crashed, because it is still running by gdb. However, I can do nothing in this crashed dwm, which means I can't use gdb to get the information why dwm crashed.
  So we need to set logging on when debugging dwm. Also can we run some commands from file by `-x` option:
  ```
  # gdbcommands.txt
  set logging on
  set logging overwrite
  set print pretty on
  set print array-indexes on
  break focusstack
  commands
      silent
      printf "\n"
      bt
      print *selmon
      continue
  end
  ```
  ```bash
  $ gdb ./dwm -x gdbcommands.txt
  ```
  This let gdb run the commands in the commands section whenever meet the breakpoint at focusstack

- I can't get any variable information by `print`
  We need to change the `Makefile`, putting `-g` option when compiling dwm.