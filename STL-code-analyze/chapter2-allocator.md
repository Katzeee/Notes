2022.06.17
---

## p47

Now, using allocator in gcc is also the same as others.

`.../bits/vector[422]`

```c++
 template<typename _Tp, typename _Alloc = std::allocator<_Tp> >
    class vector : protected _Vector_base<_Tp, _Alloc>
    {
        ...
```

## p51

Now, function `construct()` is rewrited to `_Construct()`.

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

The chosen allocator in mingw64 on windows is `__new_allocator`.

`.../x86_64-w64-mingw32/bits/c++allocator.h[39]`

```c++
  template<typename _Tp>
    using __allocator_base = __new_allocator<_Tp>;
```

Allocator is supposed to have following definitions.

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

# p72

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

      return __uninitialized_fill_n<__can_fill>::
	__uninit_fill_n(__first, __n, __x);
    }
```
