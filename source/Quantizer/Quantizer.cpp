#include "Quantizer.hpp"
#include <iostream>

Quantizer::Quantizer() {
    this->clear();
    this->range_low = MIDI::RANGE_LOW;
    this->range_high = MIDI::RANGE_HIGH;
}

auto Quantizer::quantize(int noteValue) -> int {
    // Check if the note is in range.
    if (noteValue < MIDI::RANGE_LOW || noteValue > MIDI::RANGE_HIGH) {
        return INVALID_NOTE;
    }

    // If current note is already a valid key.
    if (this->note_count == 0 || this->keyboard[noteValue]) {
        return noteValue;
    }

    // Return the original note if
    // the quantizer is disabled.
    if (!this->quantize_on) {
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

auto Quantizer::getNote(int noteValue) -> Quantizer::Note {
    // Check if the note has been set.
    if (noteValue < 0 || noteValue >= MIDI::KEYBOARD_SIZE) {
        return Note::OFF;
    }

    return this->keyboard[noteValue] ? Note::ON : Note::OFF;
}

auto Quantizer::addNote(int noteValue) -> int {
    // Add note to the keyboard.

    // Return error if the note value is out of range.
    if (noteValue <= 0 || noteValue > MIDI::KEYBOARD_SIZE || this->note_count >= MIDI::KEYBOARD_SIZE) {
        return -1;
    }

    // Depending on the mode add the note or notes to the keyboard.
    if (this->mode == QuantizeMode::TWELVE_NOTES) {
        int degree = noteValue % MIDI::OCTAVE;

        // Add this note degree in every octave
        for (int octave = 0; octave <= MIDI::KEYBOARD_OCTAVES; octave++) {
            int current_note = (MIDI::OCTAVE * octave) + degree;
            if (current_note < MIDI::KEYBOARD_SIZE && !this->keyboard[current_note]) {
                this->keyboard[current_note] = true;
                this->note_count++;
            } else {
                return -1;
            }
        }
    } else if (this->mode == QuantizeMode::ALL_NOTES) {
        // ALL_NOTES mode - just add the single note
        this->keyboard[noteValue] = true;
        this->note_count++;
    } else {
        return -1;
    }

    this->currentNoteHigh = std::max(this->currentNoteHigh, noteValue);
    this->currentNoteLow = std::min(this->currentNoteLow, noteValue);

    return 0;
}

auto Quantizer::deleteNote(int noteValue) -> int {
    if (noteValue < 0 || noteValue > MIDI::RANGE_HIGH || this->note_count <= 0) {
        return -1;
    }

    if (this->mode == QuantizeMode::ALL_NOTES) {
        this->keyboard[noteValue] = false;
        this->note_count--;
    } else if (this->mode == QuantizeMode::TWELVE_NOTES) {
        
        for (int i = noteValue % MIDI::OCTAVE; i < MIDI::KEYBOARD_SIZE; i = i +  MIDI::OCTAVE){
            this->keyboard[i] = false;
            this->note_count--;
        }
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
