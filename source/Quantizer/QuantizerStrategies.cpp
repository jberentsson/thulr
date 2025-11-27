#include "Quantizer.hpp"
#include "Utils/MIDI.hpp"

#include <iostream>

auto Quantizer::round(int noteValue) -> int {
    // Round depending on what rounding mode is active.

    switch(this->round_direction){
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
            return INVALID_NOTE;
    }
}

auto Quantizer::roundUp(int noteValue) -> int {
    // Search up from n.
    for (int i = noteValue + 1; i < MIDI::KEYBOARD_SIZE; i++) {
        // We have reached the high limit.
        if (i > this->range_high) {
            return this->roundDown(noteValue);
        }

        // We have found the correct note.
        if (this->keyboard[i]) {
            return i;
        }
    }

    return INVALID_NOTE;
}

auto Quantizer::roundDown(int noteValue) -> int {
    // Search down from n.
    for (int i = noteValue - 1; i >= 0; i--) {
        // We have reached the low limit.
        if (i < this->range_low) {
            return this->roundUp(noteValue);
        }

        // We have found the correct note.
        if (this->keyboard[i]) {
            return i;
        }
    }

    return INVALID_NOTE;
}

auto Quantizer::roundUpDown(int noteValue) -> int {
    int upValue = this->roundUp(noteValue);

    if (upValue == INVALID_NOTE){
        return this->roundDown(MIDI::Notes::NoteG10);
    }

    return upValue;
}

auto Quantizer::roundDownUp(int noteValue) -> int {
    int upValue = this->roundDown(noteValue);

    if (upValue == INVALID_NOTE){
        return this->roundUp(MIDI::Notes::NoteC0);
    }

    return upValue;
}

auto Quantizer::roundUpOverflow(int noteValue) -> int {
    int roundedUp = this->roundUp(noteValue);

    if (roundedUp == INVALID_NOTE) {
        // No valid note above so se start from the lowest note.
        return this->roundUp(this->range_low);
    }

    return roundedUp;
}

auto Quantizer::roundDownUnderflow(int noteValue) -> int {
    int roundedUp = this->roundDown(noteValue);

    if (roundedUp == INVALID_NOTE) {
        // No valid note below so se start from the highest note.
        return this->roundDown(this->range_high);
    }

    return roundedUp;
}

auto Quantizer::roundNearest(int noteValue) -> int {
    int nearestUp = this->roundUp(noteValue);
    int nearestDown = this->roundDown(noteValue);
    
    if (nearestUp == INVALID_NOTE && nearestDown == INVALID_NOTE) {
        // No valid notes.
        return INVALID_NOTE;
    }
    if (nearestUp == INVALID_NOTE) {
        // Down is only valid.
        return nearestDown;
    }
    if (nearestDown == INVALID_NOTE) {
        // Up is only valid.
        return nearestUp;
    }
    
    // Both directions are valid, choose the closest one
    int deltaUp = nearestUp - noteValue;
    int deltaDown = noteValue - nearestDown;

    return (deltaDown <= deltaUp) ? nearestDown : nearestUp;
}

auto Quantizer::roundFurthest(int noteValue) -> int {
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
