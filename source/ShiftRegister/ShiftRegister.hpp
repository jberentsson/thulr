#pragma once

#define DEFAULT_BIT_SIZE 8
#define REGISTER_COUNT 2

class ShiftRegister {
public:
  ShiftRegister();
  ShiftRegister(int n);
  ~ShiftRegister() {};

  int step();
  int data_input(int v);
  int data_through();
  int get(int i);
  int size() { return this->bits; }
  int activate();
  int get_bias();
  int calculate_bias();

private:
  int clear_register(int r);

  int bits = DEFAULT_BIT_SIZE;
  int data[REGISTER_COUNT][DEFAULT_BIT_SIZE] = {};
  int active_register = 0;
  int current_input = 0;
  int index = 0;
  int current_through = 0;
  int bias = 0;
};
