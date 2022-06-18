#include <iostream>
#include <vector>
//#include <allocator.h>
#include <ext/malloc_allocator.h>
#include <deque>

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

int main() {
    //std::deque<int, __gnu_cxx::malloc_allocator<int>> de;
    //__gnu_cxx::malloc_allocator<int>::rebind<double>::other;
    int y = 1;
    tem<x, void>()();
    return 0;
}