2022.06.18
---
> https://stackoverflow.com/questions/27687389/how-does-void-t-work
>
> Walter Brown states that the default argument has to be the exact same type as the one used in `void_t` for it to work.

```c++
template<typename T, typename = void>
struct has_member : false_type {};

template<typename T>
struct has_member<T, void_t<T::member>> : true_type {};
```

**I am wondering why I must match the `typename` in using `void_t`. Why I cannot use `int` as default `typename` in the primary template.**

The matching mechanism for partial specialization is as follows:

- When calling `has_member<X>`, it will *first be completed to match the primary template using the default arguments which (here) is `void`.* So `has_member<X>` means `has_member<X, void>`.

- Then the compiler will find whether there is a specialization for the calling. Here is `has_member<X, void_t<X::member>>`, and we suppose it is a well-formed expression, so it will be seen as `has_member<X, void>`.

- Now the primary and the specialization template are both matched to this calling, so the compiler chooses the specialized one.

But what if the primary is written as follows:

```c++
template<typename T, typename = int>
struct has_member : false_type {};
```

- The calling of `has_member<X>` is seen as `has_member<X, int>`.

- However, the specialization is `has_member<X, void>` which is not match of the primary template.

- Thus, the primary template is selected by the compiler.


