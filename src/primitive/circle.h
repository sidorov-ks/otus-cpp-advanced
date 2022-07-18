#pragma once

#include "primitive.h"
#include "point.h"

class Circle : public Primitive {
public:
  Circle(const Circle &other) : _center(other._center), _radius(other._radius) {}

  Circle(Point center, double radius) : _center(center), _radius(radius) {}

  bool contains(const Point &point) const override;

  void render() const override;

  bool operator==(const Circle &) const;

private:
  Point _center;
  double _radius;
};