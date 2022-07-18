#pragma once

#include "primitive.h"
#include "point.h"

class Line : public Primitive {
public:
  Line(const Line &other) : _from(other._from), _to(other._to) {}

  Line(Point from, Point to) : _from(from), _to(to) {}

  bool contains(const Point &point) const override;

  void render() const override;

  bool operator==(const Line &) const;

private:
  Point _from, _to;
};