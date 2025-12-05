#pragma once

#include "Utils/MIDI.hpp"

class Range {
private:
    MIDI::Note rangeLow_;
    MIDI::Note rangeHigh_;

    bool valid_;
    int maxNotes_;

public:
    Range() 
        : rangeLow_(MIDI::Note(MIDI::RANGE_LOW))
        , rangeHigh_(MIDI::Note(MIDI::RANGE_HIGH))
        , maxNotes_(calculateMaxNotes())
        , valid_(true) {}

    Range(int rangeLow, int rangeHigh) 
        : rangeLow_(MIDI::Note(rangeLow))
        , rangeHigh_(MIDI::Note(rangeHigh))
        , maxNotes_(calculateMaxNotes())
        , valid_(false)
    {
        if (this->rangeLow_.valid() && this->rangeHigh_.valid()) {
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

    auto set(int low, int high) -> bool {
        // Set the range.
        if (high < low) {
            this->rangeLow_ = MIDI::Note(high);
            this->rangeHigh_ = MIDI::Note(low);
        } else {
            this->rangeLow_ = MIDI::Note(low);
            this->rangeHigh_ = MIDI::Note(high);
        }

        this->maxNotes_ = this->calculateMaxNotes();

        return true;
    }
    
    auto calculateMaxNotes() -> int {
        int noteCount = this->rangeHigh_ - this->rangeLow_;

        // Calculate how many octaves we are working with.
        if (noteCount <= MIDI::OCTAVE) {
            return 1;
        } else if (MIDI::OCTAVE < noteCount) {
            return (noteCount / MIDI::OCTAVE);
        }

        return 0;
    }

    [[nodiscard]] auto valid() const -> bool {
        // Is the range valid.
        return this->valid_;
    }

    [[nodiscard]] auto maxNotes() const -> int {
        return this->maxNotes_;
    }
};
