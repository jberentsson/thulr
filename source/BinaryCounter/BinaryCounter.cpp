#include <cmath>
#include "BinaryCounter.hpp"

BinaryCounter::BinaryCounter(int n) {
    this->set_max_value(n);
}

int BinaryCounter::set_max_value(int n){
    this->max_value = std::pow(2, n);
    return this->max_value;
}

int BinaryCounter::get_max_value(){
    return this->max_value;
}
