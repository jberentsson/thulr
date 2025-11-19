#pragma once

#include <array>
#include <cstdint>

class ShiftRegister {
public:
  enum {
    DEFAULT_BIT_SIZE = 8,
    REGISTER_COUNT = 2
  };

  ShiftRegister() = default;
  ShiftRegister(int n);
  ~ShiftRegister() = default;

  int step();
  int dataInput(int v);
  int dataThrough();
  int get(int i);
  [[nodiscard]] int size() const { return this->bits_; }
  int activate();
  int getBias();
  int calculateBias();
  int currentInput();

private:
  int clearRegister(int r);

  int bits_ = DEFAULT_BIT_SIZE;
  std::array<std::array<uint32_t, DEFAULT_BIT_SIZE>, REGISTER_COUNT> data_ = {};
  int activeRegister_ = 0;
  int currentInput_ = 0;
  int index_ = 0;
  int currentThrough_ = 0;
  int bias_ = 0;
};
