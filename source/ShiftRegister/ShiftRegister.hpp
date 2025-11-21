#pragma once

#include <array>
#include <cstdint>

class ShiftRegister {
public:
  enum : std::uint8_t {
    DEFAULT_BIT_SIZE = 8,
    REGISTER_COUNT = 2
  };

  ShiftRegister() = default;
  ShiftRegister(int n);
  ~ShiftRegister() = default;

  auto step() -> int;
  auto dataInput(int value) -> int;
  [[nodiscard]] auto dataThrough() const -> int;
  auto get(int index) -> int; // NOLINT 
  [[nodiscard]] auto size() const -> int { return this->bits_; }
  auto activate() -> unsigned int;
  [[nodiscard]] auto getBias() const -> int;
  [[nodiscard]] auto calculateBias() const -> int;
  auto currentInput() -> int;
  auto clearRegister(int registerID) -> int;

private:
  int bits_ = DEFAULT_BIT_SIZE;
  std::array<std::array<int, DEFAULT_BIT_SIZE>, REGISTER_COUNT> data_ = {};
  unsigned int activeRegister_ = 0;
  int currentInput_ = 0;
  int index_ = 0;
  int currentThrough_ = 0;
  int bias_ = 0;
};
