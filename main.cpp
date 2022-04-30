#include "linear_allocator.hpp"

#include <chrono>
#include <iostream>
#include <list> // FIXME
#include <map>

const std::size_t MAX_MAP_SIZE = 1 << 16;
const std::size_t MAX_LIST_SIZE = 1 << 20;

using value_t = unsigned long long;
using alloc_t = std::pair<const std::size_t, value_t>;

template<typename AllocType = std::allocator<alloc_t>>
std::chrono::duration<double, std::milli>
benchmark_map(std::size_t n_elements, std::size_t n_runs = 1, bool output = false) {
  std::chrono::duration<double, std::milli> total_duration{};
  for (std::size_t run = 1; run <= n_runs; ++run) {
    auto start = std::chrono::high_resolution_clock::now();
    std::map<std::size_t, long long, std::less<>, AllocType> trial_map{};
    value_t value = 1;
    for (std::size_t key = 0; key < n_elements; ++key) {
      trial_map[key] = value;
      value *= key + 1;
    }
    auto end = std::chrono::high_resolution_clock::now();
    total_duration += end - start;
    if (output && run == 1) {
      std::cerr << "Contents of the map:\n";
      for (const auto &record: trial_map) {
        std::cerr << record.first << " " << record.second << "\n";
      }
    }
  }
  return total_duration / n_runs;
}

template<typename AllocType = std::allocator<alloc_t>>
std::chrono::duration<double, std::milli>
benchmark_list(std::size_t n_elements, std::size_t n_runs = 1, bool output = false) {
  std::chrono::duration<double, std::milli> total_duration{};
  for (std::size_t run = 1; run <= n_runs; ++run) {
    auto start = std::chrono::high_resolution_clock::now();
    std::list<std::size_t, AllocType> trial_list{};
    for (std::size_t key = 0; key < n_elements; ++key) {
      trial_list.push_back(key);
    }
    auto end = std::chrono::high_resolution_clock::now();
    total_duration += end - start;
    if (output && run == 1) {
      std::cerr << "Contents of the list:\n";
      for (const auto &record: trial_list) {
        std::cerr << record << " ";
      }
      std::cerr << "\n";
    }
  }
  return total_duration / n_runs;
}

int main(int, char **) {
  for (std::size_t n_elements = 10; n_elements <= MAX_MAP_SIZE; n_elements <<= 1) {
    std::cout << n_elements << " elements, std::allocator, std::map, avg time = "
              << benchmark_map(n_elements, 32).count() << " ms \n";
    std::cout << n_elements << " elements, linear_allocator<" << 10 << ">, std::map, avg time = "
              << benchmark_map<linear_allocator<alloc_t, 10>>(n_elements, 32, n_elements == 10).count() << " ms \n";
    std::cout << n_elements << " elements, linear_allocator<" << MAX_MAP_SIZE << ">, std::map, avg time = "
              << benchmark_map<linear_allocator<alloc_t, MAX_MAP_SIZE>>(n_elements, 32).count() << " ms \n";
  }
  for (std::size_t n_elements = 10; n_elements <= MAX_LIST_SIZE; n_elements <<= 1) {
    std::cout << n_elements << " elements, std::allocator, list, avg time = "
              << benchmark_list(n_elements, 32).count() << " ms \n";
    std::cout << n_elements << " elements, linear_allocator<" << 10 << ">, list, avg time = "
              << benchmark_list<linear_allocator<alloc_t, 10>>(n_elements, 32, n_elements == 10).count() << " ms \n";
    std::cout << n_elements << " elements, linear_allocator<" << MAX_LIST_SIZE << ">, list, avg time = "
              << benchmark_list<linear_allocator<alloc_t, MAX_LIST_SIZE>>(n_elements, 32).count() << " ms \n";
  }
}
