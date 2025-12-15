#pragma once
#include "Utils/MIDI.hpp"
#include "Key.hpp"
#include <memory>
#include <vector>
#include <random>

namespace ChordsEnums {
    // Do we want our active notes to retrigger?
    enum NoteMode : uint8_t {
        LEGATO = 0,
        RETRIGGER = 1,
        //NORMAL = 2
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
        std::mt19937 gen{std::random_device{}()}; 

    public:        
        Chords();
        ~Chords() = default;

        auto handleNoteOut(MIDI::Note note) -> void;
        auto removeFromActive(int pitchValue) -> int;
        auto note(MIDI::Note note) -> int;
        auto playNotes(MIDI::Note note) -> int;
        auto playNotesInOrder(MIDI::Note note) -> int;
        auto playNotesRandom(MIDI::Note note) -> int;
        auto reciveNotes() -> bool;
        auto addToActive(int notePitch) -> int;
        auto noteQueue() -> std::vector<std::shared_ptr<MIDI::Note>>& { return this->noteQueue_; }
        auto chordNote(MIDI::Note note) -> int;
        auto addChordNote(MIDI::Note) -> int;
        auto releaseChordNote(MIDI::Note note) -> int;
        auto setNoteMode(NoteMode mode) -> NoteMode;
        auto getNoteMode() -> NoteMode { return this->noteMode_; }
        auto setNoteOrder(NoteOrder order) -> NoteOrder;
        auto getNoteOrder() -> NoteOrder { return this->noteOrder_; }
        auto queueNote(MIDI::Note note) -> void;
        auto getKey(int keyValue) -> std::shared_ptr<Key>& { return this->keyboard_[keyValue]; }
        auto sendNoteOn(int pitch) -> bool;
        auto clear() -> void;
        auto clear(int noteValue) -> void;
        auto clearActiveNotes() -> void;
        auto setActiveKey(int key) -> int;
        [[nodiscard]] auto getActiveKey() const -> int { return this->activeKey_; }
        [[nodiscard]] auto isRecievingNotes() const -> bool { return this->isRecievingNotes_; }
};
