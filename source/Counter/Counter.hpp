#pragma once

#include <cmath>

// TODO: Find a better number.
#define MAX_VALUE 10

class Counter {
protected:
  unsigned int isEnabled_ = 1;
  unsigned int dir_ = 1;
  unsigned int firstStep_ = 0;
  unsigned int counter_ = 0;
  unsigned int maxValue_ = MAX_VALUE;
  unsigned int presetValue_ = 0;

public:
  Counter() {};
  Counter(int n) { this->maxValue_ = n; }

  virtual int direction() { return this->dir_; }
  virtual int getDirection() { return this->dir_; }
  virtual unsigned int value() { return this->counter_; }
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
