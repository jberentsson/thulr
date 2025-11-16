#pragma once

#include "../Counter/Counter.hpp"

class BinaryCounter : public Counter {
public:
  BinaryCounter();
  BinaryCounter(int n);

  int get_max_value();
  int set_max_value(int n);

  using Counter::direction;
  using Counter::enable;
  using Counter::reset;
  using Counter::step;
  using Counter::value;
};
