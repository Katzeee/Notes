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

Befor `c++11`

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
