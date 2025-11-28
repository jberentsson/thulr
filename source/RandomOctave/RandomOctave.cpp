#include "RandomOctave.hpp"
#include "Utils/MIDI.hpp"
#include <vector>
#include <ctime>
#include <random>

using namespace MIDI;



RandomOctave::RandomOctave(int low, int high) : rangeLow_(low), rangeHigh_(high) { // NOLINT

}

auto RandomOctave::note(int pitch, int velocity) -> int { // NOLINT
    if (pitch < this->rangeLow_ || pitch > this->rangeHigh_ || velocity < this->rangeLow_ || velocity > this->rangeHigh_) {
        return -1;
    }

    int randomPitch = pitch;

    this->keyboard_.add(pitch, randomPitch, velocity);

    return 0;
}

/* auto RandomOctave::clearNotesByPitchClass(int pitch, int velocity) -> int { // NOLINT
    // Clear all active notes that share a class with pitch input.
    int clearedCount = 0;
    int targetPitchClass = RandomOctave::getPitchClass(pitch);

    for (auto it = this->notesActive_.begin(); it != this->notesActive_.end();) {
        if (RandomOctave::getPitchClass((*it)->originalPitch()) == targetPitchClass) {
            // Add note off to the note queue.
            this->noteQueue_.push_back(std::make_shared<RandomOctave::ActiveNote>((*it)->pitch(), 0, 0));
            
            // Remove the note from active notes.
            it = this->notesActive_.erase(it);
            clearedCount++;
        } else {
            ++it;
        }
    }

    if (clearedCount == 0) {
        this->noteQueue_.push_back(std::make_shared<RandomOctave::ActiveNote>(pitch, pitch, 0));
    }

    return clearedCount;
} */

auto RandomOctave::removeAll() -> unsigned int {
    return this->keyboard_.removeAll();
}

auto RandomOctave::setRange(int low, int high) -> int { // NOLINT
    this->rangeLow_ = low;
    this->rangeHigh_ = high;
    return 0;
}
