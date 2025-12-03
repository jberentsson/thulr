#include "Quantizer.hpp"
#include "Utils/MIDI.hpp"

auto Quantizer::round(MIDI::Note noteValue) -> int {
    // Round depending on what rounding mode is active.
    switch(this->roundDirection_){
        case RoundDirection::UP:
            return this->roundUp(noteValue);
        case RoundDirection::DOWN:
            return this->roundDown(noteValue);
        case RoundDirection::DOWN_UP:
            return this->roundDownUp(noteValue);
        case RoundDirection::UP_DOWN:
            return this->roundUpDown(noteValue);
        case RoundDirection::NEAREST:
            return this->roundNearest(noteValue);
        case RoundDirection::FURTHEST:
            return this->roundFurthest(noteValue);
        case RoundDirection::UP_OVERFLOW:
            return this->roundUpOverflow(noteValue);
        case RoundDirection::DOWN_UNDERFLOW:
            return this->roundDownUnderflow(noteValue);
        default:
            return MIDI::INVALID_NOTE;
    }
}

auto Quantizer::roundUp(MIDI::Note noteValue) -> int {
    // Search up from n.
    for (int i = noteValue + 1; i < MIDI::KEYBOARD_SIZE; i++) {
        // We have reached the high limit.
        if (i > this->rangeHigh_) {
            return this->roundDown(noteValue);
        }

        // We have found the correct note.
        int index = this->keyboardIndex();

        if (this->keyboard_[index][i].valid()) {
            return i;
        }
    }

    return MIDI::INVALID_NOTE;
}

auto Quantizer::roundDown(MIDI::Note noteValue) -> int {
    // Search down from n.
    for (int i = noteValue - 1; i >= 0; i--) {
        // We have reached the low limit.
        if (i < this->rangeLow_) {
            return this->roundUp(noteValue);
        }

        // We have found the correct note.
        int index = this->keyboardIndex();
        if (this->keyboard_[index][i].valid()) {
            return i;
        }
    }

    return MIDI::INVALID_NOTE;
}

auto Quantizer::roundUpDown(MIDI::Note noteValue) -> int {
    int upValue = this->roundUp(noteValue);

    if (upValue == MIDI::INVALID_NOTE){
        return this->roundDown((MIDI::Note) MIDI::Notes::NoteG10);
    }

    return upValue;
}

auto Quantizer::roundDownUp(MIDI::Note noteValue) -> int {
    int upValue = this->roundDown(noteValue);

    if (upValue == MIDI::INVALID_NOTE){
        return this->roundUp((MIDI::Note) MIDI::Notes::NoteC0);
    }

    return upValue;
}

auto Quantizer::roundUpOverflow(MIDI::Note noteValue) -> int {
    int roundedUp = this->roundUp(noteValue);

    if (roundedUp == MIDI::INVALID_NOTE) {
        // No valid note above so se start from the lowest note.
        return this->roundUp((MIDI::Note) this->rangeLow_);
    }

    return roundedUp;
}

auto Quantizer::roundDownUnderflow(MIDI::Note noteValue) -> int {
    int roundedUp = this->roundDown(noteValue);

    if (roundedUp == MIDI::INVALID_NOTE) {
        // No valid note below so se start from the highest note.
        return this->roundDown((MIDI::Note) this->rangeHigh_);
    }

    return roundedUp;
}

auto Quantizer::roundNearest(MIDI::Note noteValue) -> int {
    int nearestUp = this->roundUp(noteValue);
    int nearestDown = this->roundDown(noteValue);
    
    if (nearestUp == MIDI::INVALID_NOTE && nearestDown == MIDI::INVALID_NOTE) {
        // No valid notes.
        return MIDI::INVALID_NOTE;
    }
    if (nearestUp == MIDI::INVALID_NOTE) {
        // Down is only valid.
        return nearestDown;
    }
    if (nearestDown == MIDI::INVALID_NOTE) {
        // Up is only valid.
        return nearestUp;
    }
    
    // Both directions are valid, choose the closest one
    int deltaUp = nearestUp - noteValue;
    int deltaDown = noteValue - nearestDown;

    return (deltaDown <= deltaUp) ? nearestDown : nearestUp;
}

auto Quantizer::roundFurthest(MIDI::Note noteValue) -> int {
    // Round to the furthest note value.
    int nearestUp = this->roundUp(noteValue);
    int nearestDown = this->roundDown(noteValue);
    
    int deltaUp = nearestUp - noteValue;
    int deltaDown = noteValue - nearestDown;

    if (deltaDown < deltaUp) {
        return nearestUp;
    }

    return nearestDown;
}
