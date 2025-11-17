#pragma once
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "ActiveNote/ActiveNote.hpp"

#define MIN_CAPACITY 2
#define MAX_CAPACITY 5
#define RANGE_LOW    0
#define RANGE_HIGH   127
#define MIN_OCTAVE   0
#define MAX_OCTAVE   10
#define OCTAVE       12

class Keyboard {
    private:
        using ActiveNote = AbstractActiveNote;

        std::vector<std::unique_ptr<Keyboard::ActiveNote>> activeNotes;
        int rangeLow = RANGE_LOW;
        int rangeHigh = RANGE_HIGH;
        int minOctave = MIN_OCTAVE;
        int maxOctave = MAX_OCTAVE;

        int getPitchClass(int pitch) const;
        int clampPitchToRange(int pitch);
        int randomizeNote(int pitch);

    public:
        Keyboard(int low = RANGE_LOW, int high = RANGE_HIGH);
        
        int note(int pitch, int velocity);
        
        int clearNotesByPitchClass(int pitch);
        int removeAll();
        
        void updateRange(int low, int high);
        void setRandomRange(int low, int high);
        
        int maxCapacity() const {
            return MAX_CAPACITY;
        }
            
        int minCapacity() const {
            return MIN_CAPACITY;
        }

        const std::vector<std::unique_ptr<ActiveNote>>& getActiveNotes() const;
        
        void debugPrintActiveNotes() const;
};
