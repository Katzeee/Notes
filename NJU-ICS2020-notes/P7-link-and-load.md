> GNU Binutils

- addr2line

  ```shell
  $addr2line 0x000000000000878 ./a.out #根据地址得到是程序源文件的第多少行
  ```

- objdump

  ```shell
  $ objdump -d a.out | less #反汇编a.out的text section
  $ objdump -D b.o #反汇编b.o的所有段

- readelf

- objcopy

  ```shell
  #将二进制文件一段拷贝出来
  ```

- gprof

  ```shell
  #调试程序性能
  ```

---

> 链接前的代码main.o

链接前的代码有很多00，链接之后会变成偏移量

> 链接器如何填上偏移量

Relocation section的表帮助链接器填上上述偏移量

```shell
$ readelf -a main.o #采用readelf可以看到该信息
```

> 为什么偏移量要-4

PC寄存器指向下一条指令地址

---

> `file`命令

```shell
$ file a.out #展示a.out的信息
```

> `readelf`命令

```shell
$ readelf -h a.out #展示elf文件相关信息
#Entry point address为程序入口地址
```

---

> `gdb`的`starti`

`starti`可以从程序的第一条指令开始执行，从`_start()`开始

```shell
(gdb) starti
```

> `strace`

```shell
$ strace ./a.out #查看执行过程中所有系统调用序列
```
---
> 为什么用gcc链接而不是ld, ld链接的为什么不能运行？

```shell
$ ld a.o b.o main.o #会报warning，没有entry point，此时运行会造成segmentation fault
```

此时用`starti`进行调试时，当`main()` `return`时，由于`main`是操作系统调用的而不是`_start`，返回至调用栈的顶端，为无意义的地址，段错误

```shell
(gdb) bt #可以查看系统调用栈
```

---

> 查看ld编译过程

gcc编译选项中加入 -Wl, --verbose给ld加入verbose选项

```
LDFLAGES += -Wl,--verbsoe
```

---

> `__attribute__((constructor))`和`__attribute__((destructor))`

```c
__attribute__((constructor)) void a() { } //main函数调用之前调用
__attribute__((destructor)) void a() { } //main函数调用之后调用
int main() 
{
    atexit(b); //类似destructor
}
```

---

> 动态链接

在LDFLAGES里面去掉-fno-pic和-static

> `ldd`查看动态链接信息

```shell
$ ldd a.out
#lbc.so.6是libc的动态链接库
#ld-linux-x86-64.so.2是解释器
$ vim $(which ldd) #ldd是一个脚本，可以通过vim编辑
```

> 动态链接的a.out如何运行

```shell
$ /lib64/ld-linux-x86-64.so.2 ./a.out #实际上是通过这样运行的
# 静态链接的相当于直接把二进制文件搬进内存运行
```

> 如何链接

```assembly
call *table[PRINTF] 在ELF中放入一张TABLE，调用时查表
#可以使用gcc -fno-plt -c开启此款
```

```assembly
#默认选项，使用plt
printf@plt:
	jump *table[PRINTF]
	push $PRINTF
	call resolve
```

