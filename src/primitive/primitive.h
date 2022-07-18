#pragma once

#include "point.h"

class Primitive {
public:
  virtual void render() const = 0;

  // TODO Сделать сравнение по мотивам https://stackoverflow.com/a/1765187
  bool operator==(const Primitive &other) const {
    (void) other;
    return false;
  }

  virtual bool contains(const Point &point) const = 0;
};