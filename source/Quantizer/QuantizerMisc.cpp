#include "Quantizer.hpp"
auto Quantizer::disable() -> bool {
    // Disable the quantizer.
    this->quantize_on = false;
    return this->quantize_on;
}

auto Quantizer::enable() -> bool {
    // Enable the quantizer.
    this->quantize_on = true;
    return this->quantize_on;
}

auto Quantizer::setRange(int rangeLow, int rangeHigh) -> int {
    // Set the quantizer output range.
    if (rangeLow >= 0 && rangeLow < MIDI::KEYBOARD_SIZE && rangeHigh >= 0 && rangeHigh < MIDI::KEYBOARD_SIZE && rangeLow <= rangeHigh) {
        this->range_low = rangeLow;
        this->range_high = rangeHigh;
        return 0;
    }

    return -1;
}

auto Quantizer::clear() -> int {
    // Clear all set notes from the keyboard.
    int index = 0;

    for (auto &key : this->keyboard) {
        key = false;
        index++;
    }

    return 0;
}

auto Quantizer::setRoundDirection(RoundDirection direction) -> RoundDirection {
    this->round_direction = direction;
    return this->round_direction;
}

auto Quantizer::setMode(QuantizeMode mode) -> QuantizeMode {
    this->mode = mode;
    return this->mode;
}

auto Quantizer::enableThrough() -> bool {
    this->noteThrough = false;
    return this->noteThrough;
}

auto Quantizer::disableThrough() -> bool {
    this->noteThrough = false;
    return this->noteThrough;
}
