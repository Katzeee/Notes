#c-cpp
## 2021.6.11

### cpp关键字using作为别名

```c++
typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS;
using UPtrMapSS = std::unique_ptr<std::unordered_map<std::string, std::string>>;
```

```c++
typedef void (*FP) (int, const std::string&);
using FP = void (*) (int, const std::string&);
```

```c++
typedef std::string (Foo::* fooMemFnPtr) (const std::string&);
using fooMemFnPtr = std::string (Foo::*) (const std::string&);
```

```c++
template <typename T>
using Vec = MyVector<T, MyAlloc<T>>;
// usage
Vec<int> vec;
//typedef做不到
```
