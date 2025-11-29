#pragma once

#include "Utils/MIDI.hpp"
#include <iostream>

class Quantizer {
public:
    //using Note = MIDI::Note;

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
    
    enum class NoteData : uint8_t { 
        OFF = 0,
        ON = 1
    };

    Quantizer();
    ~Quantizer() = default;

    auto quantize(MIDI::Note noteValue) -> int;
    auto addNote(MIDI::Note noteValue) -> int;
    auto deleteNote(MIDI::Note noteValue) -> int;
    auto setRange(MIDI::Note rangeLow, MIDI::Note rangeHigh) -> int;
    auto clear() -> int;
    auto getNote(MIDI::Note noteValue) -> NoteData;
    auto round(MIDI::Note noteValue) -> int;
    auto roundUp(MIDI::Note noteValue) -> int;
    auto roundUpDown(MIDI::Note noteValue) -> int;
    auto roundDown(MIDI::Note noteValue) -> int;
    auto roundDownUp(MIDI::Note noteValue) -> int;
    auto roundUpOverflow(MIDI::Note noteValue) -> int;
    auto roundDownUnderflow(MIDI::Note noteValue) -> int;
    auto roundNearest(MIDI::Note noteValue) -> int;
    auto roundFurthest(MIDI::Note noteValue) -> int;
    auto addTwelveNotes(MIDI::Note noteValue) -> int;
    auto addAllNotes(MIDI::Note noteValue) -> int;
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
    MIDI::Note range_low = MIDI::Note(MIDI::RANGE_LOW);
    MIDI::Note range_high = MIDI::Note(MIDI::RANGE_HIGH);
    int note_count[2] = { 0, 0 };
    bool quantize_on = true;
    MIDI::Note currentNoteLow = MIDI::Note(MIDI::RANGE_HIGH + 1);
    MIDI::Note currentNoteHigh = MIDI::Note(MIDI::RANGE_LOW - 1);
    bool noteThrough = false;
    
    RoundDirection round_direction = RoundDirection::UP;
    QuantizeMode mode = QuantizeMode::TWELVE_NOTES;
};
