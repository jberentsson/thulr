#include "RandomOctave.hpp"
#include "Utils/MIDI.hpp"
#include <ctime>
#include <random>

using namespace MIDI;

auto RandomOctave::getPitchClass(int pitch) -> int {
    return pitch % MIDI::OCTAVE;
}

auto RandomOctave::clampPitchToRange(int pitch) const -> int {
    return std::max(rangeLow_, std::min(pitch, rangeHigh_));
}

auto RandomOctave::randomizeNote(int pitch) -> int {
    if (pitch < RANGE_LOW || pitch > RANGE_HIGH) {
        return -1;
    }
    
    std::uniform_int_distribution<> dist(0, 10); // NOLINT
    
    int randomInt = dist(gen);
    int randomPitch = RandomOctave::getPitchClass(pitch) + (randomInt * MIDI::OCTAVE);
    return clampPitchToRange(randomPitch);
}

RandomOctave::RandomOctave(int low, int high) : rangeLow_(low), rangeHigh_(high) { // NOLINT
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

auto RandomOctave::note(int pitch, int velocity) -> int { // NOLINT
    if (pitch < RANGE_LOW || pitch > RANGE_HIGH || velocity < RANGE_LOW || velocity > RANGE_HIGH) {
        return 0;
    }

    if (velocity > 0) {
        // Note ON
        int processedPitch = randomizeNote(pitch);

        if (processedPitch >= RANGE_LOW && processedPitch <= RANGE_HIGH) {

            auto newNote = std::make_shared<RandomOctave::ActiveNote>(pitch, processedPitch, velocity);

            notesActive_.push_back(newNote);
            noteQueue_.push_back(newNote);
            
            return 1;
        }
    } else {
        // Note OFF
        return clearNotesByPitchClass(pitch, velocity);
    }

    return 0;
}

auto RandomOctave::clearNotesByPitchClass(int pitch, int velocity) -> int { // NOLINT
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
}

auto RandomOctave::removeAll() -> unsigned int {
    unsigned int count = notesActive_.size();
    this->notesActive_.clear();
    return count;
}

auto RandomOctave::setRange(int low, int high) -> int { // NOLINT
    this->rangeLow_ = low;
    this->rangeHigh_ = high;
    return 0;
}
