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

auto Quantizer::setRange(int rangeLow, int rangeHigh) -> int {
    // Set the quantizer output range.
    if (rangeLow >= 0 && rangeLow < MIDI::KEYBOARD_SIZE && rangeHigh >= 0 && rangeHigh < MIDI::KEYBOARD_SIZE && rangeLow <= rangeHigh) {
        this->rangeLow_ = rangeLow;
        this->rangeHigh_ = rangeHigh;
        return 0;
    }

    return -1;
}

auto Quantizer::clear() -> int {
    // Clear all set notes from the keyboard.
    for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
        this->keyboard_[0][i] = MIDI::Note(MIDI::INVALID_NOTE);
        this->keyboard_[1][i] = MIDI::Note(MIDI::INVALID_NOTE);
    }

    return 0;
}

auto Quantizer::setRoundDirection(RoundDirection direction) -> RoundDirection {
    this->roundDirection_ = direction;
    return this->roundDirection_;
}

auto Quantizer::setMode(QuantizeMode mode) -> QuantizeMode {
    this->mode_ = mode;
    return this->mode_;
}

auto Quantizer::modeTwelveNotes() -> bool {
    this->mode_ = QuantizeMode::TWELVE_NOTES;
    return true;
}

auto Quantizer::modeAllNotes() -> bool {
    this->mode_ = QuantizeMode::ALL_NOTES;
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
    return static_cast<int>(this->mode_);
}

auto Quantizer::noteCount() -> int {
    int index = keyboardIndex();
    return this->noteCount_[index];
}
