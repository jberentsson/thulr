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
  auto dataInput(int value) -> uint64_t;
  [[nodiscard]] auto dataThrough() const -> uint64_t;
  auto get(int index) -> uint64_t; // NOLINT 
  [[nodiscard]] auto size() const -> int { return this->bits_; }
  auto activate() -> unsigned int;
  [[nodiscard]] auto getBias() const -> int;
  [[nodiscard]] auto calculateBias() const -> int;
  auto currentInput() -> uint64_t;
  auto clearRegister(int registerID) -> int;

private:
  int bits_ = DEFAULT_BIT_SIZE;
  std::array<std::array<uint64_t, DEFAULT_BIT_SIZE>, REGISTER_COUNT> data_ = {};
  unsigned int activeRegister_ = 0;
  uint64_t currentInput_ = 0;
  int index_ = 0;
  uint64_t currentThrough_ = 0;
  int bias_ = 0;
};
