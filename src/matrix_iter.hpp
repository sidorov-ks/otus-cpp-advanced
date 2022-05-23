#pragma once

#include "matrix.hpp"
#include "cell.hpp"

#include <map>
#include <tuple>
#include <array>
#include <optional>

template<typename T, std::size_t Dim, T Default>
class Matrix;

template<typename T, std::size_t Dim, T Default>
struct MatrixIter {
  using MatrixType = Matrix<T, Dim, Default>;
  using NestedMatrixType = typename MatrixType::NestedMatrix;
  using RowIterType = typename std::map<std::size_t, NestedMatrixType>::const_iterator;
  using NestedIterType = MatrixIter<T, Dim - 1, Default>;
  using OutputType = std::tuple<std::array<std::size_t, Dim>, T>;

  using difference_type = std::size_t;
  using value_type = OutputType;
  using pointer = const OutputType *;
  using reference = const OutputType &;
  using iterator_category = std::forward_iterator_tag;

  explicit MatrixIter(const MatrixType *_matrix, bool is_end = false);

  reference operator*() const;

  pointer operator->();

  MatrixIter &operator++();

  bool operator==(const MatrixIter &) const;

private:
  const MatrixType *_matrix;
  RowIterType _row_iter;
  std::optional<NestedIterType> _nested_iter;
  OutputType _output_value;

  [[nodiscard]] bool is_end() const;

  [[nodiscard]] bool is_row_end() const;

  void refresh_output_value();

  void make_step();
};

template<typename T, T Default>
struct MatrixIter<T, 1, Default> {
  using MatrixType = Matrix<T, 1, Default>;
  using ArrayCellType = typename MatrixType::ArrayCell;
  using OutputType = std::tuple<std::array<std::size_t, 1>, T>;

  using difference_type = std::size_t;
  using value_type = OutputType;
  using pointer = const OutputType *;
  using reference = const OutputType &;
  using iterator_category = std::forward_iterator_tag;

  explicit MatrixIter(const MatrixType *_matrix, bool is_end = false);

  reference operator*() const;

  pointer operator->();

  MatrixIter &operator++();

  bool operator==(const MatrixIter &a) const;

private:
  const MatrixType *_matrix;
  typename std::map<std::size_t, ArrayCellType>::const_iterator _iter;
  OutputType _output_value;

  void refresh_output_value();

  void make_step();

  bool is_default();
};

template<typename T, std::size_t Dim, T Default>
MatrixIter<T, Dim, Default>::MatrixIter(const MatrixIter::MatrixType *const _matrix, bool is_end) :
        _matrix(_matrix),
        _row_iter(is_end ? _matrix->_elements.end() : _matrix->_elements.begin()),
        _nested_iter(is_end ? std::nullopt : std::optional{NestedIterType{&_row_iter->second}}) {
  refresh_output_value();
  while (!this->is_end() && is_row_end()) {
    make_step();
  }
}

template<typename T, std::size_t Dim, T Default>
typename MatrixIter<T, Dim, Default>::reference MatrixIter<T, Dim, Default>::operator*() const { return _output_value; }

template<typename T, std::size_t Dim, T Default>
typename MatrixIter<T, Dim, Default>::pointer MatrixIter<T, Dim, Default>::operator->() { return _output_value; }

template<typename T, std::size_t Dim, T Default>
bool MatrixIter<T, Dim, Default>::operator==(const MatrixIter &other) const {
  if (is_end() || other.is_end())
    return is_end() == other.is_end();
  return _row_iter == other._row_iter && _nested_iter == other._nested_iter;
}

template<typename T, std::size_t Dim, T Default>
MatrixIter<T, Dim, Default> &MatrixIter<T, Dim, Default>::operator++() {
  do {
    make_step();
  } while (!this->is_end() && is_row_end());
  return *this;
}

template<typename T, std::size_t Dim, T Default>
bool MatrixIter<T, Dim, Default>::is_end() const { return _row_iter == _matrix->_elements.end(); }

template<typename T, std::size_t Dim, T Default>
bool MatrixIter<T, Dim, Default>::is_row_end() const {
  return _nested_iter && *_nested_iter == _row_iter->second.end();
}

template<typename T, std::size_t Dim, T Default>
void MatrixIter<T, Dim, Default>::refresh_output_value() {
  if (is_end()) return;
  auto [_nested_index, _value] = **_nested_iter;
  std::array<std::size_t, Dim> index;
  index[0] = _row_iter->first;
  std::copy(_nested_index.begin(), _nested_index.end(), index.begin() + 1);
  _output_value = std::make_tuple(index, _value);
}

template<typename T, std::size_t Dim, T Default>
void MatrixIter<T, Dim, Default>::make_step() {
  if (_nested_iter) {
    ++*_nested_iter;
    if (is_row_end()) {
      ++_row_iter;
      if (is_end()) {
        _nested_iter.reset();
      } else {
        *_nested_iter = _row_iter->second.begin();
      }
    }
  }
  if (!is_end()) refresh_output_value();
}

template<typename T, T Default>
MatrixIter<T, 1, Default>::MatrixIter(const MatrixIter<T, 1, Default>::MatrixType *const _matrix, bool is_end) :
        _matrix(_matrix),
        _iter(is_end ? _matrix->_elements.end() : _matrix->_elements.begin()) {
  refresh_output_value();
  while (is_default()) {
    make_step();
  }
}

template<typename T, T Default>
typename MatrixIter<T, 1, Default>::reference MatrixIter<T, 1, Default>::operator*() const { return _output_value; }

template<typename T, T Default>
typename MatrixIter<T, 1, Default>::pointer MatrixIter<T, 1, Default>::operator->() { return _output_value; }

template<typename T, T Default>
bool MatrixIter<T, 1, Default>::operator==(const MatrixIter &other) const {
  return _iter == other._iter;
}

template<typename T, T Default>
MatrixIter<T, 1, Default> &MatrixIter<T, 1, Default>::operator++() {
  do {
    make_step();
  } while (is_default());
  return *this;
}

template<typename T, T Default>
void MatrixIter<T, 1, Default>::refresh_output_value() {
  if (_iter != _matrix->_elements.end()) {
    _output_value = std::make_tuple(std::array<std::size_t, 1>{_iter->first}, _iter->second);
  }
}

template<typename T, T Default>
void MatrixIter<T, 1, Default>::make_step() {
  _iter++;
  if (_iter != _matrix->_elements.end()) {
    refresh_output_value();
  }
}

template<typename T, T Default>
bool MatrixIter<T, 1, Default>::is_default() {
  return _iter != _matrix->_elements.end() && std::get<1>(_output_value) == Default;
}
