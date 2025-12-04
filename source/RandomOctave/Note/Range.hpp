#pragma once

#include "Utils/MIDI.hpp"

class Range {
private:
    MIDI::Note rangeLow_ = MIDI::Note(MIDI::RANGE_LOW);
    MIDI::Note rangeHigh_ = MIDI::Note(MIDI::RANGE_HIGH);

    bool valid_ = false;

public:
    Range() = default;
    Range(MIDI::Note rangeLow, MIDI::Note rangeHigh) : rangeLow_(rangeLow), rangeHigh_(rangeHigh){
        if (rangeLow.valid() && rangeHigh.valid()) {
            this->valid_ = true;
        }
    }

    auto low() -> MIDI::Note {
        return this->rangeLow_;
    }

    auto high() -> MIDI::Note {
        return this->rangeHigh_;
    }

    auto inRange(MIDI::Note note) -> bool {
        // Check if a note is in the correct range.
        return this->low() <= note && note <= this->high();
    }

    auto set(MIDI::Note low, MIDI::Note high) -> bool {
        // Set the range.
        if (high < low) {
            this->rangeLow_ = high;
            this->rangeHigh_ = low;
        } else {
            this->rangeLow_ = low;
            this->rangeHigh_ = high;
        }

        return true;
    }

    [[nodiscard]] auto valid() const -> bool {
        // Is the range valid.
        return this->valid_;
    }
};
