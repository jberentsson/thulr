#pragma once
#include "ActiveNote/ActiveNote.hpp"
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>

class Keyboard {
    private:
        using ActiveNote = AbstractActiveNote;

        std::vector<std::unique_ptr<Keyboard::ActiveNote>> activeNotes_;
        int rangeLow_ = RANGE_LOW;
        int rangeHigh_ = RANGE_HIGH;
        int minOctave_ = MIN_OCTAVE;
        int maxOctave_ = MAX_OCTAVE;

  int getPitchClass(int pitch) const;
  int clampPitchToRange(int pitch);
  int randomizeNote(int pitch);

    public:
        enum {            
            MIN_CAPACITY = 2,
            MAX_CAPACITY = 5,
            RANGE_LOW = 0,
            RANGE_HIGH = 127,
            MIN_OCTAVE = 0,
            MAX_OCTAVE = 10
        };

        Keyboard(int low = RANGE_LOW, int high = RANGE_HIGH);
        
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
