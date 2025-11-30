#include "Quantizer.hpp"
#include "QuantizerMisc.cpp" // NOLINT
#include "QuantizerStrategies.cpp" // NOLINT

Quantizer::Quantizer() {
    this->clear();
    this->range_low = MIDI::Note(MIDI::RANGE_LOW);
    this->range_high = MIDI::Note(MIDI::RANGE_HIGH);
}

auto Quantizer::quantize(MIDI::Note noteValue) -> int {
    // Check if the note is in range.

    if (noteValue < this->range_low) {
        noteValue = this->range_low;
    }

    if (noteValue > this->range_high) {
        noteValue = this->range_high;
    }

    bool noteFound = false;
    int index = static_cast<int> (QuantizeMode::ALL_NOTES);

    // If current note is already a valid key.
    if ((this->note_count[index] == 0) || this->keyboard[index][noteValue]) {
        noteFound = true;
    }

    // Return the original note if
    // the quantizer is disabled.
    if (!this->quantize_on) {
        noteFound = true;
    }

    // Round if we have not found the value else return original value.
    int quantizedNoteValue = noteFound ? noteValue : this->round(noteValue);

    std::shared_ptr<Note> quantizedNote = std::make_shared<Note>(
        static_cast<uint8_t>(quantizedNoteValue)
    );

    this->lastNote = quantizedNote;

    return quantizedNoteValue;
}

auto Quantizer::getNote(MIDI::Note noteValue) -> Quantizer::NoteData {
    // Check if the note has been set.
    if ((noteValue < 0) || (noteValue >= MIDI::KEYBOARD_SIZE)) {
        return NoteData::OFF;
    }

    return this->keyboard[static_cast<int> (this->mode)][noteValue] ? NoteData::ON : NoteData::OFF;
}

auto Quantizer::addNote(MIDI::Note noteValue) -> int {
    // Add note to the keyboard.

    // Return error if the note value is out of range.
    if (!noteValue.valid()) {
        return -1;
    }

    // Add to each of the modes.
    this->addAllNotes(noteValue);
    this->addTwelveNotes(noteValue);

    this->currentNoteHigh = std::max(this->currentNoteHigh, noteValue);
    this->currentNoteLow = std::min(this->currentNoteLow, noteValue);

    return 0;
}

auto Quantizer::deleteNote(MIDI::Note noteValue) -> int {
    // Delete a note from the quantizer.

    if (!noteValue.valid()) {
        return -1;
    }

    // TWELVE_NOTES
    for (int i = (uint8_t) noteValue % MIDI::OCTAVE; i < MIDI::KEYBOARD_SIZE; i = i +  MIDI::OCTAVE){
        this->keyboard[static_cast<int> (QuantizeMode::TWELVE_NOTES)][i] = false;
        this->note_count[static_cast<int> (QuantizeMode::TWELVE_NOTES)]--;
    }

    // ALL_NOTES
    this->keyboard[static_cast<int> (QuantizeMode::ALL_NOTES)][noteValue] = false;
    this->note_count[static_cast<int> (QuantizeMode::ALL_NOTES)]--;

    return 0;
}

auto Quantizer::addTwelveNotes(MIDI::Note noteValue) -> int {
    // TWELVE_NOTES mode - add a note in every octave.
    int degree = (uint8_t) noteValue % MIDI::OCTAVE;

    // Add this note degree in every octave.
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

auto Quantizer::addAllNotes(MIDI::Note noteValue) -> int {
    // ALL_NOTES mode - just add the single note.

    this->keyboard[static_cast<int> (QuantizeMode::ALL_NOTES)][(uint8_t) noteValue] = true;
    this->note_count[static_cast<int> (QuantizeMode::ALL_NOTES)]++;
    
    return 0;
}
