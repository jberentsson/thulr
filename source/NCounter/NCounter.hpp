#pragma once

#include "../Counter/Counter.hpp"

class NCounter : public Counter {
public:
  NCounter(int n);

  using Counter::enable;
  using Counter::reset;
  using Counter::step;
  using Counter::value;
  using Counter::setPreset;
};
