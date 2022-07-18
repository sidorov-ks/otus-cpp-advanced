#include "point.h"

#include <cmath>

double distance(const Point &a, const Point &b) {
  return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

bool Point::operator==(const Point &other) const {
  return x == other.x && y == other.y;
}
