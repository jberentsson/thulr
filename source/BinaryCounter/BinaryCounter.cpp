#include "BinaryCounter.hpp"
#include <cmath>

BinaryCounter::BinaryCounter(unsigned int n) {
    // TODO: We get an error here.Call to virtual method setMaxValue during
    // construction bypasses virtual dispatch.
    this->setMaxValue(n);
}

// unsigned int BinaryCounter::setMaxValue(int n) {
//   this->maxValue_ = std::pow(2, n);
//   return this->maxValue_;
// }

auto BinaryCounter::getMaxValue() -> unsigned int { return this->maxValue; }
