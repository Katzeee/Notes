#c-cpp/cpp20 #c-cpp/cpp23 #c-cpp/stl-code
## 2023.09.21

> https://github.com/CppCon/CppCon2019/blob/master/Presentations/an_overview_of_standard_ranges/an_overview_of_standard_ranges__tristan_brindle__cppcon_2019.pdf  
> https://www.youtube.com/watch?v=qv29fo9sUjY

!!! Code is based on gcc12.1

## What are ranges

A range is object on which you can call `begin()` and `end()`

`.../libstdc++-v3/include/bits/ranges_base.h[499]`
```cpp
 /// [range.range] The range concept.
  template<typename _Tp>
    concept range = requires(_Tp& __t)
      {
	ranges::begin(__t);
	ranges::end(__t);
      };
```

Call functor `begin::operator()(__t)`

```cpp
  inline namespace __cust
  {
    inline constexpr __cust_access::_Begin begin{};
    inline constexpr __cust_access::_End end{};
    ...
```
`.../libstdc++-v3/include/bits/ranges_base.h[95]`
```cpp
    struct _Begin
    {
    private:
      template<typename _Tp>
	static constexpr bool
	_S_noexcept()
	{
	  if constexpr (is_array_v<remove_reference_t<_Tp>>)
	    return true;
	  else if constexpr (__member_begin<_Tp>)
	    return noexcept(__decay_copy(std::declval<_Tp&>().begin()));
	  else
	    return noexcept(__decay_copy(begin(std::declval<_Tp&>())));
	}

    public:
      template<__maybe_borrowed_range _Tp>
	requires is_array_v<remove_reference_t<_Tp>> || __member_begin<_Tp>
	  || __adl_begin<_Tp>
	constexpr auto
	operator()[[nodiscard]](_Tp&& __t) const noexcept(_S_noexcept<_Tp&>())
	{
	  if constexpr (is_array_v<remove_reference_t<_Tp>>)
	    {
	      static_assert(is_lvalue_reference_v<_Tp>);
	      return __t + 0;
	    }
	  else if constexpr (__member_begin<_Tp>)
	    return __t.begin();
	  else
	    return begin(__t);
	}
    };
```

**First, it supposed to be `__maybe_borrowed_range`**

`.../libstdc++-v3/include/bits/ranges_base.h[81]`
```cpp
    // Part of the constraints of ranges::borrowed_range
    template<typename _Tp>
      concept __maybe_borrowed_range
	= is_lvalue_reference_v<_Tp>
	  || enable_borrowed_range<remove_cvref_t<_Tp>>;
```

`.../libstdc++-v3/include/std/type_traits[49]`
```cpp
  /// is_lvalue_reference
  template<typename>
    struct is_lvalue_reference
    : public false_type { };

  template<typename _Tp>
    struct is_lvalue_reference<_Tp&>
    : public true_type { };
```

The default value of `enable_borrowed_range` is false

`.../libstdc++-v3/include/bits/ranges_base.h[49]`
```cpp
  template<typename _Tp>
    inline constexpr bool enable_borrowed_range = false;
```
	
Also there are many specialization of `enable_borrowed_range`, like

`.../libstdc++-v3/include/std/ranges[85]`
```cpp
  template<typename _Tp>
    inline constexpr bool enable_borrowed_range<empty_view<_Tp>> = true;
```

`.../libstdc++-v3/include/std/ranges[1144]`
```cpp
  template<typename _Tp>
    inline constexpr bool enable_borrowed_range<ref_view<_Tp>> = true;
```

	
**Second, it `requires is_array_v<remove_reference_t<_Tp>> || __member_begin<_Tp> || __adl_begin<_Tp>`**

`.../libstdc++-v3/include/std/type_traits[447]`
```cpp
  /// is_array
  template<typename>
    struct is_array
    : public false_type { };

  template<typename _Tp, std::size_t _Size>
    struct is_array<_Tp[_Size]>
    : public true_type { };

  template<typename _Tp>
    struct is_array<_Tp[]>
    : public true_type { };
```

I **guess** it suppose `__t.begin()` is copiable or moveable and decay to `input_or_output_iterator`.

`.../libstdc++-v3/include/std/iterator_concepts.h[935]`
```cpp
    struct _Decay_copy final
    {
      template<typename _Tp>
	constexpr decay_t<_Tp>
	operator()(_Tp&& __t) const
	noexcept(is_nothrow_convertible_v<_Tp, decay_t<_Tp>>)
	{ return std::forward<_Tp>(__t); }
    } inline constexpr __decay_copy{};

    template<typename _Tp>
      concept __member_begin = requires(_Tp& __t)
	{
	  { __decay_copy(__t.begin()) } -> input_or_output_iterator;
	};
```

For those `__t` who don't have `.begin()`.

`.../libstdc++-v3/include/std/iterator_concepts.h[954]`
```cpp
    template<typename _Tp>
      concept __adl_begin = __class_or_enum<remove_reference_t<_Tp>>
	&& requires(_Tp& __t)
	{
	  { __decay_copy(begin(__t)) } -> input_or_output_iterator;
	};
```

## What are views?

**Lazy-evaluation**

A key feature of views is that whatever transformation they apply, they do so at the moment you request an element, not when the view is created.

**Adaptor**

`std::views::reverse` is not the view itself, it’s an adaptor that takes the underlying range (in our case the vector) and returns a view object over the vector.

!!! Warning Views may not read-only(output_range)


## Range concepts

| Concept                            | Description                                                                                                                        |
| ---------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------- |
| `std::ranges::range`               | specifies that a type is a range, that is, it provides a begin iterator and an end sentinel                                        |
| `std::ranges::borrowed_range`      | specifies that a type is a range and iterators obtained from an expression of it can be safely returned without danger of dangling |
| `std::ranges::sized_range`         | specifies that a range knows its size in constant time                                                                             |
| `std::ranges::view`                | specifies that a range is a view, that is, it has constant time copy/move/assignment                                               |
| `std::ranges::input_range`         | can be iterated from beginning to end **at least once**                                                                            |
| `std::ranges::output_range`        | specifies a range whose iterator type satisfies output_iterator                                                                    |
| `std::ranges::forward_range`       | can be iterated from beginning to end **multiple times**                                                                           |
| `std::ranges::bidirectional_range` | iterator can also move backwards with `--`                                                                                         |
| `std::ranges::random_access_range` | you can jump to elements **in constant-time** `[]`                                                                                 |
| `std::ranges::contiguous_range`    | elements are always stored consecutively in memory                                                                                 |
| `std::ranges::common_range`        | specifies that a range has identical iterator and sentinel types                                                                   |
| `std::ranges::viewable_range`      | specifies the requirements for a range to be safely convertible to a                                                               |
| `std::ranges::constant_range`      | specifies that a range has read-only elements                                                                                      |


## `end()` may returns `sentinel` rather `iterator` 

You can call find with `unreachable_sentinel` like `std::ranges::find(v.begin(), std::ranges::unreachable_sentinel, f)`.

Here the second parameter receives a `sentinel_for<_Iter>`

`.../libstdc++-v3/include/bits/ranges_utils.h[438]`
```cpp
    template<input_iterator _Iter, sentinel_for<_Iter> _Sent, typename _Tp,
	     typename _Proj = identity>
      requires indirect_binary_predicate<ranges::equal_to,
					 projected<_Iter, _Proj>, const _Tp*>
      constexpr _Iter
      operator()(_Iter __first, _Sent __last,
		 const _Tp& __value, _Proj __proj = {}) const
      {
	while (__first != __last
	    && !(std::__invoke(__proj, *__first) == __value))
	  ++__first;
	return __first;
      }

    template<input_range _Range, typename _Tp, typename _Proj = identity>
      requires indirect_binary_predicate<ranges::equal_to,
					 projected<iterator_t<_Range>, _Proj>,
					 const _Tp*>
      constexpr borrowed_iterator_t<_Range>
      operator()(_Range&& __r, const _Tp& __value, _Proj __proj = {}) const
      {
	return (*this)(ranges::begin(__r), ranges::end(__r),
		       __value, std::move(__proj));
      }
```

## Views basic

Suppose we have these pre-defined:

```cpp
std::vector<int> vec{1, 2, 3, 4, 5, 6};
std::list<int> lst{1, 2, 3, 4, 5, 6};
```

**Use concept**

```cpp
void print(const std::ranges::input_range auto &coll) {
  for (const auto &elem : coll) {
    std::cout << elem << " ";
  }
  std::cout << "\n";
}

print(vec);
print(lst);
print(std::views::take(vec, 3));
print(std::views::take(lst, 3));
```

**Use zip views**

```cpp
for (auto [idx, elem] : std::views::zip(std::views::iota(1), list)) {
  std::cout << idx << ":" << elem << std::endl;
}
```

**How views operate**

```cpp
std::vector coll{42, 9, 0, 16, 7, -1, 13}
print(vec | std::views::take(3) | std::views::transform(std::negate{}));
auto v1 = vec | std::views::take(3);
auto v2 = v1 | std::views::transform(std::negate{});
```

![Alt text](../.images/cpp-views-operate.png)

**Only provides cheap member functions**

`vVec` will provide all of these functions.

```cpp
auto vVec = vec | std::views::drop(n);
vVec.begin() // fast, vec.begin() + n
vVec.empty() // fast, vec.size() <= n
vVec.size() // fast, n >= vec.size() ? 0 : vec.size() - n
vVec[idx] // fast, vec[idx + n]
```

`vLst` will not provide `[]` because it's expensive but will provide `begin()` because it must.

```cpp
auto vLst = lst | std::views::drop(n);
vLst.begin() // slow, n times ++
vLst.empty() // fast, vec.size() <= n
vLst.size() // fast, n >= vec.size() ? 0 : vec.size() - n
vLst[idx] // (not provided) slow, n + idx times ++
```

```cpp
auto vFlt = coll | std::views::filter(pred);
vLst.begin() // (not provided)slow, pred for all elements until first true
vLst.empty() // (not provided)slow, pred for all elements until first true
vLst.size() //(not provided)slow, pred for all elements
vLst[idx] // (not provided)slow, pred for all elements until idx times true
```

Basic views interface use concept to decide whether to provide these functions or not.

`.../libstdc++-v3/include/bits/ranges_utils.h[62]`

```cpp
  /// The ranges::view_interface class template
  template<typename _Derived>
    requires is_class_v<_Derived> && same_as<_Derived, remove_cv_t<_Derived>>
    class view_interface
    {
      ...
    public:
      constexpr bool
      empty()
      noexcept(noexcept(_S_empty(_M_derived())))
      requires forward_range<_Derived>
      { return _S_empty(_M_derived()); }
      ...
      constexpr auto
      size() noexcept(noexcept(_S_size(_M_derived())))
      requires forward_range<_Derived>
	&& sized_sentinel_for<sentinel_t<_Derived>, iterator_t<_Derived>>
      { return _S_size(_M_derived()); }
      ...
```

## Views pitfalls

**Cache**

!!! Warning To speed up the expensive function calls, views will **cache** the result of the first calling. But also makes lots of unexpected error.

![Alt text](../.images/cpp-views-cache.png)

It will cause a compile time error because `print` receives a const reference. However, to cache value must modify the view which means the views should be mutable.

```cpp
void print(const auto &coll) {
  for (const auto &elem : coll) {
    std::cout << elem << " ";
  }
  std::cout << "\n";
}

print(vec);
print(lst);
print(vec | std::views::drop(3));
print(lst | std::views::drop(3)); // compile time error
fot (int v : lst | std::views::drop(3)) { // works
  std::cout << v << " ";
}
auto v = lst | std::views::drop(3);
print(std::ranges::subrange(v.begin()/* first call here */, v.end())); //works
```
Chage to universal reference works well.

```cpp
void print(auto &&coll) {
  for (const auto &elem : coll) {
    std::cout << elem << " ";
  }
  std::cout << "\n";
}

print(vec);
print(lst);
print(vec | std::views::drop(3));
print(lst | std::views::drop(3)); // works
```

**UB when use views in concurrency**

```cpp
auto printAndSum(auto &&rg) {
  std::jthread printThread([&](){
    for (const auto &elem : rg) {
      std::cout << elem << ' ';
    }
    std::cout << '\n';
  });
  // cache do a write to rg, concurrent begin() and end() only safe for containers
  return std::accumulate(rg.begin(), rg.end(), 0L); // UB, concurrent read iterations
}

auto sum = printAndSum(lst | std::views::drop(3)); // runtime error
```

**Passing containers and views by value**

```cpp
void print(auto coll) {
  for (const auto &elem : coll) {
    std::cout << elem << " ";
  }
  std::cout << "\n";
}
print(vec); // expensive
print(lst); // expensive
print(vec | std::views::drop(3));
print(lst | std::views::drop(3)); 
```

Better practice

```cpp
void print(std::ranges::view auto coll) {
  for (const auto &elem : coll) {
    std::cout << elem << " ";
  }
  std::cout << "\n";
}
 
// an overload
template <typename T>
void print(const T& coll) requires (!std::ranges::view<T>) {
  print(coll | std::views::all)
}
```

**UB when use filter_view**

```cpp
std::vector<int> coll{1, 4, 7, 10};
auto isEven = [](auto &&i){ return i % 2 == 0; }
auto collEven = coll | std::views::filter(isEven);

for (int &i : collEven) {
  i += 2;
}
print(coll); // works, 1 6 7 12

for (int &i : collEven) {
  i += 2;
}
print(coll); // works, 1 8 7 14
```

```cpp
std::vector<int> coll{1, 4, 7, 10};
auto isEven = [](auto &&i){ return i % 2 == 0; }
auto collEven = coll | std::views::filter(isEven);

for (int &i : collEven) {
  i += 1; // UB, predicate broken
} 
print(coll); // UB but works, 1 5 7 11

for (int &i : collEven) {
  i += 1; // UB, predicate broken
}
print(coll); // error, 1 6 7 11
```

```cpp
std::vector<int> coll{1, 4, 7, 10};
auto isEven = [](auto &&i){ return i % 2 == 0; }

for (int &i : coll | std::views::filter(isEven)) {
  i += 1; // UB, but works
} 
print(coll);

for (int &i : coll | std::views::filter(isEven)) {
  i += 1; // UB, but works
} 
print(coll);
```

!!! Error Do not give your view a name unless you know what you are doing

**Modification consider harmful with caching**

Cache `begin()` makes unexpected and unobvious error, can be solved by copying the view.

```cpp
auto vVec = vec | std::views::drop(4); // 5, 6
auto vLst = lst | std::views::drop(4); // 5, 6

vec.insert(vec.begin(), 0);
lst.insert(lst.begin(), 0);
print(vVec, vLst); // vVec: 4, 5, 6 vLst: 4, 5, 6

vec.insert(vec.begin(), {8, 9, 0, -1});
lst.insert(lst.begin(), {8, 9, 0, -1});
print(vVec, vLst); // vVec: 0, 1, 2, 3, 4, 5, 6 vLst(error): 4, 5, 6

auto vVecB = vVec;
auto vLstB = vLst;
print(vVecB, vLstB); // vVecB: 0, 1, 2, 3, 4, 5, 6 vLstB: 0, 1, 2, 3, 4, 5, 6
```

**cbegin()**

Views don't have `.cbegin()`.

```cpp
template<typename T>
void print(T &&coll) {
  auto pos = coll.cbegin(); // not available for views
  std::cout << *pos;
}

print(vec); // ok
print(vec | std::view::drop(3)); // error
```

```cpp
// in c++20
template<typename T>
void print(T &&coll) {
  *std::cbegin(coll) = 0; // oops, modify the view
  *std::ranges::cbegin(coll) = 0; // oops, modify the view
}

print(vec); // compile time error
print(vec | std::view::drop(3)); // oops, modify the view
```

![Alt text](../.images/cpp-ref-view-and-owning-view.png)

!!! Warning std::ranges::ref_view does ==not== propagate const,std::ranges::owning_view propagate const

```cpp
v1[0] = 42; // OK(bad)
v2[0] = 42; // ERROR(good)
```

c++23 does some fix

```cpp
// in c++23
template<typename T>
void print(T &&coll) {
  auto pos = coll.cbegin(); // available since c++23
  *std::cbegin(coll) = 0; // oops, still broken
  *std::ranges::cbegin(coll) = 0; // compile time error
}
```

!!! Hint Prefer std::ranges:: over std::

**zip_view does not propagate const**

```cpp
void printParis(const auto &coll) {
  for (const auto &elem : coll) {
    if (elem.first = 2) { // typo, = not ==, so the statement will always true
      std::cout << "* ";
    }
    std::cout << elem.first << " "<< elem.second << '\n';
  }
}

printParis(std::views::zip(v1, v2));
```

**Basic idioms ==broken== by standard views**

- You can iterate if the range is const

- A read iteration does not change state

- Concurrent read iterations are safe 

- const collections have const elements

- A copy of a range has the same state

- const-declared elements are const(c++23)

## Some const Fixes for views

![Alt text](../.images/cpp23-const-fixes-for-views.png)

## Belleviews implementation of views

> https://github.com/josuttis/belleviews

Not caching `begin()`, fixes lots of pitfalls.