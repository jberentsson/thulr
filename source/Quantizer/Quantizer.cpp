#include "Quantizer.hpp"
#include "QuantizerMisc.cpp" // NOLINT
#include "QuantizerStrategies.cpp" // NOLINT

Quantizer::Quantizer() {
    this->clear();
    this->rangeLow_ = MIDI::Note(MIDI::RANGE_LOW);
    this->rangeHigh_ = MIDI::Note(MIDI::RANGE_HIGH);
}

auto Quantizer::quantize(MIDI::Note noteValue) -> int {
    // Check if the note is in range.

    if (noteValue < this->rangeLow_) {
        noteValue = this->rangeLow_;
    }

    if (noteValue > this->rangeHigh_) {
        noteValue = this->rangeHigh_;
    }

    bool noteFound = false;
    int index = static_cast<int>(QuantizeMode::ALL_NOTES);

    // If current note is already a valid key.
    if ((this->noteCount_[index] == 0) || this->keyboard_[index][noteValue] != MIDI::Note(MIDI::INVALID_NOTE)) {
        noteFound = true;
    }

    // Return the original note if
    // the quantizer is disabled.
    if (this->quantizeEnabled_ == QuantizeEnable::OFF) {
        noteFound = true;
    }

    // Round if we have not found the value else return original value.
    int quantizedNoteValue = noteFound ? noteValue : this->round(noteValue);

    std::shared_ptr<Note> quantizedNote = std::make_shared<Note>(
        static_cast<uint8_t>(quantizedNoteValue)
    );

    this->lastNote_ = quantizedNote;

    return quantizedNoteValue;
}

auto Quantizer::getNote(MIDI::Note noteValue) -> MIDI::Note {
    // Check if the note has been set.
    if ((noteValue < 0) || (noteValue >= MIDI::KEYBOARD_SIZE)) {
        return MIDI::Note(MIDI::INVALID_NOTE);
    }

    return this->keyboard_[static_cast<int>(this->mode_)][noteValue];
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

    // TODO: Here we are comparing some weird values.
    this->currentNoteHigh_ = std::max(this->currentNoteHigh_, noteValue);
    this->currentNoteLow_ = std::min(this->currentNoteLow_, noteValue);
    
    return 0;
}

auto Quantizer::deleteNote(MIDI::Note noteValue) -> int {
    // Delete a note from the quantizer.

    if (!noteValue.valid()) {
        return -1;
    }

    // TWELVE_NOTES
    for (int i = MIDI::getPitchClass(noteValue); i < MIDI::KEYBOARD_SIZE; i = i +  MIDI::OCTAVE){
        this->keyboard_[static_cast<int>(QuantizeMode::TWELVE_NOTES)][i] = MIDI::Note(MIDI::INVALID_NOTE);
        this->noteCount_[static_cast<int>(QuantizeMode::TWELVE_NOTES)]--;
    }

    // ALL_NOTES
    this->keyboard_[static_cast<int>(QuantizeMode::ALL_NOTES)][noteValue] = MIDI::Note(MIDI::INVALID_NOTE);
    this->noteCount_[static_cast<int>(QuantizeMode::ALL_NOTES)]--;

    return 0;
}

auto Quantizer::addTwelveNotes(MIDI::Note noteValue) -> int {
    // TWELVE_NOTES mode - add a note in every octave.

    // Add this note degree in every octave.
    for (int octave = 0; octave <= MIDI::KEYBOARD_OCTAVES; octave++) {
        int currentNote = (MIDI::OCTAVE * octave) + MIDI::getPitchClass(noteValue);

        if (currentNote < MIDI::KEYBOARD_SIZE && 
            !this->keyboard_[static_cast<int>(QuantizeMode::TWELVE_NOTES)][currentNote].valid()) {
            this->keyboard_[static_cast<int>(QuantizeMode::TWELVE_NOTES)][currentNote] = MIDI::Note(currentNote);
            this->noteCount_[static_cast<int>(QuantizeMode::TWELVE_NOTES)]++;
        }
    }

    return 0;
}

auto Quantizer::addAllNotes(MIDI::Note noteValue) -> int {
    // ALL_NOTES mode - just add the single note.
    this->keyboard_[static_cast<int>(QuantizeMode::ALL_NOTES)][(uint8_t) noteValue] = noteValue;
    this->noteCount_[static_cast<int>(QuantizeMode::ALL_NOTES)]++;
    
    return 0;
}

auto Quantizer::setThrough(NoteThrough throughValue) -> NoteThrough {
    // Enable/Disable note through.
    this->noteThrough_ = throughValue;
    return this->noteThrough_;
};