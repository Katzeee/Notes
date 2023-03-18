> Static link and load

- Use `-fno-pic` compile and `-static` link

---

> Somthing about GNU Binutils(a collection of binary tools)

- addr2line

  ```shell
  $addr2line 0x000000000000878 ./a.out # Get the source line number at the object address
  ```

- objdump

  ```shell
  $ objdump -d a.out # disassemble the test section of a.out
  $ objdump -D b.o # disassemble all sections of b.o
  ```

- nm 

  Show all symbols and its address

- readelf

- objcopy

  Copy from an object file

- gprof

  profiler

---

> About main.o before linking

Jump locations are just 0, they will be set to offset after linking

```asm
14:   e8 00 00 00 00    callq 19 <main+0x19>
```

Because main.c don't know where to jump before linking, they may located at other files

> How to know those offsets(say static link)

Table in `relocation section` recorded these offsets

```shell
$ readelf -a main.o # use readelf to find it(search `offset`)
```

> Why the offset in that table need to minus 4

PC reg has pointed to the next instruction

---

> `file`

```shell
$ file a.out # display info of a.out
```

> Where is the first instruction of this file? Is it main?

```shell
$ readelf -h a.out # display info of elf files
# Entry point address shows the first instruction address
```

> `starti` in `gdb`

Use `starti` to debug a program from the first instruction, which is `_start()`

```shell
$ gdb ./a.out
(gdb) starti
```

> Use `strace` to get all of the system call of the program

```shell
$ strace ./a.out 
```

---

> Why use `gcc` link rather than `ld`? Why it cannot run via `ld`?

```shell
# Will get a warning，cannot find entry symbol, run with a segmentation fault
$ ld a.o b.o main.o 
```

Try to use `starti` to debug it. When `main` returns, it will return to the top of the call stack which is a meaningless address because `main` is called by OS not `_start`.

Use `bt` to show the call stack, then you will see it.

> Minimal executable

Use syscall to do it.

```asm
mov $0xe7 %rax # interupt number(exit)
mov $0x63 %rdi # return value
syscall
```

---

> How `gcc` link?

Use `gcc -Wl --verbose`, `-Wl` means add flags to `ld`, so `--verbose` is add to `ld`

```Makefile
LDFLAGES += -Wl,--verbsoe
```

---

> `__attribute__((constructor))` and `__attribute__((destructor))`

No standard C

```c
__attribute__((constructor)) void a() { } // before main
__attribute__((destructor)) void a() { } // after main

int main() 
{
    atexit(b); // after main(standard C) stdlib.h
}
```

---

> Dynamic link

No `-fno-pic` and `-static` in LDFLAGES

> `get_pc_thunk` 

Use to get the address of next PC, because i386 doesn't support mov to eip

> How to get the link info?

Use `ldd`

```shell
$ ldd a.out
$ vim $(which ldd) # ldd is a script, so you can edit it via vim
```
`lbc.so.6` is the dynamic link library of `libc`
`ld-linux-x86-64.so.2` is the interpreter, which is the *real thing* execute the code

> How dynamic linked a.out run

```shell
$ ./a.out # is the same as
$ /lib64/ld-linux-x86-64.so.2 ./a.out # the real thing it did
```

Static linked programs run by just putting the binary code into memory

> What does dynamic link do?

Use `gcc -fno-plt -c` to try this

```asm
call *table[PRINTF] # put a table in ELF, then every call search it from table
```

Default option, use plt

```asm
printf@plt:
	jump *table[PRINTF]
	push $PRINTF
	call resolve
```

