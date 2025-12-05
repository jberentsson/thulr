#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <ctime>
#include <random>
#include "Note/Keyboard.hpp"
#include "Note/Range.hpp"
#include "Note/Key.hpp"

class RandomOctave {
    public:
        using NoteReturnCodes = MIDI::NoteReturnCodes;

    private:
        Range range_;
        Keyboard keyboard_ = Keyboard(range_);

    public:
        std::mt19937 gen{std::random_device{}()}; 

        explicit RandomOctave();

        auto clampPitchToRange(int pitch) -> int;

        auto randomizeNote(int pitch, std::mt19937& gen) -> int;
        auto note(int pitch, int velocity) -> NoteReturnCodes;
        auto removeAll() -> size_t;
        auto setRange(int low, int high) -> int;

        auto getActiveNotes() -> std::vector<std::shared_ptr<ActiveNote>> { return this->keyboard_.getActiveNotes(); }
        auto getNoteQueue() -> std::vector<std::shared_ptr<ActiveNote>> { return this->keyboard_.getNoteQueue(); }

        auto containsNote(int noteValue) -> bool { return this->keyboard_.containsNote(noteValue); }
        auto clearQueue() -> void { this->keyboard_.clearQueue(); }
        auto maxNotes() -> int { return this->range_.maxNotes(); }
};
