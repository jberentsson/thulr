#pragma once

#include "Utils/MIDI.hpp"

class Quantizer {
  public:
    enum : uint8_t {
        // The highest MIDI value is 127.
        INVALID_NOTE = 255
    };

    enum class RoundDirection : uint8_t { UP, DOWN, UP_OVERFLOW, DOWN_UNDERFLOW, NEAREST, FURTHEST, UP_DOWN, DOWN_UP };
    enum class QuantizeMode : uint8_t { TWELVE_NOTES, ALL_NOTES };
    enum class Note : uint8_t { OFF, ON };

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
    auto roundDown(int noteValue) -> int;
    auto roundUpOverflow(int noteValue) -> int;
    auto roundDownUnderflow(int noteValue) -> int;
    auto roundNearest(int noteValue) -> int;
    auto roundFurthest(int noteValue) -> int;
    auto setRoundDirection(RoundDirection direction) -> RoundDirection;
    auto setMode(QuantizeMode mode) -> QuantizeMode;
    auto enable() -> bool;
    auto disable() -> bool;
    auto disableThrough() -> bool;
    auto enableThrough() -> bool;

    [[nodiscard]] auto getRoundDirection() const -> RoundDirection { return this->round_direction; }
    [[nodiscard]] auto noteCount() const -> int { return this->note_count; }
    [[nodiscard]] auto high() const -> int { return this->currentNoteLow; }
    [[nodiscard]] auto low() const -> int { return this->currentNoteLow; }

  private:
    bool keyboard[MIDI::KEYBOARD_SIZE];
    int range_low = MIDI::RANGE_LOW;
    int range_high = MIDI::RANGE_HIGH;
    int note_count = 0;
    bool quantize_on = true;
    int currentNoteLow = MIDI::RANGE_HIGH + 1;
    int currentNoteHigh = MIDI::RANGE_LOW - 1;
    bool noteThrough = false;
    
    RoundDirection round_direction = RoundDirection::UP;
    QuantizeMode mode = QuantizeMode::TWELVE_NOTES;
};
