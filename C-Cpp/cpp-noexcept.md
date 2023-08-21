#c-cpp/cpp11 #c-cpp/cpp14
## 2023.08.21

## No-Throw Guarantee: `noexcept` 

C++ has a mechanism for reporting errors using exceptions like many/most other programming languages. Don't worry, if you don't know what exceptions are, they will be explained in a later chapter.

The `noexcept` keyword specifies that a function promises to never throw exceptions / let exceptions escape:

```cpp
void foo () noexcept { … }
```

!!! If an exception escapes from a noexcept function anyway, the program will be aborted.

## Conditional `noexcept` Conditional

A `noexcept(expression)`

declares 'A' as noexcept if expression yields true

A `noexcept(noexcept(B))`

declares 'A' as noexcept if 'B' is noexcept

```cpp
constexpr int N = 5;
// 'foo' is noexcept if N < 9
void foo () noexcept( N < 9 ) { … }    
// 'bar' is noexcept if foo is
void bar () noexcept( noexcept(foo()) ) {  
    …
    foo();
    …
}
```

## `noexcept(_true_)` by default

**are all implicitly-declared special members**

-   default constructors
-   destructors
-   copy constructors, move constructors
-   copy-assignment operators, move-assignment operators
-   inherited constructors
-   user-defined destructors

###### unless

-   they are required to call a function that is noexcept(false)
-   an explicit declaration says otherwise