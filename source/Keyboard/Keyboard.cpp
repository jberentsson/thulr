#include "Keyboard.hpp"
#include "../Utils/MIDI.hpp"
#include <iostream>

using namespace MIDI;

auto Keyboard::getPitchClass(int pitch) -> int { return pitch % MIDI::OCTAVE; }

auto Keyboard::clampPitchToRange(int pitch) const -> int {
  return std::max(rangeLow_, std::min(pitch, rangeHigh_));
}

auto Keyboard::randomizeNote(int pitch) -> int {
    if (pitch < RANGE_LOW || pitch > RANGE_HIGH) {
        return -1;
    }
    
    return clampPitchToRange(pitch);
}

Keyboard::Keyboard(int low, int high) : rangeLow_(low), rangeHigh_(high) { // NOLINT 
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

auto Keyboard::note(int pitch, int velocity) -> int { // NOLINT 
    if (pitch < RANGE_LOW || pitch > RANGE_HIGH || velocity < RANGE_LOW || velocity > RANGE_HIGH) {
        return 0;
    }
    
    if (velocity > 0) {
        // Note ON
        int processedPitch = randomizeNote(pitch);
        
        if (processedPitch >= RANGE_LOW && processedPitch <= RANGE_HIGH) {
            this->activeNotes_.push_back(std::make_unique<Keyboard::ActiveNote>(pitch, processedPitch, velocity));
            return 1;
        }
    } else {
        // Note OFF
        return clearNotesByPitchClass(pitch);
    }
    
    return 0;
}

auto Keyboard::clearNotesByPitchClass(int pitch) -> int {
    int clearedCount = 0;
    int targetPitchClass = (int) Keyboard::getPitchClass(pitch);
    
    for (auto it = this->activeNotes_.begin(); it != this->activeNotes_.end(); ) {
        if (Keyboard::getPitchClass((*it)->originalPitch()) == targetPitchClass) {
            it = this->activeNotes_.erase(it);
            clearedCount++;
        } else {
            ++it;
        }
    }
  
    return clearedCount;
}

auto Keyboard::removeAll() -> unsigned int {
    unsigned int count = this->activeNotes_.size();
    this->activeNotes_.clear();
    return count;
}

void Keyboard::updateRange(int low, int high) { // NOLINT 
    this->rangeLow_ = low;
    this->rangeHigh_ = high;
}

void Keyboard::setRandomRange(int low, int high) {} // NOLINT 

auto Keyboard::getActiveNotes() -> const std::vector<std::unique_ptr<Keyboard::ActiveNote>>& { // NOLINT 
    return this->activeNotes_;
}
