---
title: C String Manipulation
date: 2021.09.13
categories: [C-Cpp]
tags: [c-cpp]
---

## `sscanf`

using `sscanf` for format conversion

```c
char a[] = "123456";
int i;
sscanf(a, "%d", &i);//convert string "123456" to int 123456
char b[] = "0x8000";
int j;
sscanf(b, "0x%x", &j);//convert string "0x8000" to int 0x8000 i.e. 32768
```

## `strtok`

it can break a string into a sequence of zero or more nonempty tokens, we can use it to split strings

```c
char a[] = "Th is A";
char* s;
s = strtok(a, " ");//" " is the seperator of the string
//now s points T
s = strtok(NULL, " ");//we call strtok with the first arg being NULL except the first calling as we use it for the same string to get the next part
//now s points i
```

## `strncpy`
```c
char buf[11];//定义buf
strncpy(buf, src, 10); //进行拷贝
```
