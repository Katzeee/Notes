#c-cpp #c-cpp/cpp20
## 2023.08.30

## range-based for

```cpp
std::vector<int> v { 1, 2, 3 };
for (auto &i : v) {
    ...
}
```

is equivalent to

```cpp
auto __begin1 = v.begin();
auto __end1 = v.end();
for(; __begin1 != __end1; __begin1++) {
    int &i = *__begin1;
    ...
}
```

## `for_each`(c++11) and `range::for_each`(c++20)

```cpp
template< class InputIt, class UnaryFunction >
UnaryFunction for_each( InputIt first, InputIt last, UnaryFunction f );
```

```cpp
template< ranges::input_range R, class Proj = std::identity,
          std::indirectly_unary_invocable<std::projected<ranges::iterator_t<R>, Proj>> Fun >
constexpr for_each_result<ranges::borrowed_iterator_t<R>, Fun>
    for_each( R&& r, Fun f, Proj proj = {} );
```

## Reverse traversal

```cpp
for (auto x : v | std::views::reverse) 
std::ranges::for_each(views::reverse(v), [](Type const& x){ cout << x; });
```

## Get next iterator

```cpp
std::next(@position, steps)
std::prev(@position)
```

Can be used on the iterators which not support random access like std::list(e.g. `i += 5`)
