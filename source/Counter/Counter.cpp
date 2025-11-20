#include "Counter.hpp"
#include "../Exceptions/Exceptions.hpp"
#include <type_traits>

int Counter::reset() {
  // Reset counter to zero.
  this->counter = this->firstStep;
  return this->counter;
}

int Counter::forward() {
  // Step forward.
  this->counter++;

  if (this->counter >= this->maxValue) {
    this->counter = this->firstStep;
  }

  return this->counter;
}

int Counter::back() {
  // Step backwards.
  if (this->counter == this->firstStep) {
    this->counter = this->maxValue - 1;
  } else {
    this->counter--;
  }

  return this->counter;
}

Counter::Direction Counter::toggleDirection() {
  // Toggle the counter direction.
  if (this->dir == Direction::FORWARD){
    this->dir = Direction::REVERSE;
  } else {
    this->dir = Direction::FORWARD;
  }

  return this->dir;
}

Counter::Direction Counter::setDirection(Direction direction) {
  // Set the counter direction.
  this->dir = direction;
  return this->dir;
}

unsigned int Counter::set(unsigned int val) {
  // Set the counter value.
  if (0 <= val && val < this->maxValue) {
    this->counter = val;
  }

  return this->counter;
}

int Counter::preset() {
  // Set the counter to the preset value.
  this->counter = this->presetValue - 1;
  return this->counter;
}

unsigned int Counter::setPreset(unsigned int p) {
  // Set a new preset value.
  this->presetValue = p;
  return this->presetValue;
}

int Counter::step() {
  // Trigger next step.
  if (this->dir) {
    return this->forward();
  }

  return this->back();
}

int Counter::enable() {
  // Enable or disable the output.
  this->isEnabled = !this->isEnabled;
  return this->isEnabled;
}

unsigned int Counter::setMaxValue(unsigned int m) {
  // Set the maximum value for the counter.
  this->maxValue = m;
  return this->maxValue;
}
