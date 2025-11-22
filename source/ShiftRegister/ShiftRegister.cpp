#include "ShiftRegister.hpp"
#include <iostream>

ShiftRegister::ShiftRegister(int n) {
    this->bits_ = n;
    this->clearRegister(0);
    this->clearRegister(1);
}

auto ShiftRegister::clearRegister(int registerID) -> int {
    // Clear a register.
    for (int i = 0; i < this->size(); i++) {
        this->data_[registerID][i] = 0;
    }

    return registerID;
}

auto ShiftRegister::step() -> int {
    this->bias_ = this->calculateBias();

    // Store the oldest value.
    if (this->index_ >= this->bits_) {
        // Read from the active register.
        this->currentThrough_ = this->data_[(unsigned int)!(bool)this->activeRegister_][this->bias_];
    }

    // Store current input in inactive register.
    if (this->bias_ >= 0 && this->activeRegister_ >= 0) {
        this->data_[(unsigned int)!(bool)this->activeRegister_][this->bias_] = this->currentInput_;
    }

    // If there is no through at the moment.
    if (this->index_ < this->bits_) {
        this->currentThrough_ = 0;
    }

    return this->index_++;
}

auto ShiftRegister::get(int index) -> int {
    // Simple circular buffer access
    if (this->activeRegister_ >= 0) {
        return this->data_[this->activeRegister_][index % this->bits_];
    }

    return -1;
}

auto ShiftRegister::calculateBias() const -> int {
    // Calculate the index when the register has overflowed.
    return this->index_ % this->bits_;
}

auto ShiftRegister::dataInput(int value) -> int {
    this->currentInput_ = value;
    return this->currentInput_;
}

auto ShiftRegister::dataThrough() const -> int {
    // Data through.
    return this->currentThrough_;
}

auto ShiftRegister::activate() -> unsigned int {
    // Swap registers.
    this->activeRegister_ = (unsigned int)!(bool)this->activeRegister_;

    // Clear the new inactive register.
    this->clearRegister((int)!(bool)this->activeRegister_);

    this->index_ = 0;
    this->bias_ = 0;

    return this->activeRegister_;
}
