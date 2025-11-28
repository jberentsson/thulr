#pragma once
#include "Utils/MIDI.hpp"
#include "Note/Keyboard.hpp"
#include "Note/Note.hpp"
#include <cstdint>
#include <memory>
#include <vector>
#include <random>

class RandomOctave {
    public:
        using ActiveNote = AbstractActiveNote;
        using Note = AbstractNote;
        Keyboard keyboard_;

    private:
        int rangeLow_  = MIDI::RANGE_LOW;
        int rangeHigh_ = MIDI::RANGE_HIGH;
        int minOctave_ = MIDI::RANGE_LOW;
        int maxOctave_ = MIDI::KEYBOARD_OCTAVES;

    public:
        enum : std::uint8_t {
            MIN_CAPACITY = 2,
            MAX_CAPACITY = 5,
        };

        explicit RandomOctave(int low = MIDI::RANGE_LOW, int high = MIDI::RANGE_HIGH);
        
        auto clampPitchToRange(int pitch) const -> int {
            return std::max(rangeLow_, std::min(pitch, rangeHigh_));
        }

        static auto getPitchClass(int pitch) -> int {
            return pitch % MIDI::OCTAVE;
        }

        // Alternative: pass random generator as parameter
        auto randomizeNote(int pitch, std::mt19937& gen) -> int {        
            std::uniform_int_distribution<> dist(0, 10);
            int randomInt = dist(gen);
            int randomPitch = getPitchClass(pitch) + (randomInt * MIDI::OCTAVE);
            return clampPitchToRange(randomPitch);
        }

        auto note(int pitch, int velocity) -> int;
        auto removeAll() -> unsigned int;
        auto setRange(int low, int high) -> int;

        static auto maxCapacity() -> int { return MAX_CAPACITY; }
        static auto minCapacity() -> int { return MIN_CAPACITY; }

        auto getActiveNotes() -> std::vector<std::shared_ptr<ActiveNote>> & { return this->keyboard_.getActiveNotes(); }
        auto getNoteQueue() -> std::vector<std::shared_ptr<ActiveNote>> & { return this->keyboard_.getNoteQueue(); }

        auto getActiveNoteCount() -> int { return this->keyboard_.getActiveNoteCount(); }
        auto containsNote(int noteValue) -> bool { return this->keyboard_.containsNote(noteValue); }
        auto clearQueue() -> void { this->keyboard_.clearQueue(); }
        auto clear(int noteValue) -> int { return this->keyboard_.clear(noteValue); }
};
