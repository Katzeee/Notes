2023.03.01
---

References:
> https://blog.csdn.net/tian_20032242/article/details/88793258
>
> https://blog.csdn.net/tian_20032242/article/details/88922149
> 
> https://www.jianshu.com/p/6d4744f8e5aa
>
> https://zhuanlan.zhihu.com/p/348520921


## const

Remember `const` always modifies the adjacent types on the left.

```cpp
int c = 1;
int const *p = &c;  // const modifies int, so *p can't change
const int *p = &c;  // no left, then modifies right, which is int
int *const p = &c;  // modifies int *, so p can't change
```

## auto inference

- `auto` inference uses template type inference.

  ```cpp
  auto x; // same as T where
  template<typename T>
  void foo(T x);
  auto &x; // same as T where
  template<typename T>
  void foo(T& x);
  auto &&x; // same as T where
  template<typename T>
  void foo(T&& x);
  ```

  Then for template inference, 

  - T x

  x is considered as a copy of the argument, so its const and volatile will be removed.

  ```cpp
  foo(int) -> T is int
  foo(int *) -> T is int *
  foo(const int) -> T is int
  foo(const int &) -> T is int
  foo(const int *) -> T is const int * // a copy of a pointer who point to a const vlaue, so copy this pointer will also point to a const value
  foo(int *const) -> T is int * // a copy of const pointer, so the copy can remove its const
  foo(1) -> T is int
  foo(int[n]) -> T is int *
  foo(void(int)) -> T is void (*)(int)
  ```

  - T &x

  x is a reference of argument, so can't pass a rvalue. Ignore the reference of the argument, then infer its type.

  ```cpp
  foo(int) -> T is int
  foo(int *) -> T is int *
  foo(const int) -> T is const int
  foo(const int &) -> T is const int // also a lvalue reference like above
  foo(const int *) -> T is const int * // like above, this argument is also a lvalue
  foo(int *const) -> T is int *const // like above
  foo(1) -> T is int, so invalid
  foo(int[n]) -> T is int[n], T& is int(&)[n] // array reference
  foo(void(int)) -> T is void(int), T& is void (&)(int)
  ```

  - T &&x

  x is a universal reference of argument, so can be passed a rvalue. Refer to [perfect forword](./cpp-perfect-forwarding.md)

  ```cpp
  foo(int) -> T is int &
  foo(int *) -> T is int *&
  foo(const int) -> T is const int &
  foo(const int &) -> T is const int &
  foo(const int *) -> T is const int *&
  foo(int *const) -> T is int *const &
  foo(1) -> T is int
  ```

- `auto` will be conduced to the correct type with least `const`.

  ```cpp
  const int a = 0;
  auto b = a;  // int b = a;
  auto &b = a; // const int &b = a; because int &b = a; is invalid
  auto *b = &a; // const int *b = a;
  auto &&b = a; // consider reference collapse, 
  ```

- For any `auto` expression, remove its `const` first then start to infer. Then put `const` to the right position where its modifing type.

  ```cpp
  int a = 1;
  int *p = &a;
  auto q = p; // int *q = p;
  const auto q = p; auto const q = p; // auto -> int *, const modify int *, int *const q = p;
  const auto *q = p; // auto -> int, const modify int, const int *q =p;
  auto *const q = p; // auto -> int, const modify int *, int *const q = p;
  ```
