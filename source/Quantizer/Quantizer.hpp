#pragma once
#include "Utils/MIDI.hpp"

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
    auto enable() -> bool;
    auto disable() -> bool;
    [[nodiscard]] auto noteCount() const -> int { return this->note_count; }

  private:
    bool keyboard[MIDI::KEYBOARD_SIZE];
    int range_low = MIDI::RANGE_LOW;
    int range_high = MIDI::RANGE_HIGH;
    int note_count = 0;
    bool quantize_on = true;

    RoundDirection round_direction = RoundDirection::UP;
    QuantizeMode mode = QuantizeMode::ALL_NOTES;
};
