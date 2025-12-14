#pragma once
#include "Utils/MIDI.hpp"
#include "Key.hpp"
#include <memory>
#include <iostream>
#include <vector>

namespace ChordsEnums {
    // Do we want our active notes to retrigger?
    enum NoteMode : uint8_t {
        NORMAL = 0,
        RETRIGGER = 1,
        LEGATO = 2
    };

    // Change the order of the notes that are played.
    enum NoteOrder : uint8_t {
        AS_PLAYED = 0,
        RANDOM = 1
    };
};

class Chords {
    public:
        using NoteMode = ChordsEnums::NoteMode;
        using NoteOrder = ChordsEnums::NoteOrder;

    private:
        std::vector<std::shared_ptr<Key>> keyboard_;
        std::vector<std::shared_ptr<MIDI::Note>> noteQueue_;
        
        int noteCount_[MIDI::KEYBOARD_SIZE] = {};
        NoteMode noteMode_ = NoteMode::RETRIGGER;
        NoteOrder noteOrder_ = NoteOrder::AS_PLAYED;
        bool isRecievingNotes_ = false;
        int activeKey_ = -1;

    public:        
        Chords();
        ~Chords() = default;

        auto removeFromActive(int pitchValue) -> int;
        auto note(int pitchValue, int velocityValue) -> int;
        auto playNotes(int pitchValue, int velocityValue) -> int;
        auto reciveNotes() -> bool;
        auto addToActive(int pitchValue) -> int;
        auto noteQueue() -> std::vector<std::shared_ptr<MIDI::Note>>& { return this->noteQueue_; }
        auto chordNote(int pitchValue, int velocityValue) -> int;
        auto addChordNote(int pitchValue) -> int;
        auto releaseChordNote(int pitchValue) -> int;
        auto setNoteMode(NoteMode mode) -> NoteMode;
        auto getNoteMode() -> NoteMode { return this->noteMode_; }
        auto setNoteOrder(NoteOrder order) -> NoteOrder;
        auto getNoteOrder() -> NoteOrder { return this->noteOrder_; }
        auto queueNote(int noteValue, int velocityValue) -> void;
        auto getKey(int keyValue) -> std::shared_ptr<Key>& { return this->keyboard_[keyValue]; }
        auto sendNoteOn(int pitch) -> bool;
        auto clear() -> void;
        auto clear(int noteValue) -> void;
        auto clearActiveNotes() -> void;
        auto setActiveKey(int keyValue) -> int;
        [[nodiscard]] auto getActiveKey() const -> int { return this->activeKey_; }
        [[nodiscard]] auto isRecievingNotes() const -> bool { return this->isRecievingNotes_; }
};
