#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include <random>
#include "Utils/MIDI.hpp"
#include "Note/Keyboard.hpp"
#include "Note/Note.hpp"

class RandomOctave {
    public:
        Keyboard keyboard_;

    private:
        int rangeLow_  = MIDI::RANGE_LOW;
        int rangeHigh_ = MIDI::RANGE_HIGH;
        int minOctave_ = MIDI::RANGE_LOW;
        int maxOctave_ = MIDI::KEYBOARD_OCTAVES;

    public:
        explicit RandomOctave(int low = MIDI::RANGE_LOW, int high = MIDI::RANGE_HIGH);
        
        [[nodiscard]] auto clampPitchToRange(int pitch) const -> int;
        
        static auto getPitchClass(int pitch) -> int;
        auto clearNotesByPitchClass(int pitch) -> int;

        auto randomizeNote(int pitch, std::mt19937& gen) const -> int;
        auto note(int pitch, int velocity) -> int;
        auto removeAll() -> size_t;
        auto setRange(int low, int high) -> int;

        [[nodiscard]] auto getActiveNotes() const -> std::vector<std::shared_ptr<ActiveNote>> { return this->keyboard_.getActiveNotes(); }
        auto getNoteQueue() -> std::vector<std::shared_ptr<ActiveNote>> &{ return this->keyboard_.getNoteQueue(); }

        auto containsNote(int noteValue) -> bool { return this->keyboard_.containsNote(noteValue); }
        auto clearQueue() -> void { this->keyboard_.clearQueue(); }
        auto clear(int noteValue) -> size_t { return this->keyboard_.clear(noteValue); }
};
