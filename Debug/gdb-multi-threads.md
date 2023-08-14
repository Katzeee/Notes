2023.08.09
---

> https://ftp.gnu.org/old-gnu/Manuals/gdb-5.1.1/html_node/gdb_39.html
> 
> https://stackoverflow.com/questions/6721940/how-to-restrict-gdb-debugging-to-one-thread-at-a-time
>
> https://undo.io/resources/gdb-watchpoint/debugging-multithreaded-programs-gdb/

## Gdb for multi threads

```shell
info threads # show infos of all threads
thread <num> # jump to specified thread
set scheduler-locking step # disable thread schedule, that is only step at one thread
thread apply all <command> # apply a command to all threads # ex: thread apply all bt
```

