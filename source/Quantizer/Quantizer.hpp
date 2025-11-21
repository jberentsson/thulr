#pragma once

#include "../Utils/MIDI.hpp"

class Quantizer {
public:
    enum {
        INVALID_NOTE = -1
    };
    
    enum class RoundDirection : int { UP, DOWN };
    enum class QuantizeMode : int { TWELVE_NOTES, ALL_NOTES };
    enum class Note : int { OFF, ON };

    Quantizer();
    ~Quantizer() = default;

    auto quantize(int n) -> int;
    auto addNote(int n) -> int;
    auto setRange(int l, int h) -> int;
    auto clear() -> int;
    auto getNote(int n) -> Note;
    auto round(int n) -> int;
    auto roundUp(int n) -> int;
    auto roundDown(int n) -> int;
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
