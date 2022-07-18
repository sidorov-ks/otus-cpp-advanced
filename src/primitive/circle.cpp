#include "circle.h"

#include <iostream>

void Circle::render() const {
  std::cout << "Circle(" << _center.x << "," << _center.y << ";" << _radius << ")";
}

bool Circle::operator==(const Circle &other) const {
  return _center == other._center && _radius == other._radius;
}

bool Circle::contains(const Point &point) const {
  return distance(_center, point) <= _radius;
}
