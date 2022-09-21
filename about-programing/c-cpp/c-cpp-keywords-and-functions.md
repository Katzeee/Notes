# 2021.6.11

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

# 2021.9.13

### sscanf

using sscanf for format convertion

```c
char a[] = "123456";
int i;
sscanf(a, "%d", &i);//convert string "123456" to int 123456
char b[] = "0x8000";
int j;
sscanf(b, "0x%x", &j);//convert string "0x8000" to int 0x8000 i.e. 32768
```

### strtok

it can break a string into a sequence of zero or more nonempty tokens, we can use it to split strings

```c
char a[] = "Th is A";
char* s;
s = strtok(a, " ");//" " is the seperator of the string
//now s points T
s = strtok(NULL, " ");//we call strtok with the first arg being NULL except the first calling as we use it for the same string to get the next part
//now s points i
```


# 2022.1.18
### strncpy
```c
char buf[11];//定义buf
strncpy(buf, src, 10); //进行拷贝
```

2022.09.21
---
### popen
