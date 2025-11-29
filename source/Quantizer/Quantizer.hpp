#pragma once

#include "Utils/MIDI.hpp"
#include <iostream>

class Quantizer {
public:
    enum : uint8_t {
        // The highest MIDI value is 127.
        INVALID_NOTE = 255
    };

    enum class RoundDirection : uint8_t { 
        UP = 0,
        DOWN = 1,
        UP_DOWN = 2,
        DOWN_UP = 3,
        UP_OVERFLOW = 4,
        DOWN_UNDERFLOW = 5,
        NEAREST = 6,
        FURTHEST = 7
    };
    
    enum class QuantizeMode : uint8_t { 
        TWELVE_NOTES = 0, 
        ALL_NOTES = 1
    };
    
    enum class Note : uint8_t { 
        OFF = 0,
        ON = 1
    };

    Quantizer();
    ~Quantizer() = default;

    auto quantize(int noteValue) -> int;
    auto addNote(int noteValue) -> int;
    auto deleteNote(int noteValue) -> int;
    auto setRange(int rangeLow, int rangeHigh) -> int;
    auto clear() -> int;
    auto getNote(int noteValue) -> Note;
    auto round(int noteValue) -> int;
    auto roundUp(int noteValue) -> int;
    auto roundUpDown(int noteValue) -> int;
    auto roundDown(int noteValue) -> int;
    auto roundDownUp(int noteValue) -> int;
    auto roundUpOverflow(int noteValue) -> int;
    auto roundDownUnderflow(int noteValue) -> int;
    auto roundNearest(int noteValue) -> int;
    auto roundFurthest(int noteValue) -> int;
    auto addTwelveNotes(int noteValue) -> int;
    auto addAllNotes(int noteValue) -> int;
    auto setRoundDirection(RoundDirection direction) -> RoundDirection;
    auto setMode(QuantizeMode mode) -> QuantizeMode;
    auto enable() -> bool;
    auto disable() -> bool;
    auto disableThrough() -> bool;
    auto enableThrough() -> bool;
    auto keyboardIndex() -> int;
    auto noteCount() -> int;

    [[nodiscard]] auto getRoundDirection() const -> RoundDirection { return this->round_direction; }
    [[nodiscard]] auto high() const -> int { return this->currentNoteLow; }
    [[nodiscard]] auto low() const -> int { return this->currentNoteLow; }

private:
    bool keyboard[2][MIDI::KEYBOARD_SIZE];
    int range_low = MIDI::RANGE_LOW;
    int range_high = MIDI::RANGE_HIGH;
    int note_count[2] = { 0, 0 };
    bool quantize_on = true;
    int currentNoteLow = MIDI::RANGE_HIGH + 1;
    int currentNoteHigh = MIDI::RANGE_LOW - 1;
    bool noteThrough = false;
    
    RoundDirection round_direction = RoundDirection::UP;
    QuantizeMode mode = QuantizeMode::TWELVE_NOTES;
};
