#pragma once

#include "../Counter/Counter.hpp"

class BinaryCounter : public Counter {
public:
  BinaryCounter();
  BinaryCounter(int n);

  unsigned int getMaxValue();
  //unsigned int setMaxValue(int n);

  using Counter::direction;
  using Counter::enable;
  using Counter::reset;
  using Counter::step;
  using Counter::value;
  using Counter::setPreset;
};
