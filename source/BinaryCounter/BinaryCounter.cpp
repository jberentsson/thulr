#include "BinaryCounter.hpp"
#include <cmath>

BinaryCounter::BinaryCounter(unsigned int n) {
    this->setMaxValue(n);
}

auto BinaryCounter::getMaxValue() -> unsigned int { return this->maxValue; }
