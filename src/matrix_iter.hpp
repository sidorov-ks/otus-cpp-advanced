//#pragma once
//
//#include "matrix.hpp"
//#include "cell.hpp"
//
//#include <memory>
//#include <map>
//#include <tuple>
//#include <array>
//
//template<typename T, std::size_t Dim, T Default>
//struct MatrixIter {
//  using MatrixType = Matrix<T, Dim, Default>;
//  using NestedMatrixType = typename MatrixType::NestedMatrix;
//  using NestedIterType = MatrixIter<T, Dim - 1, Default>;
//  using OutputType = std::tuple<std::array<std::size_t, Dim>, T>;
//
//  using PointerType = OutputType *;
//  using ReferenceType = OutputType &;
//
//  MatrixIter(std::shared_ptr<MatrixType> &&_matrix) :
//          _matrix(_matrix),
//          _row_iter(_matrix->_elements),
//          _nested_iter(MatrixIter{std::make_shared(*_row_iter)}) {}
//
//  ReferenceType operator*() const { return _output_value; }
//
//  PointerType operator->() { return _output_value; }
//
//  MatrixIter &operator++() {
//    _nested_iter++;
//    if (_nested_iter == _matrix->end()) {
//      _row_iter++;
//      if (_row_iter != _matrix->_elements->end()) {
//        _nested_iter = MatrixIter{std::make_shared(*_row_iter)};
//      }
//    }
//    return *this;
//  }
//
//  friend bool operator==(const MatrixIter &a, const MatrixIter &b) { return a._nested_iter == b._nested_iter; };
//private:
//  std::shared_ptr<MatrixType> _matrix;
//  typename std::map<std::size_t, NestedMatrixType>::const_iterator _row_iter;
//  NestedIterType _nested_iter;
//  OutputType _output_value;
//};