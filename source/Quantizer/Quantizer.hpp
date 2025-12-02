#pragma once

#include <memory>
#include "Utils/MIDI.hpp"
#include "Enums.hpp"

class Quantizer {
private:
    std::shared_ptr<MIDI::Note> lastNote;

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
    auto getNote(MIDI::Note noteValue) -> NoteData;
    auto setRange(MIDI::Note rangeLow, MIDI::Note rangeHigh) -> int;
    auto getLastNote() -> std::shared_ptr<Note> { return this->lastNote; }

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

    [[nodiscard]] auto getRoundDirection() const -> RoundDirection { return this->round_direction; }
    [[nodiscard]] auto high() const -> int { return this->currentNoteLow; }
    [[nodiscard]] auto low() const -> int { return this->currentNoteLow; }

private:
    MIDI::Note range_low = MIDI::Note(MIDI::RANGE_LOW);
    MIDI::Note range_high = MIDI::Note(MIDI::RANGE_HIGH);
    MIDI::Note currentNoteLow = MIDI::Note(MIDI::RANGE_HIGH + 1);
    MIDI::Note currentNoteHigh = MIDI::Note(MIDI::RANGE_LOW - 1);
    
    RoundDirection round_direction = RoundDirection::UP;
    QuantizeMode mode = QuantizeMode::TWELVE_NOTES;

    bool keyboard[2][MIDI::KEYBOARD_SIZE];
    QuantizeEnable quantizeEnabled_ = QuantizeEnable::ON;
    NoteThrough noteThrough_ = NoteThrough::ON;

    int note_count[2] = { 0, 0 };
};
