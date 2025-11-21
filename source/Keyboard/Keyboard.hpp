#pragma once
#include "../Utils/MIDI.hpp"
#include "ActiveNote/ActiveNote.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>
#include <cstdint>

class Keyboard {
    private:
        using ActiveNote = AbstractActiveNote;

        std::vector<std::unique_ptr<Keyboard::ActiveNote>> activeNotes_;
        
        int rangeLow_ = MIDI::RANGE_LOW;
        int rangeHigh_ = MIDI::RANGE_HIGH;
        int minOctave_ = MIDI::RANGE_LOW;
        int maxOctave_ = MIDI::KEYBOARD_OCTAVES;

        [[nodiscard]] auto getPitchClass(int pitch);
        auto clampPitchToRange(int pitch) const -> int;
        auto randomizeNote(int pitch) -> int;

    public:
        enum : std::uint8_t {            
            MIN_CAPACITY = 2,
            MAX_CAPACITY = 5,
        };

        Keyboard(int low = MIDI::RANGE_LOW, int high = MIDI::RANGE_HIGH);
        
        auto note(int pitch, int velocity) -> int;
        
        auto clearNotesByPitchClass(int pitch) -> int;
        auto removeAll() -> unsigned int;

        void updateRange(int low, int high);
        void setRandomRange(int low, int high);

        static auto maxCapacity() -> int { return MAX_CAPACITY; }
        static auto minCapacity() -> int { return MIN_CAPACITY; }

        [[nodiscard]] const std::vector<std::unique_ptr<ActiveNote>> &getActiveNotes() const;

        void debugPrintActiveNotes() const;
};
