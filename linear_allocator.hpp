#pragma once

#include <iostream>
#include <cstring>

template<typename T, std::size_t PoolSize = 32>
class linear_allocator {
public:
  using value_type = T;

  linear_allocator() : pool(reinterpret_cast<T *>(std::malloc(PoolSize * sizeof(T)))),
                       pool_size(PoolSize), used_count(0) {}

  ~linear_allocator() {
    std::free(pool);
  }

  linear_allocator(const linear_allocator<T, PoolSize> &) : pool(
          reinterpret_cast<T *>(std::malloc(PoolSize * sizeof(T)))),
                                                            pool_size(PoolSize), used_count(0) {
  }

  template<typename U>
  linear_allocator(const linear_allocator<U, PoolSize> &) : pool(
          reinterpret_cast<T *>(std::malloc(PoolSize * sizeof(T)))),
                                                            pool_size(PoolSize), used_count(0) {}

  template<typename U>
  struct rebind {
    typedef linear_allocator<U, PoolSize> other;
  };

  T *allocate(std::size_t n) {
    if (used_count + n > pool_size)
      throw std::bad_alloc();
    auto p = pool + used_count;
    used_count += n;
    return p;
  }

  void deallocate(T *p, std::size_t n) {
    (void) p;
    (void) n;
  }

private:
  T *pool;
  std::size_t pool_size;
  std::size_t used_count;
};
