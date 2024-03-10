#c-cpp 
## 2022.06.21

## `concept` and `requires`

There are two type of `requires`.

- One is like a function returning a `constexpr` true or false 
  ```c++
  template<typename T>
  concept can_exec = requires(T a) { a%10; }
  
  template<typename T>
  concept can_exec = requires { typename T::run; }
  ```
  It will return true if the statement in brackets can be executed.

- Another is using as a condition in instantiate the template.

  ```c++
  template<class T>
  requires can_exec<T>
  class req<T> {
  public:
      req() { std::cout << "1"; }
  };
  ```
  Only if the `concept` following the `requires` returns true, this template will possibly be instantiated.

## Use of `requires`

Sometimes we don't define a concept but use `requires` directly like this:

```c++
template<class T>
requires requires { typename T::someattr; }
class someclass<T> : public someType {
};
```

The first `requires` acts as an `if` statement and the second defines the content to be evaluated.