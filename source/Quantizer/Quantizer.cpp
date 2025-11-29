#include "Quantizer.hpp"
#include "QuantizerMisc.cpp" // NOLINT
#include "QuantizerStrategies.cpp" // NOLINT

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

    int index = static_cast<int> (QuantizeMode::ALL_NOTES);

    // If current note is already a valid key.
    if ((this->note_count[index] == 0) || this->keyboard[index][noteValue]) {
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

    return this->keyboard[static_cast<int> (this->mode)][noteValue] ? Note::ON : Note::OFF;
}

auto Quantizer::addNote(int noteValue) -> int {
    // Add note to the keyboard.

    // Return error if the note value is out of range.
    if ((noteValue < 0) || (noteValue > MIDI::KEYBOARD_SIZE) || (this->note_count[0] >= MIDI::KEYBOARD_SIZE) || (this->note_count[1] >= MIDI::KEYBOARD_SIZE)) {
        return -1;
    }

    // Add to each of the modes.
    this->addAllNotes(noteValue);
    this->addTwelveNotes(noteValue);

    this->currentNoteHigh = std::max(this->currentNoteHigh, noteValue);
    this->currentNoteLow = std::min(this->currentNoteLow, noteValue);

    return 0;
}

auto Quantizer::deleteNote(int noteValue) -> int {
    if (noteValue < 0 || noteValue > MIDI::RANGE_HIGH || this->note_count[static_cast<int> (QuantizeMode::ALL_NOTES)] <= 0 || this->note_count[static_cast<int> (QuantizeMode::TWELVE_NOTES)] <= 0) {
        return -1;
    }

    // TWELVE_NOTES
    for (int i = noteValue % MIDI::OCTAVE; i < MIDI::KEYBOARD_SIZE; i = i +  MIDI::OCTAVE){
        this->keyboard[static_cast<int> (QuantizeMode::TWELVE_NOTES)][i] = false;
        this->note_count[static_cast<int> (QuantizeMode::TWELVE_NOTES)]--;
    }

    // ALL_NOTES
    this->keyboard[static_cast<int> (QuantizeMode::ALL_NOTES)][noteValue] = false;
    this->note_count[static_cast<int> (QuantizeMode::ALL_NOTES)]--;

    return 0;
}

auto Quantizer::addTwelveNotes(int noteValue) -> int {
    // TWELVE_NOTES mode - add a note in every octave
    int degree = noteValue % MIDI::OCTAVE;

    // Add this note degree in every octave
    for (int octave = 0; octave <= MIDI::KEYBOARD_OCTAVES; octave++) {
        int current_note = (MIDI::OCTAVE * octave) + degree;

        if (current_note < MIDI::KEYBOARD_SIZE && 
            !this->keyboard[static_cast<int> (QuantizeMode::TWELVE_NOTES)][current_note]) {
            this->keyboard[static_cast<int> (QuantizeMode::TWELVE_NOTES)][current_note] = true;
            this->note_count[static_cast<int> (QuantizeMode::TWELVE_NOTES)]++;
        }
    }

    return 0;
}

auto Quantizer::addAllNotes(int noteValue) -> int {
    // ALL_NOTES mode - just add the single note
    this->keyboard[static_cast<int> (QuantizeMode::ALL_NOTES)][noteValue] = true;
    this->note_count[static_cast<int> (QuantizeMode::ALL_NOTES)]++;
    return 0;
}
