#include <gtest/gtest.h>

#include "matrix.hpp"

#include <vector>
#include <algorithm>

TEST(array_access_ops, array_write) {
  Matrix<int, 1> matrix;
  matrix[100] = 314;
  EXPECT_EQ(matrix[100], 314);
}

TEST(array_access_ops, array_chained_write) {
  Matrix<int, 1> matrix;
  ((matrix[100] = 314) = 0) = 217;
  EXPECT_EQ(matrix[100], 217);
}

TEST(array_access_ops, array_multiple_write) {
  Matrix<int, 1> matrix;
  matrix[0] = 1;
  matrix[1] = 2;
  matrix[2] = 3;
  matrix[3] = 0;
  EXPECT_EQ(matrix[0], 1);
  EXPECT_EQ(matrix[1], 2);
  EXPECT_EQ(matrix[2], 3);
  EXPECT_EQ(matrix[3], 0);
}

TEST(array_access_ops, array_size) {
  Matrix<int, 1> matrix;
  matrix[0] = 1;
  EXPECT_EQ(matrix.size(), 1);
  matrix[1] = 2;
  EXPECT_EQ(matrix.size(), 2);
  matrix[2] = 3;
  EXPECT_EQ(matrix.size(), 3);
  matrix[3] = 0;
  EXPECT_EQ(matrix.size(), 3);
  matrix[2] = 0;
  EXPECT_EQ(matrix.size(), 2);
}

TEST(ndim_matrix_access_ops, matrix_write) {
  Matrix<int, 3> matrix;
  matrix[100][100][100] = 314;
  EXPECT_EQ(matrix[100][100][100], 314);
}

TEST(ndim_matrix_access_ops, matrix_chained_write) {
  Matrix<int, 3> matrix;
  ((matrix[100][100][100] = 314) = 0) = 217;
  EXPECT_EQ(matrix[100][100][100], 217);
}

TEST(ndim_matrix_access_ops, matrix_multiple_write) {
  Matrix<int, 3> matrix;
  matrix[0][0][0] = 1;
  matrix[1][1][1] = 2;
  matrix[2][2][2] = 3;
  matrix[0][1][0] = 0;
  EXPECT_EQ(matrix[0][0][0], 1);
  EXPECT_EQ(matrix[1][1][1], 2);
  EXPECT_EQ(matrix[2][2][2], 3);
  EXPECT_EQ(matrix[0][1][0], 0);
}

TEST(ndim_matrix_access_ops, matrix_size) {
  Matrix<int, 3> matrix;
  matrix[0][0][0] = 1;
  EXPECT_EQ(matrix.size(), 1);
  matrix[1][1][1] = 2;
  EXPECT_EQ(matrix.size(), 2);
  matrix[2][2][2] = 3;
  EXPECT_EQ(matrix.size(), 3);
  matrix[0][1][0] = 0;
  EXPECT_EQ(matrix.size(), 3);
  matrix[2][2][2] = 0;
  EXPECT_EQ(matrix.size(), 2);
}

template<typename T, std::size_t Dim, T Default>
std::vector<std::tuple<std::array<std::size_t, Dim>, T>> get_values(const Matrix<T, Dim, Default> &matrix) {
  std::vector<std::tuple<std::array<std::size_t, Dim>, T>> res(matrix.begin(), matrix.end());
  std::sort(res.begin(), res.end(), [](auto lhs, auto rhs) {
    auto [ix_lhs, _lhs] = lhs;
    auto [ix_rhs, _rhs] = rhs;
    return ix_lhs < ix_rhs;
  });
  return res;
}

TEST(array_iter, iter_empty) {
  Matrix<int, 1> matrix;
  std::vector<std::tuple<std::array<std::size_t, 1>, int>> actual = get_values(matrix);
  EXPECT_TRUE(actual.empty());
}

TEST(array_iter, iter_default) {
  Matrix<int, 1> matrix;
  matrix[0] = 0;
  matrix[1] = 0;
  matrix[2] = 0;
  std::vector<std::tuple<std::array<std::size_t, 1>, int>> actual = get_values(matrix);
  EXPECT_TRUE(actual.empty());
}

TEST(array_iter, iter_mixed) {
  Matrix<int, 1> matrix;
  matrix[1] = 0;
  matrix[2] = 1;
  matrix[3] = 0;
  matrix[5] = 2;
  matrix[8] = 3;
  std::vector<std::tuple<std::array<std::size_t, 1>, int>> actual = get_values(matrix);
  EXPECT_FALSE(actual.empty());
  EXPECT_EQ(actual.size(), 3);
  {
    auto [_ix, val] = actual[0];
    auto [ix] = _ix;
    EXPECT_EQ(val, 1);
    EXPECT_EQ(ix, 2);
  }
  {
    auto [_ix, val] = actual[1];
    auto [ix] = _ix;
    EXPECT_EQ(val, 2);
    EXPECT_EQ(ix, 5);
  }
  {
    auto [_ix, val] = actual[2];
    auto [ix] = _ix;
    EXPECT_EQ(val, 3);
    EXPECT_EQ(ix, 8);
  }
}

TEST(ndim_matrix_iter, iter_empty) {
  Matrix<int, 3> matrix;
  std::vector<std::tuple<std::array<std::size_t, 3>, int>> actual = get_values(matrix);
  EXPECT_TRUE(actual.empty());
}

TEST(ndim_matrix_iter, iter_default) {
  Matrix<int, 3> matrix;
  matrix[0][0][0] = 0;
  matrix[1][1][1] = 0;
  matrix[2][2][2] = 0;
  std::vector<std::tuple<std::array<std::size_t, 3>, int>> actual = get_values(matrix);
  EXPECT_TRUE(actual.empty());
}

TEST(ndim_matrix_iter, iter_mixed_rows) {
  Matrix<int, 3> matrix;
  matrix[0][0][0] = 0;
  matrix[1][1][1] = 1;
  matrix[1][1][2] = 1;
  matrix[2][2][2] = 0;
  matrix[3][3][3] = 3;
  matrix[3][3][6] = 3;
  std::vector<std::tuple<std::array<std::size_t, 3>, int>> actual = get_values(matrix);
  EXPECT_FALSE(actual.empty());
  EXPECT_EQ(actual.size(), 4);
  {
    auto [ix, val] = actual[0];
    auto [x, y, z] = ix;
    EXPECT_EQ(val, 1);
    EXPECT_EQ(x, 1);
    EXPECT_EQ(y, 1);
    EXPECT_EQ(z, 1);
  }
  {
    auto [ix, val] = actual[1];
    auto [x, y, z] = ix;
    EXPECT_EQ(val, 1);
    EXPECT_EQ(x, 1);
    EXPECT_EQ(y, 1);
    EXPECT_EQ(z, 2);
  }
  {
    auto [ix, val] = actual[2];
    auto [x, y, z] = ix;
    EXPECT_EQ(val, 3);
    EXPECT_EQ(x, 3);
    EXPECT_EQ(y, 3);
    EXPECT_EQ(z, 3);
  }
  {
    auto [ix, val] = actual[3];
    auto [x, y, z] = ix;
    EXPECT_EQ(val, 3);
    EXPECT_EQ(x, 3);
    EXPECT_EQ(y, 3);
    EXPECT_EQ(z, 6);
  }
}
