#pragma once

#include <cmath>

struct Point {
  double x, y;

  bool operator==(const Point &other) const;
};

const double EPS = 1e-8;

double distance(const Point &a, const Point &b);