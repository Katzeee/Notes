#include <functional>
#include <map>
#include <iostream>
#include <tuple>

template <typename Func, typename... Args>
void apply(Func f, std::tuple<Args...> &&args) {
  constexpr auto size = sizeof...(Args);
  apply_helper(f, std::forward<std::tuple<Args...>>(args),
               std::make_index_sequence<size>());
}

template <typename Func, typename... Args, size_t... I>
void apply_helper(Func f, std::tuple<Args...> &&args,
                  std::index_sequence<I...>) {
  f(std::forward<Args>(std::get<I>(args))...);
}

class ComponentBase {};

template <typename... Arg> class Factory {
public:
  using CreateFunc = std::function<ComponentBase *(Arg...)>;
  ComponentBase *Create(std::string classname, std::tuple<Arg...> args) {
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

int main() {
  apply([](int a, int b) { std::cout << a << b << std::endl; },
        std::tuple(1, 2));

  std::apply([](int a, int b) { std::cout << a << b << std::endl; },
             std::tuple(1, 2));

  std::apply([](int a, int b) { return a; }, std::tuple(1, 2));
  return 0;
}
