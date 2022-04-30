#pragma once

#include <iostream>

template<typename T, std::size_t BasePoolSize = 32, std::size_t Multiplier = 2>
class linear_allocator {
public:
  using value_type = T;

  linear_allocator() : pool(std::malloc(BasePoolSize * sizeof(T))), pool_size(BasePoolSize), used_count(0) {
  }

  ~linear_allocator() {
    std::free(pool);
  }

  linear_allocator(const linear_allocator<T, BasePoolSize> &) : pool(std::malloc(BasePoolSize * sizeof(T))),
                                                                pool_size(BasePoolSize), used_count(0) {
  }

  template<typename U>
  linear_allocator(const linear_allocator<U, BasePoolSize> &) : pool(std::malloc(BasePoolSize * sizeof(T))),
                                                                pool_size(BasePoolSize), used_count(0) {}

  template<typename U>
  struct rebind {
    typedef linear_allocator<U, BasePoolSize> other;
  };

  T *allocate(std::size_t n) {
    if (used_count + n > pool_size) {
      auto new_pool_size = Multiplier * pool_size;
      T *new_pool = reinterpret_cast<T *>(std::malloc(new_pool_size * sizeof(T)));
      // FIXME На этой строке расширение аллокатора падает с SIGSEGV для T = std::string
      std::move(reinterpret_cast<T *>(pool), reinterpret_cast<T *>(pool) + pool_size, new_pool);
      pool = new_pool;
      pool_size = new_pool_size;
    }
    auto p = reinterpret_cast<T *>(pool) + used_count;
    used_count += n;
    return p;
  }

  void deallocate(T *p, std::size_t n) {
    (void) p;
    (void) n;
  }

private:
  void *pool;
  std::size_t pool_size;
  std::size_t used_count;
};
