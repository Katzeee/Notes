#include <iostream>
#include <vector>
//#include <allocator.h>
#include<utility>
#include <ext/malloc_allocator.h>
#include <deque>
#include <memory>
#include <type_traits>
#include <iterator>

struct x {
    typedef int Ttype;
};

template<typename T, typename = int>
class tem {
public:
    void operator()() {
        std::cout << "primary";
    }
};

template<class T>
class tem<T, std::__void_t<typename T::Ttype>> {
public:
    void operator()() {
        std::cout << "specified";
    }
};

template<class T>
class req {
public:
    req() { std::cout << "0"; }
};

template<class T>
requires requires { typename T::Ttype; }
class req<T> {
public:
    req() { std::cout << "1"; }
};

template<typename Func, typename Arg>
void somefunc(Func f, Arg&& somearg) {
    f(std::forward<Arg>(somearg));
}

void something(int&& arg) {
    arg++;
}


int main() {
    int x = 1;
    somefunc(something, 1);
    std::cout << x << "\n";
    return 0;
}