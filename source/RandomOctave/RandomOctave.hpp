#pragma once
#include "Utils/MIDI.hpp"
#include "ActiveNote/ActiveNote.hpp"
#include <cstdint>
#include <memory>
#include <vector>

class RandomOctave {
    public:
        using ActiveNote = AbstractActiveNote;

    private:
        int rangeLow_  = MIDI::RANGE_LOW;
        int rangeHigh_ = MIDI::RANGE_HIGH;
        int minOctave_ = MIDI::RANGE_LOW;
        int maxOctave_ = MIDI::KEYBOARD_OCTAVES;

        std::vector<std::shared_ptr<RandomOctave::ActiveNote>> notesActive_;
        std::vector<std::shared_ptr<RandomOctave::ActiveNote>> noteQueue_;

        [[nodiscard]] auto clampPitchToRange(int pitch) const -> int;

        auto randomizeNote(int pitch) -> int;

    public:
        enum : std::uint8_t {
            MIN_CAPACITY = 2,
            MAX_CAPACITY = 5,
        };

        explicit RandomOctave(int low = MIDI::RANGE_LOW, int high = MIDI::RANGE_HIGH);

        auto note(int pitch, int velocity) -> int;
        auto clearNotesByPitchClass(int pitch) -> int;
        auto removeAll() -> unsigned int;
        auto setRange(int low, int high) -> int;

        static auto getPitchClass(int pitch) -> int;
        static auto maxCapacity() -> int { return MAX_CAPACITY; }
        static auto minCapacity() -> int { return MIN_CAPACITY; }

        [[nodiscard]] auto getActiveNotes() -> std::vector<std::shared_ptr<ActiveNote>> & { return notesActive_; }
        [[nodiscard]] auto getQueuedNotes() -> std::vector<std::shared_ptr<ActiveNote>> & { return noteQueue_; }

        auto clearQueue() -> void { this->noteQueue_.clear(); }
};
