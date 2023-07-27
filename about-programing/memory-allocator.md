2023.07.25
---

## Background

We need a memory allocator for several reasons: 

- Performance Optimization: Reduce the malloc count which is time-consuming

- Memory Usage Control

- Debugging and Profiling: Add some tracking method

## Structure

Use `MemoryManagerBase` as base class, working as a singleton manager, providing interfaces:

- manage normal memory

  - init_normal(const Config &config) = 0;

  - void *allocate(uint32_t size, const void *prefix_address = nullptr) = 0;

    This function may benefit from receiving a hint(`prefix_address`) about the desired location or characteristics of the allocated memory.

- manage shared memory

  - bool init_shared(const Config &config) = 0;

  - void *allocate_shared(uint32_t size, const void *prefix_address = nullptr) = 0;

  - uint64_t get_memory_size_bytes() const = 0;

  - void *get_shared_memory_start_address() const = 0;

  - const char *get_shared_memory_name() const = 0;

- common

  - static MemoryManagerBase *GetInstance();

  - void deallocate(void *address, uint32_t size) = 0;

  - MemoryType memory_type(const void *address) = 0;

    TypeEnum: Normal, Shared, Other(If there is no `manager_item`, just return the mem from malloc)

`BasicMemManager` inherits from `MemoryManagerBase` and manages two `BasicMemManagerItem` which works as real manager of memory, providing interfaces:

- bool in_memory(const void *address)

  Check wheter the address is in the memory.

- void *allocate(uint32_t size)

  Query the bitmap to find whether there is a continuous free space to allocate.

  ```cpp
  // Ceil the size amount
  // need 17 then I will give you 32
  uint32_t get_real_size(const uint32_t &size) {
    return (size + BLOCK_MASK) & ~(BLOCK_MASK); // for 16 bytes per block: BLOCK_MASK = 0xf
  }
  ```

- void deallocate(void *address, uint32_t size)

`BasicMemManager` init the memory block then use the memory info to construct `BasicMemManagerItem`

## Difference between `normal` and `shared` memory

- The real space managed are get from `malloc` and `mmap`, respectively.

  ```cpp
  // The second parameter i.e. the lambda function serves as a custom deleter for the shared pointer.
  start_address = std::shared_ptr<void>(malloc(config.memory_size_bytes), [](void *inner_address) { free(inner_address); });
  ```


