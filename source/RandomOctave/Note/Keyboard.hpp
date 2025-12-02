#pragma once
#include <memory>
#include <vector>
#include <Utils/MIDI.hpp>
#include "RandomOctave/Note/ActiveNote.hpp"
#include "RandomOctave/Note/Note.hpp"
    


class Keyboard {
private:
    std::vector<Note> keyboard_;
    std::vector<std::shared_ptr<ActiveNote>> activeNotes_;
    std::vector<std::shared_ptr<ActiveNote>> noteQueue_;

    enum : uint8_t {
        MAX_NOTES = 4
    };

public:
    enum class NoteReturnCodes : uint8_t {
        OK = 0,
        OUT_OF_RANGE = 1,
        SOMETHING_ELSE = 2,
        DEGREE_MISMATCH = 3,
        OUT_OF_SPACE = 4
    };
    
    Keyboard() {
        // Populate the keyboard.
        keyboard_.reserve(MIDI::KEYBOARD_SIZE);

        for (int i = 0; i < MIDI::KEYBOARD_SIZE; i++) {
            keyboard_.emplace_back(MIDI::Note(i)); // NOLINT
        }
    }

    auto add(int originalPitch, int randomPitch, int velocity) -> NoteReturnCodes {
        if (originalPitch % MIDI::OCTAVE != randomPitch % MIDI::OCTAVE) {
            return NoteReturnCodes::DEGREE_MISMATCH;
        }

        if (originalPitch < 0 || originalPitch >= static_cast<int>(keyboard_.size())) {
            return NoteReturnCodes::OUT_OF_RANGE;
        }

        if (velocity > 0) {
            // NOTE ON
            if (keyboard_[originalPitch].getActiveNotes().size() >= MAX_NOTES) {
                return NoteReturnCodes::OUT_OF_SPACE;
            }

            auto result = keyboard_[originalPitch].add(originalPitch, randomPitch, velocity);
            if (result == nullptr) {
                return NoteReturnCodes::SOMETHING_ELSE;
            }

            noteQueue_.push_back(result);
            activeNotes_.push_back(result);
        } else {
            // NOTE OFF - Use your remove logic
            if (!keyboard_[originalPitch].getActiveNotes().empty()) {
                // Add note-offs for all active notes on this key
                for (const auto &activeNote : keyboard_[originalPitch].getActiveNotes()) {
                    noteQueue_.push_back(std::make_shared<ActiveNote>(
                        ActiveNote(activeNote->pitch(), activeNote->pitch(), 0)
                    ));
                }
            } else {
                // NOTE THROUGH - original pitch matches random pitch
                noteQueue_.push_back(std::make_shared<ActiveNote>(
                    ActiveNote(originalPitch, originalPitch, 0)
                ));
            }

            // Clear active notes from this key
            keyboard_[originalPitch].getActiveNotes().clear();
        }

        updateActiveNotes();
        return NoteReturnCodes::OK;
    }

    auto remove(int originalPitch) -> std::vector<std::shared_ptr<ActiveNote>> {
        // Check boundries.
        if (originalPitch >= 0 && originalPitch < keyboard_.size()) {
            // Check if there are any active notes on that key,
            if (!this->keyboard_[originalPitch].getActiveNotes().empty()) {
                // Remove note from active and to note queue.
                for (const auto &cuan : this->keyboard_[originalPitch].getActiveNotes()) {
                    // 1. Add it to the noteQueue.
                    this->noteQueue_.push_back(std::make_shared<ActiveNote>(ActiveNote((*cuan).pitch(), (*cuan).pitch(), 0)));
                }
            } 
            else {
                // NOTE THROUGH
                this->noteQueue_.push_back(std::make_shared<ActiveNote>(ActiveNote(originalPitch, originalPitch, 0)));
            }

            // 2. Remove an active note.
            this->keyboard_[originalPitch].getActiveNotes().clear();
            this->updateActiveNotes();
        }

        // 3. Return the note queue.
        return this->noteQueue_;
    }

    auto getActiveNotes(int note) -> std::vector<std::shared_ptr<ActiveNote>> {
        // Get all active notes from each of the notes of the keyboard.
        static std::vector<std::shared_ptr<ActiveNote>> empty;
        
        if (note >= 0 && note < keyboard_.size()) {
            return keyboard_[note].getActiveNotes();
        }
        
        return empty;
    }

    [[nodiscard]] auto getNote(int noteValue) const -> const Note& {
        // Get the note data from each of the keys.
        return keyboard_[noteValue];
    }

    auto containsNote(int noteValue) -> bool {
        // Check if a key has an active note with a specific pitch.
        for (int i = (noteValue % MIDI::OCTAVE); i < keyboard_.size(); i += MIDI::OCTAVE) { // NOLINT
            // Check each of the active notes for every key.
            for (const auto &currentActiveNote : keyboard_[i].getActiveNotes()) {
                if ((currentActiveNote->pitch() % MIDI::OCTAVE) == (noteValue % MIDI::OCTAVE)) {
                    return true;
                }
            }
        }

        return false;
    }

    auto removeAll() -> size_t {
        // Remove all active notes from each key.
        size_t removed_count = 0;

        for (auto& note : keyboard_) {
            removed_count += note.getActiveNotes().size();
            note.getActiveNotes().clear();
        }

        this->activeNotes_.clear();
        return removed_count;
    }

    auto getNoteQueue() -> std::vector<std::shared_ptr<ActiveNote>>& {
        // Return the keyboard notequeue.
        // Used when sending MIDI messages.
        return this->noteQueue_;
    }

    auto clearQueue() -> int {
        // Clear the note queue.
        // Used after sending all of the MIDI messages.
        this->noteQueue_.clear();
        return 0;
    }

    void updateActiveNotes() {
        // Update the active notes vector with data 
        // from each of the notes of the keyboard.
        this->activeNotes_.clear();
        for (auto& currNote : this->keyboard_) {
            const auto& notes = currNote.getActiveNotes();
            this->activeNotes_.insert(this->activeNotes_.end(), notes.begin(), notes.end());
        }
    }

    auto getActiveNotes() ->  std::vector<std::shared_ptr<ActiveNote>>& {
        return this->activeNotes_;
    }
};
