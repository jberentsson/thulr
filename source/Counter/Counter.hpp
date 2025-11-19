#pragma once

#include <cmath>
#include <cstdint>

class Counter {
protected:
  enum : std::uint8_t {
    // TODO: Find a better number.
    MAX_VALUE = 10
  };

  unsigned int isEnabled = 1;
  unsigned int dir = 1;
  unsigned int firstStep = 0;
  unsigned int counter = 0;
  unsigned int maxValue = MAX_VALUE;
  unsigned int presetValue = 0;

public:
  Counter() = default;
  Counter(int n) { this->maxValue = n; }

  virtual unsigned int direction() { return this->dir; }
  virtual unsigned int getDirection() { return this->dir; }
  virtual unsigned int value() { return this->counter; }
  virtual unsigned int set(unsigned int val);
  virtual unsigned int setMaxValue(unsigned int m);
  virtual int forward();
  virtual int preset();
  virtual int enable();
  virtual unsigned int setPreset(unsigned int p);
  virtual int reset();
  virtual int back();
  virtual int step();
  virtual int setDirection(int d);
  virtual int toggleDirection();

  virtual ~Counter() = default;
};
