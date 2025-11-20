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

        [[nodiscard]] int getPitchClass(int pitch) const;
        int clampPitchToRange(int pitch);
        int randomizeNote(int pitch);

    public:
        enum : std::uint8_t {            
            MIN_CAPACITY = 2,
            MAX_CAPACITY = 5,
        };

        Keyboard(int low = MIDI::RANGE_LOW, int high = MIDI::RANGE_HIGH);
        
        int note(int pitch, int velocity);
        
        int clearNotesByPitchClass(int pitch);
        int removeAll();

        void updateRange(int low, int high);
        void setRandomRange(int low, int high);

        static int maxCapacity() { return MAX_CAPACITY; }
        static int minCapacity() { return MIN_CAPACITY; }

        [[nodiscard]] const std::vector<std::unique_ptr<ActiveNote>> &getActiveNotes() const;

        void debugPrintActiveNotes() const;
};
