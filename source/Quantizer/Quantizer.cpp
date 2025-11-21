#include "Quantizer.hpp"
#include <iostream>

Quantizer::Quantizer() {
    this->clear();
    this->range_low = 0;
    this->range_high = MIDI::KEYBOARD_SIZE - 1;
}

auto Quantizer::quantize(int noteValue) -> int {
    // Check if the note is in range.
    if (noteValue < MIDI::RANGE_LOW || noteValue > MIDI::RANGE_HIGH) {
        return INVALID_NOTE;
    }
    
    // If current note is already a valid key.
    if (this->keyboard[noteValue] == true) {
        std::cout << "Note is already a valid key. note:" << noteValue <<"\n";
        return noteValue;
    }
    
    // Return the rounded value.
    return this->round(noteValue);
}

auto Quantizer::round(int noteValue) -> int {
    // Round depending on what rounding mode is active.
    if (this->round_direction == RoundDirection::UP) {
        return this->roundUp(noteValue);
    }
    
    return this->roundDown(noteValue);
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

    // We have found the correct note.
    if (this->keyboard[i]) {
      return i;
    }
  }

  return INVALID_NOTE;
}

auto Quantizer::roundDown(int noteValue) -> int{
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
    
    for(int k = 0; k < MIDI::KEYBOARD_SIZE; k++) {
        this->keyboard[k] = false;
        index++;
    }

    std::cout << "CLEAR keyboard.size() " << index << "\n";

    return 0;
}

auto Quantizer::getNote(int noteValue) -> Quantizer::Note {
    // Check if the note has been set.
    if (noteValue < 0 || noteValue >= MIDI::KEYBOARD_SIZE) {
        return Note::OFF;
    }

    return this->keyboard[noteValue] ? Note::ON : Note::OFF;
}

auto Quantizer::addNote(int noteValue) -> int {
    // Add note to the keyboard.

    std::cout << "adding note: " << noteValue << "\n";

    // Return error if the note value is out of range.
    if (noteValue < 0 || noteValue >= MIDI::KEYBOARD_SIZE) {
        return -1;
    }

    // Depending on the mode add the note or notes to the keyboard.
    if (this->mode == QuantizeMode::TWELVE_NOTES) {
        int degree = noteValue % MIDI::OCTAVE_SIZE;
        
        // Add this note degree in every octave
        for (int octave = 0; octave < MIDI::KEYBOARD_OCTAVES; octave++) {
            int current_note = (MIDI::OCTAVE_SIZE * octave) + degree;
            if (current_note < MIDI::KEYBOARD_SIZE) {
                this->keyboard[current_note] = true;
                this->note_count++;
            }
        }
    } else {
        // ALL_NOTES mode - just add the single note
        this->keyboard[noteValue] = true;
        this->note_count++;
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
