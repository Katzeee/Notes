#c-cpp 
## 2022.11.25

## lock_guard

There are 4 ways to initialize a `lock_guard`.(same as `unique_lock`, `shared_lock`)

```cpp
lock_guard(mutex_type& __m, adopt_lock_t) noexcept : _M_device(__m) { } // default
```


<div class="table-wrapper"><table>
<tbody>
<tr><th>method</th><th>tag type</th><th>description</th></tr>
<tr>
<td>(default)</td>
<td>null</td>
<td>block current thread until get the lock</td>
</tr>
<tr>
<td>std::defer_lock</td>
<td>std::defer_lock_t</td>
<td>don't try to get the lock</td>
</tr>
<tr>
<td>std::try_to_lock</td>
<td>std::try_to_lock_t</td>
<td>try to get the lock but don't block current thread, return instantly</td>
</tr>
<tr>
<td>std::adopt_lock</td>
<td>std::adopt_lock_t</td>
<td>pretend current thread has already acquired the lock, don't try to lock the mutex</td>
</tr>
</tbody>
</table></div>

- can manipulate the mutex within the life of lockguard(lock or unlock)

## scoped_lock(c++17)

- can't manipulate the mutex within the life of lockguard


## difference

- lock multiple lock simultaneously

```cpp
explicit scoped_lock(_MutexTypes&... __m) : _M_devices(std::tie(__m...))
  { std::lock(__m...); }
```

so if you want to avoid deadlock with two locks, you could do:

```cpp
// may cause deadlock

std::mutex mutex1, mutex2;
// thread 1
{
    std::lock_guard<std::mutex> lck1(mutex1);
    std::lock_guard<std::mutex> lck2(mutex2);
    // do something
}
// thread 2
{
    std::lock_guard<std::mutex> lck2(mutex2);
    std::lock_guard<std::mutex> lck1(mutex1);
    // do something
}
```

avoid deadlock

```cpp
// #1
std::lock(mutex1, mutex2);
std::lock_guard<std::mutex> lck1(mutex1, std::adopt_lock);
std::lock_guard<std::mutex> lck2(mutex2, std::adopt_lock);

// #2
std::unique_lock<std::mutex> lck1(mutex1, std::defer_lock);
std::unique_lock<std::mutex> lck2(mutex2, std::defer_lock);
std::lock(lck2, lck1);

// #3
std::scoped_lock<std::mutex, std::mutex> lock(mutex1, mutex2);
```