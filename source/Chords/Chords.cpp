#include <memory>
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
    if (this->isRecievingNotes_) {
        if (this->activeKey_ < MIDI::RANGE_LOW) {
            // Active key ON.
            this->setActiveKey(pitchValue);
        } else if (pitchValue == this->activeKey_ && 
                   velocityValue == 0 && 
                   this->keyboard_[pitchValue]->notes().empty()) {
            // Active key OFF.
        } else {
            this->chordNote(pitchValue, velocityValue);
        }
    } else if (!this->keyboard_[pitchValue]->notes().empty()) {
        // Play chord if key has one recorded.
        this->playNotes(pitchValue, velocityValue);
    }

    return 0;
}

auto Chords::chordNote(int pitchValue, int velocityValue) -> int { // NOLINT
     if (velocityValue > 0) {
        this->addChordNote(pitchValue);
    } else {
        this->releaseChordNote(pitchValue);
    }

    return 0;
}

auto Chords::setActiveKey(int keyValue) -> int {
    if (this->activeKey_ < MIDI::RANGE_LOW) {
        // Here we choose which key we want to assign the notes to.
        this->activeKey_ = keyValue;

        // Clear any old notes from the key.
        auto& keyPtr = this->keyboard_.at(keyValue);
        if (!keyPtr->notes().empty()) {
            keyPtr->notes().clear();
        }
    }
    
    return this->activeKey_;
}

auto Chords::addChordNote(int pitchValue) -> int {
    if (0 >= pitchValue && pitchValue > MIDI::KEYBOARD_SIZE) {
        return 1;
    }
    
    if (pitchValue >= MIDI::RANGE_LOW && pitchValue <= MIDI::RANGE_HIGH){
        // The rest of the notes are assigned to the key.
        this->keyboard_.at(this->activeKey_)->add(pitchValue);

        this->addToActive(pitchValue);
    }

    return 0;
}

auto Chords::addToActive(int pitchValue) -> int {
    // Store the notes untill we have released them all.
    this->noteCount_[pitchValue]++;
    return this->noteCount_[pitchValue];
}

auto Chords::removeFromActive(int pitchValue) -> int {
    // Just update the count
    if (this->noteCount_[pitchValue] > 0) {
        this->noteCount_[pitchValue]--;
    }
    
    return this->noteCount_[pitchValue];
}

auto Chords::releaseChordNote(int pitchValue) -> int {
    this->removeFromActive(pitchValue);

    // Check if all released
    if (this->activeKey_ >= 0) {
        bool allReleased = true;
        auto& chordNotes = this->keyboard_[this->activeKey_]->notes();
        
        for (const auto& note : chordNotes) {
            int pitch = note->pitch();
            if (this->noteCount_[pitch] > 0) {
                allReleased = false;
                break;
            }
        }
        
        if (allReleased) {
            auto& chordToPlay = this->keyboard_[this->activeKey_]->notes();
            this->isRecievingNotes_ = false;
            this->activeKey_ = -1;
        }
    }
    
    return 0;
}

auto Chords::playNotes(int pitchValue, int velocityValue) -> int { // NOLINT
    if (!this->keyboard_[pitchValue]->notes().empty()) {
        const auto& sourceNotes = this->keyboard_[pitchValue]->notes();
        
        for(const auto& currentNote : sourceNotes) {
            int pitch = (int) currentNote->pitch();
            int& count = this->noteCount_[pitch];
            
            if (velocityValue == 0) { // NOTE OFF
                if (count > 0) {
                    //bool multipleChordsUsingNote = (count > 1);
                    count--;
                    
                    if (count == 0) {
                        // Only send NOTE_OFF when no chords need this note
                        this->queueNote(pitch, 0);
                    }
                }
            } else { // NOTE ON
                if (this->sendNoteOn(pitch)) {
                    this->queueNote(pitch, velocityValue);
                }
            }
        }
    }

    return 0;
}

auto Chords::sendNoteOn(int pitch) -> bool {
    int& count = this->noteCount_[pitch];

    bool sendNoteOn = false;
    bool noteWasActive = (count > 0);
    
    count++;

    if (count == 1) {
        sendNoteOn = true; // First activation
    } else if (this->noteMode_ == NoteMode::RETRIGGER) {
        sendNoteOn = true; // Always retrigger
    } else if (this->noteMode_ == NoteMode::LEGATO) {
        sendNoteOn = !noteWasActive;
    }
    // else NORMAL mode.

    return sendNoteOn;
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
    
    this->clearActiveNotes();
}

auto Chords::clear(int noteValue) -> void {
    // Clear notes from a specific key.
    
    if ((noteValue >= MIDI::RANGE_LOW) && (noteValue <= MIDI::RANGE_HIGH)) {
        this->keyboard_[noteValue]->clear();
        this->noteCount_[noteValue] = 0;
    }
}

auto Chords::clearActiveNotes() -> void {
    for (const auto &currentNoteCount : this->noteCount_) {
        this->noteCount_[currentNoteCount] = 0;
    }
}

