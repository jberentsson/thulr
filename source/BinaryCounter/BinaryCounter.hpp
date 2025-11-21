#pragma once

#include "../Counter/Counter.hpp"

class BinaryCounter : public Counter {
public:
    BinaryCounter() = default;
    BinaryCounter(unsigned int n);

    auto getMaxValue() -> unsigned int;
    //unsigned int setMaxValue(int n);

    using Counter::direction;
    using Counter::enable;
    using Counter::reset;
    using Counter::step;
    using Counter::value;
    using Counter::setPreset;
};
