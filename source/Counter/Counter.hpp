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

  virtual Direction direction() { return this->dir; }
  virtual Direction getDirection() { return this->dir; }
  virtual unsigned int value() { return this->counter; }
  virtual unsigned int set(unsigned int value);
  virtual unsigned int setMaxValue(unsigned int maxValue);
  virtual int forward();
  virtual int preset();
  virtual int enable();
  virtual unsigned int setPreset(unsigned int presetValue);
  virtual int reset();
  virtual int back();
  virtual int step();
  virtual Direction setDirection(Direction d);
  virtual Direction toggleDirection();

  virtual ~Counter() = default;
};
