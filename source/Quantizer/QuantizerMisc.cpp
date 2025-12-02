#include "Quantizer.hpp"

auto Quantizer::disable() -> QuantizeEnable {
    // Disable the quantizer.
    this->quantizeEnabled_ = QuantizeEnable::OFF;
    return this->quantizeEnabled_;
}

auto Quantizer::enable() -> QuantizeEnable {
    // Enable the quantizer.
    this->quantizeEnabled_ = QuantizeEnable::ON;
    return this->quantizeEnabled_;
}

auto Quantizer::setRange(MIDI::Note rangeLow, MIDI::Note rangeHigh) -> int {
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
    for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
        this->keyboard[0][i] = false;
        this->keyboard[1][i] = false;
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

auto Quantizer::modeTwelveNotes() -> bool {
    this->mode = QuantizeMode::TWELVE_NOTES;
    return true;
}

auto Quantizer::modeAllNotes() -> bool {
    this->mode = QuantizeMode::ALL_NOTES;
    return true;
}

auto Quantizer::enableThrough() -> NoteThrough {
    this->noteThrough_ = NoteThrough::ON;
    return this->noteThrough_;
}

auto Quantizer::disableThrough() -> NoteThrough {
    this->noteThrough_ = NoteThrough::OFF;
    return this->noteThrough_;
}

auto Quantizer::keyboardIndex() -> int {
    return static_cast<int> (this->mode);
}

auto Quantizer::noteCount() -> int {
    int index = keyboardIndex();
    return this->note_count[index];
}
