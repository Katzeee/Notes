#c-cpp/c-cpp14
## 2023.08.29

> https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3657.htm
>
> https://stackoverflow.com/questions/20317413/what-are-transparent-comparators

## What is the purpose of making an comparator "transparent"?

The associative container lookup functions (find, lower_bound, upper_bound, equal_range) only take an argument of key_type, requiring users to construct (either implicitly or explicitly) an object of the key_type to do the lookup. 

This may be expensive, e.g. constructing a large object to search in a set when the comparator function only looks at one field of the object. 

!!! There is strong desire among users to be able to search using other types which are comparable with the key_type.

```cpp
iterator lower_bound( const Key& key );
const_iterator lower_bound( const Key& key ) const;
template< class K >
iterator lower_bound( const K& x ); // (since C++14)
template< class K >
const_iterator lower_bound( const K& x ) const; // (since C++14)
```

## new `std::less`

```cpp
template <class T = void> struct less {
    constexpr bool operator()(const T& x, const T& y) const;
    typedef T first_argument_type;
    typedef T second_argument_type;
    typedef bool result_type;
};

template <> struct less<void> {
    template <class T, class U> auto operator()(T&& t, U&& u) const
    -> decltype(std::forward<T>(t) < std::forward<U>(u));
    typedef *unspecified* is_transparent;
};
```

So for example, `std::set<T, std::less<T>>` would _not_ have a transparent comparator, but `std::set<T, std::less<>>` _would_ have one.

## Use `find` with keys

```cpp
struct Student{
    Student(std::string name, int id) : name_(name), id_(id) {}
    std::string name_;
    int id_;
};

struct StudentComp {
  	bool operator()(const Student &student1, const Student &student2) const {
        return student1.name_ < student2.name_;
    }

    bool operator()(const Student &student, int id) const {
        return student.id_ < id;
    }
  
    bool operator()(int id, const Student &student) const {
        return student.id_ < id;
    }
  using is_transparent = void;
};

std::set<Student, StudentComp> students;
students.insert(Student{"123", 0});
std::cout << (students.find(0))->name_;
```

