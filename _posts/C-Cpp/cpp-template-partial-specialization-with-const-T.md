#c-cpp
## 2023.08.29

I found that I can't declare `vector<const int>` because there isn't a correct implementation of `allocator<const int>`. However, I can declare `vector<const int *>` which is so astonishing.

## Which template does `const int *` match?

```cpp
template<typename T>
struct match {
    ...
};

template<typename T>
struct match<const T> {
    ...
};

match<const int *>;
```

!!! Danger **It's the first one**

Consider like this:

```cpp
template<typename T>
struct match {
    ...
};

template<typename T>
struct match<T const> {
    ...
};

match<int const *>;
```

!!! Danger Obviously the second template isn't match.