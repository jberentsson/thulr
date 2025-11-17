#include "BinaryCounter.hpp"
#include <cmath>

BinaryCounter::BinaryCounter(int n) {
  // TODO: We get an error here.Call to virtual method set_max_value during
  // construction bypasses virtual dispatch.
  this->set_max_value(n);
}

int BinaryCounter::set_max_value(int n) {
  this->max_value = std::pow(2, n);
  return this->max_value;
}

int BinaryCounter::get_max_value() { return this->max_value; }
