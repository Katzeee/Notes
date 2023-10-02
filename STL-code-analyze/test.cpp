#include <iostream>
#include <vector>
//#include <allocator.h>
#include<utility>
#include <ext/malloc_allocator.h>
#include <deque>
#include <memory>
#include <type_traits>
#include <iterator>
#include <array>
#include <tuple>
#include <list>

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
    std::vector<int, __gnu_cxx::malloc_allocator<int>> vec_int;
    std::vector<int>::iterator it;
    std::__new_allocator<int> al;
    auto itbegin = vec_int.begin();
    std::array<int, 10> ar;
    std::tuple<int, int, char> tup;
    std::list<int> li{9, 3, 5, 4, 7, 8};
    vec_int.push_back(1);
    li.sort();
    std::deque<int> dee;
    std::__ptr_rebind<double***, char**> xx;
    dee.push_back(10);
    dee.insert(dee.begin(), 10);
    return 0;
}
