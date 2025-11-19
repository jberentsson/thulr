#pragma once

#define DEFAULT_BIT_SIZE 8
#define REGISTER_COUNT 2

class ShiftRegister {
public:
  ShiftRegister() = default;
  ShiftRegister(int n);
  ~ShiftRegister() = default;

  int step();
  int dataInput(int v);
  int dataThrough();
  int get(int i);
  int size() { return this->bits_; }
  int activate();
  int getBias();
  int calculateBias();
  int currentInput();

private:
  int clearRegister(int r);

  int bits_ = DEFAULT_BIT_SIZE;
  int data_[REGISTER_COUNT][DEFAULT_BIT_SIZE] = {};
  int activeRegister_ = 0;
  int currentInput_ = 0;
  int index_ = 0;
  int currentThrough_ = 0;
  int bias_ = 0;
};
