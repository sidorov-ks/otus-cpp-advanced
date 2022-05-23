#pragma once

#include "cell.hpp"

#include <iostream>
#include <map>
#include <type_traits>
#include <memory>

template<typename T, std::size_t Dim, T Default = 0>
struct MatrixIter;

template<typename T, std::size_t Dim, T Default = 0>
class Matrix {
public:
  using NestedMatrix = Matrix<T, Dim - 1, Default>;

  std::size_t size() const;

  NestedMatrix &operator[](std::size_t index);

private:
  std::map<std::size_t, NestedMatrix> _elements;
  NestedMatrix _default;
  friend struct MatrixIter<T, Dim, Default>;
};

template<typename T, T Default>
class Matrix<T, 1, Default> {
public:
  using ArrayCell = Cell<T, Default>;

  std::size_t size() const;

  ArrayCell &operator[](std::size_t index);

private:
  std::map<std::size_t, ArrayCell> _elements;
  ArrayCell _default;
  friend struct MatrixIter<T, 1, Default>;
};

template<typename T, std::size_t Dim, T Default>
std::size_t Matrix<T, Dim, Default>::size() const {
  std::size_t res = 0;
  for (const auto &nested: _elements)
    res += nested.second.size();
  return res;
}

template<typename T, T Default>
std::size_t Matrix<T, 1, Default>::size() const {
  std::size_t res = 0;
  for (const auto &el: _elements) {
    if (el.second != Default) res++;
  }
  return res;
}

template<typename T, std::size_t Dim, T Default>
typename Matrix<T, Dim, Default>::NestedMatrix &Matrix<T, Dim, Default>::operator[](std::size_t index) {
  auto it = _elements.find(index);
  if (it == _elements.end())
    _elements[index] = NestedMatrix{};
  return _elements[index];
}

template<typename T, T Default>
typename Matrix<T, 1, Default>::ArrayCell &Matrix<T, 1, Default>::operator[](std::size_t index) {
  auto it = _elements.find(index);
  if (it == _elements.end())
    _elements[index] = ArrayCell{};
  return _elements[index];
}