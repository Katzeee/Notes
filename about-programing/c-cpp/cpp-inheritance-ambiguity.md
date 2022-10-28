2022.10.28
---

## Error: request for member ... is ambiguous.

Put examples directly:

```cpp
struct A {
  void f();
};

struct B {
  void f();
};

struct C : A, B { };

int main() { 
  C c; 
  c.f();    // obviously ambiguous
  c.B::f(); // not ambiguous
}
```

```cpp
struct A {
  void f();
};

struct B {
  void f(int);
};

struct C : A, B { };

int main() { 
  C c; 
  c.f(1);    // ambiguous ? yes!
}
```

This is hard to understand because A::f() has no arguments, but B::f(int) need an int as argument. We instinctively think the compiler will deduce the correct answer from the arguments list.

But for the compiler, in looking up the name, it finds functions in two different base class scopes. In this situation, the lookup is ambiguous - they don't overload each other. 

A fix would be to use a using declaration for each member name. Lookup will find names in the scope of `C` and don't look up further:

```cpp
struct C : A, B { using A::f; using B::f; };
```

Now, the call would find two functions, do overload resolution, and find that the one taking int will fit. 
