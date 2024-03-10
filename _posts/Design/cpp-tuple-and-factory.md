#c-cpp #software-design
## 2022.10.10

## tuple can store reference

```cpp
a = 1;
std::tuple<int&> t(a);
std::get<0>(t)++; // a == 2 now
```

however, `make_tuple` can't do this.

```cpp
std::tupe<int&> t = std::make_tuple<int&>(a); // fail, can't covert std::tuple<int> to std::tuple<int&>
```

## expand tuple and pass to the next caller

For c++17, we have:
```cpp
void f(int a, float b) {
    a++;
}
std::tuple t(1, 2f);
std::apply(f, t); // = f(1, 2f); 
```

But there still something we can do with c++14 which is almost the same as the `std::apply`'s implementation.

```cpp
template<typename Func, typename ...Args>
void apply(Func f, std::tuple<Args...> &&args) {
    constexpr auto size = sizeof...(Args);
    apply_helper(f, std::forward<std::tuple<Args...>>(args), 
                 std::make_index_sequence<size>());
}

template<typename Func, typename ...Args, size_t ...I>
void apply_helper(Func f, std::tuple<Args...> &&args, std::index_sequence<I...>) {
    f(std::forward<Args>(std::get<I>(args))...); // here c++17 calls std::invoke
}

```


## factory class

So there is an easy implementation of the factory class:

```cpp
class ComponentBase {};

template <typename... Arg> class Factory {
public:
  using CreateFunc = std::function<ComponentBase *(Arg...)>;
  CompnentBase *Create(std::string classname, std::tuple<Arg...> &&args) {
    // constexpr auto size = std::tuple_size_v<std::tuple<Arg...>>;
    constexpr auto size = sizeof...(Arg);
    return CreateHelper(classname, std::forward<std::tuple<Arg...>>(args),
                 std::make_index_sequence<size>());
  }

  template <size_t... I>
  ComponentBase *CreateHelper(std::string classname, std::tuple<Arg...> &&args,
                    std::index_sequence<I...>) {
    auto create_func = create_funcs_.find(classname);
    if (create_func != create_funcs_.end()) {
      return create_func->second(std::forward<Arg>(std::get<I>(args))...);
    }
    return nullptr;
  }

// if c++17
  // ComponentBase *Create(std::string classname, std::tuple<Arg...> args) {
  //   auto create_func = create_funcs_.find(classname);
  //   if (create_func != create_funcs_.end()) {
  //     return std::apply(create_func->second, args);
  //   }
  // }

  void RegistCreateFunc(std::string classname, CreateFunc create_func) {
    create_funcs_[classname] = create_func;
  }

private:
  std::map<std::string, CreateFunc> create_funcs_;
};
```
