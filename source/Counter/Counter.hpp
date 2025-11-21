#pragma once

#include <cmath>
#include <cstdint>

class Counter {
protected:
  enum : std::uint8_t {
    // TODO: Find a better number.
    MAX_VALUE = 10
  };

public:
  enum Direction {
    FORWARD = true,
    REVERSE = false,
  };

protected:
  unsigned int isEnabled = 1;
  Direction dir = Direction::FORWARD;
  unsigned int firstStep = 0;
  unsigned int counter = 0;
  unsigned int maxValue = MAX_VALUE;
  unsigned int presetValue = 0;

public:
  Counter() = default;
  Counter(int n) { this->maxValue = n; }

  virtual auto direction() -> Direction { return this->dir; }
  virtual auto getDirection() -> Direction { return this->dir; }
  virtual auto value() -> unsigned int { return this->counter; }
  virtual auto set(unsigned int value) -> unsigned int;
  virtual auto setMaxValue(unsigned int maxValue) -> unsigned int;
  virtual auto forward() -> int;
  virtual auto preset() -> int;
  virtual auto enable() -> int;
  virtual auto  setPreset(unsigned int presetValue) -> unsigned int;
  virtual auto reset() -> int;
  virtual auto back() -> int;
  virtual auto step() -> int;
  virtual auto setDirection(Direction d) -> Direction;
  virtual auto toggleDirection() -> Direction;

  virtual ~Counter() = default;
};
