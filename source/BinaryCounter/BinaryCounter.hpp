#pragma once

#include "../Counter/Counter.hpp"

class BinaryCounter : public Counter {
  public:
    BinaryCounter() = default;
    explicit BinaryCounter(unsigned int n = MAX_VALUE);

    auto getMaxValue() -> unsigned int;

    using Counter::direction;
    using Counter::enable;
    using Counter::reset;
    using Counter::setPreset;
    using Counter::step;
    using Counter::value;
};
