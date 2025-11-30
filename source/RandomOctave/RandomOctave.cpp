#include "RandomOctave.hpp"
#include "Utils/MIDI.hpp"
#include <vector>
#include <ctime>
#include <random>

std::random_device randomDevice;
std::mt19937 gen(randomDevice());

using namespace MIDI;

RandomOctave::RandomOctave(int low, int high) : rangeLow_(low), rangeHigh_(high) {} // NOLINT

auto RandomOctave::note(int pitch, int velocity) -> int { // NOLINT
    if (pitch < this->rangeLow_ || pitch > this->rangeHigh_ || velocity < this->rangeLow_ || velocity > this->rangeHigh_) {
        return -1;
    }

    int randomPitch = this->randomizeNote(pitch, gen);

    this->keyboard_.add(pitch, randomPitch, velocity);

    return 0;
}

auto RandomOctave::clampPitchToRange(int pitch) const -> int {
    return std::max(rangeLow_, std::min(pitch, rangeHigh_));
}

auto RandomOctave::getPitchClass(int pitch) -> int {
    return pitch % MIDI::OCTAVE;
}

auto RandomOctave::removeAll() -> unsigned int {
    return this->keyboard_.removeAll();
}

auto RandomOctave::setRange(int low, int high) -> int { // NOLINT
    this->rangeLow_ = low;
    this->rangeHigh_ = high;
    return 0;
}

auto RandomOctave::randomizeNote(int pitch, std::mt19937& gen) const -> int {        
    std::uniform_int_distribution<> dist(0, 10); // NOLINT
    int randomInt = dist(gen);
    int randomPitch = getPitchClass(pitch) + (randomInt * MIDI::OCTAVE);
    return clampPitchToRange(randomPitch);
}

auto RandomOctave::clearNotesByPitchClass(int pitch) -> int {
    //return this->keyboard_.clearNotesByPitchClass(pitch);
    return -1;
}

auto RandomOctave::getActiveCount() -> size_t {
    int activeCount = 0;

    for(int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
        activeCount += this->keyboard_.getActiveNotes(i).size();
    }

    return activeCount;
}
