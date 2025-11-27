#include "Quantizer.hpp"
#include "QuantizerMisc.cpp"
#include "QuantizerStrategies.cpp"

#include <iostream>

Quantizer::Quantizer() {
    this->clear();
    this->range_low = MIDI::RANGE_LOW;
    this->range_high = MIDI::RANGE_HIGH;
}

auto Quantizer::quantize(int noteValue) -> int {
    // Check if the note is in range.
    if ((noteValue < MIDI::RANGE_LOW) || (noteValue >= MIDI::KEYBOARD_SIZE)) {
        return INVALID_NOTE;
    }

    // If current note is already a valid key.
    if ((this->note_count == 0) || this->keyboard[noteValue]) {
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

auto Quantizer::getNote(int noteValue) -> Quantizer::Note {
    // Check if the note has been set.
    if ((noteValue < 0) || (noteValue >= MIDI::KEYBOARD_SIZE)) {
        return Note::OFF;
    }

    return this->keyboard[noteValue] ? Note::ON : Note::OFF;
}

auto Quantizer::addNote(int noteValue) -> int {
    // Add note to the keyboard.

    // Return error if the note value is out of range.
    if ((noteValue < 0) || (noteValue > MIDI::KEYBOARD_SIZE) || (this->note_count >= MIDI::KEYBOARD_SIZE)) {
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
