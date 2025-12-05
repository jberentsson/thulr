#include "Quantizer.hpp"
#include "QuantizerMisc.cpp" // NOLINT
#include "QuantizerStrategies.cpp" // NOLINT

Quantizer::Quantizer() {
    this->rangeLow_ = MIDI::Note(MIDI::RANGE_LOW);
    this->rangeHigh_ = MIDI::Note(MIDI::RANGE_HIGH);
    this->clear();
}

auto Quantizer::mutePreviousNote() -> void {
    if (!this->lastNote_) {
        return;
    }
    
    int lastPitch = this->lastNote_->pitch();
    int index = this->keyboardIndex();
    
    if (lastPitch >= 0 && lastPitch < MIDI::KEYBOARD_SIZE) {
        // Mark as released on keyboard.
        this->keyboard_[index][lastPitch] = MIDI::Note(lastPitch, 0);
    }
}

auto Quantizer::quantize(MIDI::Note noteValue) -> int {
    // Quantize a note.

    // 1. Mute previous note if needed.
    if (this->lastNote_ && this->lastNote_->pitch() != noteValue.pitch()) {
        this->mutePreviousNote();
    }
    
    // 2. Get pitch and clamp to range.
    int notePitch = noteValue.pitch();
    
    // 3. Fit the note inside the range.
    notePitch = std::max(notePitch, (int) this->rangeLow_);
    notePitch = std::min(notePitch, (int) this->rangeHigh_);
    
    // 4. Check if this exact pitch exists in keyboard.
    bool noteFound = false;
    int index = this->keyboardIndex();
    
    if (this->keyboard_[index][notePitch].valid()) {
        noteFound = true;
    }
    
    // 5. If quantizer is disabled, return the clamped input.
    if (this->quantizeEnabled_ == QuantizeEnable::OFF) {
        noteFound = true;
    }
    
    // 6. Check if note is released (velocity 0).
    MIDI::Note currentNote = this->keyboard_[index][notePitch];
    if (currentNote.valid() && currentNote.velocity() == 0) {
        noteFound = true;
    }
    
    // 7. Round if needed.
    int quantizedPitch = noteFound ? notePitch : this->round(MIDI::Note(notePitch));
    
    // 8. If round() failed, return the clamped input.
    if (quantizedPitch == MIDI::INVALID_NOTE || quantizedPitch < 0 || quantizedPitch >= MIDI::KEYBOARD_SIZE) {
        quantizedPitch = notePitch;
    }
    
    // 9. Create and store the new note.
    uint8_t originalVelocity = noteValue.velocity();
    this->lastNote_ = std::make_shared<Note>(static_cast<uint8_t>(quantizedPitch), originalVelocity);
    
    // 10. Update keyboard.
    this->keyboard_[index][quantizedPitch] = MIDI::Note(quantizedPitch, originalVelocity);
    
    return quantizedPitch;
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

    // Update the high and lowest current notes..
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