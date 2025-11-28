#pragma once
#include <memory>
#include <vector>
#include "RandomOctave/Note/ActiveNote.hpp"
#include "RandomOctave/Note/Note.hpp"

class AbstractKeyboard {
private:
    using Note = AbstractNote;
    using ActiveNote = AbstractActiveNote;
    std::vector<Note> keyboard_;
    std::vector<std::shared_ptr<ActiveNote>> activeNotes_;
    std::vector<std::shared_ptr<ActiveNote>> noteQueue_;
    size_t activeNoteCount_ = 0;

public:
    AbstractKeyboard() {
        keyboard_.reserve(MIDI::KEYBOARD_SIZE);

        for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
            keyboard_.emplace_back(Note(i));
        }
    }

    auto add(int originalPitch, int randomPitch, int velocity) -> std::shared_ptr<ActiveNote> {
        if (originalPitch >= 0 && originalPitch < keyboard_.size() && (originalPitch % MIDI::OCTAVE == randomPitch % MIDI::OCTAVE)) {
            size_t noteCount = keyboard_[originalPitch].getActiveNoteCount();
            std::shared_ptr<ActiveNote> result =  keyboard_[originalPitch].add(originalPitch, randomPitch, velocity);
            
            if (result != nullptr) {
                noteQueue_.push_back(result);
                this->activeNoteCount_ += (keyboard_[originalPitch].getActiveNoteCount() - noteCount);
            }

            return result;
        }

        return nullptr;
    }

    auto clear(int note) -> size_t {
        if (note >= 0 && note < keyboard_.size()) {
            size_t cleared = keyboard_[note].clear();
            this->activeNoteCount_ -= cleared;
            return cleared;
        }
        return 0;
    }

    auto getActiveNotes(int note) -> std::vector<std::shared_ptr<ActiveNote>> {
        static std::vector<std::shared_ptr<ActiveNote>> empty;
        
        if (note >= 0 && note < keyboard_.size()) {
            return keyboard_[note].getActiveNotes();
        }
        
        return empty;
    }

    [[nodiscard]] auto getActiveNoteCount() const -> int {
        return activeNoteCount_;
    }

    auto getNote(int noteValue) -> Note& {
        return keyboard_[noteValue];
    }

    [[nodiscard]] auto getNote(int noteValue) const -> const Note& {
        return keyboard_[noteValue];
    }

    auto containsNote(int noteValue) -> bool {
        for (int i = (noteValue % 12); i < keyboard_.size(); i += 12) { // NOLINT
            for (const auto &currentActiveNote : keyboard_[i].getActiveNotes()) {
                
                if (currentActiveNote->pitch() == noteValue) {
                    return true;
                }
            }
        }

        return false;
    }

    auto removeAll() -> size_t {
        size_t removed_count = 0;

        for (auto& note : keyboard_) {
            removed_count += note.getActiveNotes().size();
            note.clear();
        }

        this->activeNoteCount_ = 0;
        return removed_count;
    }

    auto getNoteQueue() -> std::vector<std::shared_ptr<ActiveNote>>& {
        return this->noteQueue_;
    }

    auto clearQueue() -> int {
        this->noteQueue_.clear();
        return 0;
    }

    auto getActiveNotes() -> std::vector<std::shared_ptr<ActiveNote>>& {
        return this->activeNotes_;
    }
};
