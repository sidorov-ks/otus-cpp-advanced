#include <gtest/gtest.h>

#include "linear_allocator.hpp"

#include <vector>

TEST(std_vector_tests, small_int_vector) {
  std::vector<int, linear_allocator<int, 32>> _array(4, 1);
  for (int el = 0; el < 4; ++el)
    ASSERT_EQ(_array[el], 1);
}

TEST(std_vector_tests, large_int_vector) {
  std::vector<int, linear_allocator<int, 32>> _array(64, 1);
  for (int el = 0; el < 64; ++el)
    ASSERT_EQ(_array[el], 1);
}

TEST(std_vector_tests, small_str_vector) {
  std::vector<std::string, linear_allocator<std::string, 4>> _array{"ab", "ba", "cab"};
  ASSERT_EQ(_array[0], "ab");
  ASSERT_EQ(_array[1], "ba");
  ASSERT_EQ(_array[2], "cab");
}

TEST(std_vector_tests, large_str_vector) {
  std::vector<std::string, linear_allocator<std::string, 4>> _array{"ab", "ba", "cab", "bac", "cabba"};
  ASSERT_EQ(_array[0], "ab");
  ASSERT_EQ(_array[1], "ba");
  ASSERT_EQ(_array[2], "cab");
  ASSERT_EQ(_array[3], "bac");
  ASSERT_EQ(_array[4], "cabba");
}