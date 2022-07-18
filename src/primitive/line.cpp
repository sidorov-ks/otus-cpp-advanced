#include "line.h"

#include <iostream>

//Line::~Line() { }

void Line::render() const {
  std::cout << "Line(" << _from.x << "," << _from.y << ";" << _to.x << "," << _to.y << ")";
}

bool Line::operator==(const Line &other) const {
  return _from == other._from && _to == other._to;
}

bool Line::contains(const Point &point) const {
  return std::abs(distance(_from, point) + distance(point, _to) - distance(_from, _to)) < EPS;
}
