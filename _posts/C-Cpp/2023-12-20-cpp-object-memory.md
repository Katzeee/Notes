---
title: Cpp Object Memory
date: 2023.12.20
categories: [C-Cpp]
tags: [c-cpp]
---

## Single inherit

```cpp
class A {
 public:
  virtual void f() {}
  int a;
};

class B : public A {
 public:
  void f() override {
    b = 1;
  }
  float b;
};

auto main() -> int {
  B *b = new B();
  A *a = b;
}
```

**Base and derived point to the same address**

```
(gdb) p *a
$1 = {_vptr.A = 0x555555557d40 <vtable for B+16>, a = 0}
(gdb) p *b
$2 = {<A> = {_vptr.A = 0x555555557d40 <vtable for B+16>, a = 0}, b = 0}
(gdb) p a
$3 = (A *) 0x55555556b2b0
(gdb) p b
$4 = (B *) 0x55555556b2b0
(gdb) p/x *(long*)a
$5 = 0x555555557d40
(gdb) p/x *(long*)b
$6 = 0x555555557d40
```

!!! `<vtable for B+16>` means this vptr points to the `vtable for B` offseting 16, because the 0-15 bytes may be the type info or something of the object

**Layout like this**

```
lower address   +------------------+
                | [Base vptr]      | <-- Base <-- Derived
                | [Base members]   | 
                +------------------+
                | [Derived members]| 
higher address  +------------------+
```

## Multi inherit

```cpp
class A {
 public:
  virtual void f() {}
  int a;
};

class B {
 public:
  virtual void g() {}
  int b;
};

class D : public A, public B {
 public:
  void f() override {
    c = 1;
  }
  float c;
};

auto main() -> int {
  D *d = new D();
  A *a = d;
  B *b = d;
}
```

**1. First base class and derived class point to the start of the object**  
**2. Second base class point to its own object**

```
(gdb) p a
$1 = (A *) 0x55555556b2b0
(gdb) p b
$2 = (B *) 0x55555556b2c0
(gdb) p d
$3 = (D *) 0x55555556b2b0
(gdb) p *a
$4 = {_vptr.A = 0x555555557cf8 <vtable for D+16>, a = 0}
(gdb) p *b
$5 = {_vptr.B = 0x555555557d10 <vtable for D+40>, b = 0}
(gdb) p *d
$6 = {<A> = {_vptr.A = 0x555555557cf8 <vtable for D+16>, a = 0}, <B> = {_vptr.B = 0x555555557d10 <vtable for D+40>, b = 0}, c = 0}
```

```
lower address   +------------------+
                | [Base1 vptr]     | <-- Base1 <-- Derived
                | [Base1 members]  | 
                +------------------+
                | [Base2 vptr]     | <-- Base2
                | [Base2 members]  | 
                +------------------+
                | [Derived members]| 
higher address  +------------------+
```

## Virtual inherit

```cpp
class A {
 public:
  virtual void f() {}
  int a;
};

class B : virtual public A {
 public:
  float b;
};

auto main() -> int {
  B *b = new B();
  A *a = b;
}
```

**1. Virtual base is at higher address.**

```
   A
 /   \
B     C
 \   /
   D
```

```
(gdb) p a
$1 = (A *) 0x55555556b2d0
(gdb) p b
$2 = (B *) 0x55555556b2b0
(gdb) p c
$3 = (C *) 0x55555556b2c0
(gdb) p d
$4 = (D *) 0x55555556b2b0
(gdb) p *a
$5 = {_vptr.A = 0x555555557c30 <vtable for D+96>, a = 0}
(gdb) p *b
$6 = {<A> = {_vptr.A = 0x555555557c30 <vtable for D+96>, a = 0}, _vptr.B = 0x555555557be8 <vtable for D+24>, b = 0}
(gdb) p *c
$7 = {<A> = {_vptr.A = 0x555555557c30 <vtable for D+96>, a = 0}, _vptr.C = 0x555555557c10 <vtable for D+64>, b = 0}
(gdb) p *d
$8 = {<B> = {<A> = {_vptr.A = 0x555555557c30 <vtable for D+96>, a = 0}, _vptr.B = 0x555555557be8 <vtable for D+24>, b = 0}, <C> = {_vptr.C = 0x555555557c10 <vtable for D+64>, b = 0}, c = 0}
```

```
lower address   +------------------+
                | [B vptr]         | <-- B <-- D
                | [B members]      | 
                +------------------+
                | [C vptr]         | <-- C
                | [C members]      | 
                +------------------+
                | [D members]      | 
                +------------------+
                | [A vptr]         | <-- A
                | [A members]      | 
higher address  +------------------+
```

**2. b points to `_vptr.B`, a points to `_vptr.A`**

```
(gdb) p/x *(long*)b
$8 = 0x555555557d08
(gdb) p/x *(long*)a
$9 = 0x555555557d20
```
```
lower address   +------------------+
                | [Derived members]| 
                | [Derived vptr]   | <-- Derived
                +------------------+
                | [VBase members]   | 
                | [VBase vptr]      | <-- Virtual Base
higher address  +------------------+
```