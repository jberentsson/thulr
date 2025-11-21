#pragma once

#include "../Utils/MIDI.hpp"

class Quantizer {
public:
    enum : uint8_t {
        INVALID_NOTE = 255
    };
    
    enum class RoundDirection : uint8_t { UP, DOWN };
    enum class QuantizeMode : uint8_t { TWELVE_NOTES, ALL_NOTES };
    enum class Note : uint8_t { OFF, ON };

    Quantizer();
    ~Quantizer() = default;

    auto quantize(int noteValue) -> int;
    auto addNote(int noteValue) -> int;
    auto setRange(int rangeLow, int rangeHigh) -> int;
    auto clear() -> int;
    auto getNote(int noteValue) -> Note;
    auto round(int noteValue) -> int;
    auto roundUp(int noteValue) -> int;
    auto roundDown(int noteValue) -> int;
    auto setRoundDirection(RoundDirection direction) -> RoundDirection;
    auto setMode(QuantizeMode mode) -> QuantizeMode;

private:
    bool keyboard[MIDI::KEYBOARD_SIZE];
    int range_low = 0;
    int range_high = 0;
    int note_count = 0;

    RoundDirection round_direction = RoundDirection::UP;
    QuantizeMode mode = QuantizeMode::ALL_NOTES;
};
