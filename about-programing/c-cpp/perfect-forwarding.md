2022.06.23
---
## Reference Collapsing Rules

- `T& & -> T&`
- `T&& & -> T&`
- `T& && -> T&`
- `T&& && -> T&&`

## Universal reference

Universal reference can not only act as a rvalue-reference but also a lvalue-reference.

```c++
template<typename T>
void somefunc(T&& somearg) {
    ...
}
```

- If `somefunc` is called with an lvalue, `T` will be expanded to `T&`, then `T& && -> T&`, so `somearg` is an lvalue-reference.
- If `somefunc` is called with an rvalue, `T` will be expanded to `T`, so `somearg` is an rvalue-reference.

We can call this function by `somefunc(var)` or `somefunc(42)`.

## use `std::forward` to implement perfect forwarding

Think about following snippet:

```c++
template<typename Func, typename Arg>
void somefunc(Func f, Arg&& somearg) {
    f(somearg);
}

void something(int&& arg) {
    arg++;
}

somefunc(something, 1);
```

When we call `f(somearg)` in `somfunc`, as `somearg` is an rvalue-reference which is also an lvalue,  but something receives an rvalue, we will get a compile error: `cannot bind rvalue reference of type 'int&&' to lvalue of type 'int'`.

Using `std::forward` can solve this problem:

```c++
template<typename Func, typename Arg>
void somefunc(Func f, Arg&& somearg) {
    f(std::forward<Arg>(somearg));
}
```

`std::forward` can forward the argument to the called function without changing the argument type which means `std::forward` forwards an lvalue-reference if `Arg` is deduced to `Arg&`, or an rvalue-reference if `Arg` is deduced to `Arg`.

Thus, with using `std::forward`, we can perfectly forward the arguments to the next called function and make the intermediate function act as it knows nothing.