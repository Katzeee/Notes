#c-cpp/stl-code #book 
## 2022.6.22

## p89

The `iterator_traits` class also has changed.

Before `c++11`, it looks like the same as the book illustrated.

`.../bits/stl_iterator_base_types.h[181]`

```cpp
  template<typename _Iterator>
    struct iterator_traits
    {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type        value_type;
      typedef typename _Iterator::difference_type   difference_type;
      typedef typename _Iterator::pointer           pointer;
      typedef typename _Iterator::reference         reference;
    };

  /// Partial specialization for pointer types.
  template<typename _Tp>
    struct iterator_traits<_Tp*>
    {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp                         value_type;
      typedef ptrdiff_t                   difference_type;
      typedef _Tp*                        pointer;
      typedef _Tp&                        reference;
    };

  /// Partial specialization for const pointer types.
  template<typename _Tp>
    struct iterator_traits<const _Tp*>
    {
      typedef random_access_iterator_tag iterator_category;
      typedef _Tp                         value_type;
      typedef ptrdiff_t                   difference_type;
      typedef const _Tp*                  pointer;
      typedef const _Tp&                  reference;
    };
#endif
```

Then before `c++20`, it is just changed a little, inheriting from a base class `__iterator_traits` which implemented with `void_t` avoiding incorrect template parameter instantiating but still defined in `stl_iterator_base_types.h`:

The specialization of `Tp*` and `const Tp*` didn't change.

`.../bits/stl_iterator_base_types.h[149]`

```cpp
  template<typename _Iterator>
    struct iterator_traits;

#if __cplusplus >= 201103L
  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 2408. SFINAE-friendly common_type/iterator_traits is missing in C++14
  template<typename _Iterator, typename = __void_t<>>
    struct __iterator_traits { };

#if ! __cpp_lib_concepts

  template<typename _Iterator>
    struct __iterator_traits<_Iterator,
			     __void_t<typename _Iterator::iterator_category,
				      typename _Iterator::value_type,
				      typename _Iterator::difference_type,
				      typename _Iterator::pointer,
				      typename _Iterator::reference>>
    {
      typedef typename _Iterator::iterator_category iterator_category;
      typedef typename _Iterator::value_type        value_type;
      typedef typename _Iterator::difference_type   difference_type;
      typedef typename _Iterator::pointer           pointer;
      typedef typename _Iterator::reference         reference;
    };
#endif // ! concepts

  template<typename _Iterator>
    struct iterator_traits
    : public __iterator_traits<_Iterator> { };
```

However, there are big changes in `c++20`, not only the implementation of the class, but the file it is defined has changed.

There are only a few lines in `stl_iterator_base_types.h`, most of it has located in `iterator_concepts.h`.

They have been rewritten by `concept` and `requires`.

`.../bits/iterator_concepts.h[365]`
```cpp
  template<typename _Iterator>
    requires __detail::__iter_with_nested_types<_Iterator>
    struct __iterator_traits<_Iterator, void>
    {
    private:
      template<typename _Iter>
	struct __ptr
	{ using type = void; };

    // this struct is also seen as a specialization of above
      template<typename _Iter> requires requires { typename _Iter::pointer; }
	struct __ptr<_Iter>
	{ using type = typename _Iter::pointer; };

    public:
      using iterator_category = typename _Iterator::iterator_category;
      using value_type	      = typename _Iterator::value_type;
      using difference_type   = typename _Iterator::difference_type;
      using pointer	      = typename __ptr<_Iterator>::type;
      using reference	      = typename _Iterator::reference;
    };
```

where `__iter_with_nested_types` is defined as follows:

`.../bits/iterator_concepts.h[348]`
```cpp
    template<typename _Iter>
      concept __iter_with_nested_types = requires {
	typename _Iter::iterator_category;
	typename _Iter::value_type;
	typename _Iter::difference_type;
	typename _Iter::reference;
      };
```

## p92

There are now six `iterator_category`s in `c++17`.

The added category is `contiguous_iterator_tag` which inherits from `random_access_iterator_tag `

`.../bits/stl_iterator_base_types.h[109]`
```cpp
#if __cplusplus > 201703L
  /// Contiguous iterators point to objects stored contiguously in memory.
  struct contiguous_iterator_tag : public random_access_iterator_tag { };
#endif
```

## p100

The `iterator` structure is not recommended being used now.

`.../bits/stl_iterator_base_types.h[125]`
```cpp
  template<typename _Category, typename _Tp, typename _Distance = ptrdiff_t,
           typename _Pointer = _Tp*, typename _Reference = _Tp&>
    struct _GLIBCXX17_DEPRECATED iterator
    {
      /// One of the @link iterator_tags tag types@endlink.
      typedef _Category  iterator_category;
      /// The type "pointed to" by the iterator.
      typedef _Tp        value_type;
      /// Distance between iterators is represented as this type.
      typedef _Distance  difference_type;
      /// This type represents a pointer-to-value_type.
      typedef _Pointer   pointer;
      /// This type represents a reference-to-value_type.
      typedef _Reference reference;
    };
```
where

```cpp
# define _GLIBCXX17_DEPRECATED [[__deprecated__]]
```

## p103

The standard `type_traits` is now defined in header file `.../bits/type_traits`, and the old one is located in `.../ext/type_traits.h`

## Summary

Use SFINAE(substitution failure is not an error) rewrite the traits, prohibiting the abuse like `iterator_traits<int>`.
