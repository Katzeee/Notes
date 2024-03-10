#c-cpp
## 2023.08.21

> https://hackingcpp.com/cpp/std/io_streams.html

## (Some) Standard Library Stream Types (Some) Standard Stream Types Stream Types


| ------------- | -------------------- | ----------------------------------------------------------------- |
| istream       | input stream         | reference istream& binds to any other kind of std:: input stream  |
| ostream       | output stream        | reference ostream& binds to any other kind of std:: output stream |
| ifstream      | input file stream    | extracted data is read from a file                                |
| ofstream      | output file stream   | inserted data is stored in a file                                 |
| ostringstream | output string stream | inserted data is stored in a string buffer                        |
| istringstream | input string stream  | extracted data is read from a string buffer                       |

## Overloading operator<< and operator>>

```cpp
struct point { int x;  int y; };

std::ostream& operator << (std::ostream& os, point const& p) {
  return os << '(' << p.x << ',' << p.y << ')';
}

std::istream& operator >> (std::istream& is, point& p) {
  return is >> p.x >> p.y;
}
point p {1,2};
cout << p << '\n';   // prints (1,2)
...
cin >> p;  // reads 2 ints into p.x and p.y
...
```

```cpp
cin  >> x >> y;  ⇔  operator>>( operator>>(cin, x), y)
cout << x << y;  ⇔  operator<<( operator<<(cout,x), y)
```

## Utilities

```cpp
std::getline (istream&, string&, stopat='\n')
```

```cpp
// forwards stream by n characters until stop character c
std::istream::ignore(n, c)
// skip next 8 characters
cin.ignore(8);
// skip by max. 10 characters or until after '='
cin.ignore(10, '=');
// skip until after next newline character
cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
```

!!! danger "`stringstream.clear()` can't clear the cache. Use `.str("")` instead."

## [Formatting Manipulators](https://hackingcpp.com/cpp/std/io_streams.html#panel-fold5a)

- std::setprecision(4) 
- std::fixed
- std::scientific
- std::boolalpha

## Input error

> https://hackingcpp.com/cpp/recipe/istream_recover_from_errors.html

Reads characters that cannot be converted to an `int`:

-   `cin`'s _failbit_ is set
-   `cin`'s buffer content is _not_ discarded and still contains the problematic input
-   any following attempt to read an `int` from `cin` will also fail

```cpp
#include <iostream>
#include <limits>

void reset_cin () {    
  // clear all error status bits
  std::cin.clear(); 
  // clear input buffer
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main () {
  std::cout << "i ";
  int i = 0;
  std::cin >> i;  // - 1st
  if (std::cin.fail()) reset_cin();
  std::cout << "j ";
  int j = 0;
  std::cin >> j;  // - 2nd
  std::cout << "i: " << i <<", " 
            << "j: " << j <<'\n';
}
```