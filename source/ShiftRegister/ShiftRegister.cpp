#include "ShiftRegister.hpp"
#include <iostream>

ShiftRegister::ShiftRegister() {
  this->clear_register(0);
  this->clear_register(1);
}

ShiftRegister::ShiftRegister(int n) {
  this->bits = n;
  this->clear_register(0);
  this->clear_register(1);
}

int ShiftRegister::step() {
  this->bias = this->calculate_bias();

  // Store the oldest value.
  if (this->index >= this->bits) {
    // Read from the active register.
    this->current_through = this->data[!this->active_register][this->bias];
  }

  // Store current input in inactive register.
  if (this->bias >= 0 && this->active_register >= 0) {
    this->data[!this->active_register][this->bias] = this->current_input;
  }

  // If there is no through at the moment.
  if (this->index < this->bits) {
    this->current_through = 0;
  }

  return this->index++;
}

int ShiftRegister::get(int i) {
  // Simple circular buffer access
  if (this->active_register >= 0) {
    return this->data[this->active_register][i % this->bits];
  }

  // TODO: Fix this we want null here.
  return -1;
}

int ShiftRegister::calculate_bias() {
  // Calculate the index when the register has overflowed.
  return this->index % this->bits;
}

int ShiftRegister::data_input(int v) {
  this->current_input = v;
  return this->current_input;
}

int ShiftRegister::data_through() {
  // Data through.
  return this->current_through;
}

int ShiftRegister::activate() {
  // Swap registers.
  this->active_register = !this->active_register;

  // Clear the new inactive register.
  this->clear_register(!this->active_register);

  this->index = 0;
  this->bias = 0;

  return this->active_register;
}

int ShiftRegister::clear_register(int r) {
  // Clear a register.
  for (int i = 0; i < this->size(); i++) {
    this->data[r][i] = 0;
  }

  return r;
}
