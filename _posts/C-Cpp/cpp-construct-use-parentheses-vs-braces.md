#c-cpp 
## 2023.08.02

> https://zhuanlan.zhihu.com/p/268894227

## Prevent narrow conversion

```cpp
C(int a);

float pi = 3.14;
C c(pi); // convert to int
C c{pi}; // error
```

## Construct without arguments

```cpp
C();

C c(); // wrong, function declear
C c; // non-parm construct
C c{}; // non-parm construct
```

## `std::initializer_list` always first

```cpp
C(int, double);
C(std::initializer_list<int>);

C c(10, 5.0); // C(int, double)
C c{10, 5.0}; // error, narrow convert
```
