2022.6.23
---

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

Meanwhile, some methods in vector are defined in `vector.tcc`, such as `emplace_back`

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

## p117

The iterator of the `vector`:

`.../bits/stl_vector.h[453]`
```c++
      typedef __gnu_cxx::__normal_iterator<pointer, vector> iterator;
      typedef __gnu_cxx::__normal_iterator<const_pointer, vector>
      const_iterator;
      typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
      typedef std::reverse_iterator<iterator>		reverse_iterator;
```

`.../bits/stl_iterator.h[1042]`

```c++
  template<typename _Iterator, typename _Container>
    class __normal_iterator
    {
    protected:
      _Iterator _M_current;

      typedef std::iterator_traits<_Iterator>		__traits_type;
```

Meanwhile, we notice that `gcc` is now using `pointer` as the type of `iterator`, so we should pay attention to the definition of `pointer`.

`.../bits/stl_vector.h[92]`

```c++
      struct _Vector_impl_data
      {
	pointer _M_start;
	pointer _M_finish;
	pointer _M_end_of_storage;
```

`.../bits/stl_vector.h[92]`

```c++
      typedef typename __gnu_cxx::__alloc_traits<_Tp_alloc_type>::pointer pointer; // _Vector_base::pointer
```

`.../ext/alloc_traits.h[55]`

```c++
    typedef std::allocator_traits<_Alloc>           _Base_type;
    typedef typename _Base_type::value_type         value_type;
    typedef typename _Base_type::pointer            pointer; // __gnu_cxx::__alloc_traits<_Alloc>::pointer
```

`.../ext/alloc_traits.h[95]`

```c++
      typedef typename _Alloc::value_type value_type;
      using pointer = __detected_or_t<value_type*, __pointer, _Alloc>; // std::allocator_traits<_Alloc>::pointer
```


`.../ext/alloc_traits.h[60]`
```c++
    template<typename _Tp>
      using __pointer = typename _Tp::pointer; // __allocator_traits_base::__pointer
```

So the `pointer` in `vector` is `Alloc::pointer` if that type exists, otherwise `value_type*`.

We take `__new_allocator` as an example because it is the default `Alloc` of `vector` in `gcc12.1`. Before `c++17`, there still existed `Alloc::pointer` which is `_Tp*`, however, after `c++17`, there only exist `value_type`. No matter how, `pointer` is an alias of `_Tp*`.

`.../bits/new_allocator.h[55]`
```c++
  template<typename _Tp>
    class __new_allocator
    {
    public:
      typedef _Tp        value_type;
      typedef std::size_t     size_type;
      typedef std::ptrdiff_t  difference_type;
#if __cplusplus <= 201703L
      typedef _Tp*       pointer;
      typedef const _Tp* const_pointer;
      typedef _Tp&       reference;
      typedef const _Tp& const_reference;
```

## p118

Take `begin` as an example:

`.../bits/stl_vector.h[866]`

```c++
      _GLIBCXX_NODISCARD _GLIBCXX20_CONSTEXPR
      iterator
      begin() _GLIBCXX_NOEXCEPT
      { return iterator(this->_M_impl._M_start); }
```

As you can see, `begin` doesn't return a `pointer` directly, but wraps it as an `iterator`.

## p121

About the constructor of `vector`:

`.../bits/stl_vector.h[563]`
```c++
      _GLIBCXX20_CONSTEXPR
      vector(size_type __n, const value_type& __value,
	     const allocator_type& __a = allocator_type())
      : _Base(_S_check_init_len(__n, __a), __a)
      { _M_fill_initialize(__n, __value); }
```

`.../bits/stl_vector.h[1697]`
```c++
      _GLIBCXX20_CONSTEXPR
      void
      _M_fill_initialize(size_type __n, const value_type& __value)
      {
	this->_M_impl._M_finish =
	  std::__uninitialized_fill_n_a(this->_M_impl._M_start, __n, __value,
					_M_get_Tp_allocator());
      }
```

`.../bits/stl_vector.h[296]`
```c++
      _GLIBCXX20_CONSTEXPR
      _Tp_alloc_type&
      _M_get_Tp_allocator() _GLIBCXX_NOEXCEPT
      { return this->_M_impl; }
```

`_M_impl` is of class `_Vector_impl`, which inherits from `_Tp_alloc_type`, so it can be seen as an allocator.

`_Tp_alloc_type` is the allocator class allocates the space of vector's type.

```c++
      struct _Vector_impl
	: public _Tp_alloc_type, public _Vector_impl_data
```

Also, the initialization of the allocator is finished by `_Base` using delegate constructor.

`.../bits/stl_vector.h[329]`
```c++
      _GLIBCXX20_CONSTEXPR
      _Vector_base(size_t __n, const allocator_type& __a)
      : _M_impl(__a)
      { _M_create_storage(__n); }
```