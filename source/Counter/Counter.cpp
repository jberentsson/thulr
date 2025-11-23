#include "Counter.hpp"
#include "Exceptions/Exceptions.hpp"
#include <type_traits>

auto Counter::reset() -> unsigned int {
    // Reset counter to zero.
    this->counter = this->firstStep;
    return this->counter;
}

auto Counter::forward() -> unsigned int {
    // Step forward.
    this->counter++;

    if (this->counter >= this->maxValue) {
        this->counter = this->firstStep;
    }

    return this->counter;
}

auto Counter::back() -> unsigned int {
    // Step backwards.
    if (this->counter == this->firstStep) {
        this->counter = this->maxValue - 1;
    } else {
        this->counter--;
    }

    return this->counter;
}

auto Counter::toggleDirection() -> Counter::Direction {
    // Toggle the counter direction.
    if (this->dir == Direction::FORWARD) {
        this->dir = Direction::REVERSE;
    } else {
        this->dir = Direction::FORWARD;
    }

    return this->dir;
}

auto Counter::setDirection(Direction newDirection) -> Counter::Direction {
    // Set the counter direction.
    this->dir = newDirection;
    return this->dir;
}

auto Counter::set(unsigned int value) -> unsigned int {
    // Set the counter value.
    if (value < this->maxValue) {
        this->counter = value;
    }

    return this->counter;
}

auto Counter::preset() -> unsigned int {
    // Set the counter to the preset value.
    this->counter = this->presetValue - 1;
    return this->counter;
}

auto Counter::setPreset(unsigned int presetValue) -> unsigned int {
    // Set a new preset value.
    this->presetValue = presetValue;
    return this->presetValue;
}

auto Counter::step() -> unsigned int {
    // Trigger next step.
    if (this->dir == Counter::Direction::FORWARD) {
        return this->forward();
    }

    return this->back();
}

auto Counter::enable() -> Status {
    // Enable or disable the output.
    this->isEnabled = this->isEnabled == Status::ENABLED ? Status::DISABLED : Status::ENABLED;
    return this->isEnabled;
}

auto Counter::setMaxValue(unsigned int maxValue) -> unsigned int {
    // Set the maximum value for the counter.
    this->maxValue = maxValue;
    return this->maxValue;
}
