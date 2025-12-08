#pragma once

#include "Utils/MIDI.hpp"
#include "Key.hpp"
#include <memory>
#include <vector>

class Chords {
    public:
        Chords();
        ~Chords() = default;

        auto note(int pitchValue, int velocityValue) -> int;
        auto playNotes(int pitchValue) -> int;
        auto reciveNotes() -> bool;
        auto noteQueue() -> std::vector<std::shared_ptr<MIDI::Note>>& { return this->noteQueue_; }
        auto notesActive() -> std::vector<std::shared_ptr<int>>& { return this->activeNotes_; }
        auto addChordNote(int pitchValue, int velocityValue) -> int;
        auto releaseChordNote(int pitchValue, int velocityValue) -> int;

        [[nodiscard]] auto isRecievingNotes() const -> bool { return this->isRecievingNotes_; }

    private:
        std::vector<std::shared_ptr<Key>> keyboard_;
        std::vector<std::shared_ptr<int>> activeNotes_;
        std::vector<std::shared_ptr<MIDI::Note>> noteQueue_;

        bool isRecievingNotes_ = false;
        int activeKey_ = -1;
};
