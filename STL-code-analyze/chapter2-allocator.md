2022.06.17
---

## p47

Now, using allocator in `gcc` is also the same as others.

`.../bits/vector[422]`

```c++
 template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc>
    {
        ...
```

## p51

Now, function `construct()` is rewritten to `_Construct()`.

`.../bits/stl_construct.h[91]`

```c++
#if __cplusplus >= 201103L
  template<typename _Tp, typename... _Args>
    _GLIBCXX20_CONSTEXPR
    inline void
    _Construct(_Tp* __p, _Args&&... __args)
    {
#if __cplusplus >= 202002L
      if (std::__is_constant_evaluated())
      {
        // Allow std::_Construct to be used in constant expressions.
        std::construct_at(__p, std::forward<_Args>(__args)...);
        return;
      }
#endif
      ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
    }
#else
  template<typename _T1, typename _T2>
    inline void
    _Construct(_T1* __p, const _T2& __value)
    {
      // _GLIBCXX_RESOLVE_LIB_DEFECTS
      // 402. wrong new expression in [some_]allocator::construct
      ::new(static_cast<void*>(__p)) _T1(__value);
    }
#endif
```

After prepossessing:

Before `c++11`

```c++
  template<typename _T1, typename _T2>
    inline void
    _Construct(_T1* __p, const _T2& __value)
    {
      ::new(static_cast<void*>(__p)) _T1(__value);
    }
#endif
```

For `c++11`

```c++
  template<typename _Tp, typename... _Args>
    _GLIBCXX20_CONSTEXPR
    inline void
    _Construct(_Tp* __p, _Args&&... __args)
    {
      ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
    }
```

For `c++20`

```c++
  template<typename _Tp, typename... _Args>
    _GLIBCXX20_CONSTEXPR
    inline void
    _Construct(_Tp* __p, _Args&&... __args)
    {
      if (std::__is_constant_evaluated())
	  {
        // this function is written in c++20
        std::construct_at(__p, std::forward<_Args>(__args)...);
        return;
	  }
      ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
    }
```

`.../bits/stl_construct.h[141]`

```c++
  /**
   * Destroy the object pointed to by a pointer type.
   */
  template<typename _Tp>
    _GLIBCXX14_CONSTEXPR inline void
    _Destroy(_Tp* __pointer)
    {
#if __cplusplus > 201703L
      std::destroy_at(__pointer);
#else
      __pointer->~_Tp();
#endif
    }
```

After prepossessing:

Before `c++17`
```c++
  template<typename _Tp>
    _GLIBCXX14_CONSTEXPR inline void
    _Destroy(_Tp* __pointer)
    {
      __pointer->~_Tp();
    }
```

For `c++17`

```c++
  template<typename _Tp>
    _GLIBCXX14_CONSTEXPR inline void
    _Destroy(_Tp* __pointer)
    {
      // This function is written in c++17
      std::destroy_at(__pointer);
    }
```

`.../bits/stl_construct.h[78]`

```c++
  template <typename _Tp>
    _GLIBCXX20_CONSTEXPR inline void
    destroy_at(_Tp* __location)
    {
      // check whether _Tp is an array
      if constexpr (__cplusplus > 201703L && is_array_v<_Tp>)
	  {
        for (auto& __x : *__location)
	      std::destroy_at(std::__addressof(__x));
      }
      else
	    __location->~_Tp();
    }
```

`.../bits/stl_construct.h[175]`

```c++
  /**
   * Destroy a range of objects.  If the value_type of the object has
   * a trivial destructor, the compiler should optimize all of this
   * away, otherwise the objects' destructors must be invoked.
   */
  template<typename _ForwardIterator>
    _GLIBCXX20_CONSTEXPR inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
                       _Value_type;
#if __cplusplus >= 201103L
      // A deleted destructor is trivial, this ensures we reject such types:
      static_assert(is_destructible<_Value_type>::value,
		    "value type is destructible");
#endif
#if __cplusplus >= 202002L
      if (std::__is_constant_evaluated())
	    return _Destroy_aux<false>::__destroy(__first, __last);
#endif
      std::_Destroy_aux<__has_trivial_destructor(_Value_type)>::
	    __destroy(__first, __last);
    }
```

After preprocessing:

Before `c++11`

```c++
  template<typename _ForwardIterator>
    _GLIBCXX20_CONSTEXPR inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
                       _Value_type;
      std::_Destroy_aux<__has_trivial_destructor(_Value_type)>::
	    __destroy(__first, __last);
    }
`
```

For `c++11`

```c++
  template<typename _ForwardIterator>
    _GLIBCXX20_CONSTEXPR inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
                       _Value_type;
      static_assert(is_destructible<_Value_type>::value,
		    "value type is destructible");
      std::_Destroy_aux<__has_trivial_destructor(_Value_type)>::
	    __destroy(__first, __last);
    }
```

For `c++20`

```c++
  template<typename _ForwardIterator>
    _GLIBCXX20_CONSTEXPR inline void
    _Destroy(_ForwardIterator __first, _ForwardIterator __last)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
                       _Value_type;
      static_assert(is_destructible<_Value_type>::value,
		    "value type is destructible");
      if (std::__is_constant_evaluated())
        return _Destroy_aux<false>::__destroy(__first, __last);
      std::_Destroy_aux<__has_trivial_destructor(_Value_type)>::
        __destroy(__first, __last);
    }
```

`_Destroy_aux` is a template struct of a bool parameter.

`.../bits/stl_construct.h[175]`

```c++
  // primary template dealing with false
  // which means trivial type
  template<bool>
    struct _Destroy_aux
    {
      template<typename _ForwardIterator>
	static _GLIBCXX20_CONSTEXPR void
	__destroy(_ForwardIterator __first, _ForwardIterator __last)
	{
	  for (; __first != __last; ++__first)
        // call the function _Destroy with one pointer parameter
	    std::_Destroy(std::__addressof(*__first));
	}
    };

  // special template dealing with true
  template<>
    struct _Destroy_aux<true>
    {
      template<typename _ForwardIterator>
        static void
        // do nothing for trivial type
        __destroy(_ForwardIterator, _ForwardIterator) { }
    };
```

## p56

Now, we don't use `malloc_alloc` and `default_alloc`, and they are moved to `.../ext/malloc_allocator.h` and `.../ext/pool_allocator.h`, respectively.

The using allocator is defined in `.../bits/allocator.h`

`.../bits/allocator.h[124]`

```c++
  template<typename _Tp>
    class allocator : public __allocator_base<_Tp>
    {
      ...
```

Class `allocator` is inherited from `__allocator_base<_Tp>` which is defined in <c++allocator.h> and is an alias of an allocator base class. 

Which allocator is chosen is decided by the compiler defining the alias in header file `c++allocator.h`.

Such as:

The chosen allocator in mingw64 on Windows is `__new_allocator`.

`.../x86_64-w64-mingw32/bits/c++allocator.h[39]`

```c++
  template<typename _Tp>
    using __allocator_base = __new_allocator<_Tp>;
```

Allocator is supposed to have the following definitions.

```c++
allocator::value_type
allocator::pointer
allocator::const_pointer
allocator::reference
allocator::const_reference
allocator::size_type
allocator::difference

allocator::rebind

allocator::allocator() 
allocator::allocator(const allocator&) 
template <class U>allocator::allocator(const allocator<U>&)
allocator::~allocator() 

pointer allocator::address(reference x) const
const_pointer allocator::address(const_reference x) const

pointer allocator::allocate(size_type n, const void* = 0)
void allocator::deallocate(pointer p, size_type n)

size_type allocator:maxsize() const

void allocator::construct(pointer p, const T& x)
void allocator::destroy(pointer p)
```

## p72

The functions `uninitialized_fill_n`, `uninitialized_copy` etc. have been rewritten to template functions, take `uninitialized_fill_n` as an example.

`.../bits/stl_uninitialized.h[258]`
```c++
  template<typename _ForwardIterator, typename _Size, typename _Tp>
    _GLIBCXX20_CONSTEXPR
    _ForwardIterator
    __do_uninit_fill_n(_ForwardIterator __first, _Size __n, const _Tp& __x)
    {
      _ForwardIterator __cur = __first;
      __try
	{
	  for (; __n > 0; --__n, (void) ++__cur)
	    std::_Construct(std::__addressof(*__cur), __x);
	  return __cur;
	}
      __catch(...)
	{
	  std::_Destroy(__first, __cur);
	  __throw_exception_again;
	}
    }

  // if not trivial type, this function will be instantiated
  template<bool _TrivialValueType>
    struct __uninitialized_fill_n
    {
      template<typename _ForwardIterator, typename _Size, typename _Tp>
	static _ForwardIterator
        __uninit_fill_n(_ForwardIterator __first, _Size __n,
			const _Tp& __x)
	{ return std::__do_uninit_fill_n(__first, __n, __x); }
    };

  // if trivial type, this function will be instantiated
  template<>
    struct __uninitialized_fill_n<true>
    {
      template<typename _ForwardIterator, typename _Size, typename _Tp>
	static _ForwardIterator
        __uninit_fill_n(_ForwardIterator __first, _Size __n,
			const _Tp& __x)
        { return std::fill_n(__first, __n, __x); }
    };
```

The trivial type evaluation will be done in function `uninitialized_fill_n`

`.../bits/stl_uninitialized.h[310]`
```c++
  template<typename _ForwardIterator, typename _Size, typename _Tp>
    inline _ForwardIterator
    uninitialized_fill_n(_ForwardIterator __first, _Size __n, const _Tp& __x)
    {
      typedef typename iterator_traits<_ForwardIterator>::value_type
	_ValueType;

      const bool __can_fill
	= _GLIBCXX_USE_ASSIGN_FOR_INIT(_ValueType, const _Tp&)
	&& __is_integer<_Size>::__value;

      return __uninitialized_fill_n<__can_fill>::   // decide to instantiate which template in complie time
	__uninit_fill_n(__first, __n, __x);
    }
```

Comparing to `GCC2.9`, `GCC12.1` has added some other functions in this header file. Such as:
- `__uninitialized_copy_a` which is a version of `__uninitialized_copy` with an allocator as a parameter.
- `uninitialized_value_construct` uses a default value to construct objects at an uninitialized range.
- `uninitialized_move` calls move constructor when constructing objects at destination. 

## p89

The `iterator_traits` class also has changed.

Before `c++11`, it looks like the same as the book illustrated.

`.../bits/stl_iterator_base_types.h[181]`

```c++
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

```c++
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
```c++
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
```c++
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
```c++
#if __cplusplus > 201703L
  /// Contiguous iterators point to objects stored contiguously in memory.
  struct contiguous_iterator_tag : public random_access_iterator_tag { };
#endif
```

## p100

The `iterator` structure is not recommended being used now.

`.../bits/stl_iterator_base_types.h[125]`
```c++
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

```c++
# define _GLIBCXX17_DEPRECATED [[__deprecated__]]
```

## p103

The standard `type_traits` is now defined in header file `.../bits/type_traits`, and the old one is located in `.../ext/type_traits.h`

## p116

Vector is implemented as a base class `_Vector_base` and `vector` itself. As the book said, both of them are in `stl_vector.h`.

`.../bits/stl_vector.h[422]`
```c++
  template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc>
    {
        ...
```

`.../bits/stl_vector.h[84]`
```c++
  template<typename _Tp, typename _Alloc>
    struct _Vector_base
    {
        ...
```

The definition of the allocator has changed.

`.../bits/stl_vector.h[87]`
```c++
      typedef typename __gnu_cxx::__alloc_traits<_Alloc>::template
	rebind<_Tp>::other _Tp_alloc_type; // template<typename _Tp, typename _Alloc> struct _Vector_base
      typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer
       	pointer;
```

It uses `rebind` to change the type parameter of the allocator to the template parameter which the vector received. Such as if we define `std::vector<int, __gnu_cxx::malloc_allocator<double>>`, then the vector will rebind the inner allocator to parameter of `int`.

Besides, this `rebind` is not just the `rebind` structure which defined in allocators, because some allocators don't implement the `rebind` method.

`.../ext/alloc_traits.h[118]`
```c++
    template<typename _Tp>
      struct rebind // __alloc_traits::rebind
      { typedef typename _Base_type::template rebind_alloc<_Tp> other; };
```

`.../ext/alloc_traits.h[55]`
```c++
    typedef std::allocator_traits<_Alloc>           _Base_type; // __alloc_traits::_Base_type
```

`.../bits/alloc_traits.h[212]`
```c++
      template<typename _Tp>
	using rebind_alloc = __alloc_rebind<_Alloc, _Tp>; // allocator_traits::rebind_alloc
```

`.../bits/alloc_traits.h[78]`
```c++
  template<typename _Alloc, typename _Up>
    using __alloc_rebind // __allocator_traits_base::__alloc_rebind
      = typename __allocator_traits_base::template __rebind<_Alloc, _Up>::type;
```

`.../bits/alloc_traits.h[51]`
```c++
    template<typename _Tp, typename _Up, typename = void>
      struct __rebind : __replace_first_arg<_Tp, _Up> { };

    template<typename _Tp, typename _Up>
      struct __rebind<_Tp, _Up,
		      __void_t<typename _Tp::template rebind<_Up>::other>>
      { using type = typename _Tp::template rebind<_Up>::other; };
```

As a result, if the allocator has implemented the `rebind` structure, we will use it, otherwise, we use `__replace_first_arg` to construct a new allocator by replacing the argument in `allocator<_Tp>`.

`.../bits/ptr_traits.h[65]`
```c++
  template<typename _Tp, typename _Up>
    struct __replace_first_arg
    { };

  template<template<typename, typename...> class _SomeTemplate, typename _Up,
           typename _Tp, typename... _Types>
    struct __replace_first_arg<_SomeTemplate<_Tp, _Types...>, _Up>
    { using type = _SomeTemplate<_Up, _Types...>; };
```

For `std::vector<int, __gnu_cxx::malloc_allocator<double>>`, we will finally call `__replace_first_arg<__gnu_cxx::malloc_allocator<double>, int>`, then get the `type` `__gnu_cxx::malloc_allocator<int>` as `_Tp_alloc_type`.

## p116

The implementation of `vector` is much more complicated than old version. The three attribute `iterator` have been moved to `struct _Vector_base::_Vector_impl_data`.

`.../bits/stl_vector.h[92]`

```c++
      struct _Vector_impl_data
      {
	pointer _M_start;
	pointer _M_finish;
	pointer _M_end_of_storage;
```

And in `class vector`, we are using the `struct _Vector_impl` which inheriting from it.

`.../bits/stl_vector.h[133]`

```c++
      struct _Vector_impl
	: public _Tp_alloc_type, public _Vector_impl_data
```

Meanwhile, some methods in vector are defined in `vector.tcc`.

`.../bits/vector.tcc[102]`

```c++
#if __cplusplus >= 201103L
  template<typename _Tp, typename _Alloc>
    template<typename... _Args>
#if __cplusplus > 201402L
      _GLIBCXX20_CONSTEXPR
      typename vector<_Tp, _Alloc>::reference
#else
      void
#endif
      vector<_Tp, _Alloc>::
      emplace_back(_Args&&... __args)
      {
	if (this->_M_impl._M_finish != this->_M_impl._M_end_of_storage)
	  {
	    _GLIBCXX_ASAN_ANNOTATE_GROW(1);
	    _Alloc_traits::construct(this->_M_impl, this->_M_impl._M_finish,
				     std::forward<_Args>(__args)...);
	    ++this->_M_impl._M_finish;
	    _GLIBCXX_ASAN_ANNOTATE_GREW(1);
	  }
	else
	  _M_realloc_insert(end(), std::forward<_Args>(__args)...);
#if __cplusplus > 201402L
	return back();
#endif
      }
#endif
```