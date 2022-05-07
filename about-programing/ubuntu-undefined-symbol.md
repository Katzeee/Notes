# 2022.5.7

I met a undefined symbol error when I was running a c project.

```
../bin/test: symbol lookup error: ../bin/test: undefined symbol: g_thread_proxy
```

To find out the reason, use `ldd ../bin/test`, you can see following,

then us `nm` and `grep` to find