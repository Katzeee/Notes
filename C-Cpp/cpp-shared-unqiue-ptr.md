#c-cpp #software-design #c-cpp/stl-code
## 2023.09.01

> https://www.reddit.com/r/cpp_questions/comments/gad4u2/why_is_the_deleter_part_of_the_type_for_stdunique/
> https://geidav.wordpress.com/2017/10/29/custom-deleters-for-smart-pointers-in-modern-c/  
> https://stackoverflow.com/questions/21355037/why-does-unique-ptr-take-two-template-parameters-when-shared-ptr-only-takes-one

I got a incomplete type error raised by `unique_ptr` when I compile code like:

```cpp
// context.hpp
class Camera;
class Context {
 public:
  void Init();
  std::unique_ptr<Camera> camera_;
};
```

```cpp
// context.cpp
#include "context.hpp"
Context::Init() {}
```
## Solution

Include `camera.hpp` in either `contex.cpp` or `context.hpp`, because `unique_ptr` must know the concrete declaration of its templete arguments.

## However

**I'm surprised by finding another solution changing `unique_ptr` to `shared_ptr`.**

You can easily find `unique_ptr` use `static_assert` to ensure `sizeof(Tp) > 0` in its defulat destructor while `shader_ptr` not.

Instead of wondering why it works, I'm wondering why it is designed like this.

## Difference of `unique_ptr` and `shader_ptr` implemented

`std::unique_ptr` carries the custom deleter as part of its type (`template<class T, class Deleter> std::unique_ptr`). 

In contrast, the custom deleter of `std::shared_ptr` is not part of the type (`template<class T> std::shared_ptr`) but part of the constructor’s template argument list (`template<class Y, class Deleter> shared_ptr(Y *ptr, Deleter d)`). 

Mostly, this difference doesn’t matter much. Though, there are use-cases, like e.g. factories returning `std::unique_ptr` with a custom deleter, were the difference does matter.

### `std::unique_ptr`

Deleter is **part of the type** and you don't need to store anything additional in the objects of this **type**(deleter) which makes it to be a _zero-overhead_ abstraction:

1.  Its size is identical to the size of a raw pointer on the underlying architecture.
2.  All calls to the deleter can be inlined.

One possible implemention which makes use of the empty base optimization is to store the wrapped pointer together with the deleter in a [_compressed pair_](http://www.boost.org/doc/libs/1_61_0/libs/utility/doc/html/compressed_pair.html). The obvious disadvantage of making the custom deleter part of the type is that two `std::unique_ptr`s with different custom deleters are of different type, even if they wrap the same pointer type.

```cpp
template <typename _Tp, typename _Dp>
  class __uniq_ptr_impl {
    tuple<pointer, _Dp> _M_t;
  };

template <typename _Tp, typename _Dp = default_delete<_Tp>>
  class unique_ptr {
    __uniq_ptr_data<_Tp, _Dp> _M_t;
  }
```

!!! Danger `sizeof(_Dp)` may be 0 if it doesn't contain any attributes. Thus, `sizeof(_M_t) == sizeof(pointer)`

### `std::shared_ptr`

`std::shared_ptr` provides the convinience of a _type erased_ deleter which means the type of the custom deleter is not dragged into `std::shared_ptr`‘s type.

Users of shared_ptr don't have to know what type the deleter has. This has a run-time cost (allocation, dereference), so it isn't performed for unique_ptr (which is overhead-free).

The rationale behind this design choice:

1. `std::shared_ptr` must anyways heap allocate memory for its shared _control block_, containing the wrapped pointer and the reference counter. Additionally including the custom deleter didn’t seem like a big cost, taking the increased flexiblity into account.

2. Shared pointers of different types can share the ownership of the same object.

The type erasure makes `std::shared_ptr` more flexible. For example changing the allocation strategy of a factory, and with it the custom deleter of the returned std::shared_ptrs, doesn’t break source/binary compatibility and thereby, doesn’t require any recompilation of client software.

```cpp
  template<typename _Tp, _Lock_policy _Lp>
    class __shared_ptr
    : public __shared_ptr_access<_Tp, _Lp>
    {
      element_type*	   _M_ptr;         // Contained pointer.
      __shared_count<_Lp>  _M_refcount;    // Reference counter.
    };

  template<_Lock_policy _Lp>
    class __shared_count
    {
      _Sp_counted_base<_Lp>*  _M_pi;
    };

  template<_Lock_policy _Lp = __default_lock_policy>
    class _Sp_counted_base
    : public _Mutex_base<_Lp>
    {}

  // Counted ptr with no deleter or allocator support
  template<typename _Ptr, _Lock_policy _Lp>
    class _Sp_counted_ptr final : public _Sp_counted_base<_Lp>
    {
    public:
      virtual void
      _M_dispose() noexcept
      { delete _M_ptr; }

      virtual void*
      _M_get_deleter(const std::type_info&) noexcept
      { return nullptr; }

    private:
      _Ptr             _M_ptr;
    };

  // Support for custom deleter and/or allocator
  template<typename _Ptr, typename _Deleter, typename _Alloc, _Lock_policy _Lp>
    class _Sp_counted_deleter final : public _Sp_counted_base<_Lp>
    {
      class _Impl : _Sp_ebo_helper<0, _Deleter>, _Sp_ebo_helper<1, _Alloc>
      {
	typedef _Sp_ebo_helper<0, _Deleter>	_Del_base;
	typedef _Sp_ebo_helper<1, _Alloc>	_Alloc_base;

      public:
	_Impl(_Ptr __p, _Deleter __d, const _Alloc& __a) noexcept
	: _Del_base(std::move(__d)), _Alloc_base(__a), _M_ptr(__p)
	{ }

	_Deleter& _M_del() noexcept { return _Del_base::_S_get(*this); }
	_Alloc& _M_alloc() noexcept { return _Alloc_base::_S_get(*this); }

	_Ptr _M_ptr;
      };

      virtual void
      _M_dispose() noexcept
      { _M_impl._M_del()(_M_impl._M_ptr); }

      virtual void*
      _M_get_deleter(const type_info& __ti [[__gnu__::__unused__]]) noexcept
      {
#if __cpp_rtti
	// _GLIBCXX_RESOLVE_LIB_DEFECTS
	// 2400. shared_ptr's get_deleter() should use addressof()
        return __ti == typeid(_Deleter)
	  ? std::__addressof(_M_impl._M_del())
	  : nullptr;
#else
        return nullptr;
#endif
      }
    private:
      _Impl _M_impl;
    };
```

!!! Danger However, `shader_ptr` must store the deleter in somewhere because `_Dp` is not the type of `shader_ptr`
