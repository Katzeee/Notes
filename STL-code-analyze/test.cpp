#include <iostream>
#include <vector>
//#include <allocator.h>
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


int main() {
    //std::deque<int, __gnu_cxx::malloc_allocator<int>> de;
    //__gnu_cxx::malloc_allocator<int>::rebind<double>::other;
    int y = 1;
    tem<x, void>()();
    std::vector<int> vec{1};
    //std::iterator_traits<std::iterator<std::random_access_iterator_tag, std::vector<int>>>::value_type X;
    req<int> cc;
    std::vector<int, __gnu_cxx::malloc_allocator<double>> testvec{1};
    return 0;
}