#pragma once

#include <memory>
#include "Utils/MIDI.hpp"
#include "Enums.hpp"

class Quantizer {
private:
    std::shared_ptr<MIDI::Note> lastNote_;

public:
    using Note = MIDI::Note;
    using NoteData = QuantizerEnums::NoteData;
    using RoundDirection = QuantizerEnums::RoundDirection;
    using QuantizeMode = QuantizerEnums::QuantizeMode;
    using NoteThrough = QuantizerEnums::NoteThrough;
    using QuantizeEnable = QuantizerEnums::QuantizeEnable;

    Quantizer();
    ~Quantizer() = default;

    auto quantize(MIDI::Note noteValue) -> int;
    auto addNote(MIDI::Note noteValue) -> int;
    auto deleteNote(MIDI::Note noteValue) -> int;
    auto getNote(MIDI::Note noteValue) -> MIDI::Note;
    auto setRange(int rangeLow, int rangeHigh) -> int;
    auto getLastNote() -> std::shared_ptr<Note> { return this->lastNote_; }

    auto round(MIDI::Note noteValue) -> int;
    auto roundUp(MIDI::Note noteValue) -> int;
    auto roundUpDown(MIDI::Note noteValue) -> int;
    auto roundDown(MIDI::Note noteValue) -> int;
    auto roundDownUp(MIDI::Note noteValue) -> int;
    auto roundUpOverflow(MIDI::Note noteValue) -> int;
    auto roundDownUnderflow(MIDI::Note noteValue) -> int;
    auto roundNearest(MIDI::Note noteValue) -> int;
    auto roundFurthest(MIDI::Note noteValue) -> int;

    auto setRoundDirection(RoundDirection direction) -> RoundDirection;
    auto setMode(QuantizeMode mode) -> QuantizeMode;
    auto setThrough(NoteThrough throughValue) -> NoteThrough;
    auto addTwelveNotes(MIDI::Note noteValue) -> int;
    auto addAllNotes(MIDI::Note noteValue) -> int;
    auto enable() -> QuantizeEnable;
    auto disable() -> QuantizeEnable;
    auto disableThrough() -> NoteThrough;
    auto enableThrough() -> NoteThrough;
    auto modeTwelveNotes() -> bool;
    auto modeAllNotes() -> bool;
    auto keyboardIndex() -> int;
    auto noteCount() -> int;
    auto clear() -> int;
    auto mutePreviousNote() -> void;
    
    [[nodiscard]] auto getRoundDirection() const -> RoundDirection { return this->roundDirection_; }
    [[nodiscard]] auto high() const -> int { return this->currentNoteLow_; }
    [[nodiscard]] auto low() const -> int { return this->currentNoteLow_; }

private:
    int rangeLow_ = MIDI::Note(MIDI::RANGE_LOW);
    int rangeHigh_ = MIDI::Note(MIDI::RANGE_HIGH);
    
    MIDI::Note currentNoteLow_ = MIDI::Note(MIDI::RANGE_HIGH);
    MIDI::Note currentNoteHigh_ = MIDI::Note(MIDI::RANGE_LOW);
    
    RoundDirection roundDirection_ = RoundDirection::UP;
    QuantizeMode mode_ = QuantizeMode::TWELVE_NOTES;

    MIDI::Note keyboard_[2][MIDI::KEYBOARD_SIZE];
    QuantizeEnable quantizeEnabled_ = QuantizeEnable::ON;
    NoteThrough noteThrough_ = NoteThrough::ON;

    int noteCount_[2] = { 0, 0 };
};
