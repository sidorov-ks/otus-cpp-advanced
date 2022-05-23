#include "matrix.hpp"

#include <cassert>

void test_example();

void test_task();

int main(int, char **) {
  test_example();
  test_task();
}

void test_task() {
  Matrix<int, 2> matrix;
  for (size_t i = 0; i < 10; ++i) {
    matrix[i][i] = i;
    matrix[9 - i][i] = i;
  }
  std::cout << "Sub-matrix [1..8] * [1..8]:\n";
  for (size_t i = 1; i < 9; ++i) {
    for (size_t j = 1; j < 9; ++j) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << "\n";
  }
  std::cout << "Cells used: " << matrix.size() << "\n";
  std::cout << "Cells taken:\n";
  for (const auto &[ix, val]: matrix) {
    std::cout << val << " @ ";
    for (auto i: ix)
      std::cout << i << " ";
    std::cout << "\n";
  }
}

void test_example() {
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
  for (auto [ix, v]: matrix) {
    auto [x, y] = ix;
    std::cout << x << y << v << std::endl;
  }
}
