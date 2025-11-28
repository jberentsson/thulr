#pragma once
#include <memory>
#include <vector>
#include <iostream>
#include "RandomOctave/Note/ActiveNote.hpp"
#include "RandomOctave/Note/Note.hpp"

class Keyboard {
private:
    using Note = AbstractNote;
    using ActiveNote = AbstractActiveNote;
    std::vector<Note> keyboard_;
    std::vector<std::shared_ptr<AbstractActiveNote>> activeNotes_;
    std::vector<std::shared_ptr<AbstractActiveNote>> noteQueue_;
    int activeNoteCount_ = 0;

public:
    Keyboard() {
        keyboard_.reserve(MIDI::KEYBOARD_SIZE);

        for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
            keyboard_.emplace_back(Note(i));
        }
    }

    auto add(int originalPitch, int randomPitch, int velocity) -> std::shared_ptr<ActiveNote> {
        if (originalPitch >= 0 && originalPitch < keyboard_.size() && (originalPitch % MIDI::OCTAVE == randomPitch % MIDI::OCTAVE)) {
            int noteCount = keyboard_[originalPitch].getActiveNoteCount();
            std::shared_ptr<ActiveNote> result =  keyboard_[originalPitch].add(originalPitch, randomPitch, velocity);
            
            if (result != nullptr) {
                noteQueue_.push_back(result);
                this->activeNoteCount_ += (keyboard_[originalPitch].getActiveNoteCount() - noteCount);
            }

            return result;
        }

        return nullptr;
    }

    auto clear(int note) -> int {
        if (note >= 0 && note < keyboard_.size()) {
            int cleared = keyboard_[note].clear();
            this->activeNoteCount_ -= cleared;
            return cleared;
        }
        return 0;
    }

    auto getActiveNotes(int note) -> std::vector<std::shared_ptr<ActiveNote>>& {
        static std::vector<std::shared_ptr<ActiveNote>> empty;
        if (note >= 0 && note < keyboard_.size()) {
            return keyboard_[note].getActiveNotes();
        }
        return empty;
    }

    auto getActiveNoteCount() const -> int {
        return activeNoteCount_;
    }

    auto getNote(int noteValue) -> Note& {
        return keyboard_[noteValue];
    }

    auto getNote(int noteValue) const -> const Note& {
        return keyboard_[noteValue];
    }

    auto containsNote(int noteValue) -> bool {
        std::cout << "containsNote " << noteValue << std::endl;

        for (int i = (noteValue % 12); i < keyboard_.size(); i += 12) {
            std::cout << "checking note " << i << " size: " << keyboard_[i].getActiveNotes().size() << std::endl;
            for (const auto &currentActiveNote : keyboard_[i].getActiveNotes()) {
                std::cout << "-------- currentActiveNote " << currentActiveNote->pitch() << " " << currentActiveNote->originalPitch() << std::endl;
                
                if (currentActiveNote->pitch() == noteValue) {
                    std::cout << "Note found!" << std::endl;
                    return true;
                } else {
                    std::cout << "Note not found!" << std::endl;
                }
            }
        }

        return false;
    }

    auto removeAll() -> int {
        int removed_count = 0;

        for (auto& note : keyboard_) {
            removed_count += note.getActiveNotes().size();
            note.clear();
        }

        this->activeNoteCount_ = 0;
        return removed_count;
    }

    auto getNoteQueue() -> std::vector<std::shared_ptr<AbstractActiveNote>>& {
        return this->noteQueue_;
    }

    auto clearQueue() -> int {
        this->noteQueue_.clear();
        return 0;
    }

    auto getActiveNotes() -> std::vector<std::shared_ptr<AbstractActiveNote>>& {
        return this->activeNotes_;
    }
};