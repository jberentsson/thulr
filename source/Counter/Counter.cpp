#include "Counter.hpp"
#include "../Exceptions/Exceptions.hpp"
#include <type_traits>

auto Counter::reset() -> int {
  // Reset counter to zero.
  this->counter = this->firstStep;
  return this->counter;
}

auto Counter::forward() -> int {
  // Step forward.
  this->counter++;

  if (this->counter >= this->maxValue) {
    this->counter = this->firstStep;
  }

  return this->counter;
}

auto Counter::back() -> int {
  // Step backwards.
  if (this->counter == this->firstStep) {
    this->counter = this->maxValue - 1;
  } else {
    this->counter--;
  }

  return this->counter;
}

auto Counter::toggleDirection() -> Counter::Direction {
  // Toggle the counter direction.
  if (this->dir == Direction::FORWARD){
    this->dir = Direction::REVERSE;
  } else {
    this->dir = Direction::FORWARD;
  }

  return this->dir;
}

auto Counter::setDirection(Direction direction) -> Counter::Direction {
  // Set the counter direction.
  this->dir = direction;
  return this->dir;
}

auto Counter::set(unsigned int val) -> unsigned int {
  // Set the counter value.
  if (0 <= val && val < this->maxValue) {
    this->counter = val;
  }

  return this->counter;
}

auto Counter::preset() -> int {
  // Set the counter to the preset value.
  this->counter = this->presetValue - 1;
  return this->counter;
}

auto Counter::setPreset(unsigned int p) -> unsigned int {
  // Set a new preset value.
  this->presetValue = p;
  return this->presetValue;
}

auto Counter::step() -> int {
  // Trigger next step.
  if (this->dir) {
    return this->forward();
  }

  return this->back();
}

auto Counter::enable() -> int {
  // Enable or disable the output.
  this->isEnabled = !this->isEnabled;
  return this->isEnabled;
}

auto Counter::setMaxValue(unsigned int m) -> unsigned int {
  // Set the maximum value for the counter.
  this->maxValue = m;
  return this->maxValue;
}
