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

auto RandomOctave::note(int pitch, int velocity) -> int { // NOLINT
    if (pitch < RANGE_LOW || pitch > RANGE_HIGH || velocity < RANGE_LOW || velocity > RANGE_HIGH) {
        return 0;
    }

    if (velocity > 0) {
        // Note ON
        int processedPitch = randomizeNote(pitch);

        if (processedPitch >= RANGE_LOW && processedPitch <= RANGE_HIGH) {
            this->activeNotes_.push_back(std::make_unique<RandomOctave::ActiveNote>(pitch, processedPitch, velocity));
            return 1;
        }
    } else {
        // Note OFF
        return clearNotesByPitchClass(pitch);
    }

    return 0;
}

auto RandomOctave::clearNotesByPitchClass(int pitch) -> int {
    int clearedCount = 0;
    int targetPitchClass = RandomOctave::getPitchClass(pitch);

    for (auto it = this->activeNotes_.begin(); it != this->activeNotes_.end();) {
        if (RandomOctave::getPitchClass((*it)->originalPitch()) == targetPitchClass) {
            it = this->activeNotes_.erase(it);
            clearedCount++;
        } else {
            ++it;
        }
    }

    return clearedCount;
}

auto RandomOctave::removeAll() -> unsigned int {
    unsigned int count = this->activeNotes_.size();
    this->activeNotes_.clear();
    return count;
}

void RandomOctave::updateRange(int low, int high) { // NOLINT
    this->rangeLow_ = low;
    this->rangeHigh_ = high;
}

void RandomOctave::setRandomRange(int low, int high) {} // NOLINT

auto RandomOctave::getActiveNotes() -> const std::vector<std::unique_ptr<RandomOctave::ActiveNote>> & { // NOLINT
    return this->activeNotes_;
}
