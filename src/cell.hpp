#pragma once

template<typename T, int Default>
class Cell {
public:
  Cell() : _value(Default) {}

  operator T() const { return _value; }

  Cell &operator=(T new_value) {
    _value = new_value;
    return *this;
  }

private:
  T _value;
};