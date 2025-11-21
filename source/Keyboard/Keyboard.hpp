#pragma once
#include "../Utils/MIDI.hpp"
#include "ActiveNote/ActiveNote.hpp"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>

class Keyboard {
  private:
    int rangeLow_ = MIDI::RANGE_LOW;
    int rangeHigh_ = MIDI::RANGE_HIGH;
    int minOctave_ = MIDI::RANGE_LOW;
    int maxOctave_ = MIDI::KEYBOARD_OCTAVES;

    [[nodiscard]] auto clampPitchToRange(int pitch) const -> int;

    auto randomizeNote(int pitch) -> int;

  public:
    enum : std::uint8_t {
        MIN_CAPACITY = 2,
        MAX_CAPACITY = 5,
    };

    using ActiveNote = AbstractActiveNote;
    std::vector<std::unique_ptr<Keyboard::ActiveNote>> activeNotes_;

    Keyboard(int low = MIDI::RANGE_LOW, int high = MIDI::RANGE_HIGH);

    auto note(int pitch, int velocity) -> int;
    auto clearNotesByPitchClass(int pitch) -> int;
    auto removeAll() -> unsigned int;

    void updateRange(int low, int high);
    void setRandomRange(int low, int high);

    static auto getPitchClass(int pitch) -> int;
    static auto maxCapacity() -> int { return MAX_CAPACITY; }
    static auto minCapacity() -> int { return MIN_CAPACITY; }

    [[nodiscard]] auto getActiveNotes() -> const std::vector<std::unique_ptr<ActiveNote>> &;
};
