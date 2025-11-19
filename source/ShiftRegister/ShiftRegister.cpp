#include "ShiftRegister.hpp"
#include <iostream>

ShiftRegister::ShiftRegister(int n) {
  this->bits_ = n;
  this->clearRegister(0);
  this->clearRegister(1);
}

int ShiftRegister::step() {
  this->bias_ = this->calculateBias();

  // Store the oldest value.
  if (this->index_ >= this->bits_) {
    // Read from the active register.
    this->currentThrough_ = this->data_[!this->activeRegister_][this->bias_];
  }

  // Store current input in inactive register.
  if (this->bias_ >= 0 && this->activeRegister_ >= 0) {
    this->data_[!this->activeRegister_][this->bias_] = this->currentInput_;
  }

  // If there is no through at the moment.
  if (this->index_ < this->bits_) {
    this->currentThrough_ = 0;
  }

  return this->index_++;
}

int ShiftRegister::get(int i) {
  // Simple circular buffer access
  if (this->activeRegister_ >= 0) {
    return this->data_[this->activeRegister_][i % this->bits_];
  }

  // TODO: Fix this we want null here.
  return -1;
}

int ShiftRegister::calculateBias() {
  // Calculate the index when the register has overflowed.
  return this->index_ % this->bits_;
}

int ShiftRegister::dataInput(int v) {
  this->currentInput_ = v;
  return this->currentInput_;
}

int ShiftRegister::dataThrough() {
  // Data through.
  return this->currentThrough_;
}

int ShiftRegister::activate() {
  // Swap registers.
  this->activeRegister_ = !this->activeRegister_;

  // Clear the new inactive register.
  this->clearRegister(!this->activeRegister_);

  this->index_ = 0;
  this->bias_ = 0;

  return this->activeRegister_;
}

int ShiftRegister::clearRegister(int r) {
  // Clear a register.
  for (int i = 0; i < this->size(); i++) {
    this->data_[r][i] = 0;
  }

  return r;
}
