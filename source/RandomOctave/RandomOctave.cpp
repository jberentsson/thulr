#include "RandomOctave.hpp"
#include "../Utils/MIDI.hpp"
#include <ctime>

using namespace MIDI;

auto RandomOctave::getPitchClass(int pitch) -> int { return pitch % MIDI::OCTAVE; }

auto RandomOctave::clampPitchToRange(int pitch) const -> int {
    return std::max(rangeLow_, std::min(pitch, rangeHigh_));
}

auto RandomOctave::randomizeNote(int pitch) -> int {
    if (pitch < RANGE_LOW || pitch > RANGE_HIGH) {
        return -1;
    }

    return clampPitchToRange(pitch);
}

RandomOctave::RandomOctave(int low, int high) : rangeLow_(low), rangeHigh_(high) { // NOLINT
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

auto RandomOctave::clearQueue() -> int {
    for (auto it = this->noteQueue_.begin(); it != this->noteQueue_.end();) {
        it = this->noteQueue_.erase(it);
    }

    return 0;
}

auto RandomOctave::note(int pitch, int velocity) -> int { // NOLINT
    if (pitch < RANGE_LOW || pitch > RANGE_HIGH || velocity < RANGE_LOW || velocity > RANGE_HIGH) {
        return 0;
    }

    if (velocity > 0) {
        // Note ON
        int processedPitch = randomizeNote(pitch);
        std::shared_ptr<RandomOctave::ActiveNote> newNote = std::make_shared<RandomOctave::ActiveNote>(pitch, processedPitch, velocity);

        if (processedPitch >= RANGE_LOW && processedPitch <= RANGE_HIGH) {
            this->noteQueue_.push_back(newNote);
            this->notesActive_.push_back(newNote);
            return 0;
        }
    } else {
        // Note OFF
        return clearNotesByPitchClass(pitch);
    }

    return 1;
}

auto RandomOctave::clearNotesByPitchClass(int pitch) -> int {
    int clearedCount = 0;
    int targetPitchClass = RandomOctave::getPitchClass(pitch);

    // Then in your loop:
    for (auto it = this->notesActive_.begin(); it != this->notesActive_.end();) {
        if (RandomOctave::getPitchClass((*it)->originalPitch()) == targetPitchClass) {
            this->noteQueue_.push_back(*it);
            it = this->notesActive_.erase(it);
            clearedCount++;
        } else {
            ++it;
        }
    }

    return 0;
}

auto RandomOctave::removeAll() -> unsigned int {
    unsigned int count = this->notesActive_.size();
    this->notesActive_.clear();
    return count;
}

auto RandomOctave::setRange(int low, int high) -> int { // NOLINT
    this->rangeLow_ = low;
    this->rangeHigh_ = high;

    return 0;
}
