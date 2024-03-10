#c-cpp/debug
## 2022.09.07

## Memory leak

I met an error caused by memory leak today, which is very hard to solve. 

Sometimes I new an object in function `ReadBuffer::GetPacket()`, I will get `malloc(): corrupted top size`, but sometimes not.

First, I thought it was caused by the memory allocation in construct function, but not so.

After several times seeking, I found if I `send` too much data through socket, I will get this error. But it confused me a lot because these two things seem having no relation.

Then I thought maybe I forgot to `free` or `delete` some pointer, still not.

Finally, by using `call malloc_stats()` in every context of `malloc` or somewhere maybe causing this error, I found I accessed a memory address in a mistake.

```c++
// Error Code
char* buffer = (char*)::malloc(sizeof(uint16_t));
...
data_size = ::recv(socket_fd_, buffer, read_buffer_->GetEmptySize(), 0);
```

I allocated 4 bytes to buffer, but `recv` too much data in buffer which caused this error.

**PS: `debuginfod` can be used to step into function in `libc` when debugging via `gdb`**

---
## 2022.10.26

## sanitizer

Add a compile option `-fsanitizer=address` may solve this problem easily.