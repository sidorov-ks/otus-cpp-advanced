#include <gtest/gtest.h>

#include "linear_allocator.hpp"
#include "list.hpp"

#include <vector>

TEST(std_vector_tests, int_vector) {
  std::vector<int, linear_allocator<int, 32>> _array(4, 1);
  for (int el = 0; el < 4; ++el)
    EXPECT_EQ(_array[el], 1);
}

struct TestStruct {
  int a;
  long long b;
};

TEST(std_vector_tests, struct_vector) {
  std::vector<TestStruct, linear_allocator<TestStruct, 4>> _array{
          {0, 0},
          {1, 1}
  };
  EXPECT_EQ(_array[0].a, 0);
  EXPECT_EQ(_array[0].b, 0);
  EXPECT_EQ(_array[1].a, 1);
  EXPECT_EQ(_array[1].b, 1);
}

TEST(std_vector_tests, str_vector) {
  std::vector<std::string, linear_allocator<std::string, 4>> _array{"ab", "ba", "cab"};
  EXPECT_EQ(_array[0], "ab");
  EXPECT_EQ(_array[1], "ba");
  EXPECT_EQ(_array[2], "cab");
}

TEST(list_tests, empty_list) {
  list<int> list{};
  EXPECT_EQ(list.size(), 0);
  EXPECT_TRUE(list.empty());
  EXPECT_EQ(list.begin(), list.end());
}

TEST(list_tests, list_iter_string) {
  list<std::string> list{};
  list.push_front("ab");
  list.push_front("ab");
  list.push_front("ab");
  for (size_t run = 1; run <= 2; run++) {
    EXPECT_EQ(list.size(), 3);
    EXPECT_FALSE(list.empty());
    for (auto &el: list) {
      EXPECT_EQ(el, "ab");
    }
  }
}

TEST(list_tests, list_iter_int) {
  list<int> list{};
  list.push_front(3);
  list.push_front(2);
  list.push_front(1);
  for (size_t run = 1; run <= 2; ++run) {
    EXPECT_EQ(list.size(), 3);
    EXPECT_FALSE(list.empty());
    int expect = 1;
    for (auto &el: list) {
      EXPECT_EQ(el, expect);
      expect += 1;
    }
    EXPECT_EQ(list.size(), 3);
    EXPECT_FALSE(list.empty());
  }
}

TEST(list_tests, list_iter_int_clear) {
  list<int> list{};
  list.push_front(3);
  list.push_front(2);
  list.push_front(1);
  list.clear();
  EXPECT_EQ(list.size(), 0);
  EXPECT_TRUE(list.empty());
  EXPECT_EQ(list.begin(), list.end());
}

TEST(list_tests_alloc, empty_list) {
  ::list<int, linear_allocator<int>> list_alloc{};
  EXPECT_EQ(list_alloc.size(), 0);
  EXPECT_TRUE(list_alloc.empty());
  EXPECT_EQ(list_alloc.begin(), list_alloc.end());
}

TEST(list_tests_alloc, list_iter_int_clear) {
  ::list<int, linear_allocator<int, 4>> list_alloc{};
  list_alloc.push_front(3);
  list_alloc.push_front(2);
  list_alloc.push_front(1);
  list_alloc.clear();
  EXPECT_EQ(list_alloc.size(), 0);
  EXPECT_TRUE(list_alloc.empty());
  EXPECT_EQ(list_alloc.begin(), list_alloc.end());
}


TEST(list_tests_alloc, list_iter_int) {
  ::list<int, linear_allocator<int, 4>> list_alloc{};
  list_alloc.push_front(3);
  list_alloc.push_front(2);
  list_alloc.push_front(1);
  for (size_t run = 1; run <= 2; ++run) {
    EXPECT_EQ(list_alloc.size(), 3);
    EXPECT_FALSE(list_alloc.empty());
    int expect = 1;
    for (auto &el: list_alloc) {
      EXPECT_EQ(el, expect);
      expect += 1;
    }
    EXPECT_EQ(list_alloc.size(), 3);
    EXPECT_FALSE(list_alloc.empty());
  }
  list_alloc.clear();
  EXPECT_EQ(list_alloc.size(), 0);
  EXPECT_TRUE(list_alloc.empty());
  EXPECT_EQ(list_alloc.begin(), list_alloc.end());
}

TEST(list_tests_alloc, list_iter_string) {
  ::list<std::string, linear_allocator<std::string, 4>> list_alloc{};
  list_alloc.push_front("ab");
  list_alloc.push_front("ab");
  list_alloc.push_front("ab");
  for (size_t run = 1; run <= 2; run++) {
    EXPECT_EQ(list_alloc.size(), 3);
    EXPECT_FALSE(list_alloc.empty());
    for (auto &el: list_alloc) {
      EXPECT_EQ(el, "ab");
    }
  }
}