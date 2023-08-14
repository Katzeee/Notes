#c-cpp #compile  
## 2022.03.10

Reference:
> https://stackoverflow.com/questions/50105581/how-do-i-get-rid-of-call-x86-get-pc-thunk-ax

When I was compling a 32bit code in 64bit machine, I got a wierd function call in every functions.

```asm
call __x86.get_pc_thunk.bx
addl $_GLOBAL_OFFSET_TABLE, %ebx
```

This is because the complier defaults to generating "position-independent" excutables.

After `addl`, `$ebx` now points to the global offset table. This will call `f()` but jump through the table because `f()` may defined in another file rather than `main`.

Now in 64bit machine, we will just say like this

```asm
call f@PLT
```
without setting up `$ebx` first.
