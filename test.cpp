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

struct TestStruct {
  int a;
  long long b;
};

TEST(std_vector_tests, small_struct_vector) {
  std::vector<TestStruct, linear_allocator<TestStruct, 4>> _array{
          {0, 0},
          {1, 1}
  };
  ASSERT_EQ(_array[0].a, 0);
  ASSERT_EQ(_array[0].b, 0);
  ASSERT_EQ(_array[1].a, 1);
  ASSERT_EQ(_array[1].b, 1);
}

TEST(std_vector_tests, large_struct_vector) {
  std::vector<TestStruct, linear_allocator<TestStruct, 4>> _array{
          {0, 0},
          {1, 1},
          {2, 2},
          {3, 3},
          {4, 4},
          {5, 5}
  };
  ASSERT_EQ(_array[0].a, 0);
  ASSERT_EQ(_array[0].b, 0);
  ASSERT_EQ(_array[1].a, 1);
  ASSERT_EQ(_array[1].b, 1);
  ASSERT_EQ(_array[2].a, 2);
  ASSERT_EQ(_array[2].b, 2);
  ASSERT_EQ(_array[3].a, 3);
  ASSERT_EQ(_array[3].b, 3);
  ASSERT_EQ(_array[4].a, 4);
  ASSERT_EQ(_array[4].b, 4);
  ASSERT_EQ(_array[5].a, 5);
  ASSERT_EQ(_array[5].b, 5);
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