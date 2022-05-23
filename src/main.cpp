#include "matrix.hpp"

#include <cassert>

int main(int, char **) {
  Matrix<int, 2, -1> matrix;
  assert(matrix.size() == 0);
  auto a = matrix[0][0];
  assert(a == -1);
  assert(matrix.size() == 0);
  matrix[100][100] = 314;
  assert(matrix[100][100] == 314);
  assert(matrix.size() == 1);
  ((matrix[100][100] = 314) = 0) = 217;
  assert(matrix[100][100] == 217);
  assert(matrix.size() == 1);
}
