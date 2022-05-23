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

  MatrixIter(const MatrixType *const _matrix, bool is_end = false) :
          _matrix(_matrix),
          _row_iter(is_end ? _matrix->_elements.end() : _matrix->_elements.begin()),
          _nested_iter(is_end ? std::nullopt : std::optional{NestedIterType{&_row_iter->second}}) {
    refresh_output_value();
    while (!this->is_end() && is_row_end()) {
      make_step();
    }
  }

  reference operator*() const {
    return _output_value;
  }

  pointer operator->() { return _output_value; }

  MatrixIter &operator++() {
    do {
      make_step();
    } while (!this->is_end() && is_row_end());
    return *this;
  }

  bool is_end() const { return _row_iter == _matrix->_elements.end(); }

  friend bool operator==(const MatrixIter &a, const MatrixIter &b) {
    if (a.is_end() || b.is_end())
      return a.is_end() == b.is_end();
    return a._row_iter == b._row_iter && a._nested_iter == b._nested_iter;
  };
private:
  const MatrixType *_matrix;
  RowIterType _row_iter;
  std::optional<NestedIterType> _nested_iter;
  OutputType _output_value;

  void refresh_output_value() {
    if (is_end()) return;
    auto [_nested_index, _value] = **_nested_iter;
    std::array<std::size_t, Dim> index;
    index[0] = _row_iter->first;
    std::copy(_nested_index.begin(), _nested_index.end(), index.begin() + 1);
    _output_value = std::make_tuple(index, _value);
  }

  void make_step() {
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

  bool is_row_end() {
    return _nested_iter && *_nested_iter == _row_iter->second.end();
  }
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

  MatrixIter(const MatrixType *const _matrix, bool is_end = false) :
          _matrix(_matrix),
          _iter(is_end ? _matrix->_elements.end() : _matrix->_elements.begin()) {
    refresh_output_value();
    while (is_default()) {
      make_step();
    }
  }

  reference operator*() const { return _output_value; }

  pointer operator->() { return _output_value; }

  MatrixIter &operator++() {
    do {
      make_step();
    } while (is_default());
    return *this;
  }

  friend bool operator==(const MatrixIter &a, const MatrixIter &b) { return a._iter == b._iter; };
private:
  const MatrixType *_matrix;
  typename std::map<std::size_t, ArrayCellType>::const_iterator _iter;
  OutputType _output_value;

  void refresh_output_value() {
    if (_iter != _matrix->_elements.end()) {
      _output_value = std::make_tuple(std::array<std::size_t, 1>{_iter->first}, _iter->second);
      return;
    }
  }

  void make_step() {
    _iter++;
    if (_iter != _matrix->_elements.end()) {
      refresh_output_value();
    }
  }

  bool is_default() {
    return _iter != _matrix->_elements.end() && std::get<1>(_output_value) == Default;
  }
};