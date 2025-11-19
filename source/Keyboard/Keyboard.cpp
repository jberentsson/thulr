#include "Keyboard.hpp"
#include "../Utils/MIDI.hpp"
#include <iostream>

using namespace MIDI;

int Keyboard::getPitchClass(int pitch) const { return pitch % MIDI::OCTAVE; }

int Keyboard::clampPitchToRange(int pitch) {
  return std::max(rangeLow_, std::min(pitch, rangeHigh_));
}

int Keyboard::randomizeNote(int pitch) {
    if (pitch < RANGE_LOW || pitch > RANGE_HIGH) {
        return -1;
    }
    
    return clampPitchToRange(pitch);
}

Keyboard::Keyboard(int low, int high) : rangeLow_(low), rangeHigh_(high) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

int Keyboard::note(int pitch, int velocity) {
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
  

  if (velocity > 0) {
    // Note ON
    int processedPitch = randomizeNote(pitch);

    if (processedPitch >= 0 && processedPitch <= 127) {
      activeNotes_.push_back(
          std::make_unique<ActiveNote>(pitch, processedPitch, velocity));
      return 1;
    }
  } else {
    // Note OFF
    return clearNotesByPitchClass(pitch);
  }

  return 0;
}

int Keyboard::clearNotesByPitchClass(int pitch) {
    int clearedCount = 0;
    int targetPitchClass = getPitchClass(pitch);
    
    for (auto it = this->activeNotes_.begin(); it != this->activeNotes_.end(); ) {
        if (getPitchClass((*it)->originalPitch()) == targetPitchClass) {
            it = this->activeNotes_.erase(it);
            clearedCount++;
        } else {
            ++it;
        }
    }
  

  return clearedCount;
}

int Keyboard::removeAll() {
    int count = this->activeNotes_.size();
    this->activeNotes_.clear();
    return count;
}

void Keyboard::updateRange(int low, int high) {
    this->rangeLow_ = low;
    this->rangeHigh_ = high;
}

void Keyboard::setRandomRange(int low, int high) {}

const std::vector<std::unique_ptr<Keyboard::ActiveNote>>& Keyboard::getactiveNotes_() const {
    return this->activeNotes_;
}

void Keyboard::debugPrintactiveNotes_() const {
    std::cout << "Active notes: " << this->activeNotes_.size() << std::endl;
    for (const auto& note : this->activeNotes_) {
        std::cout << "  Original: " << note->originalPitch() 
                  << " -> Processed: " << note->pitch() 
                  << " Velocity: " << note->velocity() << std::endl;
    }
}
