#include "Counter.hpp"
#include "../Exceptions/Exceptions.hpp"
#include <type_traits>

int Counter::reset() {
  // Reset counter to zero.
  this->counter_ = this->firstStep_;
  return this->counter_;
}

int Counter::forward() {
  // Step forward.
  this->counter_++;

  if (this->counter_ >= this->maxValue_) {
    this->counter_ = this->firstStep_;
  }

  return this->counter_;
}

int Counter::back() {
  // Step backwards.
  if (this->counter_ == this->firstStep_) {
    this->counter_ = this->maxValue_ - 1;
  } else {
    this->counter_--;
  }

  return this->counter_;
}

int Counter::toggleDirection() {
  // Toggle the counter direction.
  this->dir_ = !this->dir_;
  return this->dir_;
}

int Counter::setDirection(int d) {
  // Set the counter direction.
  this->dir_ = d;
  return this->dir_;
}

unsigned int Counter::set(unsigned int val) {
  // Set the counter value.
  if (0 <= val && val < this->maxValue_) {
    this->counter_ = val;
  }

  return this->counter_;
}

int Counter::preset() {
  // Set the counter to the preset value.
  this->counter_ = this->presetValue_ - 1;
  return this->counter_;
}

unsigned int Counter::setPreset(unsigned int p) {
  // Set a new preset value.
  this->presetValue_ = p;
  return this->presetValue_;
}

int Counter::step() {
  // Trigger next step.
  if (this->dir_) {
    return this->forward();
  }

  return this->back();
}

int Counter::enable() {
  // Enable or disable the output.
  this->isEnabled_ = !this->isEnabled_;
  return this->isEnabled_;
}

unsigned int Counter::setMaxValue(unsigned int m) {
  // Set the maximum value for the counter.
  this->maxValue_ = m;
  return this->maxValue_;
}
