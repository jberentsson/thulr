#include <memory>
#include <algorithm>
#include <iterator>
#include <vector>
#include "Utils/MIDI.hpp"
#include "Chords.hpp"

Chords::Chords() {
    for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
        this->keyboard_.push_back(std::make_shared<Key>(Key(i)));
    }
}

auto Chords::note(int pitchValue, int velocityValue) -> int {
    // Handles note input.

    // Finish if this is the active note off message.
    if (this->keyboard_[pitchValue]->notes().empty() && 
        (velocityValue == 0) && 
        (this->activeKey_ == pitchValue)) {
        return 0;
    }

    if (this->isRecievingNotes_) {
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

auto Chords::reciveNotes() -> bool {
    this->isRecievingNotes_ = true;
    return this->isRecievingNotes_;
}

auto Chords::addChordNote(int pitchValue, int velocityValue) -> int {
    if (this->activeKey_ < 0) {
        // Here we choose which key we want to assign the notes to.
        this->activeKey_ = pitchValue;

        // Clear any old notes from the key.
        if (!this->keyboard_.at(pitchValue)->notes().empty()) {
            this->keyboard_.at(pitchValue)->notes().clear();
        }
    } else {
        // The rest of the notes are assigned to the key.
        this->keyboard_.at(this->activeKey_)->add(pitchValue, velocityValue);

        // Store the notes untill we have released them all.
        this->activeNotes_.push_back(std::make_shared<int>(pitchValue));
    }

    return 0;
}

auto Chords::releaseChordNote(int pitchValue, int velocityValue) -> int { // NOLINT
    // Here we remove the released note from active notes vector.
    int count = 0;

    // Loop through the active notes and find our value.
    for (auto it = this->activeNotes_.begin(); it != this->activeNotes_.end(); ) {
        if (*(*it) == pitchValue) {
            it = this->activeNotes_.erase(it);
            count++;
        } else {
            ++it;
        }
    }

    if (this->activeNotes_.empty()) {
        // When all of the notes have been released we quit the recording mode.
        this->isRecievingNotes_ = false;
        this->activeKey_ = -1;
    }

    return 0;
}

auto Chords::playNotes(int pitchValue, int velocityValue) -> int { // NOLINT
    // Take the notes from the key and add them to the note queue vector.
    if (!this->keyboard_[pitchValue]->notes().empty()) {
        const auto& sourceNotes = this->keyboard_[pitchValue]->notes();
        
        // Reserve space first.
        this->noteQueue_.reserve(this->noteQueue_.size() + sourceNotes.size());
        
        // Use copy which handles self-insertion better.
        //std::copy(
        //    sourceNotes.begin(),
        //    sourceNotes.end(),
        //    std::back_inserter(this->noteQueue_)
        //);

        // TODO: Maybe we want to randomize the order of the notes.
        // TODO: Add some kind of velcity randomization, +/- values
        for(const auto& currentNote : sourceNotes) {
            this->noteQueue_.push_back(std::make_shared<MIDI::Note>(MIDI::Note(currentNote->pitch(), velocityValue)));
        }
    }

    return 0;
}