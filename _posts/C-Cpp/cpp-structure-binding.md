#c-cpp/c-cpp17
## 2023.11.03

> https://github.com/MeouSker77/Cpp17/blob/master/markdown/src/ch01.md#%E4%BD%BF%E7%94%A8%E4%BF%AE%E9%A5%B0%E7%AC%A6

## Simple examples for structure binding

You can bind to a `pair`, `tuple`, C-style array, `std::array` or a POD struct.

```cpp
std::pair<int, std::string> p{ 1, "name" };
auto [a, b] = p;

std::tuple<int, int, std::string> t{ 1, "name" }
auto [a, b, c] = t;

struct S {
    int x;
    std::string y;
};
S s{ 1, "name" };
auto [a, b] = s;
auto [a, b](s);
auto [a, b]{s};
```

## Implementation

```cpp
auto [u, v] = ms;

// equals to
// e is a copy of ms
auto e = ms; // anonymous e, can't access
aliasname u = ms.u;
aliasname u = ms.v;
```

!!! note `u` and `v` are not the reference of `ms.u`, `ms.v`, but an alias name. `decltype(u)` will returns the type of `ms.u`, which can means structure binding to `e`, rather than `u` and `v`.

## Decorations

All decorations are implement on anonymous `e`:

```cpp
const auto &[u, v] = ms; 

// equals to
const auto &e = ms; 
```

!!! Warning You can bind a const lvalue reference to a right value and extent its life to the life of the reference

```cpp
MyStruct getStruct();
...
const int &x = 1; // OK
const auto &[a, b] = getStruct(); // OK
```

also: align e not u and v

```cpp
alignas(16) auto [u, v] = ms; // u will be align because it's the first member
```

also: no decay

```cpp
struct S {
    const char x[6];
    const char y[3];
};
S s1{};
auto [a, b] = s1; // decltype(a) = const char[6], not const char *
auto a2 = a; // decltype(a2) = const char *
```

## `move` semantic

```cpp
MyStruct ms = {42, "Jim"};
auto &&[v, n] = std::move(ms); // anonymous e is the rvalue reference of ms
// ms still hold the value

std::string s = std::move(n);
// ms doesn't hold the value
std::cout << ms.s; // incorrect
std::cout << n; // incorrect
std::cout << s; // OK
```

```cpp
MyStruct ms = {42, "Jim"};
auto [v, n] = std::move(ms);
// ms doesn't hold the value

std::cout << ms.s; // incorrect
std::cout << n; // OK
```

## Assign new values for structure binding of `pair` and `tuple`

```cpp
std::boyer_moore_searcher bmsearch{sub.begin(), sub.end()};
for (auto [beg, end] = bmsearch(text.begin(), text.end());
    beg != text.end();
    std::tie(beg, end) = bmsearch(end, text.end())) {
    ...
}
```

## For any type

You need to implement the following api to use structure binding for your own type:

- `struct std::tuple_size<type>::value`
- `struct std::tuple_element<idx, type>::type`
- `get<idx>()`
    - readonly `get` returns value
    - read write `get` returns reference
        - `get<idx>(C c)`
        - `get<idx>(const C &c)`
        - `get<idx>(C &&c)`

```cpp
#include <string>
#include <utility>  // for std::move()

class Customer {
private:
    std::string first;
    std::string last;
    long val;
public:
    Customer (std::string f, std::string l, long v)
        : first{std::move(f)}, last{std::move(l)}, val{v} {
    }
    const std::string& firstname() const {
        return first;
    }
    std::string& firstname() {
        return first;
    }
    const std::string& lastname() const {
        return last;
    }
    long value() const {
        return val;
    }
    long& value() {
        return val;
    }
};
```

```cpp
#include "customer2.hpp"
#include <utility>  // for tuple-like API

// implement tuple-like API to support structure binding
template<>
struct std::tuple_size<Customer> {
    static constexpr int value = 3;
};

template<>
struct std::tuple_element<2, Customer> {
    using type = long;
}
template<std::size_t Idx>
struct std::tuple_element<Idx, Customer> {
    using type = std::string;
}

// specialized getter
template<std::size_t I> decltype(auto) get(Customer& c) {
    static_assert(I < 3);
    if constexpr (I == 0) {
        return c.firstname();
    }
    else if constexpr (I == 1) {
        return c.lastname();
    }
    else {  // I == 2
        return c.value();
    }
}
template<std::size_t I> decltype(auto) get(const Customer& c) {
    static_assert(I < 3);
    if constexpr (I == 0) {
        return c.firstname();
    }
    else if constexpr (I == 1) {
        return c.lastname();
    }
    else {  // I == 2
        return c.value();
    }
}
template<std::size_t I> decltype(auto) get(Customer&& c) {
    static_assert(I < 3);
    if constexpr (I == 0) {
        return std::move(c.firstname());
    }
    else if constexpr (I == 1) {
        return std::move(c.lastname());
    }
    else {  // I == 2
        return c.value();
    }
}
```