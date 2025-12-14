#include <memory>
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>
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
            this->setActiveKey(pitchValue);
            std::cout << "Active NOTE ON: " << this->activeKey_ << "\n";
        } else if (pitchValue == this->activeKey_ && velocityValue == 0 && this->keyboard_[pitchValue]->notes().empty()) {
            std::cout << "Active NOTE OFF: " << this->activeKey_ << "\n";
        } else {
            this->chordNote(pitchValue, velocityValue);
        }
        return 0;
    }
    
    // Play chord if key has one recorded.
    if (!this->keyboard_[pitchValue]->notes().empty()) {
        //const auto& chord = this->keyboard_[pitchValue]->notes();
        //std::cout << "are we here?\n";
        //for (const auto& note : chord) {
        //    this->queueNote(note->pitch(), velocityValue);
        //}
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
        
        std::cout << "added key " << keyValue << "\n";
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
        std::cout << "added chord note " << pitchValue << " " 
                  << this->keyboard_.at(this->activeKey_)->notes().size() << " " << this->noteCount_[pitchValue] << "\n";
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
    
    std::cout << "removeFromActive: pitch=" << pitchValue 
              << ", count=" << this->noteCount_[pitchValue] << "\n";
    
    return this->noteCount_[pitchValue];
}

auto Chords::releaseChordNote(int pitchValue) -> int {
    this->removeFromActive(pitchValue);

    std::cout << "DEBUG releaseChordNote - pitch: " << pitchValue 
              << ", count: " << this->noteCount_[pitchValue] << "\n";

    // Check if all released
    if (this->activeKey_ >= 0) {
        bool allReleased = true;
        auto& chordNotes = this->keyboard_[this->activeKey_]->notes();
        
        for (const auto& note : chordNotes) {
            int pitch = note->pitch();
            if (this->noteCount_[pitch] > 0) {
                allReleased = false;
                std::cout << "Note " << pitch << " still active (count: " 
                          << this->noteCount_[pitch] << ")\n";
                break;
            }
        }
        
        if (allReleased) {
            auto& chordToPlay = this->keyboard_[this->activeKey_]->notes();
            std::cout << "All chord notes released! Notes Added: "<< chordToPlay.size() << "\n";
            
            this->isRecievingNotes_ = false;
            this->activeKey_ = -1;
        }
    }
    
    return 0;
}

auto Chords::playNotes(int pitchValue, int velocityValue) -> int {
    if (!this->keyboard_[pitchValue]->notes().empty()) {
        const auto& sourceNotes = this->keyboard_[pitchValue]->notes();
        
        for(const auto& currentNote : sourceNotes) {
            int pitch = (int) currentNote->pitch();
            int& count = this->noteCount_[pitch];
            
            if (velocityValue == 0) { // NOTE OFF
                if (count > 0) {
                    bool multipleChordsUsingNote = (count > 1);
                    count--;
                    
                    if (count == 0) {
                        // Only send NOTE_OFF when no chords need this note
                        this->queueNote(pitch, 0);
                    }else {
                        std::cout << "Not sending NOTE OFF " << (int) currentNote->pitch() << "\n";
                    }
                    // Note: count < 0 shouldn't happen with correct logic
                }
            } else { // NOTE ON
                if (this->sendNoteOn(pitch)) {
                    std::cout << "Sending NOTE ON " << (int) currentNote->pitch() << "\n";
                    this->queueNote(pitch, velocityValue);
                } else {
                    std::cout << "Not sending NOTE ON " << (int) currentNote->pitch() << "\n";
                }
            }
        }
    } else {
        std::cout << "There are notes on that key("<< pitchValue <<")!\n";
    }

    return 0;
}

auto Chords::sendNoteOn(int pitch) -> bool {
    int& count = this->noteCount_[pitch];

    bool sendNoteOn = false;
    bool noteWasActive = (count > 0);
    
    count++;

    if (count == 1) {
        std::cout << "First activation\n";
        sendNoteOn = true; // First activation
    } else if (this->noteMode_ == NoteMode::RETRIGGER) {
        std::cout << "RETRIGGER MODE\n";
        sendNoteOn = true; // Always retrigger
    } else if (this->noteMode_ == NoteMode::LEGATO) {
        std::cout << "LEGATO MODE Note("<< pitch <<") Was Active"<< (noteWasActive ? "TRUE" : "FALSE") <<"\n";
        sendNoteOn = !noteWasActive; // Only if newly activated
    } else {
        std::cout << "sendNoteOn did not do anything\n";
    }
    // else: Normal mode - no duplicate NOTE_ON
    return sendNoteOn;
}

auto Chords::queueNote(int noteValue, int velocityValue) -> void {
    std::cout << ">>>> QUEUE NOTE CALLED: " << noteValue << " " << velocityValue << "\n";
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
    for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
        this->noteCount_[i] = 0;
    }
}
