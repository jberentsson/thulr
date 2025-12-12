#include <memory>
#include <algorithm>
#include <iterator>
#include <vector>
#include "Utils/MIDI.hpp"
#include "Chords.hpp"

Chords::Chords() {
    this->noteMode_ = NoteMode::RETRIGGER;

    for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
        this->keyboard_.push_back(std::make_shared<Key>(Key(i)));
        this->noteCount_[i] = 0;
    }
}

auto Chords::reciveNotes() -> bool {
    this->isRecievingNotes_ = true;
    return this->isRecievingNotes_;
}

auto Chords::note(int pitchValue, int velocityValue) -> int {
    // Handles note input.

    if (this->isRecievingNotes_) {
        // Finish if this is the active note off message.
        if ((velocityValue == 0) && 
            (this->activeKey_ == pitchValue) &&
            this->keyboard_[pitchValue]->notes().empty()) {
            return 0;
        }

        // Add a note to a key.
        if (velocityValue > 0) {
            this->addChordNote(pitchValue, velocityValue);
        } else {
            this->releaseChordNote(pitchValue, 0);
        }
    } else if (!keyboard_[pitchValue]->notes().empty()) {
        // Play the notes that have been assigned to a key.
        this->playNotes(pitchValue, velocityValue);
    }
    
    return 0;
}

auto Chords::addChordNote(int pitchValue, int velocityValue) -> int {
    if (0 >= pitchValue && pitchValue > MIDI::KEYBOARD_SIZE) {
        return 1;
    }

    if (this->activeKey_ < 0) {
        // Here we choose which key we want to assign the notes to.
        this->activeKey_ = pitchValue;

        // Clear any old notes from the key.
        if (!this->keyboard_.at(pitchValue)->notes().empty()) {
            this->keyboard_.at(pitchValue)->notes().clear();
        }
    } else if (pitchValue >= 0 && pitchValue < (MIDI::KEYBOARD_SIZE - 1)){
        // The rest of the notes are assigned to the key.
        this->keyboard_.at(this->activeKey_)->add(pitchValue, velocityValue);

        // Store the notes untill we have released them all.
        this->activeNotes_[pitchValue]++;
    }

    return 0;
}

auto Chords::removeFromActive(int pitchValue) -> int {
    int count = this->activeNotes_[pitchValue]--;
    return std::max(count, 0);
}

auto Chords::releaseChordNote(int pitchValue, int velocityValue) -> int { // NOLINT
    // Here we remove the released note from active notes vector.
    this->removeFromActive(pitchValue);

    if (this->activeNotes_[pitchValue] == 0) {
        // When all of the notes have been released we quit the recording mode.
        this->isRecievingNotes_ = false;
        this->activeKey_ = -1;
    }

    return 0;
}

auto Chords::playNotes(int pitchValue, int velocityValue) -> int { // NOLINT
    if (!this->keyboard_[pitchValue]->notes().empty()) {
        const auto& sourceNotes = this->keyboard_[pitchValue]->notes();
        
        // Calculate total capacity needed.
        size_t notesToAdd = 0;

        for (const auto& currentNote : sourceNotes) {
            bool isNoteOn = (velocityValue > 0);
            bool isNoteHeld = (this->noteCount_[currentNote->pitch()] > 0);
            
            if (isNoteOn) {
                if (this->noteMode_ == NoteMode::RETRIGGER || 
                   (this->noteMode_ == NoteMode::LEGATO && !isNoteHeld)) {
                    notesToAdd++;
                }
            } else if (isNoteHeld) {
                // NOTE OFF and note was held.
                notesToAdd++;
            }
        }
        
        if (notesToAdd > 0) {
            this->noteQueue_.reserve(this->noteQueue_.size() + notesToAdd);
        }

        for(const auto& currentNote : sourceNotes) {
            bool isNoteOn = (velocityValue > 0);
            bool isNoteOff = (velocityValue == 0);
            bool wasNoteHeld = (this->noteCount_[currentNote->pitch()] > 0);

            if (isNoteOff) {
                // Only send NOTE OFF if note was actually held.
                if (wasNoteHeld && this->noteMode_ != Chords::NoteMode::LEGATO) {
                    this->queueNote(currentNote->pitch(), 0);
                    this->noteCount_[currentNote->pitch()]--;
                }
            } else if (isNoteOn) {
                // Update count first.
                this->noteCount_[currentNote->pitch()]++;
                
                if (this->noteMode_ == NoteMode::LEGATO) {
                    // Legato: Only trigger if note wasn't already held.
                    if (!wasNoteHeld) {
                        this->queueNote(currentNote->pitch(), velocityValue);
                    }
                } else if (this->noteMode_ == NoteMode::RETRIGGER) {
                    // Retrigger: Always trigger NOTE ON.
                    this->queueNote(currentNote->pitch(), velocityValue);
                }
            }
        }
    }

    return 0;
}

auto Chords::queueNote(int noteValue, int velocityValue) -> void {
    this->noteQueue_.push_back(std::make_shared<MIDI::Note>(MIDI::Note(noteValue, velocityValue)));
}

auto Chords::setNoteMode(NoteMode mode) -> NoteMode {
    this->noteMode_ = mode;
    return this->noteMode_;
}

auto Chords::setNoteOrder(NoteOrder order) -> NoteOrder {
    this->noteOrder_ = order;
    return this->noteOrder_;
}

auto Chords::clear() -> void {
    // Clear all notes from every key.
    for (const auto &currentNote : this->keyboard_) {
        currentNote->clear();
    }
}

auto Chords::clear(int noteValue) -> void {
    // Clear notes from a specific key.
    this->keyboard_[noteValue]->clear();
}
