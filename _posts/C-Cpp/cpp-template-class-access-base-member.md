#c-cpp
## 2023.09.14

> https://stackoverflow.com/questions/4643074/why-do-i-have-to-access-template-base-class-members-through-the-this-pointer  
> https://stackoverflow.com/questions/4010281/accessing-protected-members-of-superclass-in-c-with-templates 


```cpp
template<typename T> struct Superclass {
 protected:
  int b;
  void g() {}
};

template<typename T> struct Subclass : public Superclass<T> {
  void f() {
    g(); // compiler error: uncategorized
    b = 3; // compiler error: unrecognized
  }
};
```

## Solution

This can be amended by pulling the names into the current scope using `using`:

```cpp
template<typename T> struct Subclass : public Superclass<T> {
  using Superclass<T>::b;
  using Superclass<T>::g;

  void f() {
    g();
    b = 3;
  }
};
```

Or by qualifying the name via the `this` pointer access:

```cpp
template<typename T> struct Subclass : public Superclass<T> {
  void f() {
    this->g();
    this->b = 3;
  }
};
```

## Reason

When a compiler sees a template, it is supposed to perform certain checks immediately, without seeing the template parameter. Others are deferred until the parameter is known. It's called two-phase compilation.

The checks that are performed on the template itself, rather than on particular instantiations of it, require that the compiler be able to resolve the grammar of the code in the template.

In C++ (and C), in order to resolve the grammar of code, you sometimes need to know whether something is a type or not. For example:

```cpp
#if WANT_POINTER
    typedef int A;
#else
    int A;
#endif
static const int x = 2;
template <typename T> void foo() { A *x = 0; }
```

If A is a type, that declares a pointer. If A is an object, that's multiplication. 

If it is wrong, this error must be diagnosed _in phase 1_, it's defined by the standard to be an error _in the template_, not in some particular instantiation of it. 

Even if the template is never instantiated, if A is an `int` then the above code is ill-formed and must be diagnosed, just as it would be if `foo` wasn't a template at all, but a plain function.

**The standard says that names which aren't dependent on template parameters must be resolvable in phase 1.** `A` here is not a dependent name, it refers to the same thing regardless of type `T`. So it needs to be defined before the template is defined in order to be found and checked in phase 1.

!!! `T::A` would be a name that depends on T, But A would not.
