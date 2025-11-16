#include <iostream>
#include "Keyboard.hpp"

int Keyboard::getPitchClass(int pitch) const {
    return pitch % OCTAVE;
}

int Keyboard::clampPitchToRange(int pitch) {
    return std::max(rangeLow, std::min(pitch, rangeHigh));
}

int Keyboard::randomizeNote(int pitch) {
    if (pitch < 0 || pitch > 127) {
        return -1;
    }
    
    return clampPitchToRange(pitch);
}

Keyboard::Keyboard(int low, int high) : rangeLow(low), rangeHigh(high) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Lock to full MIDI range 0-127
    rangeLow = 0;
    rangeHigh = 127;
}

int Keyboard::note(int pitch, int velocity) {
    if (pitch < 0 || pitch > 127 || velocity < 0 || velocity > 127) {
        return 0;
    }
    
    if (velocity > 0) {
        // Note ON
        int processedPitch = randomizeNote(pitch);
        
        if (processedPitch >= 0 && processedPitch <= 127) {
            activeNotes.push_back(std::make_unique<ActiveNote>(pitch, processedPitch, velocity));
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
    
    for (auto it = activeNotes.begin(); it != activeNotes.end(); ) {
        if (getPitchClass((*it)->originalPitch()) == targetPitchClass) {
            it = activeNotes.erase(it);
            clearedCount++;
        } else {
            ++it;
        }
    }
    
    return clearedCount;
}

int Keyboard::removeAll() {
    int count = activeNotes.size();
    activeNotes.clear();
    return count;
}

void Keyboard::updateRange(int low, int high) {
    rangeLow = low;
    rangeHigh = high;
}

void Keyboard::setRandomRange(int low, int high) {
}

const std::vector<std::unique_ptr<ActiveNote>>& Keyboard::getActiveNotes() const {
    return activeNotes;
}

void Keyboard::debugPrintActiveNotes() const {
    std::cout << "Active notes: " << activeNotes.size() << std::endl;
    for (const auto& note : activeNotes) {
        std::cout << "  Original: " << note->originalPitch() 
                  << " -> Processed: " << note->pitch() 
                  << " Velocity: " << note->velocity() << std::endl;
    }
}
