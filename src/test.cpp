#include <gtest/gtest.h>

#include "matrix.hpp"

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