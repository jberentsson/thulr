#include <type_traits>
#include "../Exceptions/Exceptions.hpp"
#include "Counter.hpp"

int Counter::reset() {
    // Reset counter to zero.
    this->counter = this->first_step;
    return this->counter;
}

int Counter::forward() {
    // Step forward.
    this->counter++;

    if (this->counter >= this->max_value) {
        this->counter = this->first_step;
    }

    return this->counter;
}

int Counter::back() {
    // Step backwards.
    if (this->counter == this->first_step) {
        this->counter = this->max_value - 1;
    } else {
        this->counter--;
    }

    return this->counter;
}

int Counter::toggle_direction(){
    // Toggle the counter direction.
    this->dir = !this->dir;
    return this->dir;
}

int Counter::set_direction(int d){
    // Set the counter direction.
    this->dir = d;
    return this->dir;
}

int Counter::set(int val) {
    // Set the counter value.
    if (0 <= val && val < this->max_value){
        this->counter = val;
    }

    return this->counter;
}

int Counter::preset(){
    // Set the counter to the preset value.
    this->counter = this->preset_value - 1;
    return this->counter;
}

int Counter::set_preset(int p){
    // Set a new preset value.
    this->preset_value = p;
    return this->preset_value;
}

int Counter::step(){
    // Trigger next step.
    if (this->dir){
        return this->forward();
    }

    return this->back();
}

int Counter::enable(){
    // Enable or disable the output.
    this->is_enabled = !this->is_enabled;
    return this->is_enabled;
}

int Counter::set_max_value(int m){
    // Set the maximum value for the counter.
    this->max_value = m;
    return this->max_value;
}
