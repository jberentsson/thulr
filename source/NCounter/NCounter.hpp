#pragma once

#include "../Counter/Counter.hpp"

class NCounter : public Counter {
  public:
    explicit NCounter(unsigned int n = MAX_VALUE);

    using Counter::enable;
    using Counter::reset;
    using Counter::setPreset;
    using Counter::step;
    using Counter::value;
};
