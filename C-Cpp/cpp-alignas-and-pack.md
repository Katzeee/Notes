#c-cpp 
## 2023.03.04

## Memory alignment

Memory are seen as multiple bytes. But CPU reads data as `words`, for 32-bits CPU, it always start to read from the addresses where can be divided by 4(bytes), like 0x0, 0x4.

If an interger stores at 0x3, CPU needs to read 0x0 to 0x3 at first time, 0x4 to 0x7 for the second time, then concat them to get that interger.

Memory alignment is used to promote the CPU access efficiency, `align n` means the address of the data must start at some place whose address must can be divided by n.

## `#pragma pack`

Use to specify the data alignment.

```cpp
#pragma pack(push, 1)
struct A {
  char x;
  int y;
};
#pragma pack(pop)
// sizeof(A) == 5

struct B {
  char x;
  int y;
};
// sizeof(A) == 8
```

## `alinas`

```cpp
char buf[64]; // align 1
alinas(64) char buf[64]; // align 64, used in align_membuf to maintain its origin alignment
```
Mention that `alinas(n)`, `n` must greater or equal than `alignof(T)`, and `n` must be `$2^x$`

```cpp
struct alinas(1) B { // invalid
  char x;
  int y;
};

// sizeof(B) == 8 or complie error
```
## Is 8-byte alignment for `double` necessary on 32bit CPU?

It does, for cache. Or it may divided into two cache line.
