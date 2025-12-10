#pragma once

#include "Utils/MIDI.hpp"
#include "Key.hpp"
#include <memory>
#include <vector>

class Chords {
    public:
        // Do we want our active notes to retrigger?
        enum NoteMode : uint8_t {
            RETRIGGER = 0,
            LEGATO = 1
        };

        // Change the order of the notes that are played.
        enum NoteOrder : uint8_t {
            AS_PLAYED = 0,
            RANDOM = 1
        };
        
        Chords();
        ~Chords() = default;
        
        auto note(int pitchValue, int velocityValue) -> int;
        auto playNotes(int pitchValue, int velocityValue) -> int;
        auto reciveNotes() -> bool;
        auto noteQueue() -> std::vector<std::shared_ptr<MIDI::Note>>& { return this->noteQueue_; }
        auto notesActive() -> std::vector<std::shared_ptr<int>>& { return this->activeNotes_; }
        auto addChordNote(int pitchValue, int velocityValue) -> int;
        auto releaseChordNote(int pitchValue, int velocityValue) -> int;
        auto setNoteMode(NoteMode mode) -> NoteMode;
        auto setNoteOrder(NoteOrder order) -> NoteOrder;
        auto queueNote(int noteValue, int velocityValue) -> void;
        auto clear() -> void;
        auto clear(int noteValue) -> void;
        [[nodiscard]] auto isRecievingNotes() const -> bool { return this->isRecievingNotes_; }

    private:
        std::vector<std::shared_ptr<Key>> keyboard_;
        std::vector<std::shared_ptr<int>> activeNotes_;
        std::vector<std::shared_ptr<MIDI::Note>> noteQueue_;
        
        int noteCount_[MIDI::KEYBOARD_SIZE] = {};
        NoteMode noteMode_ = NoteMode::RETRIGGER;
        NoteOrder noteOrder_ = NoteOrder::AS_PLAYED;
        bool isRecievingNotes_ = false;
        int activeKey_ = -1;
};
