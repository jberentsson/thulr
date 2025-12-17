#include "RandomOctave.hpp"
#include <vector>

using namespace MIDI;

RandomOctave::RandomOctave() {
    this->range_ = Range(MIDI::RANGE_LOW, MIDI::RANGE_HIGH);
};

auto RandomOctave::note(int pitch, int velocity) -> NoteReturnCodes { // NOLINT
    // Check if the pitch is in the correct pitch range.
    if (!MIDI::Note(pitch, velocity).valid() || !this->range_.inRange(MIDI::Note(pitch))) {
        return NoteReturnCodes::OUT_OF_RANGE;
    }

    this->keyboard_.add(pitch, this->randomizeNote(pitch, gen), velocity);

    return NoteReturnCodes::OK;
}

auto RandomOctave::clampPitchToRange(int pitch) -> int {
    return std::max((int) this->range_.low(), std::min(pitch, (int) this->range_.high()));
}

auto RandomOctave::removeAll() -> size_t {
    return this->keyboard_.removeAll();
}

auto RandomOctave::setRange(int low, int high) -> int { // NOLINT
    this->range_ = Range(low, high);
    return 0;
}

auto RandomOctave::randomizeNote(int pitch, std::mt19937& gen) -> int {      
    int rLow = this->range_.octaveLow();
    int rHigh = this->range_.octaveHigh();
    std::uniform_int_distribution<> dist(rLow, rHigh); // NOLINT
    int randomInt = dist(gen);
    int randomPitch = MIDI::valueInOctave(pitch, randomInt);
    return clampPitchToRange(randomPitch);
}
