#include "Keyboard.hpp"
#include <iostream>

int Keyboard::getPitchClass(int pitch) const { return pitch % OCTAVE; }

int Keyboard::clampPitchToRange(int pitch) {
  return std::max(rangeLow, std::min(pitch, rangeHigh));
}

int Keyboard::randomizeNote(int pitch) {
    if (pitch < RANGE_LOW || pitch > RANGE_HIGH) {
        return -1;
    }
    
    return clampPitchToRange(pitch);
}

Keyboard::Keyboard(int low, int high) : rangeLow(low), rangeHigh(high) {
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
            this->activeNotes.push_back(std::make_unique<Keyboard::ActiveNote>(pitch, processedPitch, velocity));
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
      activeNotes.push_back(
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
    
    for (auto it = this->activeNotes.begin(); it != this->activeNotes.end(); ) {
        if (getPitchClass((*it)->originalPitch()) == targetPitchClass) {
            it = this->activeNotes.erase(it);
            clearedCount++;
        } else {
            ++it;
        }
    }
  

  return clearedCount;
}

int Keyboard::removeAll() {
    int count = this->activeNotes.size();
    this->activeNotes.clear();
    return count;
}

void Keyboard::updateRange(int low, int high) {
    this->rangeLow = low;
    this->rangeHigh = high;
}

void Keyboard::setRandomRange(int low, int high) {}

const std::vector<std::unique_ptr<Keyboard::ActiveNote>>& Keyboard::getActiveNotes() const {
    return this->activeNotes;
}

void Keyboard::debugPrintActiveNotes() const {
    std::cout << "Active notes: " << this->activeNotes.size() << std::endl;
    for (const auto& note : this->activeNotes) {
        std::cout << "  Original: " << note->originalPitch() 
                  << " -> Processed: " << note->pitch() 
                  << " Velocity: " << note->velocity() << std::endl;
    }
}
